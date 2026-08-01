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
#include <DfxEntityUtil.h>
#include <DfxTraitUID.h>
#include <DfxUtility.h>
#include <DfxGlobal.h>
#include <algorithm>
#include <limits>
#include <random>
#include <ctime>
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxEntityDB.h"
#include "DfxVectorMeshSampler.h"


namespace dfxcore {

	static DfxGlobalEntityName DfxUniformMeshSamplerNode_Identifier = "2F2X252X_DFX_UNIFORM_MESH_SAMPLER_NODE";

	class DfxUniformMeshSamplerNode : public DfxEntity {
	private:

		void randomisePoints(vector<glm::vec3> &);
	public:

		static const DfxPropertyID outPoints				= 0;
		static const DfxPropertyID inRef					= 1;
		static const DfxPropertyID inScale					= 2;
		static const DfxPropertyID inSpacing				= 3;
		static const DfxPropertyID inRandomDisplacement		= 4;

		DfxUniformMeshSamplerNode(DfxGlobalEntityName uniquename);

		virtual ~DfxUniformMeshSamplerNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();

	};
}
