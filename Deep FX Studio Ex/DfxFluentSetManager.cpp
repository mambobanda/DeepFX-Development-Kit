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

#include "DfxFluentSetManager.h"

size_t DfxFluentSetManager::SOFT_SET_ID_GEN = 0xFAFA;

DfxFluentSetManager *DfxFluentSetManager::_instance = nullptr;

DfxFluentSetManager::DfxFluentSetManager() {
	_pointsDeadSet  = new DfxFluentSet(DfxFluentSet::STREAM);
	_simplexDeadSet = new DfxFluentSet(DfxFluentSet::SIMPLEX);

	_root = new DfxFluentSet(DfxFluentSet::ROOT);
}

DfxFluentSetManager::~DfxFluentSetManager() {

}

DfxFluentSetManager *DfxFluentSetManager::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxFluentSetManager;
	}

	return _instance;
}

bool DfxFluentSetManager::hasSet(std::string name) {
	if (isSoftSetId(name)) {
		QString str(name.c_str());
		auto itr = _softSetMap.find(str.toLower().toStdString());

		if (itr != _softSetMap.end()) return true;
	}
	else {
		QString str(name.c_str());
		auto itr = _setMap.find(str.toLower().toStdString());

		if (itr != _setMap.end()) return true;
	}

	return false;
}

void DfxFluentSetManager::addSet(std::string name, DfxFluentSet *flt) {
	QString str(name.c_str());
	flt->setName(name);
	_setMap[str.toLower().toStdString()] = flt;
	_root->addChild(flt);
}

void DfxFluentSetManager::removeSet(std::string name) {

	if (isSoftSetId(name)) {
		QString str(name.c_str());
		auto itr = _softSetMap.find(str.toLower().toStdString());

		if (itr != _softSetMap.end()) {
			itr->second->unparent();
			_softSetMap.erase(itr);
		}
	}
	else {
		QString str(name.c_str());
		auto itr = _setMap.find(str.toLower().toStdString());

		if (itr != _setMap.end()) {
			itr->second->unparent();
			_setMap.erase(itr);
		}
	}
}

DfxFluentSet *DfxFluentSetManager::getSet(std::string name) {
	if (isSoftSetId(name)) {
		QString str(name.c_str());
		auto itr = _softSetMap.find(str.toLower().toStdString());

		if (itr != _softSetMap.end()) return itr->second;
	}
	else {
		QString str(name.c_str());
		auto itr = _setMap.find(str.toLower().toStdString());

		if (itr != _setMap.end()) return itr->second;
	}

	return nullptr;
}

std::pair<std::string, DfxFluentSet*> DfxFluentSetManager::createSoftSet(uint8_t type) {
	std::string name = "~://" + std::to_string(++SOFT_SET_ID_GEN);
	auto set = new DfxFluentSet(type);
	set->forceSoftSet();
	
	QString str(name.c_str());
	_softSetMap[str.toLower().toStdString()] = set;

	return std::pair<std::string, DfxFluentSet*>(name, set);
}

DfxFluentSet *DfxFluentSetManager::pointsDeadSet() {
	return _pointsDeadSet;
}

DfxFluentSet *DfxFluentSetManager::simplexDeadSet() {
	return _simplexDeadSet;
}

std::unordered_map<std::string, DfxFluentSet*> &DfxFluentSetManager::setMap() {
	return _setMap;
}

void DfxFluentSetManager::clear() {
	for (auto p: _setMap) {
		p.second->unparent();
		delete p.second;
	}
	_setMap.clear();

	for (auto p : _softSetMap) {
		p.second->unparent();
		delete p.second;
	}
	_softSetMap.clear();
}

bool DfxFluentSetManager::isSoftSetId(std::string str) {
	if (std::regex_match(str, std::regex("(~://)(.*)"))) return true;
	else return false;
}

void DfxFluentSetManager::clearSoftSets() {
	for (auto st : _softSetMap) {
		delete st.second;
	}

	_softSetMap.clear();
}