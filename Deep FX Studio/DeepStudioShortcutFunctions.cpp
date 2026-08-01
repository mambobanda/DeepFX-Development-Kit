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


#include <DeepStudioShortcutFunctions.h>
#include <DfxCoreApplication.h>


void DeepStudioShortcutFunctions::activeViewportSolidMode() {
	DfxCoreApplication::getInstance()->getViewport(0)->renderSolidRenderMode();
}

void DeepStudioShortcutFunctions::activeViewportWireframeMode() {
	DfxCoreApplication::getInstance()->getViewport(0)->renderWireRenderMode();
}

void DeepStudioShortcutFunctions::activeViewportHideSelected() {

	auto all = DfxScene::getInstance()->getSelectedItems();

	if (all.size() > 0) {
		auto layer = DfxLayerManager::getInstance()->findNodeByID(all[0]->getInstanceID());
		layer->hide();
	}

	DeepStudioGlobal::getInstance()->updateLayerManager();
}

void DeepStudioShortcutFunctions::activeViewportHideUnselected() {

	auto all = DfxScene::getInstance()->getUnSelectedItems();

	for (auto it : all) {
		auto layer = DfxLayerManager::getInstance()->findNodeByID(it->getInstanceID());
		layer->hide();
	}

	DfxLayerManager::getInstance()->updateView();
}

void DeepStudioShortcutFunctions::activeViewportHideAll() {

	auto all = DfxScene::getInstance()->getAllItems();
	
	for (auto it : all) {
		auto layer = DfxLayerManager::getInstance()->findNodeByID(it->getInstanceID());
		layer->hide();
	}

	DfxLayerManager::getInstance()->updateView();
}

void DeepStudioShortcutFunctions::activeViewportUnHideAll() {
	
	auto all = DfxScene::getInstance()->getAllItems();

	for (auto it : all) {
		auto layer = DfxLayerManager::getInstance()->findNodeByID(it->getInstanceID());
		layer->unHide();
	}

	DfxLayerManager::getInstance()->updateView();
}