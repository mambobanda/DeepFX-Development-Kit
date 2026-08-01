/*
* The Mailo Today Solutions Software License
*
* Copyright 2016 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#pragma once

#include <glm\geometric.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtx\transform.hpp>
#include "DfxUtility.h"
#include "DfxGlobal.h"
#include "DfxViewItem.h"

static DfxGlobalEntityName DfxObjectItemCopy_Identifier = "2F2X252X_DFX_OBJECT_ITEM_COPY";

class DfxObjectItemCopy : public DfxEntity {
private:
	bool glInitialized;
public:
	static const DfxPropertyID outRef       = 0;
	static const DfxPropertyID inRef		= 1;
	static const DfxPropertyID inPoints		= 2;
	static const DfxPropertyID inScales		= 3;
	static const DfxPropertyID inRotations	= 4;
	static const DfxPropertyID outProxies	= 5;
public:

	DfxObjectItemCopy(DfxGlobalEntityName uniquename);
	virtual ~DfxObjectItemCopy();
	
	void compute() override;

	void onDataChanged(DfxEntityDataInstance *instance) override;

	void importData(std::vector<std::pair<std::string, std::string>> data) override;

	std::vector<std::pair<std::string, std::string>> exportData() override;

	bool isExportable() override;

	static void *creator(DfxGlobalEntityName uniquename);

	static void initialize();

	Dfx3dMeshData *getMeshData(size_t inx);
	
};
