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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/
#include "ResourceManager.h"
#include "ResourceQModel.h"
ResourceManager *ResourceManager::_instance = nullptr;

ResourceManager::ResourceManager() {
	_root = new Resource;
	_root->setType(Resource::NONE);

	_rootApplication = new Resource;
	_rootApplication->setType(Resource::ROOT);
	_rootApplication->setName("Application Root");

	_root->addChild(_rootApplication);

	_model = new ResourceQModel;

	_major = 0;
	_minor = 0;
	_patch = 0;

	initConfig();
	initPackage();
	initInstallScript();
}

ResourceManager::~ResourceManager() {

}

ResourceManager *ResourceManager::getInstance() {
	if (_instance == nullptr) {
		_instance = new ResourceManager;
	}

	return _instance;
}

Resource *ResourceManager::root() {
	return _root;
}

Resource *ResourceManager::rootApplication() {
	return _rootApplication;
}

QAbstractItemModel *ResourceManager::model() {
	return _model;
}

void ResourceManager::clearAll(Resource *resources) {

	for (auto chd: resources->children()) {
		clearAll(chd);
	}

	delete resources;
}

void ResourceManager::clearAll() {
	auto rModel = (ResourceQModel*)_model;

	rModel->beginReset();

	clearAll(_root);

	_root = new Resource;
	_root->setType(Resource::NONE);

	_rootApplication = new Resource;
	_rootApplication->setType(Resource::ROOT);
	_rootApplication->setName("Application Root");

	_root->addChild(_rootApplication);

	rModel->endReset();
}

std::string ResourceManager::configXML() {
	return _configXML;
}

void ResourceManager::setConfigXML(std::string txt) {
	_configXML = txt;
}

std::string ResourceManager::packageXML() {
	return _packageXML;
}

void ResourceManager::setPackageXML(std::string txt) {
	_packageXML = txt;
}

std::string ResourceManager::installScriptQS() {
	return _installScriptQS;
}

void ResourceManager::setInstallScriptQS(std::string txt) {
	_installScriptQS = txt;
}

void ResourceManager::initConfig() {
	_configXML =
		"<?xml version='1.0' encoding='UTF-8' ?>\n"
		"<Installer>\n"
		"\t<Name>Start Menu Shortcut Example</Name>\n"
		"\t<Version>1.0.0</Version>\n"
		"\t<Title>Start Menu Shortcut Example</Title>\n"
		"\t<Publisher>Qt - Project</Publisher>\n"
		"\t<!--Directory name is used in component.xml-->\n"
		"\t<StartMenuDir>Qt Installer Framework Examples</StartMenuDir>\n"
		"\t<TargetDir>@HomeDir@/IfwExamples/startmenu</TargetDir>\n"
		"</Installer>\n";
}

void ResourceManager::initPackage() {
	_packageXML =
		"<?xml version='1.0' encoding='UTF-8'?>\n"
		"<Package>\n"
		"\t<DisplayName>The root component</DisplayName>\n"
		"\t<Description>Install this example.</Description>\n"
		"\t<Version>0.1.0-1 </Version>\n"
		"\t<ReleaseDate>2010-09-21</ReleaseDate>\n"
		"\t<Licenses>\n"
		"\t\t<License file='license.txt' name ='Beer Public License Agreement'/>\n"
		"\t</Licenses>\n"
		"\t<Default>script</Default>\n"
		"\t<Script>installscript.qs</Script>\n"
		"</Package>\n";
}

void ResourceManager::initInstallScript() {
	_installScriptQS =
		"function Component() {\n"
		"\t// default constructor\n"
		"}\n\n"

		"Component.prototype.createOperations = function(){\n"
		"\t// call default implementation to actually install README.txt!\n"
		"\tcomponent.createOperations();\n"
		"}\n";
}

std::string &ResourceManager::name() {
	return _name;
}

void ResourceManager::setName(std::string val) {
	_name = val;
}

std::string &ResourceManager::buildDir() {
	return _buildDir;
}

void ResourceManager::setBuildDir(std::string val) {
	_buildDir = val;
}

int &ResourceManager::major() {
	return _major;
}

void ResourceManager::setMajor(int val) {
	_major = val;
}

int &ResourceManager::minor() {
	return _minor;
}

void ResourceManager::setMinor(int val) {
	_minor = val;
}

int &ResourceManager::patch() {
	return _patch;
}

void ResourceManager::setPatch(int val) {
	_patch = val;
}

std::string &ResourceManager::binaryCreator() {
	return _binaryCreator;
}

void ResourceManager::setBinaryCreator(std::string val) {
	_binaryCreator = val;
}
