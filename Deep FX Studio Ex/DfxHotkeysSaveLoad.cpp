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

#include "DfxHotkeysSaveLoad.h"

void DfxHotkeysSaveLoad::load(QString path) {

	QFile file(path);
	if (!file.exists()) {
		DfxGlobal::console::println("Failed to load hotkeys.");
		return;
	}

	QDomDocument domDocument;
	domDocument.setContent(&file);
	QDomElement mapElement = domDocument.documentElement();
	QDomNode groupNode = mapElement.firstChild();

	if (mapElement.tagName().compare("map") == 0) {

		while (!groupNode.isNull()) {

			QDomElement groupElement = groupNode.toElement();
			if (!groupElement.isNull()) {

				if (groupElement.tagName().compare("group") == 0) {

					DfxShortcutsDB::Group grp;
					std::string gname = groupElement.attribute("name").toStdString();

					if (DfxShortcutsDB::getInstance()->findGroup(gname, grp)) {

						QDomNode shortcutNode = groupElement.firstChild();

						while (!shortcutNode.isNull()) {
							QDomElement shortcutElement = shortcutNode.toElement();

							if (!shortcutElement.isNull()) {

								if (shortcutElement.tagName().compare("shortcut") == 0) {

									std::string sname = shortcutElement.attribute("name").toStdString().c_str();
									int type = std::atoi(shortcutElement.attribute("type").toStdString().c_str());
									int f00 = std::atoi(shortcutElement.attribute("k0").toStdString().c_str());
									int f01 = std::atoi(shortcutElement.attribute("k1").toStdString().c_str());
									int f02 = std::atoi(shortcutElement.attribute("k2").toStdString().c_str());
									int f03 = std::atoi(shortcutElement.attribute("k3").toStdString().c_str());
									int m00 = std::atoi(shortcutElement.attribute("m").toStdString().c_str());
								
									DfxShortcutsDB::getInstance()->setHotkey(gname, sname, type, f00, f01, f02, f03, m00);
								}

							}
							shortcutNode = shortcutNode.nextSibling();
						}
					}
				}

			}

			groupNode = groupNode.nextSibling();
		}
	}
}

void DfxHotkeysSaveLoad::save(QString path) {

	std::stringstream ss;
	ss << "<map>\n";
	for (auto group : DfxShortcutsDB::getInstance()->findAllGroups()) {

		ss << "\t" << "<group name=\"" << group.name << "\" >\n";
		for (auto shortcut : group.shortcuts) {
			auto sht = shortcut.second;
			ss << "\t\t" << "<shortcut name=\"" << sht.name << "\" type=\"" << sht.type <<"\"  k0=\"" << sht.k00 << "\" k1=\"" << sht.k01 << "\" k2=\"" << sht.k02 << "\" k3=\"" << sht.k03 << "\" m=\"" << sht.mouseButton << "\"/>\n"; 
		}
		ss << "\t" << "</group>\n";
	}
	ss << "</map>\n";

	QFile file(path);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&file);
	out << ss.str().c_str();

	file.close();
}