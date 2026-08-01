/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <amp.h>
#include <amp_math.h>
#include <iostream>
#include <vector>
#include "EntropyCore.h"
#include "Timer.h"

namespace xamp = concurrency;

namespace boom {

	class Entropy {
	private:

		int GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z;

		float RDX;
		float VISC;
		float VORT;
		float DENS_DISSIP;
		float BUOY_RISE;
		float BUOY_FALL;
		float AMB_TEMP;
		float DT;
		int DIFF_ITER;
		int PROJ_ITER;

		float DIFFUSION_ALPHA;
		float DIFFUSION_BETA_RECIPROCAL;
		float PRESSURE_ALPHA;
		float PRESSURE_BETA_RECIPROCAL;
		float HALF_RDX;

		int advectionMethod;
		int vorticityFlag;

		float gravity_x;
		float gravity_y;
		float gravity_z;

		xamp::accelerator accelerator;

		xamp::extent<1> extent;

		xamp::array<float> u;
		xamp::array<float> v;
		xamp::array<float> w;
		xamp::array<float> p;
		xamp::array<float> dens;
		xamp::array<float> temperature;
		xamp::array<float> floatTemp00;
		xamp::array<float> floatTemp01;
		xamp::array<float> floatTemp02;
		xamp::array<float> udiv;
		xamp::array<int> cellType;
		xamp::array<int> fixedEmitters;
	private:
		std::vector<emitters::FixedEmitterSettings> fixedEmitterSettings;

	public:
		

		Entropy(xamp::accelerator accelerator, int dimx, int dimy, int dimz);

		void step();

		////void localForces();
		////void printUV();

		//// AMP Kernels
		void advectVelocity();
		void macCormackAdvectVelocity();
		void advectDensity();
		void macCormackAdvectDensity();
		void advectTemperature();
		void macCormackAdvectTemperature();
		void diffuseVelocity();
		void diffuseDensity();
		void enforceBoundary();
		void initBoundary();
		void divergence();
		void pressure();
		void subtractPressureVelocity();
		void buoyancy();
		void vorticity();
		void dissipation();
		void emitFixedEmitters();

	public:
		
		// Getter sand Setters
		int getGridSizeX();
		int getGridSizeY();
		int getGridSizeZ();

		xamp::array<float> &getU();
		xamp::array<float> &getV();
		xamp::array<float> &getW();
		xamp::array<float> &getP();
		xamp::array<float> &getDens();
		xamp::array<float> &getTemperature();
		xamp::array<float> &getTemp00();
		xamp::array<float> &getTemp01();
		xamp::array<float> &getTemp02();
		xamp::array<float> &getUDiv();
		xamp::array<int> &getCellType();
		xamp::array<int> &getFixedEmitters();

		float getRDX();
		void setRDX(float rdx);
		
		float getViscosity();
		void setViscosity(float visc);

		float getTimeStep();
		void setTimeStep(float dt);
		
		int getDiffuseIterations();
		void setDiffuseIterations(int iter);

		int getProjectionIterations();
		void setProjectionIterations(int iter);

		float getDiffusionAlpha();
		void setDiffusionAlpha(float);

		float getDiffusionBeta();
		void setDiffusionBeta(float);

		float getPressureAlpha();
		void setPressureAlpha(float);

		float getPressureBeta();
		void setPressureBeta(float val);

		float getHalfRDX();
		void setHalfRDX(float );

		float getVorticity();
		void setVorticity(float);

		float getDensityDissipation();
		void setDensityDissipation(float);

		float getBuoyancyRise();
		void setBuoyancyRise(float);

		float getBuoyancyFall();
		void setBuoyancyFall(float);

		float getAmbiantTemperature();
		void setAmbiantTemperature(float);

		int getAdvectionType();
		void setAdvectionType(int);

		int getVorticityConfinementFlag();
		void setVorticityConfinementFlag(int);

		std::vector<float> getGravity();
		void setGravity(float x, float y, float z);

		void syncSettings();

		// Emitters
		void addFixedEmitter(std::vector<int> &emitterMap, emitters::FixedEmitterSettings settings);

		// Util
		static int index3d(int i, int j, int k, int GRID_SIZE_X, int GRID_SIZE_Y);
	};


	
}