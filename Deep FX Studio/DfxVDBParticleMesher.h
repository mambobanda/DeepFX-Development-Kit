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

#include "DfxVDBGlobal.h"

#ifdef USE_OPENVDB_LIB 

#include <vector>
#include <queue>
#include <DfxEntity.h>
#include <DfxTraitUID.h>
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxEntityDB.h"
#include "DfxGlobal.h"
#include "DfxVDBFiltersNodes.h"
#include <glm/geometric.hpp>
#include "DfxTempIndexedMesh.h"
#include "DfxUtility.h"
#include "DfxVDBSkinner.h"

namespace DfxNodes {
	
	static DfxGlobalEntityName DfxVDBParticleMesher_Identifier = "2F2X252X_DFX_VDB_PARTICLE_MESHER";

	class DfxVDBParticleMesher : public DfxEntity {
	public:
		static const DfxPropertyID outRef			= 0;
		static const DfxPropertyID inFilters		= 1;
		static const DfxPropertyID inGridSize		= 2;
		static const DfxPropertyID inParticleSize	= 3;
		static const DfxPropertyID inPoints			= 4;
		static const DfxPropertyID mesh				= 5;

		DfxVDBParticleMesher(DfxGlobalEntityName uniquename);

		virtual ~DfxVDBParticleMesher();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();

		std::queue<DfxEntity*> findAllFilters(DfxEntityPropertyTypes::UID &uid);

	};
}

#endif