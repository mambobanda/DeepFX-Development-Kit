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


#include "DfxShortcutAction.h"

DfxShortcutAction::DfxShortcutAction() {
	uniqueName = "None";
	parent = nullptr;
	shortcut = nullptr;
}

DfxShortcutAction::~DfxShortcutAction() {

}

std::string DfxShortcutAction::getName() {
	return uniqueName;
}

std::string DfxShortcutAction::getActionName() {
	return actionName;
}

void DfxShortcutAction::setActionName(std::string str) {
	actionName = str;
}

void DfxShortcutAction::setName(std::string name) {
	uniqueName = name;
}

QWidget* DfxShortcutAction::getParent() {
	return parent;
}
void DfxShortcutAction::setParent(QWidget *pt) {
	parent = pt;
}

QShortcut *DfxShortcutAction::getShorcut() {
	return shortcut;
}

void DfxShortcutAction::setShorcut(QShortcut *st) {
	shortcut = st;
}

void DfxShortcutAction::clearShortcut() {
	if (shortcut != nullptr) delete shortcut;
	shortcut = nullptr;
}

void DfxShortcutAction::updateShorcut(QKeySequence sq) {
	clearShortcut();
	QShortcut *sc = new QShortcut(sq, parent);

	QObject::connect(sc, &QShortcut::activated, function);

	shortcut = sc;
}

DfxShortcutActionFunction DfxShortcutAction::getFunction() {
	return function;
}

void DfxShortcutAction::setFunction(DfxShortcutActionFunction func) {
	function = func;
}

QKeySequence DfxShortcutAction::getDefualtSequence() {
	return defaultSequence;
}

void DfxShortcutAction::setDefaultSequence(QKeySequence seq) {
	defaultSequence = seq;
}