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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#pragma warning( disable : 4146 )

#ifndef ENTROPY_LIB
#include <openvdb/openvdb.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "timer.h"
#include "Entropy.h"
#include "EntropyCore.h"

using namespace std;

#define GRID_X 800
#define GRID_Y 250
#define GRID_Z 250

boom::Entropy *sim_ptr;
std::vector<float> density;
std::vector<float> temperature;
std::vector<float> v;
std::vector<int> cellType;
std::vector<int> emitter00;
std::vector<int> emitter01;
std::vector<int> emitter02;
std::vector<int> emitter03;
std::vector<int> emitter04;
std::vector<int> emitterJet;


int inx3d(int i, int j, int k, int GRID_SIZE_X, int GRID_SIZE_Y);

void emitter(std::vector<float> &dens, std::vector<float> &temp, std::vector<float> &v, xamp::array<float> &_dens, xamp::array<float> &_temp, xamp::array<float> &_v,
	int GRID_SIZE_X, int GRID_SIZE_Y, int GRID_SIZE_Z);

int main(int argc, char** argv) {

	boom::emitters::FixedEmitterSettings emitter00Settings;
	boom::emitters::FixedEmitterSettings emitter01Settings;
	boom::emitters::FixedEmitterSettings emitter02Settings;
	boom::emitters::FixedEmitterSettings emitter03Settings;
	boom::emitters::FixedEmitterSettings emitter04Settings;
	boom::emitters::FixedEmitterSettings emitterJetSettings;

	emitter00Settings.density = 2;
	emitter00Settings.temperature = 500;
	emitter00Settings.useVelocity = 0;

	emitter01Settings.density = 3;
	emitter01Settings.temperature = 650;
	emitter01Settings.useVelocity = 0;


	emitter02Settings.density = 2;
	emitter02Settings.temperature = 800;
	emitter02Settings.useVelocity = 0;

	emitter03Settings.density = 4;
	emitter03Settings.temperature = 900;
	emitter03Settings.useVelocity = 0;

	emitter04Settings.density = 3;
	emitter04Settings.temperature = 300;
	emitter04Settings.useVelocity = 0;

	emitterJetSettings.density = 3;
	emitterJetSettings.temperature = 300;
	emitterJetSettings.useVelocity = 1;
	emitterJetSettings.velocity = boom::emitters::Vec3{-4200.0f, 0.0f, 0.f};


	density.resize(GRID_X * GRID_Y * GRID_Z, 0.f);
	temperature.resize(GRID_X * GRID_Y * GRID_Z, 0.f);
	v.resize(GRID_X * GRID_Y * GRID_Z, 0.f);

	emitter00.resize(GRID_X * GRID_Y * GRID_Z, -1);
	emitter01.resize(GRID_X * GRID_Y * GRID_Z, -1);
	emitter02.resize(GRID_X * GRID_Y * GRID_Z, -1);
	emitter03.resize(GRID_X * GRID_Y * GRID_Z, -1);
	emitter04.resize(GRID_X * GRID_Y * GRID_Z, -1);
	emitterJet.resize(GRID_X * GRID_Y * GRID_Z, -1);
	
	cellType.resize(GRID_X * GRID_Y * GRID_Z, 0);

	xamp::accelerator accelerator;

	boom::Entropy sim(accelerator, GRID_X, GRID_Y, GRID_Z);
	sim_ptr = &sim;

	boom::fd::fillFloat(sim.getU(), 0.f);
	boom::fd::fillFloat(sim.getV(), 0.f);
	boom::fd::fillFloat(sim.getW(), 0.f);
	boom::fd::fillFloat(sim.getDens(), 0.f);
	boom::fd::fillFloat(sim.getTemperature(), 0.f);
	boom::fd::fillInt(sim.getCellType(), boom::constants::FLUID);
	boom::fd::fillInt(sim.getFixedEmitters(), -1);
	
	sim.initBoundary();

	///emitter(density, temperature, v, sim.getDens(), sim.getTemperature(), sim.getV(), GRID_X, GRID_Y, GRID_Z);

	for (int i = 0; i < GRID_X; i++) {
		for (int j = 0; j < GRID_Y; j++) {
			for (int k = 0; k < GRID_Z; k++) {

				int width = 5;
				int height = 10;
				int depth = 10;

				/*if (i > (GRID_X / 2) - width && i < (GRID_X / 2) + width && j < height + 6 && j > 6 &&
					k >(GRID_Z / 2) - depth && k < (GRID_Z / 2)) {

					emitter00[inx3d(i, j, k, GRID_X, GRID_Y)] = 0;
				}

				if (i > (25) - width && i < (25) + width && j < height + 6 && j > 6 &&
					k >(25) - depth && k < (25)) {

					emitter01[inx3d(i, j, k, GRID_X, GRID_Y)] = 0;
				}

				if (i > (GRID_X - 25) - width && i < (GRID_X - 25) + width && j < height + 6 && j > 6 &&
					k >(25) - depth && k < (25)) {

					emitter02[inx3d(i, j, k, GRID_X, GRID_Y)] = 0;
				}

				if (i > (25) - width && i < (25) + width && j < height + 6 && j > 6 &&
					k >(GRID_Z - 25) - depth && k < (GRID_Z - 25)) {

					emitter03[inx3d(i, j, k, GRID_X, GRID_Y)] = 0;
				}

				if (i > (GRID_X - 25) - width && i < (GRID_X - 25) + width && j < height + 6 && j > 6 &&
					k >(GRID_Z - 25) - depth && k < (GRID_Z - 25)) {

					emitter04[inx3d(i, j, k, GRID_X, GRID_Y)] = 0;
				}*/

				if (i > (GRID_X) - width && i < (GRID_X) + width && j < (GRID_Y / 2) + height && j >(GRID_Y / 2) - height &&
					k >(GRID_Z / 2) - depth && k < (GRID_Z / 2)) {

					emitterJet[inx3d(i, j, k, GRID_X, GRID_Y)] = 0;
				}
			}
		}
	}

	//sim.addFixedEmitter(emitter00, emitter00Settings);
	//sim.addFixedEmitter(emitter01, emitter01Settings);
	//sim.addFixedEmitter(emitter02, emitter02Settings);
	//sim.addFixedEmitter(emitter03, emitter03Settings);
	//sim.addFixedEmitter(emitter04, emitter04Settings);

	sim.addFixedEmitter(emitterJet, emitterJetSettings);


	int FRAMES = 2000;
	for (int frame = 0; frame < FRAMES; frame++) {
		cout << "Frame.." << frame << endl;

		// emitter(dens, temperature, u, v);


		for(int st = 0; st < 3; st++) sim.step();


		openvdb::FloatGrid::Ptr grid = openvdb::FloatGrid::create(0.0);
		grid->setName("density");

		auto accessor = grid->getAccessor();

		xamp::copy(sim_ptr->getDens(), density.begin());
		xamp::copy(sim_ptr->getCellType(), cellType.begin());

		for (int i = 0; i < GRID_X; i++) {
			for (int j = 0; j < GRID_Y; j++) {
				for (int k = 0; k < GRID_Z; k++) {

					float value = density[inx3d(i, j, k, GRID_X, GRID_Y)];
					if (value > 0 && cellType[inx3d(i, j, k, GRID_X, GRID_Y)] != boom::constants::BOUNDARY) {
						accessor.setValue(openvdb::Coord(i, j, k), value);
					}

				}
			}
		}

		std::string path = "C:/test/smoke_r&D05/";
		std::string full_path = path + "mygrids_" + std::to_string(frame) + ".vdb";
		openvdb::io::File file(full_path);

		openvdb::GridPtrVec grids;
		grids.push_back(grid);

		file.write(grids);
		file.close();
	}
	
	return 0;
}

int inx3d(int i, int j, int k, int GRID_SIZE_X, int GRID_SIZE_Y) {

	int index = i + j * GRID_SIZE_X + GRID_SIZE_X * GRID_SIZE_Y * k;

	return index;
}

void emitter(std::vector<float> &dens, std::vector<float> &temp, std::vector<float> &v, xamp::array<float> &_dens, xamp::array<float> &_temp,  xamp::array<float> &_v, 
	int GRID_SIZE_X, int GRID_SIZE_Y, int GRID_SIZE_Z) {
	
	for (int i = 0; i < GRID_SIZE_X; i++) {
		for (int j = 0; j < GRID_SIZE_Y; j++) {
			for (int k = 0; k < GRID_SIZE_Z; k++) {


				int width = 5;
				int height = 10;
				int depth = 10;

				if (i > (GRID_SIZE_X / 2) - width && i < (GRID_SIZE_X / 2) + width && j < height + 6 && j > 6 &&
					k >(GRID_SIZE_Z / 2) - depth && k < (GRID_SIZE_Z / 2)) {
					
					dens[inx3d(i, j, k, GRID_X, GRID_Y)] = 2;
					temp[inx3d(i, j, k, GRID_X, GRID_Y)] = 500;
					//v[inx3d(i, j, k, GRID_X, GRID_Y)] = 100;
				}
			}
		}
	}

	xamp::copy(dens.begin(), _dens);
	xamp::copy(temp.begin(), _temp);
	xamp::copy(v.begin(), _v);
}


#endif // !ENTROPY_LIB