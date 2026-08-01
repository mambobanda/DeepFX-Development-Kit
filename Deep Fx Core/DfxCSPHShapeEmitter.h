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

#include "DfxCSimulationEntity.h"
#include "DfxCSPHParticle.h"
#include "util.h"

namespace dfxcore {

	namespace sph {

		static DfxGlobalEntityName SPHShapeEmitter_Identifier = "2F2X252X_DFX_SPH_SHAPE_EMITTER";

		class SPHShapeEmitter : public DfxCSimulationEntity {
		private:
			std::vector<glm::vec3> emissionPositions;
			std::vector<Particle*> particles;

			glm::vec3 initialVelocity;

			int randomnessCounter;
		private:
			float distanceCounter;
		public:
			static const DfxPropertyID outID				= 0;
			static const DfxPropertyID inParticleRadius     = 1;
			static const DfxPropertyID inParticleCount		= 2;
			static const DfxPropertyID inShape				= 3;
			static const DfxPropertyID inStrength			= 4;
			static const DfxPropertyID inURandomness		= 5;
			static const DfxPropertyID inVRandomness		= 6;
			static const DfxPropertyID inCircleRadius		= 7;
			static const DfxPropertyID inRectWidth			= 8;
			static const DfxPropertyID inRectHeight			= 9;
			
		public:

			SPHShapeEmitter(DfxGlobalEntityName uniquename);
			virtual ~SPHShapeEmitter();

			void compute() override;

			size_t particleCount();

			Particle *getParticle(size_t index);

			void init() override;

			void clear() override;

			void done() override;

			void load(std::ifstream &) override;

			void draw() override;

			void drawSelection() override;

			DfxBounds bounds() override;

			bool selectable() override;

			void calculateBounds() override;

			void drawCircleEmitter(float lnwdth);

			void drawRectangleEmitter(float lnwdth);

			DfxBounds circleEmitterBounds();

			DfxBounds rectangleEmitterBounds();

			void onDataChanged(DfxEntityDataInstance *instance) override;

			void importData(std::vector<std::pair<std::string, std::string>> data) override;

			std::vector<std::pair<std::string, std::string>> exportData() override;

			bool isExportable() override;

			static void *creator(DfxGlobalEntityName uniquename);

			static void initialize();

			// Emitter Functions
			void initEmissionPositions();

			void generateCirclePositions();

			void generateRectanglePositions();

			glm::vec3 uvwRandomness(glm::vec3 pos);

			void step(float timestep, glm::vec3 *vi);

			void advanceEmitter(glm::vec3 *vi);

			glm::vec3 getFinalVelocity();

			glm::mat4 getFinalTransformation();

			void initDefualtValues();
		};

	}
}