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

#include <algorithm>
#include <limits>
#include <random>
#include <ctime>
#include <Dfx3dMeshData.h>
#include <DfxGlobal.h>
#include <DfxEntity.h>
#include <DfxEntityDB.h>
#include <DfxEntityBasicPropertyTypes.h>
#include <DfxBounds.h>
#include <DfxCVoxel.h>
#include <glm\mat4x4.hpp>
#include <glm\geometric.hpp>
#include <glm\gtx\transform.hpp>

namespace dfxcore {

	static DfxGlobalEntityName DfxPointGrid_Identifier = "2F2X252X_DFX_POINT_GRID";

	class DfxPointGrid : public DfxEntity {
	public:
		static const DfxPropertyID inNumberOfPoints			= 0;
		static const DfxPropertyID inPointSpacing			= 1;
		static const DfxPropertyID inRandomisePosition		= 2;
		static const DfxPropertyID inGlobalPosition			= 3;
		static const DfxPropertyID outPointArray			= 4;

	public:

		DfxPointGrid(DfxGlobalEntityName uniquename);
		virtual ~DfxPointGrid();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();

	};
}
