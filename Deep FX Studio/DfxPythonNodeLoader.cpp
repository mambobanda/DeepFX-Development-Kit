/*
* The Mailo Today Solutions Software License
*
* Copyright 2020 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#include "DfxPythonNodeLoader.h"


DfxPythonNodeLoader::DfxPythonNodeLoader() {

}

DfxPythonNodeLoader::PyNodeDesc* DfxPythonNodeLoader::getPythonNodeInfo(std::string path) {
	
	QFile file(path.c_str());

	std::string name = "N/A";
	std::string version = "N/A";
	std::string description = "N/A";
	std::string menu = "N/A";
	std::string temp_path = "N/A";
	bool autoload = false;
	std::string script;

	QDomDocument document;

	if (!file.open(QIODevice::ReadOnly)) {
		DfxGlobal::console::println("Failed to load python node: " + path);
		return nullptr;
	}

	QByteArray content = file.readAll();
	QString errStr = "";
	if (!document.setContent(content, &errStr)) {
		DfxGlobal::console::println("Failed to load python node : " + errStr.toStdString());
		return nullptr;
	}

	file.close();

	QDomElement docElem = document.documentElement();

	QString tag_name = docElem.tagName();

	if (tag_name.compare("pynode", Qt::CaseInsensitive) == 0) {
		QDomNode dom_node = docElem.firstChild();

		while (!dom_node.isNull()) {
			auto element = dom_node.toElement();

			if (!element.isNull()) {
				tag_name = element.tagName();

				if(tag_name.compare("name", Qt::CaseInsensitive) == 0){
					name = element.text().trimmed().toStdString();
				}
				else if (tag_name.compare("version", Qt::CaseInsensitive) == 0) {
					version = element.text().trimmed().toStdString();
				}
				else if (tag_name.compare("description", Qt::CaseInsensitive) == 0) {
					description = element.text().trimmed().toStdString();
				}
				else if (tag_name.compare("menu", Qt::CaseInsensitive) == 0) {
					menu = element.text().trimmed().toStdString();
				}
				else if (tag_name.compare("autoload", Qt::CaseInsensitive) == 0) {
					QString txt = element.text().trimmed();

					if (txt.compare("yes", Qt::CaseInsensitive) == 0)
						autoload = true;
					else
						autoload = false;
				}
				else if (tag_name.compare("path", Qt::CaseInsensitive) == 0) {
					temp_path = element.attribute("url").toStdString();
				}
			}

			dom_node = dom_node.nextSibling();
		}

		QFile script_file(temp_path.c_str());

		if (!script_file.open(QIODevice::ReadOnly)) {
			DfxGlobal::console::println("Failed to load python script: " + temp_path);
			return nullptr;
		}

		script = script_file.readAll().toStdString();

		script_file.close();

		PyNodeDesc *info = new PyNodeDesc;
		info->name = name;
		info->version = version;
		info->description = description;
		info->menu = menu;
		info->autoload = autoload;
		info->script = script;

		return info;
	}
	else {
		DfxGlobal::console::println("Failed to load python node: '<pynode>' tag missing.");
		return nullptr;
	}
}

void DfxPythonNodeLoader::savePythonNodeInfo(DfxPythonNodeLoader::PyNodeDesc *info, std::string packageName, std::string path) {

	QFile file((path  + "/" + packageName + ".pynode").c_str());
	QFile script_file((path + "/" + packageName + ".py").c_str());

	QDomDocument document;

	QDomElement root = document.createElement("pynode");

	QDomElement name = document.createElement("name");
	QDomText name_value = document.createTextNode(info->name.c_str());
	name.appendChild(name_value);

	QDomElement version = document.createElement("version");
	QDomText version_value = document.createTextNode(info->version.c_str());
	version.appendChild(version_value);

	QDomElement description = document.createElement("description");
	QDomText description_value = document.createTextNode(info->description.c_str());
	description.appendChild(description_value);

	QDomElement menu = document.createElement("menu");
	QDomText menu_value = document.createTextNode(info->menu.c_str());
	menu.appendChild(menu_value);

	QDomElement autoload = document.createElement("autoload");
	QDomText autoload_value = document.createTextNode(info->autoload ? "Yes" : "No");
	autoload.appendChild(autoload_value);

	QDomElement _path = document.createElement("path");
	_path.setAttribute("url", (path + "/" + packageName + ".py").c_str());
	
	root.appendChild(name);
	root.appendChild(version);
	root.appendChild(description);
	root.appendChild(menu); 
	root.appendChild(autoload);
	root.appendChild(_path);

	document.appendChild(root);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		DfxGlobal::console::println("install Python Node Error: Failed to open/create " + path);
	}
	else {
		QTextStream stream(&file);
		stream << document.toString();
		file.close();

		DfxGlobal::console::println("Save saved to " + path);
	}

	file.close();


	if (script_file.open(QIODevice::ReadWrite)) {
		QTextStream st(&script_file);
		st << info->script.c_str();
	}
	else {
		DfxGlobal::console::println("Failed to install python script");
	}

	script_file.close();
	
}
