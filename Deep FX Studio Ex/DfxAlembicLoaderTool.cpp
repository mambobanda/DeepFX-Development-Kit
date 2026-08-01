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

#include <SgGeoNode.h>
#include <DfxGeoAdapterType.h>
#include "DfxOpenGLCacheQueue.h"
#include "DfxTimelineManager.h"
#include "DfxAlembicIO.h"
#include "DfxAlembicLoaderTool.h"
#include "DfxAlembicLoaderHandler.h"

std::string DfxAlembicLoaderTool::_uniqueName = "UID_ALEMBIC_LOADER_TOOL";

DfxAlembicLoaderTool::DfxAlembicLoaderTool() {
	_parent = nullptr;
	_dispatcher = nullptr;
	_active = false;
}

DfxAlembicLoaderTool::~DfxAlembicLoaderTool() {

}

std::string DfxAlembicLoaderTool::uniqueName() {
	return _uniqueName;
}

std::string DfxAlembicLoaderTool::getUniqueName() {
	return _uniqueName;
}

void DfxAlembicLoaderTool::onSetReferencePath(std::string path) {
	if (_dispatcher != nullptr) {
		delete _dispatcher;
		_dispatcher = nullptr;
	}

	auto any = DfxSpace::getInstance()->findAbsolute(path);

	if (any != nullptr) {
		_dispatcher = new DfxToolFlDispatcher(this);
		_dispatcher->attachBranch(any);
	}

	DfxToolNode::onSetReferencePath(path);
}

bool DfxAlembicLoaderTool::isReady() {
	return true;
}

bool DfxAlembicLoaderTool::evaluateOnUpdate() {
	return false;
}

bool DfxAlembicLoaderTool::evaluateOnNewFrame() {
	return true;
}

bool DfxAlembicLoaderTool::evaluateOnValueChanged() {
	return true;
}

void DfxAlembicLoaderTool::create(SgNode *parent) {
	if (parent->getType() == deepsg::SgNodeType::GEOMETRY) {
		auto geo = (deepsg::SgGeoNode*)parent;

		if (geo->requestDynaMesh()) {
			_parent = geo;
			_active = true;
		}
	}
}

void DfxAlembicLoaderTool::destroy() {
	if (_active) {
		_parent->releaseDynaMesh();
		_parent = nullptr;
		_active = false;
	}
}

void DfxAlembicLoaderTool::evaluate(SgNode *parent) {

	if (parent->getType() == deepsg::SgNodeType::GEOMETRY) {

		auto geo = (deepsg::SgGeoNode*)parent;
		auto adapter = geo->getGeometryAdapter();

		if (adapter->type() == DfxGeoAdapterType::DYNA_MESH) {

			DfxAlembicLoaderHandler handler;
			if (handler.fetch(referencePath())) {
				auto dao = handler.dataObject()->rawData<fl::DataObject>();

				auto current_frame = DfxTimelineManager::getInstance()->currentFrame();
				auto fpath = dao->property("file_path")->data<std::string>();
				auto opath = dao->property("object_path")->data<std::string>();
				auto scale = dao->property("scale")->data<float>();
				auto offset = dao->property("offset")->data<unsigned int>();
				auto animated = dao->property("animated")->data<bool>();
				auto loop = dao->property("loop")->data<bool>();
				auto apply_xform = dao->property("apply_xform")->data<bool>();

				if (!fpath.empty()) {

					auto dynaMesh = (DfxDynaMesh*)geo->getDynaMesh();
					dynaMesh->cache()->clear();
					dynaMesh->clearAll();

					if (scale != 0) {
						bool rc = false;

						if (!animated) current_frame = 0;
						
						if (!opath.empty()) {
							Alembic::AbcGeom::IPolyMesh pmesh;
							rc = DfxAlembicIO::findLoadPolyMesh(fpath, opath, pmesh, nullptr, glm::dmat4(1.f), apply_xform, parent->referencePath(), dynaMesh, current_frame, scale, offset, loop);
						}

						if (!rc) {
							rc = DfxAlembicIO::loadFirstPolyMesh(fpath, parent->referencePath(), dynaMesh, current_frame, scale, offset, loop, apply_xform);
						}

						if (rc) {
							DfxOpenGLCacheQueue::getInstance()->queueCache(dynaMesh->cache());
						}
						else {
							DfxGlobal::console::println("Failed to load Alembic File.");
						}
					}
				}
			}
		}
	}
}

QWidget *DfxAlembicLoaderTool::form() {

	QWidget *widget = new QWidget;


	QVBoxLayout *main_layout = new QVBoxLayout;
	

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);


	auto fileControlPanel = new DfxControlPanel();

	DfxQControlArray *fileControlArray = new DfxQControlArray();


	std::string path = referencePath() + "/";
	std::string fpath = DfxSpace::getInstance()->findAbsolute(path + "file_path")->data<std::string>();
	std::string opath = DfxSpace::getInstance()->findAbsolute(path + "object_path")->data<std::string>();

	// File Controls
	auto file_path   = new DfxQFilePathControl(path + "file_path", true, false, "Open Alembic File", "c:/", "(*.abc)", "", false);
	auto object_path = new DfxQStringComboBoxControl(path + "object_path", "", std::vector<std::string> {}, false);
	auto scale = new DfxQDecimalSpinnerControl(path + "scale", "", false);
	auto offset = new DfxQIntegerSpinnerControl(path + "offset", "", false);
	auto animated = new DfxQCheckBoxControl(path + "animated", "", false);
	auto loop = new DfxQCheckBoxControl(path + "loop", "", false);
	auto apply_xform = new DfxQCheckBoxControl(path + "apply_xform", "", false);

	object_path->comboBox()->setEditable(true);

	// File
	fileControlArray->addControl("File", file_path);
	fileControlArray->addSpacer(0, 5);
	fileControlArray->addControl("Object", object_path);
	fileControlArray->addSpacer(0, 5);
	fileControlArray->addControl("Scale", scale);
	fileControlArray->addSpacer(0, 5);
	fileControlArray->addControl("Offset", offset);
	fileControlArray->addSpacer(0, 5);
	fileControlArray->addControl("Use Transform", apply_xform);
	fileControlArray->addSpacer(0, 5);
	fileControlArray->addControl("Animated", animated);
	fileControlArray->addSpacer(0, 5);
	fileControlArray->addControl("Loop", loop);

	fileControlPanel->setTitle("Alembic File");
	fileControlPanel->setWidget(fileControlArray);

	object_path->comboBox()->clear();
	filesystem::path p(fpath);

	if (filesystem::exists(p)) {
		std::vector<std::string> paths;
		DfxAlembicIO::shapePathList(fpath, paths, nullptr);

		if (!paths.empty()) {
			object_path->comboBox()->addItem(QString(opath.c_str()));

			for (auto p : paths) {
				object_path->comboBox()->addItem(QString(p.c_str()));
			}
		}
	}

	QObject::connect(file_path->lineEdit(), &QLineEdit::textChanged, [path, object_path](const QString &text) {
		std::string opath = DfxSpace::getInstance()->findAbsolute(path + "object_path")->data<std::string>();

		object_path->comboBox()->clear();
		filesystem::path p(text.toStdString());

		if (filesystem::exists(p)) {
			std::vector<std::string> paths;
			DfxAlembicIO::shapePathList(text.toStdString(), paths, nullptr);

			if (!paths.empty()) {
				object_path->comboBox()->addItem(QString(opath.c_str()));

				for (auto p : paths) {
					object_path->comboBox()->addItem(QString(p.c_str()));
				}
			}
		}
	});

	// Main layout
	main_layout->addWidget(fileControlPanel);

	widget->setLayout(main_layout);


	return widget;
}

std::vector<std::pair<QWidget*, QString>> DfxAlembicLoaderTool::propertyForms() {
	std::pair<QWidget*, QString> props(form(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> {props};
}