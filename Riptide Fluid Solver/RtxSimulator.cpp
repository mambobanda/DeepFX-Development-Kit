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
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include <string>
#include "RtxSimulator.h"
#include "RtxSPHSystem.h"

namespace rtx {

	void Simulator::init(rtx::SPHSystem *system, SimulationConfig config) {
		
		if (config.searchMethod == 0) {
			CompactSearch search;
			search.findNeighbours(
				system->nbParticles(),
				system->nbListSize(),
				system->h(),
				system->pradius(),
				system->positions(),
				system->alive(),
				system->type(),
				system->nbCount(), 
				system->neighbours());
		}

		rtx::calc_boundary_masses(
			system->kernel(),
			system->nbParticles(),
			system->nbListSize(),
			system->rdensity(),
			system->positions(),
			system->alive(),
			system->type(),
			system->nbCount(),
			system->neighbours(),
			system->boundary_mass());

		rtx::calc_densities(
			system->kernel(),
			config.boundaryMethod,
			system->nbParticles(),
			system->nbListSize(),
			system->mass(),
			system->positions(),
			system->alive(),
			system->type(),
			system->nbCount(),
			system->neighbours(),
			system->boundary_mass(),
			system->density());

		rtx::calc_alpha(
			system->kernel(),
			config.boundaryMethod,
			system->nbParticles(),
			system->nbListSize(),
			system->mass(),
			system->positions(),
			system->alive(),
			system->type(),
			system->nbCount(),
			system->neighbours(),
			system->boundary_mass(),
			system->alpha());
	}

	void Simulator::step(rtx::SPHSystem *system, rtx::SimulationConfig config, SimulationCallback callback, LoggingCallback log, void *userdata) {
		if (system->nbParticles() <= 0) return;

		auto RTX_LOG = [log](const char* msg){
			if (log != nullptr) {
				log(msg);
			}
		};

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

		Timer stepTimer;
		stepTimer.startTimer();
		RTX_LOG("--------------------------");
		RTX_LOG("Rtx Simulation Stepping");
		RTX_LOG(std::string("  -Particle Count: " + std::to_string(system->nbParticles())).c_str());
		while (totalTime < targetTime) {
			Timer substepTimer;
			substepTimer.startTimer();			

			// Calculate forces
			rtx::calc_gravity(
				system->nbParticles(),
				config.gravity,
				system->alive(),
				system->type(),
				system->forces());

			rtx::calc_viscosity(
				system->kernel(),
				config.boundaryMethod,
				system->nbParticles(),
				system->nbListSize(),
				system->mass(),
				system->h(),
				system->useViscosity(),
				system->viscosity(),
				system->positions(),
				system->velocity(),
				system->alive(),
				system->type(),
				system->nbCount(),
				system->neighbours(),
				system->boundary_mass(),
				system->forces(),
				system->density());

			rtx::calc_vorticity_confinement(
				system->kernel(),
				config.boundaryMethod,
				system->nbParticles(),
				system->nbListSize(),
				system->mass(),
				system->positions(),
				system->velocity(),
				system->alive(),
				system->type(),
				system->nbCount(),
				system->neighbours(),
				system->density(),
				system->forces(),
				system->useVorticity(),
				system->vorticity());

			rtx::calc_surface_tension(
				system->kernel(),
				config.boundaryMethod,
				system->nbParticles(),
				system->nbListSize(),
				system->mass(),
				system->pradius(),
				system->rdensity(),
				system->positions(),
				system->velocity(),
				system->alive(),
				system->type(),
				system->nbCount(),
				system->neighbours(),
				system->density(),
				system->forces(),
				system->boundary_mass(),
				system->useSurfaceTension(),
				system->surfaceTension());

			for (auto &heater : system->heaters()) {
				heater.heat(
					system->nbParticles(),
					system->alive(),
					system->frozen(),
					system->type(),
					system->positions(),
					system->viscosity()
				);
			}

			rtx::freeze(
				system->nbParticles(),
				system->alive(),
				system->frozen(),
				system->type(),
				system->forces());
			
			if (useAdaptiveDt) {
				dt = rtx::updateTimeStepSizeCFL(
					system->nbParticles(),
					system->velocity(),
					system->forces(),
					dt,
					system->pradius(),
					config.min_timestep,
					config.max_timestep);
			}

			// Advance Velocity
			rtx::advance_velocity(
				system->nbParticles(),
				dt,
				system->velocity(),
				system->velocity_i(),
				system->alive(),
				system->type(),
				system->forces());

			// Correct Density Error
			rtx::correct_density_error(
				system->kernel(),
				config.boundaryMethod,
				system->nbParticles(),
				system->nbListSize(),
				system->mass(),
				system->rdensity(),
				config.density_error,
				dt,
				system->h(),
				config.pressure_stiffness,
				system->positions(),
				system->velocity_i(),
				system->alive(),
				system->type(),
				system->nbCount(),
				system->neighbours(),
				system->boundary_mass(),
				system->density(),
				system->density_i(),
				system->density_gradient(),
				system->alpha(),
				system->pressure_force(),
				system->ki00());

			// Update Positions
			rtx::calc_position(
				system->nbParticles(),
				dt,
				system->frozen(),
				system->positions(),
				system->velocity_i(),
				system->alive(),
				system->type());

			if(callback != nullptr) callback(stepCounter, dt, system, userdata);

			Timer nbSearchTimer;
			nbSearchTimer.startTimer();
			if (config.searchMethod == 0) {
				CompactSearch search;
				search.findNeighbours(
					system->nbParticles(),
					system->nbListSize(),
					system->h(),
					system->pradius(),
					system->positions(),
					system->alive(),
					system->type(),
					system->nbCount(),
					system->neighbours());
			}
			else {
				system->findNeighbours();
			}
			nbSearchTimer.stopTimer();
			std::string log_msg = "  -RTX Find Neighbours: " + std::to_string(nbSearchTimer.getTime() / 1000) + "s";
			RTX_LOG(log_msg.c_str());

			rtx::calc_densities(
				system->kernel(),
				config.boundaryMethod,
				system->nbParticles(),
				system->nbListSize(),
				system->mass(),
				system->positions(),
				system->alive(),
				system->type(),
				system->nbCount(),
				system->neighbours(),
				system->boundary_mass(),
				system->density());

			rtx::calc_alpha(
				system->kernel(),
				config.boundaryMethod,
				system->nbParticles(),
				system->nbListSize(),
				system->mass(),
				system->positions(),
				system->alive(),
				system->type(),
				system->nbCount(),
				system->neighbours(),
				system->boundary_mass(),
				system->alpha());

			// Correct Divergence
			rtx::correct_divergence_error(
				system->kernel(),
				config.boundaryMethod,
				system->nbParticles(),
				system->nbListSize(),
				system->mass(),
				system->rdensity(),
				config.coherence_error,
				dt,
				system->h(),
				system->positions(),
				system->velocity_i(),
				system->alive(),
				system->type(),
				system->nbCount(),
				system->neighbours(),
				system->boundary_mass(),
				system->density_gradient(),
				system->alpha(),
				system->pressure_force(),
				system->ki01());

			// Correct Velocites
			rtx::damp_velocity(
				system->nbParticles(),
				config.velocity_dampening,
				system->frozen(),
				system->velocity(),
				system->velocity_i(),
				system->alive(),
				system->type());

			totalTime += dt;
			stepCounter++;

			substepTimer.stopTimer();
			log_msg = "  -RTX Substep: " + std::to_string(substepTimer.getTime() / 1000.0) + "s";
			RTX_LOG(log_msg.c_str());
		}
		stepTimer.stopTimer();
		std::string log_msg = "  -RTX Total Step: " + std::to_string(stepTimer.getTime() / 1000) + "s\n";
		RTX_LOG(log_msg.c_str());
	}
}