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

#include "SgAssetState.h"
#include "SgAssociationHandler.h"

std::stack<deepsg::SgAssetHandler> assetHandlerStack;
std::stack<deepsg::SgAssociationHandler> associationHandlerStack;
std::stack<deepsg::SgVariantsHandler> variantsHandlerStack;
std::stack<deepsg::SgAssetGroupHandler> groupHandlerStack;

namespace deepsg {

	SgStateIO::BlockReader SgAssetState::root;
	SgStateIO::BlockReader SgAssetState::asset;
	SgStateIO::BlockReader SgAssetState::association;
	SgStateIO::BlockReader SgAssetState::variants;
	SgStateIO::BlockReader SgAssetState::group;

	SgAssetState::SgAssetState() {

	}

	SgAssetState::~SgAssetState() {

	}

	void SgAssetState::saveAsset(SgAssetHandler &handler) {
		beginSaveBlock("asset");
		
		saveString("name", handler.name());
		saveString("context", SgAsset::enumToString(handler.context()));
		saveString("kind", SgAsset::enumToString(handler.kind()));
		saveString("path", handler.path());

		for (auto &var : handler.assets()) {
			saveAsset(var);
		}

		for (auto &var : handler.associations()) {
			saveAssociation(var);
		}

		for (auto &var : handler.variants()) {
			saveVariant(var);
		}

		for (auto &var : handler.groups()) {
			saveGroup(var);
		}

		endSaveBlock();
	}

	void SgAssetState::saveAssociation(SgAssociationHandler &handler) {
		beginSaveBlock("association");

		saveString("name", handler.name());
		saveString("kind", SgAssociation::enumToString(handler.kind()));
		saveString("surface", handler.surface());
		saveString("path", handler.path());

		endSaveBlock();
	}

	void SgAssetState::saveVariant(SgVariantsHandler &handler) {
		beginSaveBlock("variant");

		saveString("name", handler.name());
		saveString("kind", SgVariants::enumToString(handler.kind()));

		for (auto &var : handler.associations()) {
			saveAssociation(var);
		}

		endSaveBlock();
	}

	void SgAssetState::saveGroup(SgAssetGroupHandler &handler) {
		beginSaveBlock("group");

		saveString("name", handler.name());

		for (auto &var : handler.associations()) {
			saveAssociation(var);
		}

		endSaveBlock();
	}

	void SgAssetState::save(std::string path) {
		beginSave();

		for (auto handler : SgAssetManager::getInstance()->assets()) {
			saveAsset(handler);
		}

		endSave();
	}

	void SgAssetState::load(std::string file) {

		loadFileToDOM(file);

		if (isDocumentValid()) {
			root.addBlock("asset", assetLoadObject);

			// Asset Block
			asset.addBlock("context", assetLoadContext);
			asset.addBlock("name", assetLoadName);
			asset.addBlock("kind", assetLoadKind);
			asset.addBlock("path", assetLoadPath);
			asset.addBlock("asset", assetLoadAsset);
			asset.addBlock("association", assetLoadAssociation);
			asset.addBlock("variant", assetLoadVariants);
			asset.addBlock("group", assetLoadGroup);

			// Association Block
			association.addBlock("name", associationLoadName);
			association.addBlock("kind", associationLoadKind);
			association.addBlock("surface", associationLoadSurface);
			association.addBlock("path", associationLoadPath);

			// Variants Block
			variants.addBlock("name", variantsLoadName);
			variants.addBlock("kind", variantsLoadKind);
			variants.addBlock("association", variantsLoadAssociation);

			// Variants Block
			group.addBlock("name", groupLoadName);
			group.addBlock("association", groupLoadAssociation);

			root.readObject(rootObject());
		}
	}

	// Asset Load Callbacks
	void SgAssetState::assetLoadObject(rapidjson::Value::Member &m) {
		auto tempHandler = SgAssetManager::getInstance()->newAsset("__temp__", 0);
		assetHandlerStack.push(tempHandler);

		if (m.value.IsObject()) {
			asset.readObject(m.value.GetObject());
		}

		assetHandlerStack.pop();
	}

	void SgAssetState::assetLoadName(rapidjson::Value::Member &m) {
		assetHandlerStack.top().rename(loadString(m));
	}

	void SgAssetState::assetLoadContext(rapidjson::Value::Member &m) {
		assetHandlerStack.top().setContext(SgAsset::stringToEnum(loadString(m)));
	}

	void SgAssetState::assetLoadKind(rapidjson::Value::Member &m) {
		assetHandlerStack.top().setKind(SgAsset::stringToEnum(loadString(m)));
	}

	void SgAssetState::assetLoadPath(rapidjson::Value::Member &m) {
		assetHandlerStack.top().setPath(loadString(m));
	}

	void SgAssetState::assetLoadAsset(rapidjson::Value::Member &m) {
		auto tempHandler = assetHandlerStack.top().newAsset("__temp__", 0);
		assetHandlerStack.push(tempHandler);

		if (m.value.IsObject()) {
			asset.readObject(m.value.GetObject());
		}

		assetHandlerStack.pop();

	}

	void SgAssetState::assetLoadAssociation(rapidjson::Value::Member &m) {
		SgAssociationHandler tempHandler = assetHandlerStack.top().addAssociation("__temp__", SgAssociation());
		
		associationHandlerStack.push(tempHandler);
	
		if (m.value.IsObject()) {
			association.readObject(m.value.GetObject());
		}

		associationHandlerStack.pop();
	}

	void SgAssetState::assetLoadVariants(rapidjson::Value::Member &m) {
		auto tempHandler = assetHandlerStack.top().newVariant("__temp__", 0);
		variantsHandlerStack.push(tempHandler);

		if (m.value.IsObject()) {
			variants.readObject(m.value.GetObject());
		}

		variantsHandlerStack.pop();
	
	}

	void SgAssetState::assetLoadGroup(rapidjson::Value::Member &m) {
		auto tempHandler = assetHandlerStack.top().newGroup("__temp__");
		groupHandlerStack.push(tempHandler);

		if (m.value.IsObject()) {
			group.readObject(m.value.GetObject());
		}

		groupHandlerStack.pop();
	}

	// Association Load Callbacks
	void SgAssetState::associationLoadName(rapidjson::Value::Member &m) {
		associationHandlerStack.top().rename(loadString(m));
	}

	void SgAssetState::associationLoadKind(rapidjson::Value::Member &m) {
		associationHandlerStack.top().setKind(SgAssociation::stringToEnum(loadString(m)));
	}

	void SgAssetState::associationLoadSurface(rapidjson::Value::Member &m) {
		associationHandlerStack.top().setSurface(loadString(m));
	}

	void SgAssetState::associationLoadPath(rapidjson::Value::Member &m) {
		associationHandlerStack.top().setPath(loadString(m));
	}

	// Variant Block Callbacks
	void SgAssetState::variantsLoadName(rapidjson::Value::Member &m) {
		variantsHandlerStack.top().rename(loadString(m));
	}

	void SgAssetState::variantsLoadKind(rapidjson::Value::Member &m) {
		variantsHandlerStack.top().setKind(SgAssociation::stringToEnum(loadString(m)));
	}

	void SgAssetState::variantsLoadAssociation(rapidjson::Value::Member &m) {
		SgAssociationHandler tempHandler = variantsHandlerStack.top().addAssociation("__temp__", SgAssociation());

		associationHandlerStack.push(tempHandler);

		if (m.value.IsObject()) {
			association.readObject(m.value.GetObject());
		}

		associationHandlerStack.pop();
	}

	// Variant Block Callbacks
	void SgAssetState::groupLoadName(rapidjson::Value::Member &m) {
		groupHandlerStack.top().rename(loadString(m));
	}

	void SgAssetState::groupLoadAssociation(rapidjson::Value::Member &m) {
		SgAssociationHandler tempHandler = groupHandlerStack.top().addAssociation("__temp__", SgAssociation());

		associationHandlerStack.push(tempHandler);

		if (m.value.IsObject()) {
			association.readObject(m.value.GetObject());
		}

		associationHandlerStack.pop();
	}

}