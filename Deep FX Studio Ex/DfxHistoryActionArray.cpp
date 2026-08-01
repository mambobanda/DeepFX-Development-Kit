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

#include "DfxHistoryActionArray.h"

DfxHistoryActionArray::DfxHistoryActionArray() {

}

DfxHistoryActionArray::~DfxHistoryActionArray() {
	for (auto act : _actions) delete act;
}

bool DfxHistoryActionArray::execute() {

	for (auto act : _actions) {
		if (!act->execute()) {
			return false;
		}
	}

	return true;
}

DfxHistoryAction *DfxHistoryActionArray::inverse() {
	auto hactionArray = new DfxHistoryActionArray;
	for (auto act : _actions) hactionArray->addAction(act->inverse());

	return hactionArray;
}

void DfxHistoryActionArray::addAction(DfxHistoryAction *act) {
	_actions.push_back(act);
}