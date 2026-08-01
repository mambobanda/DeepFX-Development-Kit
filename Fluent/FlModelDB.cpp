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

#include "FlModelDB.h"

namespace fl {

	ModelDB *ModelDB::_instance = nullptr;

	ModelDB::ModelDB() {

	}

	ModelDB::~ModelDB() {

	}

	ModelDB *ModelDB::getInstance() {
		if (_instance == nullptr) {
			_instance = new ModelDB;
		}

		return _instance;
	}

	void ModelDB::addModel(std::string key, fl::Model *mdl) {
		if (mdl != nullptr) _map[key] = mdl;
	}

	void ModelDB::removeModel(std::string key) {
		_map.erase(key);
	}

	Any *ModelDB::fetchModel(std::string key, fl::Space *space) {
		auto itr = _map.find(key);

		if (itr != _map.end()) {
			return itr->second->createModel(space);
		}

		return nullptr;
	}
}