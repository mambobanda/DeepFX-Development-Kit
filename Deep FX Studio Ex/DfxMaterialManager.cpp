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

#include "DfxMaterialManager.h"
#include "DfxMaterialLinker.h"

DfxMaterialManager *DfxMaterialManager::_instance = nullptr;

DfxMaterialManager::DfxMaterialManager() {
	_materialModel = new DfxQMaterialListModel();
}

DfxMaterialManager::~DfxMaterialManager() {

}

DfxMaterialManager *DfxMaterialManager::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxMaterialManager;
	}

	return _instance;
}

void DfxMaterialManager::newMaterial(DfxMaterialInitializer init) {
	auto space = DfxSpace::getInstance();

	auto any = space->findAbsolute("~/MATS");

	if (any != nullptr) {
		int tail = any->data<fl::DataObject>().properties().size();
		((DfxQMaterialListModel*)_materialModel)->beginAppendRowOp(tail);
		init();
		((DfxQMaterialListModel*)_materialModel)->endAppendRowOp();
	}
}

void DfxMaterialManager::removeMaterial(std::string path) {
	auto material = DfxSpace::getInstance()->findAbsolute(path);

	if (material != nullptr) {
		if (material->type() == fl::type::DATA_OBJECT) {
			auto matObj = material->rawData<fl::DataObject>();
			auto lnks = DfxSpace::getInstance()->findLinks(matObj->property("material_output")->absolutePath());

			for (auto lk : lnks) DfxSpace::getInstance()->removeBinding(lk.output_path, lk.input_path);
			
			DfxMaterialLinker::getInstance()->removeAllMaterialLinks(path);

			DfxSpace::getInstance()->remove(material);
		}
	}
}

std::unordered_map<int, std::string> DfxMaterialManager::materialIDMap() {
	std::unordered_map<int, std::string> _matIDMap;
	auto space = DfxSpace::getInstance();

	auto any = space->findAbsolute("~/MATS");

	if ( any != nullptr ) {
		auto dao = any->rawData<fl::DataObject>();

		for (auto prop : dao->properties()) {
			auto pdao = prop->rawData<fl::DataObject>();

			auto mid = pdao->property("material_id")->data<int>();
			_matIDMap[mid] = prop->absolutePath();
		}
	}

	return _matIDMap;
}

QAbstractItemModel *DfxMaterialManager::materialModel() {
	return _materialModel;
}

void DfxMaterialManager::addMaterialInstance(std::string name, DfxMaterialMapInstancer instance) {
	materialMap[name] = instance;
}

void DfxMaterialManager::removeMaterialInstance(std::string name) {
	materialMap.erase(name);
}

fl::Any *DfxMaterialManager::createMaterialInstance(std::string name) {
	auto space = DfxSpace::getInstance();
	
	auto itr = materialMap.find(name);

	if (itr != materialMap.end()) {
		return itr->second(space);
	}

	return nullptr;
}