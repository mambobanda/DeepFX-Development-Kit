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
#include "DfxCSimulationCache.h"
#include "DfxCSimulationEntity.h"
#include "util.h"

namespace dfxcore {

	static DfxGlobalEntityName ParticleKillVolume_Identifier = "2F2X252X_DFX_PARTICLE_KILL_VOLUME";

	class ParticleKillVolume : public DfxCSimulationEntity {
	public:
		static const DfxPropertyID inRef			= 0;
		static const DfxPropertyID inPosition		= 1;
		static const DfxPropertyID inRotation		= 2;
		static const DfxPropertyID inScale			= 3;
		static const DfxPropertyID inType			= 4;
		static const DfxPropertyID inPositionArray	= 5;
		static const DfxPropertyID inVelocityArray	= 6;
		static const DfxPropertyID outPositionArray = 7;
		static const DfxPropertyID outVelocityArray = 8;

	public:
		ParticleKillVolume(DfxGlobalEntityName uniquename);
		virtual ~ParticleKillVolume();

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

		glm::mat4  ParticleKillVolume::getTransform();
	};
}