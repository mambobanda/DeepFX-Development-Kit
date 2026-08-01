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

#include "DfxVDBFiltersNodes.h"

namespace DfxNodes {

	// Dilate Node
	DfxVDBDilateNode::DfxVDBDilateNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("VDB Dilate");

		((DfxTraitUID<size_t>*)getProperty(outRef))->setRootID(getInstanceID());

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inOffset, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inIterations, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);

		*(float*)getProperty(inOffset) = -0.02f;
		*(float*)getProperty(inIterations) = 1.f;
	}

	DfxVDBDilateNode::~DfxVDBDilateNode() {

	}

	void DfxVDBDilateNode::compute() {
		using namespace DfxEntityPropertyTypes;

		dfxutil::traitUIDLinkRelated(this, outRef);

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void DfxVDBDilateNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void DfxVDBDilateNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxVDBDilateNode::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxVDBDilateNode::isExportable() {
		return true;
	}

	void *DfxVDBDilateNode::creator(DfxGlobalEntityName uniquename) {
		return new DfxVDBDilateNode(uniquename);
	}

	void DfxVDBDilateNode::initialize() {

		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inOffset, "in_offset", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inIterations, "in_iterations", new DfxEntityPropertyTypes::FloatPropertyType());
	}

	// Erode Node
	DfxVDBErodeNode::DfxVDBErodeNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("VDB Erode");

		((DfxTraitUID<size_t>*)getProperty(outRef))->setRootID(getInstanceID());

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inOffset, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inIterations, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);

		*(float*)getProperty(inOffset) = 0.015f;
		*(float*)getProperty(inIterations) = 1.f;
	}

	DfxVDBErodeNode::~DfxVDBErodeNode() {

	}

	void DfxVDBErodeNode::compute() {
		using namespace DfxEntityPropertyTypes;

		dfxutil::traitUIDLinkRelated(this, outRef);

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void DfxVDBErodeNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void DfxVDBErodeNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxVDBErodeNode::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxVDBErodeNode::isExportable() {
		return true;
	}

	void *DfxVDBErodeNode::creator(DfxGlobalEntityName uniquename) {
		return new DfxVDBErodeNode(uniquename);
	}

	void DfxVDBErodeNode::initialize() {

		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inOffset, "in_offset", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inIterations, "in_iterations", new DfxEntityPropertyTypes::FloatPropertyType());
	}

	// Median Node
	DfxVDBMedianNode::DfxVDBMedianNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("VDB Median");

		((DfxTraitUID<size_t>*)getProperty(outRef))->setRootID(getInstanceID());

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inWidth, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inIterations, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);

		*(float*)getProperty(inWidth) = 1.f;
		*(float*)getProperty(inIterations) = 1.f;
	}

	DfxVDBMedianNode::~DfxVDBMedianNode() {

	}

	void DfxVDBMedianNode::compute() {
		using namespace DfxEntityPropertyTypes;

		dfxutil::traitUIDLinkRelated(this, outRef);

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void DfxVDBMedianNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void DfxVDBMedianNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxVDBMedianNode::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxVDBMedianNode::isExportable() {
		return true;
	}

	void *DfxVDBMedianNode::creator(DfxGlobalEntityName uniquename) {
		return new DfxVDBMedianNode(uniquename);
	}

	void DfxVDBMedianNode::initialize() {

		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inWidth, "in_width", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inIterations, "in_iterations", new DfxEntityPropertyTypes::FloatPropertyType());
	}

	// Mean Node
	DfxVDBMeanNode::DfxVDBMeanNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("VDB Mean");

		((DfxTraitUID<size_t>*)getProperty(outRef))->setRootID(getInstanceID());

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inWidth, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inIterations, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);

		*(float*)getProperty(inWidth) = 1.f;
		*(float*)getProperty(inIterations) = 1.f;
	}

	DfxVDBMeanNode::~DfxVDBMeanNode() {

	}

	void DfxVDBMeanNode::compute() {
		using namespace DfxEntityPropertyTypes;

		dfxutil::traitUIDLinkRelated(this, outRef);

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void DfxVDBMeanNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void DfxVDBMeanNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxVDBMeanNode::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxVDBMeanNode::isExportable() {
		return true;
	}

	void *DfxVDBMeanNode::creator(DfxGlobalEntityName uniquename) {
		return new DfxVDBMeanNode(uniquename);
	}

	void DfxVDBMeanNode::initialize() {

		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inWidth, "in_width", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inIterations, "in_iterations", new DfxEntityPropertyTypes::FloatPropertyType());
	}

	// Gaussian Node
	DfxVDBGaussianNode::DfxVDBGaussianNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("VDB Gaussian");

		((DfxTraitUID<size_t>*)getProperty(outRef))->setRootID(getInstanceID());

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inWidth, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inIterations, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);

		*(float*)getProperty(inWidth) = 1.f;
		*(float*)getProperty(inIterations) = 1.f;
	}

	DfxVDBGaussianNode::~DfxVDBGaussianNode() {

	}

	void DfxVDBGaussianNode::compute() {
		using namespace DfxEntityPropertyTypes;

		dfxutil::traitUIDLinkRelated(this, outRef);

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void DfxVDBGaussianNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void DfxVDBGaussianNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxVDBGaussianNode::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxVDBGaussianNode::isExportable() {
		return true;
	}

	void *DfxVDBGaussianNode::creator(DfxGlobalEntityName uniquename) {
		return new DfxVDBGaussianNode(uniquename);
	}

	void DfxVDBGaussianNode::initialize() {

		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inWidth, "in_width", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inIterations, "in_iterations", new DfxEntityPropertyTypes::FloatPropertyType());
	}
}