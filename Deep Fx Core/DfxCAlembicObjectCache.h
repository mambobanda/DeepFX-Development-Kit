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
#include <DfxBounds.h>
#include <glm\geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreOgawa/All.h>
#include <Alembic/AbcGeom/All.h>
#include <unordered_map>
#include "DfxCSimulationEntity.h"
#include "DfxSimulationCoreGlobal.h"
#include "DfxAlembicMeshProxy.h"
#include "Util.h"

namespace dfxcore {

	static DfxGlobalEntityName DfxAlembicObjectCache_Identifier = "2F2X252X_DFX_ALEMBIC_OBJECT_CACHE";


	class DfxCAlembicObjectCache : public DfxCSimulationEntity {
	private:
		Alembic::AbcGeom::OArchive *archive;
		Alembic::AbcGeom::TimeSamplingPtr *timeSamplePtr;
		Alembic::AbcGeom::OXform *xform;
		Alembic::AbcGeom::OPolyMesh *meshobj;

		void sampleMeshData(Dfx3dMeshData *, DfxAlembicMeshProxy &);
		DfxBounds calcMeshBounds(Dfx3dMeshData *);
		void clearAlembicProxy(DfxAlembicMeshProxy &);

		void cacheSequence();
		void cache();

	public:
		static const DfxPropertyID inRef		= 0;
		static const DfxPropertyID inIndex		= 1;
		static const DfxPropertyID inSequence	= 2;
		static const DfxPropertyID inDirPath	= 3;
		static const DfxPropertyID inPrefix		= 4;
		static const DfxPropertyID inFPS		= 5;
		static const DfxPropertyID inStartFrame	= 6;

	public:

		DfxCAlembicObjectCache(DfxGlobalEntityName uniquename);
		virtual ~DfxCAlembicObjectCache();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void init() override;

		void clear() override;

		void done() override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};
}