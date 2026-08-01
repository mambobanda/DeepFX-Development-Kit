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

#include <vector>
#include <DfxEntity.h>
#include <DfxTraitUID.h>
#include <DfxVDBSkinner.h>
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxEntityDB.h"
#include "DfxGlobal.h"

#pragma once


static DfxGlobalEntityName DfxVDBPointMesher_Identifier = "2F2X252X_DFX_VDB_POINT_MESHER";

class DfxVDBPointMesher : public DfxEntity {
public:
	static DfxPropertyID outID;
	static DfxPropertyID inRef;
	static DfxPropertyID inGridSpacing;
	static DfxPropertyID inPointRadius;
	static DfxPropertyID inNarrowband;
	static DfxPropertyID inPoints;

	DfxVDBPointMesher(DfxGlobalEntityName uniquename);

	virtual ~DfxVDBPointMesher();

	void compute() override;

	void onDataChanged(DfxEntityDataInstance *instance) override;

	void importData(std::vector<std::pair<std::string, std::string>> data) override;

	std::vector<std::pair<std::string, std::string>> exportData() override;

	bool isExportable() override;

	static void *creator();

	static void initialize();

};
