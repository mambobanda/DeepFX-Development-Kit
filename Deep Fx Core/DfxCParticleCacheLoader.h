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

#include <Partio.h>
#include "DfxCSimulationCache.h"
#include "DfxCSimulationEntity.h"
#include "util.h"

namespace dfxcore {

	static DfxGlobalEntityName ParticleCacheLoader_Identifier = "2F2X252X_DFX_PARTICLE_CACHE_READER";

	class ParticleCacheLoader : public DfxCSimulationEntity {
	private:

		std::unordered_map<std::string, std::string> fileCacheMap;
		SimulationFrame *getFileCacheFrame(int frm);

		size_t current_index;
		SimulationFrame *frame;
	public:
		static const DfxPropertyID inFilePath		= 0;
		static const DfxPropertyID inParticleSize	= 1;
		static const DfxPropertyID inIndex			= 2;
		static const DfxPropertyID outPositionArray	= 3;
		static const DfxPropertyID outVelocityArray	= 4;

	public:
		ParticleCacheLoader(DfxGlobalEntityName uniquename);
		virtual ~ParticleCacheLoader();

		void init()  override;

		void clear() override;

		void done()  override;

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void draw() override;

		void load(std::ifstream &) override;

		void loadParticleCache();

		void loadOuputArrays();

		void setFilepath(std::string );

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};
}
