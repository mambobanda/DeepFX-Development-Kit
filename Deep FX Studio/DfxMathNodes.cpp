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

#include "DfxMathNodes.h"

namespace DfxNodes {

	// sRandomArray
	sRandomArrayNode::sRandomArrayNode(DfxGlobalEntityName uniquename): DfxEntity(uniquename) {
		setLogicalName("Random Scalar Array");

		setPropertyState(inSeed, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inMin, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inMax, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inSize, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outArray, DfxEntityPropertyState::OUTPUT_STATE);

		*(float*)getProperty(inSeed) = 47743.f;
		*(float*)getProperty(inMin) = 1.f;
		*(float*)getProperty(inMax) = 10.f;
		*(float*)getProperty(inSize) = 100.f;
	}

	sRandomArrayNode::~sRandomArrayNode() {

	}

	void sRandomArrayNode::compute() {
		using namespace DfxEntityPropertyTypes;

		FloatArray *output = (FloatArray*)getProperty(outArray);

		float seed = *(float*)getProperty(inSeed);
		float min = *(float*)getProperty(inMin);
		float max = *(float*)getProperty(inMax);
		int size = (int)*(float*)getProperty(inSize);

		std::default_random_engine genx(seed);
		std::uniform_real_distribution<> distPos(min, max);

		output->deleteArray();

		if (size > 0) {

			output->resize(size);
			for (size_t i = 0; i < size; i++) {
				float val = (float)distPos(genx);
				(*output)[i] = val;
			}
		}

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void sRandomArrayNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void sRandomArrayNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> sRandomArrayNode::exportData() {
		return mapEntityProperties(this);
	}

	bool sRandomArrayNode::isExportable() {
		return true;
	}

	void *sRandomArrayNode::creator(DfxGlobalEntityName uniquename) {
		return new sRandomArrayNode(uniquename);
	}

	void sRandomArrayNode::initialize() {
		DfxEntityDB::createProperty(inSeed, "seed", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inMin, "min_value", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inMax, "max_value", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inSize, "array_size", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(outArray, "values", new DfxEntityPropertyTypes::FloatArrayPropertyType());
	}

	// sIncrement
	sIncrementNode::sIncrementNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("Increment Scalar");

		setPropertyState(inValue, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outValue, DfxEntityPropertyState::OUTPUT_STATE);
	}

	sIncrementNode::~sIncrementNode() {

	}

	void sIncrementNode::compute() {
		using namespace DfxEntityPropertyTypes;

		float incrementedValue = *(float*)getProperty(inValue) + 1.f;
		*(float*)getProperty(outValue) = incrementedValue;

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void sIncrementNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void sIncrementNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> sIncrementNode::exportData() {
		return mapEntityProperties(this);
	}

	bool sIncrementNode::isExportable() {
		return true;
	}

	void *sIncrementNode::creator(DfxGlobalEntityName uniquename) {
		return new sIncrementNode(uniquename);
	}

	void sIncrementNode::initialize() {
		DfxEntityDB::createProperty(inValue, "in_value", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(outValue, "out_value", new DfxEntityPropertyTypes::FloatPropertyType());
	}

	// sPrint Scalar
	sPrintValueNode::sPrintValueNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("Print Scalar");

		setPropertyState(inValue, DfxEntityPropertyState::INPUT_STATE);
	}

	sPrintValueNode::~sPrintValueNode() {

	}

	void sPrintValueNode::compute() {
		using namespace DfxEntityPropertyTypes;

		float incrementedValue = *(float*)getProperty(inValue);
		DfxGlobal::console::println(std::string("Scalar Value: ") + std::to_string(incrementedValue));

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void sPrintValueNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void sPrintValueNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> sPrintValueNode::exportData() {
		return mapEntityProperties(this);
	}

	bool sPrintValueNode::isExportable() {
		return true;
	}

	void *sPrintValueNode::creator(DfxGlobalEntityName uniquename) {
		return new sPrintValueNode(uniquename);
	}

	void sPrintValueNode::initialize() {
		DfxEntityDB::createProperty(inValue, "in_value", new DfxEntityPropertyTypes::FloatPropertyType());
	}
	
	// Scalar Count Node
	sCountNode::sCountNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("Count");

		setPropertyState(inArray, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outCount, DfxEntityPropertyState::OUTPUT_STATE);
	}

	sCountNode::~sCountNode() {

	}

	void sCountNode::compute() {
		using namespace DfxEntityPropertyTypes;

		if (propertyHasBinding(inArray)) {
			FloatArray *input = (FloatArray *)getProperty(inArray);
			*(float*)getProperty(outCount) = input->size();
		}
		else {
			*(float*)getProperty(outCount) = .0f;
		}

		makePropertyBindingDirty(outCount);
		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void sCountNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void sCountNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> sCountNode::exportData() {
		return mapEntityProperties(this);
	}

	bool sCountNode::isExportable() {
		return true;
	}

	void *sCountNode::creator(DfxGlobalEntityName uniquename) {
		return new sCountNode(uniquename);
	}

	void sCountNode::initialize() {

		DfxEntityDB::createProperty(inArray, "array", new DfxEntityPropertyTypes::FloatArrayPropertyType());
		DfxEntityDB::createProperty(outCount, "result", new DfxEntityPropertyTypes::FloatPropertyType());
	}

	// Scalar Uniform Array Node
	sUniformArrayNode::sUniformArrayNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("Uniform Array");

		setPropertyState(inValue, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inCount, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outArray, DfxEntityPropertyState::OUTPUT_STATE);
	}

	sUniformArrayNode::~sUniformArrayNode() {

	}

	void sUniformArrayNode::compute() {
		using namespace DfxEntityPropertyTypes;

		float value = *(float*)getProperty(inValue);
		size_t size = (size_t)*(float*)getProperty(inCount);
		FloatArray *output = (FloatArray*)getProperty(outArray);

		output->deleteArray();

		if (size > 0) {

			output->resize(size);

			for (int inx = 0; inx < size; inx++) {
				(*output)[inx] = value;
			}
		}

		makePropertyBindingDirty(outArray);
		makeChildrenDirty();
		getDataInstance()->makeClean();
	}

	void sUniformArrayNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void sUniformArrayNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> sUniformArrayNode::exportData() {
		return mapEntityProperties(this);
	}

	bool sUniformArrayNode::isExportable() {
		return true;
	}

	void *sUniformArrayNode::creator(DfxGlobalEntityName uniquename) {
		return new sUniformArrayNode(uniquename);
	}

	void sUniformArrayNode::initialize() {

		DfxEntityDB::createProperty(inValue, "value", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inCount,"size", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(outArray, "result", new DfxEntityPropertyTypes::FloatArrayPropertyType());
	}

	// Scalar Value Node
	sValueNode::sValueNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("Value");

		setPropertyState(inA, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outC, DfxEntityPropertyState::OUTPUT_STATE);
	}

	sValueNode::~sValueNode() {

	}

	void sValueNode::compute() {

		float *output = (float*)getProperty(outC);
		*output = *(float*)getProperty(inA);

		makePropertyBindingDirty(outC);
		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void sValueNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void sValueNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> sValueNode::exportData() {
		return mapEntityProperties(this);
	}

	bool sValueNode::isExportable() {
		return true;
	}

	void *sValueNode::creator(DfxGlobalEntityName uniquename) {
		return new sValueNode(uniquename);
	}

	void sValueNode::initialize() {

		DfxEntityDB::createProperty(inA, "a", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(outC, "result", new DfxEntityPropertyTypes::FloatPropertyType());
	}

	// Scalar Addition Node
	sAddNode::sAddNode(DfxGlobalEntityName uniquename): DfxEntity(uniquename) {
		setLogicalName("Add");

		setPropertyState(inA, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inB, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outC, DfxEntityPropertyState::OUTPUT_STATE);
	}

	sAddNode::~sAddNode() {

	}

	void sAddNode::compute() {

		float *output = (float*)getProperty(outC);
		float a = *(float*)getProperty(inA);
		float b = *(float*)getProperty(inB);
		*output = a + b;

		makePropertyBindingDirty(outC);
		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void sAddNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void sAddNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> sAddNode::exportData() {
		return mapEntityProperties(this);
	}

	bool sAddNode::isExportable() {
		return true;
	}

	void *sAddNode::creator(DfxGlobalEntityName uniquename) {
		return new sAddNode(uniquename);
	}

	void sAddNode::initialize() {

		DfxEntityDB::createProperty(inA, "a", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inB, "b", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(outC, "result", new DfxEntityPropertyTypes::FloatPropertyType());
	}

	// Subtraction Node
	sSubtractNode::sSubtractNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("Subtract");

		setPropertyState(inA, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inB, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outC, DfxEntityPropertyState::OUTPUT_STATE);
	}

	sSubtractNode::~sSubtractNode() {

	}

	void sSubtractNode::compute() {

		float *output = (float*)getProperty(outC);
		float a = *(float*)getProperty(inA);
		float b = *(float*)getProperty(inB);
		*output = a - b;

		makePropertyBindingDirty(outC);
		getDataInstance()->makeClean();
		makeChildrenDirty();

	}

	void sSubtractNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void sSubtractNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> sSubtractNode::exportData() {
		return mapEntityProperties(this);
	}

	bool sSubtractNode::isExportable() {
		return true;
	}

	void *sSubtractNode::creator(DfxGlobalEntityName uniquename) {
		return new sSubtractNode(uniquename);
	}

	void sSubtractNode::initialize() {
		DfxEntityDB::createProperty(inA, "a", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inB, "b", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(outC, "result", new DfxEntityPropertyTypes::FloatPropertyType());
	}

	// Multiplication Node
	sMultiplyNode::sMultiplyNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("Multiply");

		setPropertyState(inA, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inB, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outC, DfxEntityPropertyState::OUTPUT_STATE);
	}

	sMultiplyNode::~sMultiplyNode() {

	}

	void sMultiplyNode::compute() {

		float *output = (float*)getProperty(outC);
		float a = *(float*)getProperty(inA);
		float b = *(float*)getProperty(inB);
		*output = a * b;

		makePropertyBindingDirty(outC);
		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void sMultiplyNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void sMultiplyNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> sMultiplyNode::exportData() {
		return mapEntityProperties(this);
	}

	bool sMultiplyNode::isExportable() {
		return true;
	}

	void *sMultiplyNode::creator(DfxGlobalEntityName uniquename) {
		return new sMultiplyNode(uniquename);
	}

	void sMultiplyNode::initialize() {

		DfxEntityDB::createProperty(inA, "a", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inB, "b", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(outC,"result", new DfxEntityPropertyTypes::FloatPropertyType());
	}

	// Scalar Division Node
	sDivideNode::sDivideNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("Divide");
		
		setPropertyState(inA, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inB, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outC, DfxEntityPropertyState::OUTPUT_STATE);
	}

	sDivideNode::~sDivideNode() {

	}

	void sDivideNode::compute() {

		float *output = (float*)getProperty(outC);
		float a = *(float*)getProperty(inA);
		float b = *(float*)getProperty(inB);
		*output = a / b;

		makePropertyBindingDirty(outC);
		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void sDivideNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void sDivideNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> sDivideNode::exportData() {
		return mapEntityProperties(this);
	}

	bool sDivideNode::isExportable() {
		return true;
	}

	void *sDivideNode::creator(DfxGlobalEntityName uniquename) {
		return new sDivideNode(uniquename);
	}

	void sDivideNode::initialize() {

		DfxEntityDB::createProperty(inA, "a", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inB, "b", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(outC, "result", new DfxEntityPropertyTypes::FloatPropertyType());
	}

	// Vector Count Node
	vCountNode::vCountNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("Count");

		setPropertyState(inArray, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outCount, DfxEntityPropertyState::OUTPUT_STATE);
	}

	vCountNode::~vCountNode() {

	}

	void vCountNode::compute() {
		using namespace DfxEntityPropertyTypes;

		if (propertyHasBinding(inArray)) {
			Float3Array *input = (Float3Array *)getProperty(inArray);
			*(float*)getProperty(outCount) = input->size();
		}
		else {
			*(float*)getProperty(outCount) = .0f;
		}

		makePropertyBindingDirty(outCount);
		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void vCountNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void vCountNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> vCountNode::exportData() {
		return mapEntityProperties(this);
	}

	bool vCountNode::isExportable() {
		return true;
	}

	void *vCountNode::creator(DfxGlobalEntityName uniquename) {
		return new vCountNode(uniquename);
	}

	void vCountNode::initialize() {

		DfxEntityDB::createProperty(inArray, "array", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
		DfxEntityDB::createProperty(outCount, "result", new DfxEntityPropertyTypes::FloatPropertyType());
	}

	// Vector Uniform Array Node
	vUniformArrayNode::vUniformArrayNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("Uniform Array");

		setPropertyState(inValue, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inCount, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outArray, DfxEntityPropertyState::OUTPUT_STATE);
	}

	vUniformArrayNode::~vUniformArrayNode() {

	}

	void vUniformArrayNode::compute() {
		using namespace DfxEntityPropertyTypes;

		Float3 value = *(Float3*)getProperty(inValue);
		size_t size = (size_t)*(float*)getProperty(inCount);
		Float3Array *output = (Float3Array*)getProperty(outArray);

		output->deleteArray();

		if (size > 0) {

			output->resize(size);

			for (int inx = 0; inx < size; inx++) {
				(*output)[inx] = value;
			}
		}

		makePropertyBindingDirty(outArray);
		makeChildrenDirty();
		getDataInstance()->makeClean();
	}

	void vUniformArrayNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void vUniformArrayNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> vUniformArrayNode::exportData() {
		return mapEntityProperties(this);
	}

	bool vUniformArrayNode::isExportable() {
		return true;
	}

	void *vUniformArrayNode::creator(DfxGlobalEntityName uniquename) {
		return new vUniformArrayNode(uniquename);
	}

	void vUniformArrayNode::initialize() {

		DfxEntityDB::createProperty(inValue, "value", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inCount, "size",  new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(outArray , "result", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
	}

	// Vector Value Node
	vValueNode::vValueNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("Value");

		setPropertyState(inA, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outC, DfxEntityPropertyState::OUTPUT_STATE);
	}

	vValueNode::~vValueNode() {

	}

	void vValueNode::compute() {

		DfxEntityPropertyTypes::Float3 *output = (DfxEntityPropertyTypes::Float3*)getProperty(outC);
		DfxEntityPropertyTypes::Float3 *a = (DfxEntityPropertyTypes::Float3 *)getProperty(inA);
		output->x = a->x;
		output->y = a->y;
		output->z = a->z;

		makePropertyBindingDirty(outC);
		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void vValueNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void vValueNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> vValueNode::exportData() {
		return mapEntityProperties(this);
	}

	bool vValueNode::isExportable() {
		return true;
	}

	void *vValueNode::creator(DfxGlobalEntityName uniquename) {
		return new sValueNode(uniquename);
	}

	void vValueNode::initialize() {

		DfxEntityDB::createProperty(inA, "a", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(outC, "result", new DfxEntityPropertyTypes::Float3PropertyType());
	}

	// Vector Addition Node
	vAddNode::vAddNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("Add");

		setPropertyState(inA, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inB, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outC, DfxEntityPropertyState::OUTPUT_STATE);
	}

	vAddNode::~vAddNode() {

	}

	void vAddNode::compute() {

		DfxEntityPropertyTypes::Float3 *output = (DfxEntityPropertyTypes::Float3*)getProperty(outC);
		DfxEntityPropertyTypes::Float3 *a = (DfxEntityPropertyTypes::Float3 *)getProperty(inA);
		DfxEntityPropertyTypes::Float3 *b = (DfxEntityPropertyTypes::Float3 *)getProperty(inB);
		output->x = a->x + b->x;
		output->y = a->y + b->y;
		output->z = a->z + b->z;

		makePropertyBindingDirty(outC);
		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void vAddNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void vAddNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> vAddNode::exportData() {
		return mapEntityProperties(this);
	}

	bool vAddNode::isExportable() {
		return true;
	}

	void *vAddNode::creator(DfxGlobalEntityName uniquename) {
		return new vAddNode(uniquename);
	}

	void vAddNode::initialize() {

		DfxEntityDB::createProperty(inA, "a", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inB, "b", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(outC, "result", new DfxEntityPropertyTypes::Float3PropertyType());
	}
}