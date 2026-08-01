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

#include "DfxEnvelopeMap.h"

DfxEnvelopeMap *DfxEnvelopeMap::instance = nullptr;

DfxEnvelopeMap::DfxEnvelopeMap() {

}

DfxEnvelopeMap *DfxEnvelopeMap::getInstance() {
	if (instance == nullptr) {
		instance = new DfxEnvelopeMap();
	}

	return instance;
}

void DfxEnvelopeMap::addEnvelope(size_t globalId, DfxEnvelope *env) {

	auto ref = envMap.find(globalId);

	if (ref == envMap.end()) {
		std::vector<DfxEnvelope*> envList;
		envList.push_back(env);

		std::pair<size_t, std::vector<DfxEnvelope*>> p(globalId, envList);
		envMap.insert(p);
	}
	else {
		ref->second.push_back(env);
	}
}

bool DfxEnvelopeMap::hasEnvelope(size_t iID, size_t propID) {
	auto ref = envMap.find(iID);

	if (ref == envMap.end()) {
		return false;
	}
	else {
		for (auto env : ref->second) {
			if (env->getEntityPropID() == propID) return true;
		}
	}

	return false;
}

std::vector<DfxEnvelope*> DfxEnvelopeMap::findEnvelopes(size_t instanceid) {

	auto ref = envMap.find(instanceid);

	if (ref != envMap.end()) {
		return ref->second;
	}

	return std::vector<DfxEnvelope*>();
}

DfxEnvelope* DfxEnvelopeMap::findEnvelope(size_t instanceid, size_t propertyid) {

	auto list = findEnvelopes(instanceid);

	for (auto itm : list) {

		if (itm->getEntityPropID() == propertyid) return itm;
	}

	return nullptr;
}

void DfxEnvelopeMap::removeEnvelope(size_t globalId, size_t propertyID) {

	auto ref = envMap.find(globalId);

	if (ref != envMap.end()) {
		std::vector<size_t> indicies;

		for (size_t i = 0; i < ref->second.size(); i++) {

			if (ref->second[0]->getEntityPropID() == propertyID) indicies.push_back(i);
		}

		for (auto inx : indicies) {
			delete ref->second[inx];
			ref->second.erase(ref->second.begin() + inx);
		}
	}
}

void DfxEnvelopeMap::removeAllEnvelopes(size_t globalId) {

	auto ref = envMap.find(globalId);

	if (ref != envMap.end()) {

		for (size_t i = 0; i < ref->second.size(); i++) {
			delete ref->second[i];
		}

		ref->second.clear();
	}
}