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

#include <DfxUtility.h>
#include <Dfx3dMeshData.h>
#include <DfxGlobal.h>
#include <DfxEntity.h>
#include <DfxEntityDB.h>
#include <DfxEntityBasicPropertyTypes.h>
#include <DfxBounds.h>
#include <DfxCVoxel.h>
#include <glm\geometric.hpp>

#ifdef WIN32
#include <concurrent_unordered_map.h>
#else
#include <unordered_map.h>
#endif

namespace dfxcore {

	static DfxGlobalEntityName MeshVoxelizer_Identifier = "2F2X252X_DFX_MESH_VOXELIZER";

	class MeshVoxelizer: public DfxEntity {
	public:
		static const DfxPropertyID inRef			= 0;
		static const DfxPropertyID inSource			= 1;
		static const DfxPropertyID inVoxelSize		= 2;
		static const DfxPropertyID inScale			= 3;
		static const DfxPropertyID outPointArray	= 4;

	public:

		MeshVoxelizer(DfxGlobalEntityName uniquename);
		virtual ~MeshVoxelizer();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();

	};
}