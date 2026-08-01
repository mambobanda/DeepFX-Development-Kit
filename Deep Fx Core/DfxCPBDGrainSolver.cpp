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

#include "DfxCPBDGrainSolver.h"


namespace dfxcore {

	namespace pbd {

		GrainSolver::GrainSolver(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
			setLogicalName("Grain Solver");

			DfxEntityPropertyTypes::UID *uid01 = (DfxEntityPropertyTypes::UID*)getProperty(outRef);
			uid01->setRootID(getInstanceID());

			setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);
			setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inTrigger, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inTimestep, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inSubsteps, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inIterations, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inParticleRadius, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inKinematicFriction, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inStaticFriction, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inCStiffness, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inVDampening, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inGravity, DfxEntityPropertyState::INPUT_STATE);

			using namespace DfxEntityPropertyTypes;

			mass = 1.f;
			imass = imass = 1.f / mass;

			*(float*)getProperty(inTimestep) = 0.002f;
			*(float*)getProperty(inSubsteps) = 6.f;
			*(float*)getProperty(inParticleRadius) = 0.02f;
			*(float*)getProperty(inStaticFriction) = .02f;
			*(float*)getProperty(inKinematicFriction) = .04f;
			*(float*)getProperty(inIterations) = 5.f;
			*(float*)getProperty(inCStiffness) = 1.f;

			((Float3*)getProperty(inGravity))->x = 0.f;
			((Float3*)getProperty(inGravity))->y = -9.81f;
			((Float3*)getProperty(inGravity))->z = 0.f;
		}

		GrainSolver::~GrainSolver() {
			done();
		}

		void GrainSolver::compute() {

			if (propertyHasBinding(inRef)) {

				if (nbparticles > 0) {

					int substeps = (int)*(float*)getProperty(inSubsteps);
					int iterations = (int)*(float*)getProperty(inIterations);
					float pradius = *(float*)getProperty(inParticleRadius);
					float rdistance = pradius * 2;
					float h = 4.f * pradius;

					int curr_iter = 0;

					glm::vec3 *forces = new glm::vec3[nbparticles];
					glm::vec3 *lastX = new glm::vec3[nbparticles];

					auto robjects = findRigidObjects();

					for (size_t inx = 0; inx < substeps; inx++) {

						calc_forces(particles, nbparticles, forces);

						intergrate_velocity_postion(particles, nbparticles, forces, lastX);

						calcInterpenetrationDistances(particles, nbparticles);

						while (curr_iter < iterations) {

							solvePenetrationConstraints(particles, nbparticles, lastX);			
							curr_iter++;
						}

						curr_iter = 0;

						while (curr_iter < iterations) {

							solveFrictionConstraints(particles, nbparticles, lastX);
							curr_iter++;
						}

						curr_iter = 0;

						calcFinalPositions(particles, nbparticles, lastX);

						resolveCollisions(particles, nbparticles, lastX);

						compactSearch.findNeighbours(particles, nbparticles, h, pradius);
					}

					delete[] forces;
					delete[] lastX;


					makeChildrenDirty();
				}
			}

			getDataInstance()->makeClean();
		}

		void GrainSolver::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void GrainSolver::init() {

		}

		void GrainSolver::postInit() {
			using namespace DfxEntityPropertyTypes;

			DfxGlobal::info::message("Initializing Grain Solver...");

			bool  validparticleRadius = false;
			float pradius = *(float*)getProperty(inParticleRadius);

			UID *uid = (UID*)getProperty(outRef);
			uid->clearChildren();

			if (propertyHasBinding(inRef)) {
				uid->addChild((UID*)getProperty(inRef));
			}

			//auto rigids = findRigidObjects();

			//auto bulletRigids = findBulletRigidObjects();

			//auto emitters = findEmitters();

			auto grains = findGrainObjects();

			auto robjects = findRigidObjects();

			auto rpobjects = findPointRigidObjects();

			nbparticles = 0;

			for (auto elem : grains) {
				((PBDGrains*)elem)->pingParents();
				((PBDGrains*)elem)->pingParents();
				nbparticles += ((PBDGrains*)elem)->particleCount();
			}

			for (auto elem : robjects) {
				((ParticleRigidObject*)elem)->pingParents();
			}

			for (auto elem : rpobjects) {
				((GrainPointRigid*)elem)->pingParents();
				nbparticles += ((GrainPointRigid*)elem)->particleCount();

			}

			/*
			for (auto elem : bulletRigids) {
				for (auto handle : ((SPHBulletRigid*)elem)->getRigidHandles()) {
					nbparticles += handle->particleCount();
				}

			for (auto elem : emitters) {
				((SPHShapeEmitter*)elem)->pingParents();
				nbparticles += ((SPHShapeEmitter*)elem)->particleCount();
			}
			*/

			pingParents(false);

			if (nbparticles > 0) {
				//loadProperties();
				if (pradius <= 0) {
					validparticleRadius = false;
					nbparticles = 0;
					DfxGlobal::console::println("Grain Solver: Invalid Particle Radius");
				}
				else {
					validparticleRadius = true;
				}

				if (validparticleRadius) {
					//Global::getInstance()->reset(h, Global::CUBIC_KERNEL);

					particles = new Particle*[nbparticles];

					int counter = 0;

					for (auto elem : grains) {
						PBDGrains *_elem = (PBDGrains*)elem;

						for (int inx = 0; inx < _elem->particleCount(); inx++) {
							_elem->getParticle(inx)->setIndex(counter);
							particles[counter] = _elem->getParticle(inx);
							counter++;
						}
					}

					for (auto elem : rpobjects) {

						GrainPointRigid *_elem = (GrainPointRigid*)elem;

						for (int inx = 0; inx < _elem->particleCount(); inx++) {
							_elem->getParticle(inx)->setIndex(counter);
							particles[counter] = _elem->getParticle(inx);
							counter++;
						}

					}

					/*
					for (auto elem : bulletRigids) {

						for (auto handle : ((SPHBulletRigid*)elem)->getRigidHandles()) {

							for (size_t inx = 0; inx < handle->particleCount(); inx++) {
								handle->getParticle(inx)->setIndex(counter);
								particles[counter] = handle->getParticle(inx);
								counter++;
							}
						}
					}

					for (auto elem : emitters) {
						SPHShapeEmitter *_elem = (SPHShapeEmitter*)elem;

						for (int inx = 0; inx < _elem->particleCount(); inx++) {
							_elem->getParticle(inx)->setIndex(counter);
							particles[counter] = _elem->getParticle(inx);
							counter++;
						}
					}*/

				}
			}

			DfxGlobal::info::message("");
		}

		void GrainSolver::clear() {

		}

		void GrainSolver::done() {

			delete[] particles;
			particles = nullptr;

			nbparticles = 0;
		}

		void GrainSolver::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> GrainSolver::exportData() {
			return mapEntityProperties(this);
		}

		bool GrainSolver::isExportable() {
			return true;
		}

		void *GrainSolver::creator(DfxGlobalEntityName uniquename) {
			return new GrainSolver(uniquename);
		}

		void GrainSolver::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inTrigger, "trigger", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inTimestep, "timestep", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inSubsteps, "substeps", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inIterations, "iterations", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inParticleRadius, "particle_radius", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inKinematicFriction, "kinematic_friction", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inStaticFriction, "static _friction", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inCStiffness, "stiffness", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inVDampening, "velocity_dampening", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inGravity, "gravity", new DfxEntityPropertyTypes::Float3PropertyType());
		}

		size_t GrainSolver::getParticleCount() {
			return nbparticles;
		}

		Particle *GrainSolver::getParticle(size_t inx) {
			return particles[inx];
		}

		std::vector<DfxCSimulationEntity*> GrainSolver::findGrainObjects() {
			using namespace DfxEntityPropertyTypes;

			std::vector<DfxCSimulationEntity*> robjects;

			if (propertyHasBinding(inRef)) {

				UID *uid = (UID*)getProperty(inRef);

				auto eobjects = dfxutil::traitUIDSearch(*uid, pbd::PBDGrains_Identifier);

				for (auto eobj : eobjects) {
					robjects.push_back((DfxCSimulationEntity*)eobj);
				}
			}

			return robjects;
		}

		void GrainSolver::calc_forces(Particle **particles, int nbParticles, glm::vec3 *forces) {

			using namespace DfxEntityPropertyTypes;

			float viscosity = 0.01f;
			float pradius = *(float*)getProperty(inParticleRadius);
			float rdistance = pradius * 2;
			float h = 4.f * pradius;
			float h2 = h * h;

			glm::vec3 gravity = glm::vec3(((Float3*)getProperty(inGravity))->x,
				((Float3*)getProperty(inGravity))->y,
				((Float3*)getProperty(inGravity))->z);

			//calc_densities(particles, nbParticles);

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != GRANULAR_PARTICLE)
						continue;

					forces[i] = gravity;
				}
			}

			//#pragma omp parallel default(shared)
			//			{
			//#pragma omp for schedule(static)
			//				for (int i = 0; i < nbParticles; i++) {
			//					for (int j = 0; j < particles[i]->nbCount; j++) {
			//						Particle *np = particles[i]->nbs[j].neighbour;
			//
			//						if (!np->isAlive()) continue;
			//
			//						glm::vec3 xixj = particles[i]->x - np->x;
			//						glm::vec3 gradw = cubicSplineGradW(xixj);
			//						glm::vec3 vxvj = particles[i]->v - np->v;
			//
			//						if (np->getType() == GRANULAR_PARTICLE) {
			//							glm::glm::vec3 ai = 2.0f * viscosity * (sph_mass / np->density) * vxvj * (glm::dot(xixj, gradw)) / (pow2(glm::length(xixj) + 0.01f*h2));
			//							forces[i] += ai;
			//						}
			//					}
			//				}
			//			}
		}

		void GrainSolver::intergrate_velocity_postion(Particle **particles, int nbParticles, glm::vec3 *forces, glm::vec3 *lastX) {
			float dt = *(float*)getProperty(inTimestep);

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != GRANULAR_PARTICLE)
						continue;

					particles[i]->v += forces[i] * dt;
					lastX[i] = particles[i]->x;
					particles[i]->x += particles[i]->v * dt;
				}
			}
		}

		void GrainSolver::solvePenetrationConstraints(Particle **particles, int nbParticles, glm::vec3 *lastX) {

			float cstiffness = *(float*)getProperty(inCStiffness);
			float pradius = *(float*)getProperty(inParticleRadius);
			float rdistance = pradius * 2;
			float h = 4.f * pradius;

			PenetrationConstraint pconstraint(rdistance, cstiffness, imass);

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != GRANULAR_PARTICLE)
						continue;

					pconstraint.solveConstraint(particles, particles[i], lastX);
				}
			}
		}


		void GrainSolver::calcInterpenetrationDistances(Particle **particles, int nbParticles) {

			float pradius = *(float*)getProperty(inParticleRadius);
			float rdistance = pradius * 2;
			float h = 4.f * pradius;

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != GRANULAR_PARTICLE)
						continue;

					for (int j = 0; j < particles[i]->nbCount; j++) {
						Particle *np = particles[i]->nbs[j].neighbour;
						if (!np->isAlive()) continue;

						particles[i]->nbs[j].interDistance = glm::length(particles[i]->x - np->x) - rdistance;
					}

				}
			}
		}


		void GrainSolver::solveFrictionConstraints(Particle **particles, int nbParticles, glm::vec3 *lastX) {

			float staticFriction = *(float*)getProperty(inStaticFriction);
			float kinematicFriction = *(float*)getProperty(inKinematicFriction);
			float pradius = *(float*)getProperty(inParticleRadius);
			float rdistance = pradius * 2;
			float h = 4.f * pradius;

			FrictionConstraint fconstraint(kinematicFriction, staticFriction, rdistance, imass);

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != GRANULAR_PARTICLE)
						continue;

					fconstraint.solveConstraint(particles, particles[i], lastX);
				}
			}
		}

		void GrainSolver::solveDistanceConstraints(Particle **particles, int nbParticles, glm::vec3 *lastX) {

			float pradius = *(float*)getProperty(inParticleRadius);
			float rdistance = pradius * 2;

			DistanceConstraint sconstraint(particles, constraint_array, rdistance, imass);

			//#pragma omp parallel default(shared)
			//{
			//	#pragma omp for schedule(static)
			for (int i = 0; i < nbParticles; i++) {
				if (!particles[i]->isAlive()) continue;

				if (particles[i]->getType() != GRANULAR_PARTICLE)
					continue;

				sconstraint.solveConstraint(particles, particles[i], lastX);
			}
			//}
		}

		void GrainSolver::calcFinalPositions(Particle **particles, int nbParticles, glm::vec3 *lastX) {

			float dt = *(float*)getProperty(inTimestep);
			float vdampening = *(float*)getProperty(inVDampening);

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != GRANULAR_PARTICLE)
						continue;

					particles[i]->v = (particles[i]->x - lastX[i]) / dt;
					particles[i]->v -= particles[i]->v * vdampening;
				}
			}
		}

		void GrainSolver::init_rigid_constraints(Particle **particles, int nbParticles) {

			for (int i = 0; i < nbParticles; i++) {
				if (!particles[i]->isAlive()) continue;

				if (particles[i]->getType() != GRANULAR_PARTICLE)
					continue;

				for (int j = 0; j < particles[i]->nbCount; j++) {
					Particle *np = particles[i]->nbs[j].neighbour;
					if (!np->isAlive()) continue;

					if (np->getType() != GRANULAR_PARTICLE) continue;

					constraint_array[i].push_back(np->getIndex());
				}
			}
		}

		std::vector<DfxCSimulationEntity*> GrainSolver::findRigidObjects() {
			using namespace DfxEntityPropertyTypes;

			std::vector<DfxCSimulationEntity*> robjects;

			if (propertyHasBinding(inRef)) {

				UID *uid = (UID*)getProperty(inRef);

				auto eobjects = dfxutil::traitUIDSearch(*uid, ParticleRigidObject_Identifier);

				for (auto eobj : eobjects) {
					robjects.push_back((DfxCSimulationEntity*)eobj);
				}
			}

			return robjects;
		}

		void GrainSolver::resolveCollisions(Particle **particles, int nbParticles, glm::vec3 *lastPos) {

			auto robjects = findRigidObjects();

			for (auto robj : robjects) {
				for (size_t inj = 0; inj < nbparticles; inj++) {
					auto _robj = (ParticleRigidObject*)robj;

					if (_robj->inBounds(particles[inj]->x)) {
						glm::vec3 newPos;
						glm::vec3 newVel;

						_robj->resolveCollison(particles[inj]->x,
							lastPos[inj],
							particles[inj]->v,
							newPos,
							newVel);

						particles[inj]->x = newPos;
						particles[inj]->v = newVel;
					}
				}
			}
		}

		std::vector<DfxCSimulationEntity*> GrainSolver::findPointRigidObjects() {
			using namespace DfxEntityPropertyTypes;

			std::vector<DfxCSimulationEntity*> robjects;

			if (propertyHasBinding(inRef)) {

				UID *uid = (UID*)getProperty(inRef);

				auto eobjects = dfxutil::traitUIDSearch(*uid, GrainPointRigid_Identifier);

				for (auto eobj : eobjects) {
					robjects.push_back((DfxCSimulationEntity*)eobj);
				}
			}

			return robjects;
		}
	}
}