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


#include "DfxQViewContainer.h"

DfxQViewContainer::DfxQViewContainer(QWidget *parent):QWidget(parent) {
	_root = false;
	splitter = nullptr;
	A = nullptr;
	B = nullptr;
	parentContainer = nullptr;
	view = nullptr;
	viewid = DfxQViewID::NONE;

	blayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	blayout->setMargin(0);
	setLayout(blayout);
}

DfxQViewContainer::~DfxQViewContainer() {
	// qDebug("DfxQViewContainer Dieing :(");
}

DfxQViewContainer *DfxQViewContainer::getParentContainer() {
	return parentContainer;
}

void DfxQViewContainer::setParentContainer(DfxQViewContainer *cnt) {
	parentContainer = cnt;
}

void DfxQViewContainer::setRoot(bool b) {
	_root = b;
}

bool DfxQViewContainer::isRoot() {
	return _root;
}

DfxQViewContainer *DfxQViewContainer::getA() {
	return A;
}

DfxQViewContainer *DfxQViewContainer::getB() {
	return B;
}

void DfxQViewContainer::deleteA() {

	if (A != nullptr) {
		A->deleteA();
		A->deleteB();
		A->deleteView();
		delete A;

		A = nullptr;
	}
}

void DfxQViewContainer::deleteB() {

	if(B != nullptr){
		B->deleteA();
		B->deleteB();
		B->deleteView();

		delete B;

		B = nullptr;
	}
}

void DfxQViewContainer::deleteView() {
	delete view;
	view = nullptr;
}

void DfxQViewContainer::setA(DfxQViewContainer *aa) {
	A = aa;
	A->setParentContainer(this);
}

void DfxQViewContainer::setB(DfxQViewContainer *bb) {
	B = bb;
	B->setParentContainer(this);
}


DfxQView *DfxQViewContainer::getView() {
	return view;
}

void DfxQViewContainer::setView(DfxQView *wgt) {
	view = wgt;
	wgt->setContainer(this);
	blayout->addWidget(wgt);
}

void DfxQViewContainer::removeView(DfxQView *wgt) {
	blayout->removeWidget(wgt);
	wgt->setContainer(nullptr);
	view = nullptr;
	//wgt->hide();
}


bool DfxQViewContainer::isSplitterNull() {
	return (splitter == nullptr);
}

void DfxQViewContainer::deleteSplitter() {

	if (!isSplitterNull()) {
		blayout->removeWidget(splitter);

		A->setParent(nullptr);
		B->setParent(nullptr);

		delete splitter;
		splitter = nullptr;
	}
}

void DfxQViewContainer::viewToNull() {
	if (view != nullptr) {
		view->setParent(nullptr);
		view = nullptr;
	}
}

QSplitter *DfxQViewContainer::getSplitter() {
	return splitter;
}

DfxQViewID DfxQViewContainer::getViewID() {
	return viewid;
}

void DfxQViewContainer::setViewID(DfxQViewID vid) {
	viewid = vid;
}

void DfxQViewContainer::newSplitter(Qt::Orientation dir, DfxQViewContainer *a, DfxQViewContainer *b, QSize ratio) {

	a->setParentContainer(this);
	b->setParentContainer(this);

	setA(a);
	setB(b);

	splitter = new QSplitter(dir);
	splitter->addWidget(a);
	splitter->addWidget(b);

	QList<int> qlist;
	qlist.append(ratio.width());
	qlist.append(ratio.height());

	splitter->setSizes(qlist);

	blayout->addWidget(splitter);
}