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

#include "DfxQPropertiesHub.h"

DfxQPropertiesHub *DfxQPropertiesHub::_instance = nullptr;

DfxQPropertiesHub::DfxQPropertiesHub() {

}

DfxQPropertiesHub *DfxQPropertiesHub::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxQPropertiesHub;
	}

	return _instance;
}

void DfxQPropertiesHub::newSceneNodeSelected(std::vector<deepsg::SgNode*> nodes) {
	emit newSceneNodeSelected_signal(nodes);
}

void DfxQPropertiesHub::newDataObjectSelected(std::string path) {
	emit newDataObjectSelected_signal(path);
}
