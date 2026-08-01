/*
* The Mailo Today Solutions Software License
*
* Copyright 2018 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#include "DfxVDBPointMesher.h"

DfxPropertyID DfxVDBPointMesher::outID;
DfxPropertyID DfxVDBPointMesher::inRef;
DfxPropertyID DfxVDBPointMesher::inGridSpacing;
DfxPropertyID DfxVDBPointMesher::inPointRadius;
DfxPropertyID DfxVDBPointMesher::inNarrowband;
DfxPropertyID DfxVDBPointMesher::inPoints;

DfxVDBPointMesher::DfxVDBPointMesher(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {

	DfxEntityPropertyTypes::UID *uid01 = (DfxEntityPropertyTypes::UID*)getProperty(outID);
	uid01->setRootID(getInstanceID());

	setPropertyState(outID, DfxEntityPropertyState::OUTPUT_STATE);
	setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inGridSpacing, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inPointRadius, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inNarrowband, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inPoints, DfxEntityPropertyState::INPUT_STATE);
}

DfxVDBPointMesher::~DfxVDBPointMesher() {

}

void DfxVDBPointMesher::compute() {

	OpenvdbParticleSkinner skinner;
	Dfx3dMeshData *mesh = new Dfx3dMeshData;
	std::vector<vdb::Filter*> filters;

	glm::vec3 particles[1000];

	skinner.skinParticles(particles, 1000, mesh, filters);

	cleanAllProperties();
	getDataInstance()->makeClean();
	makeChildrenDirty();
}

void DfxVDBPointMesher::onDataChanged(DfxEntityDataInstance *instance) {

}

void DfxVDBPointMesher::importData(std::vector<std::pair<std::string, std::string>> data) {

}

std::vector<std::pair<std::string, std::string>> DfxVDBPointMesher::exportData() {
	return mapEntityProperties(this);
}

bool DfxVDBPointMesher::isExportable() {
	return true;
}

void *DfxVDBPointMesher::creator() {
	return new DfxVDBPointMesher(DfxVDBPointMesher_Identifier);
}

void DfxVDBPointMesher::initialize() {
	outID = DfxEntityDB::createProperty("@ref", new DfxEntityPropertyTypes::UIDPropertyType());
	inRef = DfxEntityDB::createProperty("in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
	inGridSpacing = DfxEntityDB::createProperty("grid_spacing", new DfxEntityPropertyTypes::FloatPropertyType());
	inPointRadius = DfxEntityDB::createProperty("point_radius", new DfxEntityPropertyTypes::FloatPropertyType());
	inNarrowband  = DfxEntityDB::createProperty("narrow_band", new DfxEntityPropertyTypes::FloatPropertyType());
	inPoints = DfxEntityDB::createProperty("points", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
}