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

#include "ProjectManager.h"
#include "ResourceQModel.h"
#include "VH5IResource.h"
#include "VH5Buffer.h"

ProjectManager *ProjectManager::_instance = nullptr;

ProjectManager::ProjectManager() {

}

ProjectManager::~ProjectManager() {

}

ProjectManager *ProjectManager::getInstance() {
	if (_instance == nullptr) {
		_instance = new ProjectManager;
	}

	return _instance;
}

xvault::Document *ProjectManager::resourcesToVDocument(xvault::Node *node, Resource *resource, unsigned int uid) {

	if (node == nullptr) {
		xvault::Document *_vdoc = new xvault::rapid::rjDocument;
		resourcesToVDocument(_vdoc->root(), ResourceManager::getInstance()->root()->children()[0], 0xffee);
		return _vdoc;
	}
	else {

		xvault::Node *c_node = new xvault::rapid::rjNode(std::to_string(uid++));

		if (resource->type() == Resource::ROOT) {
			auto attr00 = xvault::h5::h5Attribute::newValueUInt("type", xvault::type::UNSIGNED_INT, resource->type());
			auto attr01 = xvault::h5::h5Attribute::newValueString("name", xvault::type::STRING, resource->name());
			auto attr02 = xvault::h5::h5Attribute::newValueString("installation_dir", xvault::type::STRING, resource->installation_dir);
			auto attr03 = xvault::h5::h5Attribute::newValueString("main_executable", xvault::type::STRING, resource->main_executable);
			auto attr04 = xvault::h5::h5Attribute::newValueString("executable_name", xvault::type::STRING, resource->executable_name);
			auto attr05 = xvault::h5::h5Attribute::newValueString("component_name", xvault::type::STRING, resource->component_name);
			auto attr06 = xvault::h5::h5Attribute::newValueString("eula", xvault::type::STRING, resource->eula);

			c_node->attributes().push_back(attr00);
			c_node->attributes().push_back(attr01);
			c_node->attributes().push_back(attr02);
			c_node->attributes().push_back(attr03);
			c_node->attributes().push_back(attr04);
			c_node->attributes().push_back(attr05);
			c_node->attributes().push_back(attr06);

			node->nodes().push_back(c_node);
		}
		else if(resource->type() == Resource::FILE){
			auto attr00 = xvault::h5::h5Attribute::newValueUInt("type", xvault::type::UNSIGNED_INT, resource->type());
			auto attr01 = xvault::h5::h5Attribute::newValueString("name", xvault::type::STRING, resource->name());
			auto attr02 = xvault::h5::h5Attribute::newValueString("resourcePath", xvault::type::STRING, resource->resourcePath);

			c_node->attributes().push_back(attr00);
			c_node->attributes().push_back(attr01);
			c_node->attributes().push_back(attr02);

			node->nodes().push_back(c_node);
		}
		else if (resource->type() == Resource::DIR) {
			auto attr00 = xvault::h5::h5Attribute::newValueUInt("type", xvault::type::UNSIGNED_INT, resource->type());
			auto attr01 = xvault::h5::h5Attribute::newValueString("name", xvault::type::STRING, resource->name());
			
			c_node->attributes().push_back(attr00);
			c_node->attributes().push_back(attr01);

			node->nodes().push_back(c_node);
		}

		for (auto schild : resource->children()) {
			resourcesToVDocument(c_node, schild, uid++);
		}
	}

	return nullptr;
}

void ProjectManager::loadResources(xvault::Node *resource_node, Resource*resource) {

	auto type = resource_node->findAttributeByName("type");
	auto name = resource_node->findAttributeByName("name");

	auto r_model = (ResourceQModel*)ResourceManager::getInstance()->model();

	if (type->valueUInt() == Resource::ROOT) {
		auto inst_dir = resource_node->findAttributeByName("installation_dir");
		auto exe = resource_node->findAttributeByName("main_executable");
		auto exe_name = resource_node->findAttributeByName("executable_name");
		auto comp_name = resource_node->findAttributeByName("component_name");
		auto eula = resource_node->findAttributeByName("eula");

		ResourceManager::getInstance()->rootApplication()->installation_dir = inst_dir->valueString();
		ResourceManager::getInstance()->rootApplication()->main_executable = exe->valueString();
		ResourceManager::getInstance()->rootApplication()->executable_name = exe_name->valueString();
		ResourceManager::getInstance()->rootApplication()->component_name = comp_name->valueString();
		ResourceManager::getInstance()->rootApplication()->eula = eula->valueString();

		for (auto _node: resource_node->nodes()) {
			loadResources(_node, ResourceManager::getInstance()->rootApplication());
		}
	}
	else if (type->valueUInt() == Resource::FILE) {
		auto rpath = resource_node->findAttributeByName("resourcePath");
		
		auto new_rsc = new Resource;
		new_rsc->setName(name->valueString());
		new_rsc->setType(Resource::FILE);
		new_rsc->resourcePath = rpath->valueString();

		r_model->beginAppendRowOp(resource);
		resource->addChild(new_rsc);
		r_model->endAppendRowOp();
	}
	else if (type->valueUInt() == Resource::DIR) {
		auto new_rsc = new Resource;
		new_rsc->setName(name->valueString());
		new_rsc->setType(Resource::DIR);

		r_model->beginAppendRowOp(resource);
		resource->addChild(new_rsc);
		r_model->endAppendRowOp();

		for (auto _node : resource_node->nodes()) {
			loadResources(_node, new_rsc);
		}
	}
}

xvault::Document *ProjectManager::buildToVDocument() {
	xvault::Document *_vdoc = new xvault::rapid::rjDocument;

	auto attr00 = xvault::h5::h5Attribute::newValueString("name", xvault::type::STRING, ResourceManager::getInstance()->name());
	auto attr01 = xvault::h5::h5Attribute::newValueString("build_dir", xvault::type::STRING, ResourceManager::getInstance()->buildDir());
	auto attr02 = xvault::h5::h5Attribute::newValueString("binary_creator", xvault::type::STRING, ResourceManager::getInstance()->binaryCreator());
	auto attr03 = xvault::h5::h5Attribute::newValueUInt("major", xvault::type::UNSIGNED_INT, ResourceManager::getInstance()->major());
	auto attr04 = xvault::h5::h5Attribute::newValueUInt("minor", xvault::type::UNSIGNED_INT, ResourceManager::getInstance()->minor());
	auto attr05 = xvault::h5::h5Attribute::newValueUInt("patch", xvault::type::UNSIGNED_INT, ResourceManager::getInstance()->patch());

	_vdoc->root()->attributes().push_back(attr00);
	_vdoc->root()->attributes().push_back(attr01);
	_vdoc->root()->attributes().push_back(attr02);
	_vdoc->root()->attributes().push_back(attr03);
	_vdoc->root()->attributes().push_back(attr04);
	_vdoc->root()->attributes().push_back(attr05);

	return _vdoc;
}

void ProjectManager::save() {
	QSettings settings;
	auto dir = settings.value("save/directory", "c:/").toString();

	auto dirPath = QFileDialog::getSaveFileName(nullptr, QString("Save Project"), dir, "*.lproj");

	if (!dirPath.isEmpty()) {

		std::unique_ptr<xvault::File> ofile(new xvault::h5::h5File);

		if (ofile->open(dirPath.toStdString(), xvault::io::OPEN)) {
			auto resource_vdoc = resourcesToVDocument(nullptr, nullptr, 1000);
			auto build_vdoc = buildToVDocument();

			if (resource_vdoc != nullptr && build_vdoc != nullptr) {
				xvault::h5::h5ODocumentResource _resources(".document");
				xvault::h5::h5ODocumentResource _buildConfig(".document");
				xvault::h5::h5ODocumentResource _globalConfig(".document");
				xvault::h5::h5ODocumentResource _packageConfig(".document");
				xvault::h5::h5ODocumentResource _installConfig(".document");

				_resources.setText(resource_vdoc->toString());
				_buildConfig.setText(build_vdoc->toString());
				_globalConfig.setText(ResourceManager::getInstance()->configXML());
				_packageConfig.setText(ResourceManager::getInstance()->packageXML());
				_installConfig.setText(ResourceManager::getInstance()->installScriptQS());

				ofile->writeResource("/resources", _resources);
				ofile->writeResource("/build_config", _buildConfig);
				ofile->writeResource("/global_config", _globalConfig);
				ofile->writeResource("/package_config", _packageConfig);
				ofile->writeResource("/install_script", _installConfig);

				settings.setValue("save/directory", QDir(dirPath).path());

				QMessageBox msgBox;
				msgBox.setText("File sucessfully saved.");
				msgBox.exec();
			}
			else {
				QMessageBox msgBox;
				msgBox.setText("Failed to save project: Resources vdoc or Build vdoc nullptr.");
				msgBox.exec();
			}

			ofile->close();

			delete resource_vdoc;
			delete build_vdoc;
		}
		else {

			QMessageBox msgBox;
			msgBox.setText("Failed to save project: Failed to open file.");
			msgBox.exec();
		}
	}
}

void ProjectManager::load() {
	ResourceManager::getInstance()->clearAll();

	QSettings settings;
	auto dir = settings.value("load/directory", "c:/").toString();

	auto dirPath = QFileDialog::getOpenFileName(nullptr, QString("Save Project"), dir, "*.lproj");

	if (!dirPath.isEmpty()) {

		std::unique_ptr<xvault::File> ifile(new xvault::h5::h5File);

		if (ifile->open(dirPath.toStdString(), xvault::io::OPEN_READ_ONLY)) {

			auto iresources = new xvault::h5::h5IResource();
			auto ibuild_config = new xvault::h5::h5IResource();
			auto iglobal_config = new xvault::h5::h5IResource();
			auto ipackage_config = new xvault::h5::h5IResource();
			auto iinstall_script_config = new xvault::h5::h5IResource();

			xvault::rapid::rjDocument rresources;
			xvault::rapid::rjDocument rbuild;

			if (ifile->readResource("/resources", *iresources) &&
				ifile->readResource("/build_config", *ibuild_config) &&
				ifile->readResource("/global_config", *iglobal_config) &&
				ifile->readResource("/package_config", *ipackage_config) &&
				ifile->readResource("/install_script", *iinstall_script_config)) {

				std::string errorMsg = "";
				if (rbuild.fromString(ibuild_config->buffers()[0]->bufferChar(), errorMsg)) {
					auto bname  = rbuild.root()->findAttributeByName("name");
					auto bdir   = rbuild.root()->findAttributeByName("build_dir");
					auto binCreator = rbuild.root()->findAttributeByName("binary_creator");
					auto bmajor = rbuild.root()->findAttributeByName("major");
					auto bminor = rbuild.root()->findAttributeByName("minor");
					auto bpatch = rbuild.root()->findAttributeByName("patch");

					ResourceManager::getInstance()->setName(bname->valueString());
					ResourceManager::getInstance()->setBuildDir(bdir->valueString());
					ResourceManager::getInstance()->setBinaryCreator(binCreator->valueString());
					ResourceManager::getInstance()->setMajor(bmajor->valueUInt());
					ResourceManager::getInstance()->setMinor(bminor->valueUInt());
					ResourceManager::getInstance()->setPatch(bpatch->valueUInt());
				}
				else {
					QMessageBox msgBox;
					msgBox.setText("Failed to parse build config: " + QString(errorMsg.c_str()));
					msgBox.exec();
				}

				ResourceManager::getInstance()->setConfigXML(iglobal_config->buffers()[0]->bufferChar());
				ResourceManager::getInstance()->setPackageXML(ipackage_config->buffers()[0]->bufferChar());
				ResourceManager::getInstance()->setInstallScriptQS(iinstall_script_config->buffers()[0]->bufferChar());

				if (rresources.fromString(iresources->buffers()[0]->bufferChar(), errorMsg)) {
					auto r_model = (ResourceQModel*)ResourceManager::getInstance()->model();
						
					r_model->beginReset();
					loadResources(rresources.root()->nodes()[0], nullptr);
					r_model->endReset();
				}
				else {
					QMessageBox msgBox;
					msgBox.setText("Failed to parse resources: " + QString(errorMsg.c_str()));
					msgBox.exec();
				}
			}
			else {
				QMessageBox msgBox;
				msgBox.setText("Failed to open project: Unknown File Format.");
				msgBox.exec();
			}

			ifile->close();

			settings.setValue("load/directory", QDir(dirPath).path());

			delete iresources;
			delete ibuild_config;
			delete iglobal_config;
			delete ipackage_config;
			delete iinstall_script_config;

			emit projectChanged();
		}
	}
}