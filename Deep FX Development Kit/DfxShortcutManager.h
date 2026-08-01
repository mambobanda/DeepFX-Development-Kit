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

#include <QKeySequence>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "DfxShortcutAction.h"

class DfxShortcutManager {
private:
	static DfxShortcutManager *instance;
private:
	std::pair<int, int> viewportZoomSequence;
	std::pair<int, int> viewportPanSequence;
	std::pair<int, int> viewportRotateSequence;

	std::unordered_map <std::string, DfxShortcutAction*> shortcutMap;
	std::unordered_map < std::string, std::vector<DfxShortcutAction*> > groupMap;

	DfxShortcutManager();
public:

	~DfxShortcutManager();

	static DfxShortcutManager *getInstance();

	void setViewportZoomSequence(std::pair<int, int>);
	std::pair<int, int> getViewportZoomSequence();

	void setViewportPanSequence(std::pair<int, int>);
	std::pair<int, int> getViewportPanSequence();

	void setViewportRotateSequence(std::pair<int, int>);
	std::pair<int, int> getViewportRotateSequence();

	void addShortcutAction(std::string name, std::string action_name, std::string group, QWidget *parent, QKeySequence defualt, DfxShortcutActionFunction func);

	void updateAction(std::string name, QShortcut *shortcut);

	QWidget *getParent(std::string name);

	bool hasShortcutAction(std::string name);

	std::vector<DfxShortcutAction*> getAllShortcutActions();
	
	std::vector<DfxShortcutAction*> getGroupShortcutActions(std::string);

	std::vector<std::string> getAllGroupNames();


};
