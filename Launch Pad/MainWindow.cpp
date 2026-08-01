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

#include "MainWindow.h"
#include "ResourceManager.h"
#include "ProjectManager.h"

MainWindow::MainWindow(QWidget *parent) {
	_tabs = new QTabWidget;
	
	save = new QAction("Save");
	load = new QAction("Load");
	clear = new QAction("Clear");

	_resourceView	= new ResourceQView;
	_configView		= new ConfigQView;
	_packageView	= new PackageQView;
	_installScriptQView = new InstallScriptQView;
	_buildView = new BuildQView;

	auto menuBar = new QMenuBar;

	auto file = new QMenu("File");
	file->addAction(save);
	file->addAction(load);
	file->addAction(clear);

	menuBar->addMenu(file);

	setMenuBar(menuBar);

	_tabs->addTab(_resourceView, "Resources");
	_tabs->addTab(_configView, "Global Config");
	_tabs->addTab(_packageView, "Package Config");
	_tabs->addTab(_installScriptQView, "Install Script");
	_tabs->addTab(_buildView, "Build");

	setCentralWidget(_tabs);

	QObject::connect(save, &QAction::triggered, this, &MainWindow::saveSlot);
	QObject::connect(load, &QAction::triggered, this, &MainWindow::loadSlot);
	QObject::connect(clear, &QAction::triggered, this, &MainWindow::clearSlot);
}

MainWindow::~MainWindow() {

}

void MainWindow::saveSlot() {
	if (!ResourceManager::getInstance()->root()->children().empty()) {
		ProjectManager::getInstance()->save();
	}
}

void MainWindow::loadSlot() {
	ProjectManager::getInstance()->load();
}

void MainWindow::clearSlot() {
	ResourceManager::getInstance()->clearAll();
}