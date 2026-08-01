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

#include "SgAssetManager.h"

namespace deepsg {

	SgAssetManager *SgAssetManager::_instance = nullptr;

	SgAssetManager::SgAssetManager() {
		_assetSpace = new fl::Space();
	}

	SgAssetManager::~SgAssetManager() {
		delete _assetSpace;
	}

	SgAssetManager *SgAssetManager::getInstance() {
		if (_instance == nullptr) {
			newInstance();
		}

		return _instance;
	}

	void SgAssetManager::newInstance() {

		if (_instance != nullptr) {
			_instance->clear();
			delete _instance;
			_instance = nullptr;
		}

		_instance = new SgAssetManager;
	}

	void SgAssetManager::clear() {
		for (auto prop : _assetSpace->root()->rawData<fl::DataObject>()->properties()) {
			_assetSpace->remove(prop);
		}
	}

	SgAssetHandler SgAssetManager::newAsset(std::string name, unsigned int type) {

		auto asset = fl::DataObject::Builder()
			.setName(name)
			.setLabel("")
			.build(_assetSpace);

		asset->rawData<fl::DataObject>()->value()->newData<SgAsset>(ASSET_DESCRIPTION);
		asset->rawData<fl::DataObject>()->value()->rawData<SgAsset>()->setKind(type);

		return SgAssetHandler(_assetSpace, asset);
	}

	std::vector<SgAssetHandler> SgAssetManager::assets() {
		std::vector<SgAssetHandler> output;

		for (auto any : _assetSpace->root()->rawData<fl::DataObject>()->properties()) {

			if (any->type() == fl::type::DATA_OBJECT) {
				if (any->rawData<fl::DataObject>()->value()->type() == ASSET_DESCRIPTION) {
					output.push_back(SgAssetHandler(_assetSpace, any));
				}
			}
		}

		return output;
	}
}