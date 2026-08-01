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
#include "DfxCSimulationCache.h"
#include "DfxCSimulationEntity.h"
#include "DfxSimulationCoreGlobal.h"
#include "DfxCBulletRigidObject.h"
#include "DfxAlembicMeshProxy.h"
#include "Util.h"

namespace dfxcore {

	namespace bullet {

		static DfxGlobalEntityName DfxCBulletPointCache_Identifier = "2F2X252X_DFX_BULLET_POINT_CACHE";

		class BulletCacheObject {
		public:
			SimulationCache cache;
		};

		class DfxCBulletPointCache : public DfxCSimulationEntity {
		private:
			std::unordered_map<size_t, BulletCacheObject*> cacheMap;
			std::unordered_map<size_t, DfxAlembicMeshProxy> alembicPolyMeshMap;
			Alembic::AbcGeom::OArchive *archive;
			Alembic::AbcGeom::TimeSamplingPtr *timeSamplePtr;

			Alembic::AbcGeom::OPolyMeshSchema::Sample *sampleMeshData(Dfx3dMeshData *, DfxAlembicMeshProxy &);
			DfxBounds calcMeshBounds(Dfx3dMeshData *);
		public:
			static const DfxPropertyID outID		= 0;
			static const DfxPropertyID inRef		= 1;
			static const DfxPropertyID inIndex		= 2;
			static const DfxPropertyID inBuffer		= 3;
			static const DfxPropertyID inType		= 4;
			static const DfxPropertyID inSequence	= 5;
			static const DfxPropertyID inDirPath	= 6;
			static const DfxPropertyID inPrefix		= 7;
			
		public:

			DfxCBulletPointCache(DfxGlobalEntityName uniquename);
			virtual ~DfxCBulletPointCache();

			void compute() override;

			void onDataChanged(DfxEntityDataInstance *instance) override;

			void init() override;

			void clear() override;

			void done() override;

			void draw() override;

			void importData(std::vector<std::pair<std::string, std::string>> data) override;

			std::vector<std::pair<std::string, std::string>> exportData() override;

			bool isExportable() override;

			static void *creator(DfxGlobalEntityName uniquename);

			static void initialize();

			std::vector<size_t> findRigidObjects();

			void cacheCurrentIndexToMemory();

			void cacheCurrentIndexToDisc();

			void clearObjects();
		};
	}
}