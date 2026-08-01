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

#include "DfxGlobal.h"
#include "DfxShortcutManager.h"

DfxShortcutManager *DfxShortcutManager::instance = nullptr;

DfxShortcutManager::DfxShortcutManager() {

}

DfxShortcutManager::~DfxShortcutManager() {

}

DfxShortcutManager *DfxShortcutManager::getInstance() {

	if (instance == nullptr) {
		instance = new DfxShortcutManager();
	}

	return instance;
}

void DfxShortcutManager::setViewportZoomSequence(std::pair<int, int> sq) {
	viewportZoomSequence = sq;
}

std::pair<int, int> DfxShortcutManager::getViewportZoomSequence() {
	return viewportZoomSequence;
}

void DfxShortcutManager::setViewportPanSequence(std::pair<int, int> sq) {
	viewportPanSequence = sq;
}

std::pair<int, int> DfxShortcutManager::getViewportPanSequence() {
	return viewportPanSequence;
}

void DfxShortcutManager::setViewportRotateSequence(std::pair<int, int> sq) {
	viewportRotateSequence = sq;
}

std::pair<int, int> DfxShortcutManager::getViewportRotateSequence() {
	return viewportRotateSequence;
}

void DfxShortcutManager::addShortcutAction(std::string name, std::string action_name, std::string group, QWidget *parent, QKeySequence defualt, DfxShortcutActionFunction func) {
	DfxShortcutAction *saction = new DfxShortcutAction();
	saction->setName(name);
	saction->setParent(parent);
	saction->setDefaultSequence(defualt);
	saction->setFunction(func);
	saction->setActionName(action_name);

	std::pair < std::string, DfxShortcutAction *> p(name, saction);
	shortcutMap.insert(p);

	auto it = groupMap.find(group);

	if (it != groupMap.end()) {
		it->second.push_back(saction);
	}
	else {
		std::vector<DfxShortcutAction*> g;
		g.push_back(saction);
		std::pair<std::string, std::vector<DfxShortcutAction*>> p(group, g);
		groupMap.insert(p);
	}
}

void DfxShortcutManager::updateAction(std::string name, QShortcut *shortcut) {

	auto it = shortcutMap.find(name);

	if (it != shortcutMap.end()) {
		it->second->clearShortcut();
		it->second->setShorcut(shortcut);
	}
	else {
		DfxGlobal::console::println(std::string("Failed to update shortcut action: ") + name);
	}
}

QWidget *DfxShortcutManager::getParent(std::string name) {

	auto it = shortcutMap.find(name);

	if (it != shortcutMap.end()) {
		return it->second->getParent();
	}
	else {
		DfxGlobal::console::println(std::string("Failed to get shortcut parent: ") + name);
		return nullptr;
	}
}

bool DfxShortcutManager::hasShortcutAction(std::string name) {

	auto it = shortcutMap.find(name);

	if (it != shortcutMap.end()) {
		return true;
	}
	else {
		DfxGlobal::console::println(std::string("Failed to get shortcut parent: ") + name);
		return false;
	}
}

std::vector<DfxShortcutAction*> DfxShortcutManager::getAllShortcutActions() {

	std::vector<DfxShortcutAction*> all;
	for (auto it : shortcutMap) {
		all.push_back(it.second);
	}

	return all;
}

std::vector<DfxShortcutAction*> DfxShortcutManager::getGroupShortcutActions(std::string group) {
	std::vector<DfxShortcutAction*> actions;

	auto it = groupMap.find(group);

	if (it != groupMap.end()) {
		return it->second;
	}

	return actions;
}

std::vector<std::string> DfxShortcutManager::getAllGroupNames() {
	std::vector<std::string> names;

	for (auto nm : groupMap) {
		names.push_back(nm.first);
	}

	return names;
}