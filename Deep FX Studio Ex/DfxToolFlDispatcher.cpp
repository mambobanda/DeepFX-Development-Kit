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

#include "DfxToolFlDispatcher.h"

DfxToolFlDispatcher::DfxToolFlDispatcher(deepsg::SgToolNode *tool) {
	_toolNode = tool;
}

DfxToolFlDispatcher::~DfxToolFlDispatcher() {

}

void DfxToolFlDispatcher::valueChanged(fl::Any *any) {
	if (_toolNode->evaluateOnValueChanged()) {
		if (_toolNode->parent() != nullptr) {
			_toolNode->parent()->evaluateTools();
		}
	}
}
