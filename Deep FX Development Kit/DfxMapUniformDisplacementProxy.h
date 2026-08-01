/*
* The Mailo Today Solutions Software License
*
* Copyright 2017 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#pragma once

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "DfxEntityDB.h"
#include "DfxEntityUtil.h"
#include "DfxDefaultGlobalIDs.h"
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxEntity.h"
#include "DfxObjectItem.h"
#include "DfxObjectDisplacementMap.h"
#include "DfxScene.h"

static DfxGlobalEntityName DfxMapUniformDisplacementProxy_Identifier = "2F2X252X_DFX_MAP_UNIFORM_DISPLACEMENT_PROXY";

class DfxMapUniformDisplacementProxy : public DfxEntity {
public:
	static const DfxPropertyID inProxies		= 0;
	static const DfxPropertyID inTrigger		= 1;
	static const DfxPropertyID inFrame			= 2;
	static const DfxPropertyID inTransforms		= 3;

	DfxMapUniformDisplacementProxy(DfxGlobalEntityName uniquename);
	virtual ~DfxMapUniformDisplacementProxy();

	void compute() override;

	void onDataChanged(DfxEntityDataInstance *instance) override;

	void importData(std::vector<std::pair<std::string, std::string>> data) override;

	std::vector<std::pair<std::string, std::string>> exportData() override;

	bool isExportable() override;

	static void *creator(DfxGlobalEntityName uniquename);

	static void initialize();
};
