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

#include "SctSimulator.h"
#include "SctIntergrationFunctions.h"
#include "SctSimulationFunctions.h"

namespace sct{
	
	void Simulator::init(sct::ParticleSystem *system, SimulationConfig config) {

	}
	
	void Simulator::step(sct::ParticleSystem *system, SimulationConfig config, SimulationCallback callback, void *userdata) {
		if (system->nbParticles() <= 0) return;

		float dt = 0.001f;
		float targetTime = 0.f;
		bool useAdaptiveDt = false;

		if (config.timestepMethod == TimestepMethod::ADAPTIVE) {
			useAdaptiveDt = true;
			targetTime = 1.f / (float)config.fps;
		}
		else if (config.timestepMethod == TimestepMethod::FPS) {
			targetTime = 1.f / (float)config.fps;
			dt = config.timestep;
		}
		else {
			targetTime = config.timestep * (float)config.substeps;
			dt = config.timestep;
		}

		int stepCounter = 0;
		float totalTime = 0.f;
		while (totalTime < targetTime) {
			sct::clear_forces(
				system->nbParticles(),
				system->forces());

			sct::calc_gravity(
				system->nbParticles(),
				config.gravity,
				system->forces(),
				system->alive()
			);

			sct::calc_intermediate_velocity_position(
				system->nbParticles(),
				dt,
				system->velocity(),
				system->positions(),
				system->positions_i(),
				system->alive(),
				system->type(),
				system->forces()
			);

			sct::calc_final_velocity_position(
				system->nbParticles(),
				dt,
				system->velocity(),
				system->positions(),
				system->positions_i(),
				system->alive(),
				system->type(),
				system->forces()
			);
		}
	}

};