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

#include "DfxToolbarPage.h"
#include "DfxQStyleScrollBar.h"


DfxToolbarPage::DfxToolbarPage(QWidget *parent):QScrollArea(parent) {
	layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
	pageWidget = new QWidget();

	setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	style::styleQScrollBar(horizontalScrollBar());
	style::styleQScrollBar(verticalScrollBar());

	setWidgetResizable(true);

	layout->setContentsMargins(3, 10, 3, 0);
	layout->setAlignment(Qt::AlignTop);
	layout->setSpacing(20);
	
	pageWidget->setLayout(layout);

	pageWidget->setStyleSheet("background-color: transparent;");

	setWidget(pageWidget);
}

DfxToolbarPage::~DfxToolbarPage() {

}

void DfxToolbarPage::addGroup(DfxToolbarGroup *group) {
	layout->addWidget(group);
}