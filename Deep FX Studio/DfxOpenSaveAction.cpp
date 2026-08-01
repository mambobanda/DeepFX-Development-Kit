
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

#include <DfxSimulationCoreGlobal.h>
#include "DfxOpenSceneAction.h"
#include "DfxLayerManager.h"
#include "DeepStudioGlobal.h"
#include "DfxXMLSceneLoader.h"


DfxOpenSceneAction::DfxOpenSceneAction(QObject *parent): QAction(parent){
	path = "";

	QObject::connect(this, &QAction::triggered, this, &DfxOpenSceneAction::openFile);
}

void DfxOpenSceneAction::openFile() {

	DfxGlobal::console::println("Opening: " + path.toStdString());
	DeepStudioGlobal::getInstance()->resetFrameState();
	DfxSimulationCoreGlobal::getInstance()->deinitializeSimulationDomain();

	DfxLayerManager::getInstance()->reset();
	DfxGlobal::command::exec("resetViewport");
	DfxLayerManager::getInstance()->removeLayer(0);

	DfxXMLSceneLoader loader;
	loader.load(path.toStdString());

	DeepStudioGlobal::getInstance()->setOpenScenePath(path.toStdString());

	DfxLayerManager::getInstance()->forceMaximise();
	DfxLayerManager::getInstance()->setUseMaximiseScene(true);

	DeepStudioGlobal::getInstance()->selectLayerRoot();
}

void DfxOpenSceneAction::setPath(QString pt) {
	path = pt;
}
	
QString DfxOpenSceneAction::getPath() {
	return path;
}