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

#include <string>
#include <QtWidgets/QWidget>
#include <QtWidgets/QShortcut>

using DfxShortcutActionFunction = void(*)();

class DfxShortcutAction {
private:
	std::string uniqueName;
	std::string actionName;
	QWidget *parent;
	QShortcut *shortcut;
	QKeySequence defaultSequence;
	DfxShortcutActionFunction function;
public:

	DfxShortcutAction();
	~DfxShortcutAction();

	std::string getActionName();
	void setActionName(std::string);

	std::string getName();
	void setName(std::string);

	QWidget* getParent();
	void setParent(QWidget *);

	QShortcut *getShorcut();
	void setShorcut(QShortcut *);
	void updateShorcut(QKeySequence);

	DfxShortcutActionFunction getFunction();
	void setFunction(DfxShortcutActionFunction func);

	QKeySequence getDefualtSequence();
	void setDefaultSequence(QKeySequence);

	void clearShortcut();
};