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

#include <unordered_map>
#include <vector>
#include "DfxEntity.h"


class DfxGlobalProperties {
public:

	struct GlobalProperty {
		size_t instanceID;
		DfxPropertyID propertyID;
	};

private:

	std::unordered_map<std::string, std::vector<GlobalProperty*>> globals;

private:

	static DfxGlobalProperties *instance;
	DfxGlobalProperties();

public:

	static DfxGlobalProperties *getInstance();

	bool hasProperty(std::string path, DfxPropertyID propertyID);

	DfxGlobalProperties::GlobalProperty *getProperty(std::string path, DfxPropertyID propertyID);

	DfxGlobalProperties::GlobalProperty *addProperty(std::string path, size_t instanceID, DfxPropertyID propertyID);

	void removeProperty(std::string path, size_t instanceID, DfxPropertyID propertyID);

	void removeGlobal(std::string path);
};