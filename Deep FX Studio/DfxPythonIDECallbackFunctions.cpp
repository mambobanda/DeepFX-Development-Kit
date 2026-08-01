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

#include "DfxInstallPythonScriptDialogs.h"
#include "DfxPythonIDECallbackFunctions.h"


bool save_script(std::string script, std::string &ret, std::string path, bool use_path) {

	if (!use_path) {
		QString fileName
			= QFileDialog::getSaveFileName(nullptr, QString("Save Script"), "C:/", QString("Files (*.py)"));

		if (fileName.size() > 0) {
			QFile file(fileName);

			if (file.open(QIODevice::ReadWrite)) {
				QTextStream st(&file);
				st << script.c_str();

				ret = fileName.toStdString();
			}
			else {
				DfxGlobal::console::println("Failed to save python script");
				return false;
			}

			file.close();
		}
		else
		{
			return false;
		}
	}
	else {
		QFile file(path.c_str());

		if (file.open(QIODevice::ReadWrite)) {
			QTextStream st(&file);
			st << script.c_str();
		}
		else {
			DfxGlobal::console::println("Failed to save python script");
			return false;
		}

		file.close();
	}

	return true;
}

bool save_script_as(std::string script, std::string &ret) {

	
	QString fileName
		= QFileDialog::getSaveFileName(nullptr, QString("Save Script"), "C:/", QString("Files (*.py)"));

	if (fileName.size() > 0) {
		QFile file(fileName);

		if (file.open(QIODevice::ReadWrite)) {
			QTextStream st(&file);
			st << script.c_str();

			ret = fileName.toStdString();
		}
		else {
			DfxGlobal::console::println("Failed to save python script");
			return false;
		}

		file.close();
	}
	else
	{
		return false;
	}
	
	return true;
}

void install_script(std::string scrpt) {

	DfxInstallPythonScriptDialogs diag;
	diag.start(scrpt);
}

bool load_script(std::string &script, std::string&ret) {

	QString fileName
		= QFileDialog::getOpenFileName(nullptr, QString("Load Script"), "C:/", QString("Files (*.py;)"));

	if (fileName.size() > 0) {

		QFile file(fileName);

		if (file.open(QIODevice::ReadOnly)) {
			script = file.readAll().toStdString();

			ret = fileName.toStdString();
		}
		else
		{
			DfxGlobal::console::println("Failed to load python script.");
			return false;
		}
	}
	else {
		return false;
	}

	return true;
}