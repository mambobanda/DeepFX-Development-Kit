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

#pragma once

#include <QMenuBar>
#include <QMenu>
#include <QSettings>
#include "DfxQMenuEditor.h"

class DfxQMainMenu: public QMenuBar {
private:
	static DfxQMainMenu *_instance;

	QMenu *recentProjects;
	QStringList list;

	DfxQMainMenu();
public:

	static DfxQMainMenu *getInstance();

public:

	void addAction(QString path, DfxQMenuEditor::EntryAction act, bool addSeparator=false);

	void initRecentSceneMenu();
	void initRecentSceneList();
	void updateRecentOpenedSceneList(std::string path);
};