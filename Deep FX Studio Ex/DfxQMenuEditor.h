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

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <string>
#include <QStringList>

class DfxQMenuEditor {
private:
	static QMenu *getMenu(QString name, QMenu *);
	static QMenu *getMenu(QString name, QMenuBar *);

public:
	using EntryAction = std::function<void()>;

public:
	static void addAction(QMenu *menu, QString path, EntryAction act, bool addSeperator=false);
	static void addAction(QMenuBar *menu, QString path, EntryAction act, bool addSeperator = false);
	static void addMenu(QMenuBar *menu, QMenu *submenu, QString path, bool addSeperator = false);

};