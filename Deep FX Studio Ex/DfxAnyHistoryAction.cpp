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

#include "DfxAnyHistoryAction.h"


DfxAnyHistoryAction::DfxAnyHistoryAction() {
	_old = nullptr;
	_new = nullptr;
}

DfxAnyHistoryAction::~DfxAnyHistoryAction() {
	if (_old != nullptr) delete _old;
	if (_new != nullptr) delete _new;
}

bool DfxAnyHistoryAction::execute() {
	if (_old != nullptr && _new != nullptr) {
		auto refAny = DfxSpace::getInstance()->findAbsolute(_referencePath);

		if (refAny != nullptr) {
			if (refAny->type() == _old->type()) {

				if (refAny->type() == fl::type::FLOAT) {
					refAny->setData<float>(_old->data<float>());
					refAny->notifyObservers();
					return true;
				}
			}
		}
	}

	return false;
}

DfxHistoryAction *DfxAnyHistoryAction::inverse() {
	if (_new != nullptr && _old != nullptr) {
		auto haction = new DfxAnyHistoryAction();
		
		if (_new->type() == fl::type::FLOAT) {
			haction->setValue<float>(fl::type::FLOAT, _referencePath, _old->data<float>(), _new->data<float>());
			return haction;
		}
	}

	return nullptr;
}