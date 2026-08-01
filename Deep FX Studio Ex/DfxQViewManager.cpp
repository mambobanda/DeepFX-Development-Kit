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

#include "DfxQViewManager.h"
#include <QTextEdit>

DfxQViewManager *DfxQViewManager::_instance = nullptr;

DfxQViewManager::DfxQViewManager() {
	_root = new DfxQViewContainer;
	_root->setRoot(true);
}

DfxQViewManager *DfxQViewManager::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxQViewManager;
	}

	return _instance;
}

void DfxQViewManager::splitViewContainer(Qt::Orientation dir, DfxQViewContainer *container, DfxQView *other, DfxQViewID vid, QSize ratio) {

	if (container->isSplitterNull()) {

		DfxQView *temp = container->getView();
		DfxQViewID tid = container->getViewID();

		container->removeView(temp);
		container->setViewID(DfxQViewID::NONE);

		DfxQViewContainer *containerA = new DfxQViewContainer;
		DfxQViewContainer *containerB = new DfxQViewContainer;

		containerA->setView(temp);
		containerA->setViewID(tid);

		containerB->setView(other);
		containerB->setViewID(vid);

		container->newSplitter(dir, containerA, containerB, ratio);
	}
}

void DfxQViewManager::mergeViewContainerA(DfxQViewContainer *container) {

	DfxQViewContainer *pcont = container->getParentContainer();
	
	DfxQView *tempA = pcont->getA()->getView();
	auto vid = pcont->getA()->getViewID();

	if (tempA != nullptr) {
		pcont->getA()->viewToNull();

		pcont->deleteSplitter();

		pcont->deleteA();
		pcont->deleteB();

		pcont->setView(tempA);
		pcont->setViewID(vid);
	}
	else {
		auto containerA = pcont->getA()->getA();
		auto containerB = pcont->getA()->getB();

		auto dir = pcont->getSplitter()->orientation();
		auto size = pcont->getA()->getSplitter()->sizes();

		pcont->getA()->deleteSplitter();
		pcont->deleteSplitter();

		pcont->deleteB();
		delete pcont->getA();

		pcont->newSplitter(dir, containerA, containerB, QSize(size[0], size[1]));
	}
}

void DfxQViewManager::mergeViewContainerB(DfxQViewContainer *container) {

	DfxQViewContainer *pcont = container->getParentContainer();

	DfxQView *tempB = pcont->getB()->getView();
	auto vid = pcont->getB()->getViewID();

	if(tempB != nullptr) {
		pcont->getB()->viewToNull();

		pcont->deleteSplitter();

		pcont->deleteA();
		pcont->deleteB();

		pcont->setView(tempB);
		pcont->setViewID(vid);
	}
	else {

		auto containerA = pcont->getB()->getA();
		auto containerB = pcont->getB()->getB();

		auto dir = pcont->getSplitter()->orientation();
		auto size = pcont->getB()->getSplitter()->sizes();

		pcont->getB()->deleteSplitter();
		pcont->deleteSplitter();

		pcont->deleteA();	
		delete pcont->getB();

		pcont->newSplitter(dir, containerA, containerB, QSize(size[0], size[1]));
	}
	
}

DfxQViewContainer *DfxQViewManager::root() {
	return _root;
}

void DfxQViewManager::merge(DfxQView *view) {

	auto container = view->getContainer();

	if (!container->isRoot()) {
		if (view == container->getParentContainer()->getA()->getView()) {
			mergeViewContainerB(container);
		}
		else {
			mergeViewContainerA(container);		
		}
	}
	else {
		qDebug("Is Root Container");
	}
}

void DfxQViewManager::minMaxView(DfxQView *view) {

	auto container = view->getContainer();

	if (!container->isRoot()) {
		auto splitter = container->getParentContainer()->getSplitter();
		bool viewIsA = view == container->getParentContainer()->getA()->getView();

		if (view->isViewMaximised()) {

			QList<int> ls;
			ls.append(1);
			ls.append(1);
			splitter->setSizes(ls);

			view->setViewIsMaximised(false);

		}
		else {

			QList<int> ls;

			if (viewIsA) {
				ls.append(1);
				ls.append(0);
			}
			else {
				ls.append(0);
				ls.append(1);
			}

			splitter->setSizes(ls);

			view->setViewIsMaximised(true);
		}
	}
	else {
		qDebug("Is Root Container");
	}
}

void DfxQViewManager::clear(DfxQViewContainer *container) {

	container->deleteSplitter();
	container->deleteA();
	container->deleteB();
	container->deleteView();
}