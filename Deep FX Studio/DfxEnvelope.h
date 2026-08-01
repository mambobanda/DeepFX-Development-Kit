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

#include <DfxEntity.h>
#include <DfxEntityDB.h>
#include <DfxTime.h>
#include <DfxGlobal.h>
#include <glm\vec2.hpp>
#include <glm\gtx\compatibility.hpp>
#include <string>
#include <vector>
#include "DfxEnvelopeValues.h"

static DfxGlobalEntityName DfxEnvelope_Identifier = "2F2X252X_DFX_ENVELOPE";

class DfxEnvelope: public DfxEntity {
private:
	std::vector<DfxEnvelopeFloatValue>  nullValue;
	std::vector<DfxEnvelopeFloatValue>  floatVector;
	std::vector<DfxEnvelopeFloat3Value> float3Vector;

	size_t size;

	bool null;
private:

	DfxEntity *entity;
	size_t propertyID;
	size_t type;

	int currentFrame;

	bool active;
private:
	int findPreviousValueFloat3(int current_frame);
	int findPreviousValueFloat(int current_frame);
	int findNextValueFloat3(int current_frame);
	int findNextValueFloat(int current_frame);
	void updateProperty(int frame);
public:
	static const DfxPropertyID _id = 0;
public:

	DfxEnvelope(DfxGlobalEntityName uniquename, DfxEntity *entity, size_t propertyID);
	~DfxEnvelope();

	void resizeToFit(unsigned int);

	void setValue(size_t index, DfxEntityPropertyTypes::Float3);

	void setValue(size_t index, float);

	void getValue(size_t index, DfxEntityPropertyTypes::Float3 &);

	void getValue(size_t index, float &);

	bool hasKey(size_t index);

	void clearValue(size_t index);

	std::string getPropertyName();

	size_t getEntityPropID();

	size_t getEntityPropType();

	size_t keyCount();

	size_t getType();

	DfxEntity *getEntity();

	bool isActive();

	void setActive(bool);

	void compute() override;

	void onDataChanged(DfxEntityDataInstance *instance) override;

	void importData(std::vector<std::pair<std::string, std::string>> data) override;

	std::vector<std::pair<std::string, std::string>> exportData() override;

	bool isExportable() override;

	bool isNull();

	static void initialize();
};