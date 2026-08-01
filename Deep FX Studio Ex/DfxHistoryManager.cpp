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

#include "DfxHistoryManager.h"

DfxHistoryManager *DfxHistoryManager::_instance = nullptr;

DfxHistoryManager::DfxHistoryManager() {
	_limit = 99;
}

DfxHistoryManager::~DfxHistoryManager() {

}

DfxHistoryManager *DfxHistoryManager::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxHistoryManager;
	}

	return _instance;
}

void DfxHistoryManager::pushHistoryAction(DfxHistoryAction *act) {
	if (_undoStack.size() == _limit) _undoStack.erase(_undoStack.begin());
	_undoStack.push_back(act);
	
	clearRedoHistory();
}

void DfxHistoryManager::undo() {
	if (!_undoStack.empty()) {
		auto back = _undoStack.back();
		_undoStack.pop_back();

		if (back->execute()) {
			_redoStack.push_back(back->inverse());
		}
		else {
			DfxGlobal::console::println("Undo Failed, clearing history");
			clearUndoHistory();
			clearRedoHistory();
		}

		delete back;

		DfxGlobal::command::exec("updateScene");
	}
}

void DfxHistoryManager::redo() {
	if (!_redoStack.empty()) {
		auto back = _redoStack.back();
		_redoStack.pop_back();

		if (back->execute()) {
			_undoStack.push_back(back->inverse());
		}
		else {
			DfxGlobal::console::println("Redo Failed, clearing history");
			clearUndoHistory();
			clearRedoHistory();
		}

		delete back;

		DfxGlobal::command::exec("updateScene");
	}
}

unsigned int DfxHistoryManager::limit() {
	return _limit;
}

void DfxHistoryManager::setLimit(int lmt) {
	_limit = lmt;
}

void DfxHistoryManager::clearUndoHistory() {
	for (auto act: _undoStack) {
		delete act;
	}

	_undoStack.clear();
}

void DfxHistoryManager::clearRedoHistory() {
	for (auto act : _redoStack) {
		delete act;
	}

	_redoStack.clear();
}