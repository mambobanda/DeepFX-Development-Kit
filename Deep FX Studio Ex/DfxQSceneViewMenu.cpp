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

#include "DfxQSceneViewMenu.h"

QMenu *DfxQSceneViewMenu::_currentMenu = nullptr;

void DfxQSceneViewMenu::execObjectMode() {
	clear();
}

void DfxQSceneViewMenu::execVertexMode() {
	clear();
}

void DfxQSceneViewMenu::execEdgeMode() {
	clear();
}

void DfxQSceneViewMenu::execFaceMode() {

	clear();

	QMenu *selectSet = nullptr;
	QMenu *selectSurface = nullptr;

	auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

	std::vector<std::string> sets;
	std::set<unsigned int> idSet;

	if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {
		auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
		auto mesh = adapter->getMesh()->resource<ds::Mesh>();

		sets = DfxSetTools::allFaceSets(mesh);
		
		for (auto face : mesh->faces()) {
			auto tid = face->getSurfID();

			if (tid > 0) idSet.insert(tid);
		}

	}
	else if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DYNA_MESH) {

		auto adapter = (DfxDynaMeshGeoAdapter*)geonode->getGeometryAdapter();
		auto mesh = adapter->getMesh();

		for (unsigned int i = 0; i < mesh->triangleCount(); i++) {
			auto tid = mesh->faceIDs()[i];

			if (tid > 0) idSet.insert(tid);
		}
	}

	if (!sets.empty()) {
		selectSet = new QMenu("Select Set");

		for (auto &s : sets) {
			auto temp = new QAction(s.c_str());
			selectSet->addAction(temp);

			QObject::connect(temp, &QAction::triggered, [s]() {DfxGlobal::command::exec(std::string("selectFaceSet ") + s); });
		}
	}
	
	if (!idSet.empty()) {
		selectSurface = new QMenu("Select Surface");

		for (auto &i : idSet) {
			auto temp = new QAction(DfxSurfaceDictionary::getInstance()->string(geonode->referencePath(), i).c_str());
			selectSurface->addAction(temp);

			QObject::connect(temp, &QAction::triggered, [i]() {DfxGlobal::command::exec(std::string("selectSurface ") + std::to_string(i)); });
		}
	}

	_currentMenu = new QMenu("");
	_currentMenu->setStyleSheet("QMenu::separator { background-color: rgb(50, 50, 50) }");

	auto selectAll = new QAction("Select All");
	auto selectFaceLoop = new QAction("Select Face Loop");
	auto selectConnected = new QAction("Select Connected");

	auto createSet  = new QAction("New Set");
	auto tagSurface = new QAction("Tag Surface");

	_currentMenu->addAction(selectAll);
	_currentMenu->addAction(selectFaceLoop);
	_currentMenu->addAction(selectConnected);
	if (selectSet != nullptr) _currentMenu->addMenu(selectSet);
	if (selectSurface != nullptr) _currentMenu->addMenu(selectSurface);
	_currentMenu->addSeparator();
	_currentMenu->addAction(createSet);
	_currentMenu->addSeparator();
	_currentMenu->addAction(tagSurface);

	QObject::connect(selectAll, &QAction::triggered, []() {DfxGlobal::command::exec("selectAllFaces"); });
	QObject::connect(selectFaceLoop, &QAction::triggered, []() {DfxGlobal::command::exec("selectFaceLoop"); });
	QObject::connect(selectConnected, &QAction::triggered, []() {DfxGlobal::command::exec("selectConnectedFaces"); });

	QObject::connect(createSet, &QAction::triggered, []() {DfxGlobal::command::exec("createFaceSet"); });
	QObject::connect(tagSurface, &QAction::triggered, []() {DfxGlobal::command::exec("tagSurface"); });

	_currentMenu->exec(QCursor::pos());
}

void DfxQSceneViewMenu::exec() {

	if (DfxMode::getInstance()->mode() == DfxMode::Mode::OBJECT_EDIT) {
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::FACE_EDIT) {
		execFaceMode();
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::VERTEX_EDIT) {
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::EDGE_EDIT) {
	}
}

void DfxQSceneViewMenu::clear() {

	if (_currentMenu != nullptr) {
		_currentMenu->close();
		delete _currentMenu;
		_currentMenu = nullptr;
	}
}