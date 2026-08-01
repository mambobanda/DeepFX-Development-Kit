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

#include "DfxToolbarGroup.h"

DfxToolbarGroup::DfxToolbarGroup(QString tit, QWidget *parent):QWidget(parent) {
	label = new GroupLabel;
	label->setText(tit);
	label->setAlignment(Qt::AlignCenter);
	label->setStyleSheet("QLabel { background: rgb(128, 0, 0 ); }");

	layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);

	layout->addWidget(label);

	layout->setSpacing(1);
	layout->setMargin(0);

	layout->setAlignment(Qt::AlignTop);

	setLayout(layout);
}

DfxToolbarGroup::~DfxToolbarGroup() {

}

void DfxToolbarGroup::addItem(DfxToolbarItem *item) {
	layout->addWidget(item);
}

void DfxToolbarGroup::addMenu(DfxToolbarMenu *menu) {
	layout->addWidget(menu);
}