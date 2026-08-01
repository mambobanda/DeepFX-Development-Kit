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

#include "DfxQSceneViewViewportWrangler.h"
#include "DfxQSceneViewport.h"

DfxQSceneViewViewportWrangler::DfxQSceneViewViewportWrangler(QWidget *parent):QWidget(parent) {
	//installEventFilter(this);
	a_split = nullptr;
	b_split = nullptr;
	c_split = nullptr;
	d_split = nullptr;
}

DfxQSceneViewViewportWrangler::~DfxQSceneViewViewportWrangler() {
	viewport00->setParent(nullptr);
	viewport01->setParent(nullptr);
	viewport02->setParent(nullptr);
	viewport03->setParent(nullptr);

	delete viewport00;
	delete viewport01;
	delete viewport02;
	delete viewport03;
}

void DfxQSceneViewViewportWrangler::setup() {
	blayout = new QBoxLayout(QBoxLayout::Direction::RightToLeft, this);
	blayout->setMargin(0);
	blayout->addWidget(viewport00);
	setLayout(blayout);
}

void DfxQSceneViewViewportWrangler::setViewport00(DfxQSceneViewport *vp) {
	viewport00 = vp;
}

DfxQSceneViewport *DfxQSceneViewViewportWrangler::getViewport00() {
	return viewport00;
}

void DfxQSceneViewViewportWrangler::setViewport01(DfxQSceneViewport *vp) {
	viewport01 = vp;
}

DfxQSceneViewport *DfxQSceneViewViewportWrangler::getViewport01() {
	return viewport01;
}

void DfxQSceneViewViewportWrangler::setViewport02(DfxQSceneViewport *vp) {
	viewport02 = vp;
}

DfxQSceneViewport *DfxQSceneViewViewportWrangler::getViewport02() {
	return viewport02;
}

void DfxQSceneViewViewportWrangler::setViewport03(DfxQSceneViewport *vp) {
	viewport03 = vp;
}

DfxQSceneViewport *DfxQSceneViewViewportWrangler::getViewport03() {
	return viewport03;
}

bool DfxQSceneViewViewportWrangler::eventFilter(QObject *obj, QEvent *event) {

	if (event->type() == QEvent::KeyPress) {
		DfxGlobal::console::println("Event Filter");
	}

	return false;
}

void DfxQSceneViewViewportWrangler::clear() {
	
	viewport00->setParent(nullptr);
	viewport01->setParent(nullptr);
	viewport02->setParent(nullptr);
	viewport03->setParent(nullptr);

	if (a_split != nullptr) a_split->setParent(nullptr);
	if (b_split != nullptr) b_split->setParent(nullptr);
	if (c_split != nullptr) c_split->setParent(nullptr);
	if (d_split != nullptr) d_split->setParent(nullptr);

	delete a_split;
	delete b_split;
	delete c_split;
	delete d_split;

	a_split = nullptr;
	b_split = nullptr;
	c_split = nullptr;
	d_split = nullptr;
}

void DfxQSceneViewViewportWrangler::singleLayout() {
	clear();

	blayout->addWidget(viewport00);
}

void DfxQSceneViewViewportWrangler::quadLayout() {
	clear();

	a_split = new QSplitter(Qt::Orientation::Vertical);
	b_split = new QSplitter();
	c_split = new QSplitter();

	b_split->addWidget(viewport00);
	b_split->addWidget(viewport01);

	c_split->addWidget(viewport02);
	c_split->addWidget(viewport03);

	a_split->addWidget(b_split);
	a_split->addWidget(c_split);

	blayout->addWidget(a_split);

	a_split->setSizes(QList<int> { 1, 1 });
	b_split->setSizes(QList<int> { 1, 1 });
	c_split->setSizes(QList<int> { 1, 1 });
}

void DfxQSceneViewViewportWrangler::doubleVerticalLayout() {
	clear();

	a_split = new QSplitter(Qt::Orientation::Horizontal);
	b_split = new QSplitter(Qt::Orientation::Horizontal);
	c_split = new QSplitter(Qt::Orientation::Horizontal);
	
	a_split->addWidget(viewport00);
	a_split->addWidget(viewport01);
		
	blayout->addWidget(a_split);

	a_split->setSizes(QList<int> { 1, 1 });
}

void DfxQSceneViewViewportWrangler::doubleHorizontalLayout() {
	clear();

	a_split = new QSplitter(Qt::Orientation::Vertical);

	a_split->addWidget(viewport00);
	a_split->addWidget(viewport01);

	blayout->addWidget(a_split);

	a_split->setSizes(QList<int> { 1, 1 });

}

void DfxQSceneViewViewportWrangler::top2Bottom1Layout() {
	clear();

	a_split = new QSplitter(Qt::Orientation::Vertical);
	b_split = new QSplitter();

	
	b_split->addWidget(viewport01);
	b_split->addWidget(viewport02);
	a_split->addWidget(b_split);
	a_split->addWidget(viewport00);
	

	blayout->addWidget(a_split);

	a_split->setSizes(QList<int> {1, 4});
	b_split->setSizes(QList<int> {1, 1});
}

void DfxQSceneViewViewportWrangler::top1Bottom2Layout() {
	clear();

	a_split = new QSplitter(Qt::Orientation::Vertical);
	b_split = new QSplitter();

	a_split->addWidget(viewport00);
	b_split->addWidget(viewport01);
	b_split->addWidget(viewport02);
	a_split->addWidget(b_split);

	blayout->addWidget(a_split);

	a_split->setSizes(QList<int> {4, 1});
	b_split->setSizes(QList<int> {1, 1});
}

void DfxQSceneViewViewportWrangler::top3Bottom1Layout() {
	clear();

	a_split = new QSplitter(Qt::Orientation::Vertical);
	b_split = new QSplitter();

	b_split->addWidget(viewport01);
	b_split->addWidget(viewport02);
	b_split->addWidget(viewport03);
	a_split->addWidget(b_split);
	a_split->addWidget(viewport00);

	blayout->addWidget(a_split);

	a_split->setSizes(QList<int> {1, 4});
	b_split->setSizes(QList<int> {1, 1, 1});
}

void DfxQSceneViewViewportWrangler::top1Bottom3Layout() {
	clear();

	a_split = new QSplitter(Qt::Orientation::Vertical);
	b_split = new QSplitter();

	a_split->addWidget(viewport00);
	b_split->addWidget(viewport01);
	b_split->addWidget(viewport02);
	b_split->addWidget(viewport03);
	a_split->addWidget(b_split);

	blayout->addWidget(a_split);

	a_split->setSizes(QList<int> {4, 1});
	b_split->setSizes(QList<int> {1, 1, 1});
}

void DfxQSceneViewViewportWrangler::left2Right1Layout() {
	clear();

	a_split = new QSplitter(Qt::Orientation::Horizontal);
	b_split = new QSplitter(Qt::Orientation::Vertical);
	
	b_split->addWidget(viewport01);
	b_split->addWidget(viewport02);
	a_split->addWidget(b_split);
	a_split->addWidget(viewport00);

	blayout->addWidget(a_split);

	a_split->setSizes(QList<int> {1, 4});
	b_split->setSizes(QList<int> {1, 1, 1});
}

void DfxQSceneViewViewportWrangler::left1Right2Layout() {
	clear();

	a_split = new QSplitter(Qt::Orientation::Horizontal);
	b_split = new QSplitter(Qt::Orientation::Vertical);

	a_split->addWidget(viewport00);
	b_split->addWidget(viewport01);
	b_split->addWidget(viewport02);
	a_split->addWidget(b_split);

	blayout->addWidget(a_split);

	a_split->setSizes(QList<int> {4, 1});
	b_split->setSizes(QList<int> {1, 1, 1});
}

void DfxQSceneViewViewportWrangler::left3Right1Layout() {
	clear();

	a_split = new QSplitter(Qt::Orientation::Horizontal);
	b_split = new QSplitter(Qt::Orientation::Vertical);

	b_split->addWidget(viewport01);
	b_split->addWidget(viewport02);
	b_split->addWidget(viewport03);
	a_split->addWidget(b_split);
	a_split->addWidget(viewport00);

	blayout->addWidget(a_split);

	a_split->setSizes(QList<int> {1, 4});
	b_split->setSizes(QList<int> {1, 1, 1});
}

void DfxQSceneViewViewportWrangler::left1Right3Layout() {
	clear();

	a_split = new QSplitter(Qt::Orientation::Horizontal);
	b_split = new QSplitter(Qt::Orientation::Vertical);

	a_split->addWidget(viewport00);
	b_split->addWidget(viewport01);
	b_split->addWidget(viewport02);
	b_split->addWidget(viewport03);
	a_split->addWidget(b_split);

	blayout->addWidget(a_split);

	a_split->setSizes(QList<int> {4, 1});
	b_split->setSizes(QList<int> {1, 1, 1});
}
