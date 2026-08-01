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
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <QAbstractItemModel>
#include "Resource.h"

class ResourceManager {
private:
	static ResourceManager *_instance;

	Resource *_root;
	Resource *_rootApplication;
	QAbstractItemModel *_model;
	ResourceManager();

	void clearAll(Resource *resources);

private:
	std::string _configXML;
	std::string _packageXML;
	std::string _installScriptQS;

	std::string _name;

	std::string _buildDir;

	std::string _binaryCreator;

	int _major;
	int _minor;
	int _patch;

	void initConfig();
	void initPackage();
	void initInstallScript();
public:
	virtual ~ResourceManager();

	static ResourceManager *getInstance();

	Resource *root();
	Resource *rootApplication();

	void clearAll();

	QAbstractItemModel *model();

	std::string configXML();
	void setConfigXML(std::string );

	std::string packageXML();
	void setPackageXML(std::string );

	std::string installScriptQS();
	void setInstallScriptQS(std::string);

	std::string &name();
	void setName(std::string name);

	std::string &buildDir();
	void setBuildDir(std::string);

	int &major();
	void setMajor(int val);

	int &minor();
	void setMinor(int val);
	
	int &patch();
	void setPatch(int val);

	std::string &binaryCreator();
	void setBinaryCreator(std::string val);
};