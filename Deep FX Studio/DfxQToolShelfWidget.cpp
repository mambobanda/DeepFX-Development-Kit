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

#include "DfxQToolShelfWidget.h"

DfxQToolShelf *DfxQToolShelf::instance = nullptr;


DfxQToolShelf::DfxQToolShelf() {

	layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
	tabLayout = new QTabWidget(this);

	layout->addWidget(tabLayout);
	layout->setMargin(0);

	setLayout(layout);

	counter = 0;

	setMinimumHeight(80);
	setMaximumHeight(80);
}

DfxQToolShelf *DfxQToolShelf::getInstance() {

	if (instance == nullptr) {
		instance = new DfxQToolShelf();
	}

	return instance;
}

void DfxQToolShelf::newSelf(std::string name) {

	QToolBar *bar = new QToolBar();
	bar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	std::pair<std::string, QToolBar*> p(name, bar);
	shelves.insert(p);

	tabLayout->insertTab(counter, bar, name.c_str());

	counter++;
}

void DfxQToolShelf::addToolToShelf(std::string shelfname, QAction *action) {

	auto it = shelves.find(shelfname);

	if (it != shelves.end()) {
		it->second->addAction(action);
	}
}