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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <vector>
#include <DfxGlobal.h>
#include "DfxHistoryAction.h"

class DfxHistoryManager {
private:
	static DfxHistoryManager *_instance;

	std::vector<DfxHistoryAction*> _undoStack;
	std::vector<DfxHistoryAction*> _redoStack;

	unsigned int _limit;

	DfxHistoryManager();
public:

	virtual ~DfxHistoryManager();

	static DfxHistoryManager *getInstance();

	void pushHistoryAction(DfxHistoryAction *act);

	void undo();

	void redo();

	unsigned int limit();

	void setLimit(int lmt);

	void clearUndoHistory();

	void clearRedoHistory();
};