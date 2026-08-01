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
#include <DfxEntity.h>
#include <DfxTraitUID.h>
#include <DfxUtility.h>
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxEntityDB.h"
#include "DfxGlobal.h"
#include "DfxNvBlastGlobal.h"


namespace dfxcore {

	static DfxGlobalEntityName DfxVoronoiFractureNode_Identifier = "2F2X252X_DFX_VORONOI_FRACTURE_NODE";

	class DfxVoronoiFracture : public DfxEntity {
	private:
		std::vector<Dfx3dMeshData *> fructureMesh(Dfx3dMeshData *mesh, glm::mat4 trans, int32_t cellcount, int32_t seed, std::vector<glm::vec3> samples, bool &success);
	public:
		static const DfxPropertyID outRef			= 0;
		static const DfxPropertyID proxies			= 1;
		static const DfxPropertyID inRef			= 2;
		static const DfxPropertyID inSeed			= 3;
		static const DfxPropertyID inCellCount		= 4;
		static const DfxPropertyID inPosition		= 5;
		static const DfxPropertyID inRotation		= 6; 
		static const DfxPropertyID inScale			= 7;
		static const DfxPropertyID inSamples        = 8;


		DfxVoronoiFracture(DfxGlobalEntityName uniquename);

		virtual ~DfxVoronoiFracture();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();

		void addUID(DfxEntityPropertyTypes::UID*);

		void removeUID(DfxEntityPropertyTypes::UID*);

		glm::mat4 getAbsoluteTransform();

		glm::mat4 getAbsoluteTransform(DfxEntityPropertyTypes::ProxyObject *);
	};
}