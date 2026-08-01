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

#include "SgAssetHandler.h"
#include "SgVariantsHandler.h"
#include "SgAssetGroup.h"
#include "SgAssetGroupHandler.h"
#include "SgVariants.h"
#include "SgAsset.h"

namespace deepsg {

	SgAssetHandler::SgAssetHandler(fl::Space *_space) : SgDescriptionHandler(_space, nullptr) {

	}

	SgAssetHandler::SgAssetHandler(fl::Space *_space, fl::Any *_aPtr) : SgDescriptionHandler(_space, _aPtr) {

	}

	SgAssetHandler::~SgAssetHandler() {

	}

	void SgAssetHandler::setContext(unsigned int ctx) {
		_dataObject->rawData<fl::DataObject>()->value()->rawData<SgAsset>()->setContext(ctx);
	}

	unsigned int SgAssetHandler::context() {
		return _dataObject->rawData<fl::DataObject>()->value()->rawData<SgAsset>()->context();
	}

	void SgAssetHandler::setKind(unsigned int knd) {
		_dataObject->rawData<fl::DataObject>()->value()->rawData<SgAsset>()->setKind(knd);
	}

	unsigned int SgAssetHandler::kind() {
		return _dataObject->rawData<fl::DataObject>()->value()->rawData<SgAsset>()->kind();
	}

	std::string SgAssetHandler::path() {
		return _dataObject->rawData<fl::DataObject>()->value()->rawData<SgAsset>()->path();
	}

	void SgAssetHandler::setPath(std::string pth) {
		_dataObject->rawData<fl::DataObject>()->value()->rawData<SgAsset>()->setPath(pth);
	}

	SgAssetHandler SgAssetHandler::newAsset(std::string name, unsigned int type) {

		auto asset = fl::DataObject::Builder()
			.setName("__Temp__")
			.setLabel("")
			.build(_space);

		_space->move(asset, _dataObject);
		asset->setName(name);
		asset->rawData<fl::DataObject>()->value()->newData<SgAsset>(ASSET_DESCRIPTION);
		asset->rawData<fl::DataObject>()->value()->rawData<SgAsset>()->setKind(type);

		return SgAssetHandler(_space, asset);
	}

	SgVariantsHandler SgAssetHandler::newVariant(std::string name, unsigned int kind) {

		auto variant = fl::DataObject::Builder()
			.setName("__Temp__")
			.setLabel("")
			.build(_space);

		_space->move(variant, _dataObject);

		variant->setName(name);
		variant->rawData<fl::DataObject>()->value()->newData<SgVariants>(VARIANTS_DESCRIPTION);
		variant->rawData<fl::DataObject>()->value()->rawData<SgVariants>()->setKind(kind);

		return SgVariantsHandler(_space, variant);
	}

	SgAssetGroupHandler SgAssetHandler::newGroup(std::string name) {
		auto group = fl::DataObject::Builder()
			.setName("__Temp__")
			.setLabel("")
			.build(_space);

		_space->move(group, _dataObject);

		group->setName(name);
		group->rawData<fl::DataObject>()->value()->newData<SgAssetGroup>(GROUP_DESCRIPTION);

		return SgAssetGroupHandler(_space, group);
	}

	std::vector<SgAssetHandler> SgAssetHandler::assets() {
		std::vector<SgAssetHandler> output;

		for (auto ast : _dataObject->rawData<fl::DataObject>()->properties()) {

			if (ast->type() == fl::type::DATA_OBJECT) {
				if (ast->rawData<fl::DataObject>()->value()->type() == ASSET_DESCRIPTION) {
					output.push_back(SgAssetHandler(_space, ast));
				}
			}
		}

		return output;
	}

	std::vector<SgVariantsHandler> SgAssetHandler::variants() {
		std::vector<SgVariantsHandler> output;

		for (auto ast : _dataObject->rawData<fl::DataObject>()->properties()) {

			if (ast->type() == fl::type::DATA_OBJECT) {
				if (ast->rawData<fl::DataObject>()->value()->type() == VARIANTS_DESCRIPTION) {
					output.push_back(SgVariantsHandler(_space, ast));
				}
			}
		}

		return output;
	}

	std::vector<SgAssetGroupHandler> SgAssetHandler::groups() {
		std::vector<SgAssetGroupHandler> output;

		for (auto ast : _dataObject->rawData<fl::DataObject>()->properties()) {

			if (ast->type() == fl::type::DATA_OBJECT) {
				if (ast->rawData<fl::DataObject>()->value()->type() == GROUP_DESCRIPTION) {
					output.push_back(SgAssetGroupHandler(_space, ast));
				}
			}
		}

		return output;
	}
}