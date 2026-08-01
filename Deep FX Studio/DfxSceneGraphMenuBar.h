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
#include <QAction>
#include <functional>
#include <queue>
#include "DfxStudioCommon.h"

using EntryAction = std::function<void()>;

class DfxSceneGraphMenuBar: public QMenuBar {
private:

	QMenu *rootMenu;

	
public:
	DfxSceneGraphMenuBar(QWidget *parent = Q_NULLPTR);

	void addMenuEntry( std::string path, EntryAction at );

	void addMenuEntry( std::string path, EntryAction at, std::queue<std::string> &mqueue, QAction *parent, bool root=false);

	void setRootMenu(QMenu *);

	void createFileMenu(QMenu *menu);

	void createAddMenu(QMenu *menu);

	void createViewMenu(QMenu *menu);
};