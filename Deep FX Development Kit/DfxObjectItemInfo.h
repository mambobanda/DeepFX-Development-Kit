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

#include "DfxScene.h"
#include "DfxObjectItem.h"
#include "DfxEntityDB.h"
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxEntity.h"

static DfxGlobalEntityName DfxObjectItemInfo_Identifier = "2F2X252X_DFX_OBJECT_ITEM_INFO";

class DfxObjectItemInfo : public DfxEntity {
public:
	static const DfxPropertyID inObjectID		= 0;
	static const DfxPropertyID outVertexCount	= 1;
	static const DfxPropertyID outFaceCount		= 2;
	static const DfxPropertyID outPosition		= 3;
	static const DfxPropertyID outRotation		= 4;
	static const DfxPropertyID outScale			= 5;

public:

	DfxObjectItemInfo(DfxGlobalEntityName uniquename);
	virtual ~DfxObjectItemInfo();

	void compute() override;

	void onDataChanged(DfxEntityDataInstance *instance) override;

	void importData(std::vector<std::pair<std::string, std::string>> data) override;

	std::vector<std::pair<std::string, std::string>> exportData() override;

	bool isExportable() override;

	static void *creator(DfxGlobalEntityName uniquename);

	static void initialize();

};
