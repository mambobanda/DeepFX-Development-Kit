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

#pragma once

#include <QFile>
#include <QTextStream>
#include <QtXML/QDomDocument>
#include <DfxGlobal.h>
#include <experimental\filesystem>
#include <string>
 

class DfxPythonNodeLoader {
public:
	class PyNodeDesc {
	public:
		std::string name;
		std::string version;
		std::string description;
		std::string menu;
		bool autoload;
		std::string script;
	};
private:
public:
	DfxPythonNodeLoader();

	static PyNodeDesc* getPythonNodeInfo(std::string path);
	static void savePythonNodeInfo(PyNodeDesc *info, std::string packageName, std::string path);
};