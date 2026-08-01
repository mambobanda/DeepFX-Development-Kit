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

#include "DfxPropertyConduit.h"
#include "DfxSpace.h"

DfxPropertyConduit *DfxPropertyConduit::_conduitInstance = nullptr;

DfxPropertyConduit::DfxPropertyConduit() {

}

DfxPropertyConduit::~DfxPropertyConduit() {

}

DfxPropertyConduit *DfxPropertyConduit::getInstance() {
	if (_conduitInstance == nullptr) {
		_conduitInstance = new DfxPropertyConduit;
	}

	return _conduitInstance;
}

void DfxPropertyConduit::createKeyframeFloatOverride(std::string path, float originalValue) {
	if (!hasKeyframeOverride(path)) {
		auto any = DfxSpace::getInstance()->findAbsolute(path);

		if (any != nullptr) {
			if (any->type() == fl::type::FLOAT) {
				_conduitMap[path] = DfxPropertyConduitType::KEY_FRAME;
				DfxKeyFrameManager::getInstance()->addFloatMap(path, originalValue);
			}

			emit keyFrameUpdated(path);
		}
	}
}

void DfxPropertyConduit::destroyframeFloatOverride(std::string path) {
	if (hasKeyframeOverride(path)) {
		_conduitMap.erase(path);
		DfxKeyFrameManager::getInstance()->removeFloatMap(path);
		emit keyFrameUpdated(path);
	}
}

size_t DfxPropertyConduit::floatKeyframeOverrideSize(std::string path) {
	if (hasKeyframeOverride(path)) {
		return DfxKeyFrameManager::getInstance()->floatMapSize(path);
	}
	return 0;
}

void DfxPropertyConduit::removeKeyframeFloatOverride(std::string path, int key) {
	if (hasKeyframeOverride(path)) {
		DfxKeyFrameManager::getInstance()->removeFloatMapValue(path, key);
		emit keyFrameUpdated(path);
	}
}

void DfxPropertyConduit::updateKeyframeFloatOverride(std::string path, int key, float value) {
	if (hasKeyframeOverride(path)) {
		DfxKeyFrameManager::getInstance()->setFloatMapValue(path, key, value);
		emit keyFrameUpdated(path);
	}
}

void DfxPropertyConduit::updateKeyframeFloatInterpolationType(std::string path, int key, unsigned int type) {
	if (hasKeyframeOverride(path)) {
		auto fmap = DfxKeyFrameManager::getInstance()->getFloatMap(path);
		fmap->changeInterpolationType(key, type);
		emit keyFrameUpdated(path);
	}
}

void DfxPropertyConduit::updateKeyframeFloatControlPoints(std::string path, int key, std::pair<glm::vec2, glm::vec2> points) {
	if (hasKeyframeOverride(path)) {
		auto fmap = DfxKeyFrameManager::getInstance()->getFloatMap(path);
		fmap->updateControlPoints(key, points);
		emit keyFrameUpdated(path);
	}
}

float DfxPropertyConduit::evaluateKeyframeFloatOverride(std::string path, int key) {
	if (hasKeyframeOverride(path)) {
		return DfxKeyFrameManager::getInstance()->getFloatMapValue(path, key);
	}

	return 0.f;
}

bool DfxPropertyConduit::hasKeyframeOverride(std::string path){
	auto itr = _conduitMap.find(path);

	if (itr != _conduitMap.end()) return true;
	else return false;
}

//int DfxPropertyConduit::keyframeOverrideType(std::string path) {
//	if (hasKeyframeOverride(path)) {
//		return _conduitMap.find(path)->second;
//	}
//
//	return -1;
//}

fl::AnyMap<int, float> *DfxPropertyConduit::findFloatKeyFrameMap(std::string path) {
	return DfxKeyFrameManager::getInstance()->getFloatMap(path);
}

void DfxPropertyConduit::announceNewKeyframeCreated() {
	emit newKeyFrameCreated();
}

void DfxPropertyConduit::announceKeyFrameUpdated(std::string path) {
	emit keyFrameUpdated(path);
}


void DfxPropertyConduit::applyKeyframeFloatOverride(std::string path, int key) {
	auto itr = _conduitMap.find(path);
	auto any = DfxSpace::getInstance()->findAbsolute(path);

	if (itr != _conduitMap.end() && any != nullptr) {
		if (any->type() == fl::type::FLOAT) {
			float f_value = DfxKeyFrameManager::getInstance()->getFloatMapValue(path, key);
			any->setData<float>(f_value);
			any->notifyObservers();
		}
	}

}

std::vector<std::string> DfxPropertyConduit::allConduitPaths() {
	std::vector<std::string> paths;

	for (auto p: _conduitMap) {
		paths.push_back(p.first);
	}

	return paths;
}

void DfxPropertyConduit::clearKeyframeFloatOverrideCallback(fl::Any *any) {
	auto path = any->absolutePath();

	if (DfxPropertyConduit::getInstance()->hasKeyframeOverride(path)) {
		DfxPropertyConduit::getInstance()->destroyframeFloatOverride(path);
	}
}
