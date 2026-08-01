/*
* The Mailo Today Solutions Software License
*
* Copyright 2019 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#pragma once

#include <QDialog>
#include <QListWidget>
#include <QBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QDockWidget>
#include "DfxQKeymapEditor.h"

class DfxQPreferencesDialog : public QDialog {
	Q_OBJECT
private:
	QBoxLayout *mainLayout;
	QMainWindow *mainWindow;

	QDockWidget *preferenceViewDock;
	QListWidget *peferenceView;

	void setupPreferencesView();
public:

	DfxQPreferencesDialog(QWidget *parent = nullptr);
	~DfxQPreferencesDialog();
};