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

#pragma once

#include <vector>
#include <pybind11/embed.h>
#include "DfxGlobal.h"
#include "DfxEntity.h"

namespace py = pybind11;

class DfxPyEntityDefinition {
public:
	struct PropertyDesc
	{
		std::string name;
		size_t propertyType;
	};

private:
	std::vector<PropertyDesc> description;

public:
	void addProperty(std::string name, size_t prop);
	std::vector<PropertyDesc> getProperties();
};


class DfxGenericPythonEntity : public DfxEntity{
private:
	
	std::string _uname;
	std::string _logicalName;
	struct PropertyDesc
	{
		std::string name;
		size_t propertyType;
	};

private:
	std::vector<PropertyDesc> description;
	std::shared_ptr<DfxGenericPythonEntity> self;

	bool findPropertyID(std::string, size_t &_id);

	Dfx3dMeshData _dummy_mesh_;
	DfxEnum _dummy_enum_;
	DfxEntityPropertyTypes::Float3Array _dummy_float3_array_;
	DfxEntityPropertyTypes::FloatArray _dummy_float_array_;
	DfxEntityPropertyTypes::UID _dummy_uid_;
public:

	DfxGenericPythonEntity();
	virtual ~DfxGenericPythonEntity();

	virtual void compute() = 0;
	virtual void define() = 0;
	virtual void setup() = 0;

	void setUniqueName(std::string);
	void setName(std::string);
	void addProperty(std::string name, size_t prop);
	void buildInstance();

	size_t _getInstanceID();
	DfxEntity *castToBase();

	void _cleanEntity();
	void _dirtyChildren();

public: // Property Helper functions

	void setPropetryType(std::string, std::string);

	float getFloatProperty(std::string name);
	void setFloatProperty(std::string name, float value);

	DfxEntityPropertyTypes::Float3 getFloat3Property(std::string name);
	void setFloat3Property(std::string name, DfxEntityPropertyTypes::Float3);

	DfxEntityPropertyTypes::FloatArray &getFloatArrayProperty(std::string name);

	DfxEntityPropertyTypes::Float3Array &getFloat3ArrayProperty(std::string name);

	Dfx3dMeshData &getMeshProperty(std::string name);

	DfxEnum &getEnumProperty(std::string name);

	DfxEntityPropertyTypes::UID &getUIDProperty(std::string name);
};


