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

#include "DfxQNodePropertiesPanel.h"

DfxQNodePropertiesPanel::DfxQNodePropertiesPanel(QGraphicsItem *parent): QGraphicsProxyWidget(parent) {

	area     = nullptr;
	groupBox = nullptr;
	layout   = nullptr;

	//area     = new QScrollArea;
	//groupBox = new QGroupBox("");
	//layout   = new QFormLayout;

	//groupBox->setLayout(layout);
	/*area->setWidget(groupBox);
	setWidget(area);
	fixedWidth  = 0;
	dynaHeight  = 0;*/

	fixedHeight = 400;
	pwidth = parent->boundingRect().width();
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

}


DfxQNodePropertiesPanel::~DfxQNodePropertiesPanel() {
	for (auto cont : controls) delete cont;
}

void DfxQNodePropertiesPanel::setTitle(QString tit) {
	groupBox->setTitle(tit);
}

void DfxQNodePropertiesPanel::setHeight(int height) {
	area->setFixedSize(groupBox->size().width() + 30, 100);
}

void DfxQNodePropertiesPanel::init() {

	area = new QScrollArea;
	groupBox = new QGroupBox("");
	
	area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	groupBox->setLayout(layout);
	area->setWidget(groupBox);
	setWidget(area);

	if(groupBox->size().height() > fixedHeight)
		area->setFixedHeight(fixedHeight);

	
	setPos((pwidth * .5) + 12, 0);
}


void DfxQNodePropertiesPanel::addRow(QWidget *a, QWidget *b) {

	if (layout == nullptr) layout = new QFormLayout;

	controls.push_back(a);
	controls.push_back(b);

	layout->addRow(a, b);
}
