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

#include "DfxResourceManager.h"


DfxResourceManager *DfxResourceManager::_instance = nullptr;

DfxResourceManager::DfxResourceManager() {

	auto none = new DfxResource(resource::mode::NONE, resource::type::NONE, "");
	none->setName("None");
	
	_root = new DfxResource(resource::mode::PROJECT, resource::type::NONE, "");
	_root->setName("Project");
	_root->setParent(none);
	_root->setId(0x3e8);

	_geo = new DfxResource(resource::mode::FOLDER, resource::type::NONE, "");
	_geo->setName("Geometry");
	_geo->setParent(_root);
	_geo->setId(0);

	_img = new DfxResource(resource::mode::FOLDER, resource::type::NONE, "");
	_img->setName("Images");
	_img->setParent(_root);
	_img->setId(0);

	_root->addChild(_geo);
	_root->addChild(_img);

	none->addChild(_root);

	_model = new DfxQResourceModel();
}

DfxResourceManager::~DfxResourceManager() {

}

DfxResourceManager *DfxResourceManager::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxResourceManager();
	}

	return _instance;
}

DfxResource *DfxResourceManager::root() {
	return _root;
}

DfxResource *DfxResourceManager::geo() {
	return _geo;
}

DfxResource *DfxResourceManager::img() {
	return _img;
}

DfxQResourceModel *DfxResourceManager::model() {
	return _model;
}

void DfxResourceManager::resetId() {
	DfxResource::setBaseId(0x3e8);
}

void DfxResourceManager::maxId() {
	auto maxi = _root->maxId();
	DfxResource::setBaseId(++maxi);
}