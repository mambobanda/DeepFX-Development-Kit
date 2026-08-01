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

#include <Partio.h>
#include <experimental\filesystem>
#include "DfxCSimulationCache.h"
#include "DfxCSimulationEntity.h"
#include "util.h"

namespace filesystem = std::experimental::filesystem;

namespace dfxcore {

	static DfxGlobalEntityName ParticleCache_Identifier = "2F2X252X_DFX_PARTICLE_CACHE";

	class ParticleCache : public DfxCSimulationEntity {
	private:
		SimulationCache memCache;

	public:
		static const DfxPropertyID inType				= 0;
		static const DfxPropertyID inDirPath			= 1;
		static const DfxPropertyID inPrefix				= 2;
		static const DfxPropertyID inParticleSize		= 3;
		static const DfxPropertyID inIndex				= 4;
		static const DfxPropertyID inPositionArray		= 5;
		static const DfxPropertyID inVelocityArray		= 6;

	public:
		ParticleCache(DfxGlobalEntityName uniquename);
		virtual ~ParticleCache();

		void init()  override;

		void clear() override;

		void done()  override;

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void draw() override;

		void load(std::ifstream &) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();

		void cacheToMemory();

		void cacheToDisk();
	};
}
