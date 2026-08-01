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

#include <pybind11/embed.h>
#include "DfxEntityDB.h"
#include "DfxGenericPythonEntity.h"

namespace py = pybind11;


void DfxPyEntityDefinition::addProperty(std::string name, size_t prop) {
	PropertyDesc desc;
	desc.name = name;
	desc.propertyType = prop;

	description.push_back(desc);
}

std::vector<DfxPyEntityDefinition::PropertyDesc> DfxPyEntityDefinition::getProperties() {
	return description;
}


DfxGenericPythonEntity::DfxGenericPythonEntity() {
	_logicalName = "Python Node";
	ORIGINAL_NAME = "2F2X252X_DFX_DYNAMIC_CUSTOM_PYTHON_NODE";
}

DfxGenericPythonEntity::~DfxGenericPythonEntity() {

}

void DfxGenericPythonEntity::setUniqueName(std::string nme) {
	_uname = nme;
}

void DfxGenericPythonEntity::setName(std::string logical) {
	_logicalName = logical;

}

size_t DfxGenericPythonEntity::_getInstanceID() {
	return getInstanceID();
}


DfxEntity *DfxGenericPythonEntity::castToBase() {
	return static_cast<DfxEntity*>(this);
}

void DfxGenericPythonEntity::addProperty(std::string name, size_t prop) {
	PropertyDesc desc;
	desc.name = name;
	desc.propertyType = prop;

	description.push_back(desc);
}


void DfxGenericPythonEntity::buildInstance() {

	define();

	globalID = hash(_uname);
	logicalName = _logicalName;

	instanceID = ++ENTITY_INSTANCE_ID_GEN;

	DfxEntityDataInstance* newInstance = new DfxEntityDataInstance(this);

	int counter = 0;
	for (auto spec : description) {
		size_t pid = counter;
		std::string pname = spec.name;
		DfxEntityPropertyType *typeInstance = nullptr;

		if (spec.propertyType == DfxEntityPropertyTypes::iFLOAT_TYPE) {
			typeInstance = new DfxEntityPropertyTypes::FloatPropertyType();
		}
		else if (spec.propertyType == DfxEntityPropertyTypes::iFLOAT_ARRAY_TYPE) {
			typeInstance = new DfxEntityPropertyTypes::FloatArrayPropertyType();
		}
		else if (spec.propertyType == DfxEntityPropertyTypes::iFLOAT3_TYPE) {
			typeInstance = new DfxEntityPropertyTypes::Float3PropertyType();
		}
		else if (spec.propertyType == DfxEntityPropertyTypes::iFLOAT3_ARRAY_TYPE) {
			typeInstance = new DfxEntityPropertyTypes::Float3ArrayPropertyType();
		}
		else if (spec.propertyType == DfxEntityPropertyTypes::iENUM_TYPE) {
			typeInstance = new DfxEntityPropertyTypes::DfxEnumPropertyType();
		}
		else if (spec.propertyType == DfxEntityPropertyTypes::i3DMESH_TYPE) {
			typeInstance = new DfxEntityPropertyTypes::Dfx3DMeshPropertyType();
		}
		else if (spec.propertyType == DfxEntityPropertyTypes::TRAIT_UID_TYPE) {
			typeInstance = new DfxEntityPropertyTypes::UIDPropertyType();
		}

		if (typeInstance != nullptr) {
			DfxEntityProperty *newProperty = new DfxEntityProperty(pid, typeInstance, pname, newInstance);
			newInstance->insertProperty(pid, newProperty);

			counter++;
		}
		else {
			DfxGlobal::console::println("Python Node Instance Error: " + pname + " unknown type.");
		}
	}

	DfxEntityDB::insertDataInstance(instanceID, newInstance);

	setup();
}

bool DfxGenericPythonEntity::findPropertyID(std::string name, size_t &_id) {
	DfxEntityDataInstance *dinstance = getDataInstance();

	std::vector<size_t> lst = dinstance->getPropertyList();

	for (auto id : lst) {

		QString str(dinstance->getPropertyName(id).c_str());

		if (str.compare(name.c_str(), Qt::CaseInsensitive) == 0) {
			_id = id;
			return true;
		}
	}

	return false;
}

void DfxGenericPythonEntity::setPropetryType(std::string name, std::string type){

	size_t id;
	if (findPropertyID(name, id)) {

		QString _input("input");
		QString _output("output");
		QString _datastore("datastore");

		if (_input.compare(type.c_str(), Qt::CaseInsensitive) == 0) {
			setPropertyState(id, DfxEntityPropertyState::INPUT_STATE);
		}
		else if (_output.compare(type.c_str(), Qt::CaseInsensitive) == 0) {
			setPropertyState(id, DfxEntityPropertyState::OUTPUT_STATE);
		}
		else if (_datastore.compare(type.c_str(), Qt::CaseInsensitive) == 0) {
			setPropertyState(id, DfxEntityPropertyState::DATA_STORE_STATE);
		}
		else {
			setPropertyState(id, DfxEntityPropertyState::NORMAL_STATE);
		}
	}
}

float DfxGenericPythonEntity::getFloatProperty(std::string name) {
	size_t id;
	if (findPropertyID(name, id)) {

		if (getPropertyType(id) == DfxEntityPropertyTypes::iFLOAT_TYPE) {
			return *(float*)getProperty(id);
		}
		else {
			DfxGlobal::console::println("Property Type Mismatch (Not Float): " + name + "in " + getLogicalName() + ".");
			return 0;
		}
	}
	else {
		DfxGlobal::console::println("Unknown Property Name: " + name + "in " + getLogicalName() + ".");
		return 0;
	}
}


void DfxGenericPythonEntity::setFloatProperty(std::string name, float value) {
	size_t id;
	if (findPropertyID(name, id)) {

		if (getPropertyType(id) == DfxEntityPropertyTypes::iFLOAT_TYPE) {
			*(float*)getProperty(id) = value;
		}
		else {
			DfxGlobal::console::println("Property Type Mismatch (Not Float): " + name + "in " + getLogicalName() + ".");
		}	
	}
	else {
		DfxGlobal::console::println("Unknown Property Name: " + name + "in " + getLogicalName() + ".");
	}
}

DfxEntityPropertyTypes::Float3 DfxGenericPythonEntity::getFloat3Property(std::string name) {
	size_t id;
	if (findPropertyID(name, id)) {
	
		if (getPropertyType(id) == DfxEntityPropertyTypes::iFLOAT3_TYPE) {
			return *(DfxEntityPropertyTypes::Float3*)getProperty(id);
		}
		else {
			DfxGlobal::console::println("Property Type Mismatch (Not Float3): " + name + "in " + getLogicalName() + ".");
			return DfxEntityPropertyTypes::Float3();
		}
	}
	else {
		DfxGlobal::console::println("Unknown Property Name: " + name + "in " + getLogicalName() + ".");
		return DfxEntityPropertyTypes::Float3();
	}
}

void DfxGenericPythonEntity::setFloat3Property(std::string name, DfxEntityPropertyTypes::Float3 value) {
	size_t id;
	if (findPropertyID(name, id)) {
		
		if (getPropertyType(id) == DfxEntityPropertyTypes::iFLOAT3_TYPE) {
			*(DfxEntityPropertyTypes::Float3*)getProperty(id) = value;
		}
		else {
			DfxGlobal::console::println("Property Type Mismatch (Not Float3): " + name + "in " + getLogicalName() + ".");
		}
	}
	else {
		DfxGlobal::console::println("Unknown Property Name: " + name + "in " + getLogicalName() + ".");
	}
}

DfxEntityPropertyTypes::FloatArray &DfxGenericPythonEntity::getFloatArrayProperty(std::string name) {
	size_t id;
	if (findPropertyID(name, id)) {
	
		if (getPropertyType(id) == DfxEntityPropertyTypes::iFLOAT_ARRAY_TYPE) {
			return *(DfxEntityPropertyTypes::FloatArray*)getProperty(id);
		}
		else {
			DfxGlobal::console::println("Property Type Mismatch (Not Float3): " + name + "in " + getLogicalName() + ".");
			return _dummy_float_array_;
		}
	}
	else {
		DfxGlobal::console::println("Unknown Property Name: " + name + "in " + getLogicalName() + ".");
		return _dummy_float_array_;
	}
}


DfxEntityPropertyTypes::Float3Array &DfxGenericPythonEntity::getFloat3ArrayProperty(std::string name) {
	size_t id;
	if (findPropertyID(name, id)) {
		if (getPropertyType(id) == DfxEntityPropertyTypes::iFLOAT3_ARRAY_TYPE) {
			return *(DfxEntityPropertyTypes::Float3Array*)getProperty(id);
		}
		else {
			DfxGlobal::console::println("Property Type Mismatch (Not Float3): " + name + "in " + getLogicalName() + ".");
			return _dummy_float3_array_;
		}
	}
	else {
		DfxGlobal::console::println("Unknown Property Name: " + name + "in " + getLogicalName() + ".");
		return _dummy_float3_array_;
	}
}

Dfx3dMeshData &DfxGenericPythonEntity::getMeshProperty(std::string name) {

	size_t id;
	if (findPropertyID(name, id)) {
		
		if (getPropertyType(id) == DfxEntityPropertyTypes::i3DMESH_TYPE) {
			return *(Dfx3dMeshData*)getProperty(id);
		}
		else {
			DfxGlobal::console::println("Property Type Mismatch (Not Float3): " + name + "in " + getLogicalName() + ".");
			return _dummy_mesh_;
		}
	}
	else {
		DfxGlobal::console::println("Unknown Property Name: " + name + "in " + getLogicalName() + ".");
		return _dummy_mesh_;
	}
}

DfxEnum &DfxGenericPythonEntity::getEnumProperty(std::string name) {
	size_t id;
	if (findPropertyID(name, id)) {
	
		if (getPropertyType(id) == DfxEntityPropertyTypes::iENUM_TYPE) {
			return *(DfxEnum*)getProperty(id);
		}
		else {
			DfxGlobal::console::println("Property Type Mismatch (Not Float3): " + name + "in " + getLogicalName() + ".");
			return DfxEnum();
		}
	}
	else {
		DfxGlobal::console::println("Unknown Property Name: " + name + "in " + getLogicalName() + ".");
		return _dummy_enum_;
	}
}

DfxEntityPropertyTypes::UID &DfxGenericPythonEntity::getUIDProperty(std::string name) {
	size_t id;
	if (findPropertyID(name, id)) {
	
		if (getPropertyType(id) == DfxEntityPropertyTypes::TRAIT_UID_TYPE) {
			return *(DfxEntityPropertyTypes::UID*)getProperty(id);
		}
		else {
			DfxGlobal::console::println("Property Type Mismatch (Not Float3): " + name + "in " + getLogicalName() + ".");
			return _dummy_uid_;
		}
	}
	else {
		DfxGlobal::console::println("Unknown Property Name: " + name + "in " + getLogicalName() + ".");
		return _dummy_uid_;
	}
}

void DfxGenericPythonEntity::_cleanEntity() {
	getDataInstance()->makeClean();
}

void DfxGenericPythonEntity::_dirtyChildren() {
	makeChildrenDirty();
}