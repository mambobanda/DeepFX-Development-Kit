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

#include "DfxQPreferencesDialog.h"

DfxQPreferencesDialog::DfxQPreferencesDialog(QWidget *parent): QDialog(parent) {

	setWindowTitle("Deep FX Studio Preferences");

	mainLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
	mainWindow = new QMainWindow();

	preferenceViewDock = new QDockWidget(nullptr, Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
	peferenceView = new QListWidget(parent);

	preferenceViewDock->setTitleBarWidget(new QWidget);

	preferenceViewDock->setAllowedAreas(Qt::LeftDockWidgetArea);
	preferenceViewDock->setWidget(peferenceView);

	mainWindow->addDockWidget(Qt::LeftDockWidgetArea, preferenceViewDock);
	mainWindow->setCentralWidget(new DfxQKeymapEditor());

	mainLayout->addWidget(mainWindow);
	setLayout(mainLayout);

	setMinimumWidth(700);
	setMinimumHeight(400);

	setupPreferencesView();
}

DfxQPreferencesDialog::~DfxQPreferencesDialog() {

}

void DfxQPreferencesDialog::setupPreferencesView() {

	new QListWidgetItem(tr("Keymap"), peferenceView);
}