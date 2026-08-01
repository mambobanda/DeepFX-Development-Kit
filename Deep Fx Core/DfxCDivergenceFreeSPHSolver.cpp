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

#include "DfxCBulletSolver.h"
#include "DfxCDivergenceFreeSPHSolver.h"

namespace dfxcore {

	namespace sph {

		DivergenceFreeSPHSolver::DivergenceFreeSPHSolver(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
			setLogicalName("DFSPH Solver");

			*((size_t*)getProperty(outSolverID)) = getInstanceID();

			DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outSolverID);
			uid->setRootID(getInstanceID());

			setPropertyState(outSolverID, DfxEntityPropertyState::OUTPUT_STATE);
			setPropertyState(outTrigger, DfxEntityPropertyState::OUTPUT_STATE);
			setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inTrigger, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inParticleRadius, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inTimestep, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inSubstep, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inDivergenceFree, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inDensityError, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inDivergenceError, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inPressureRelaxation, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inRigidCouplingStiffness, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inVelocityDampening, DfxEntityPropertyState::INPUT_STATE);
			//setPropertyState(inViscosity, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inForce, DfxEntityPropertyState::INPUT_STATE);

			((DfxEnum*)getProperty(inDivergenceFree))->add("No");
			((DfxEnum*)getProperty(inDivergenceFree))->add("Yes");

			nbparticles = 0;
			particles = nullptr;

			bmasses = nullptr;
			lastX = nullptr;
			falpha = nullptr;
			fbeta = nullptr;
			rhoi = nullptr;
			rho_grad = nullptr;
			vi = nullptr;
			forces = nullptr;
			psiForce = nullptr;

			validparticleRadius = false;
			
			defaultSettings();
		}

		DivergenceFreeSPHSolver::~DivergenceFreeSPHSolver() {

		}

		void DivergenceFreeSPHSolver::loadProperties() {

			timestep			= *(float*)getProperty(inTimestep);
			pradius      		= *(float*)getProperty(inParticleRadius);
			substep				= *(float*)getProperty(inSubstep);
			diveregenceFree		= ((DfxEnum*)getProperty(inDivergenceFree))->getCurrentIndex() == 0 ? false : true;
			densityError		= *(float*)getProperty(inDensityError);
			divergenceError		= *(float*)getProperty(inDivergenceError);
			pressureRelaxation	= *(float*)getProperty(inPressureRelaxation);
			rigidCouplingStiffness = *(float*)getProperty(inRigidCouplingStiffness);
			velocityDampening	= *(float*)getProperty(inVelocityDampening);
			viscosity			= *(float*)getProperty(inViscosity);
			force				= *(DfxEntityPropertyTypes::Float3*)getProperty(inViscosity);
		}

		void DivergenceFreeSPHSolver::syncSettings() {
			loadProperties();

			rdensity = 1000.f;
			float diam = pradius * 2.f;
			mass = 0.8f * diam * diam * diam * rdensity;
			h = 4.0f * pradius;
			invh = 1.f / h;
		}

		void DivergenceFreeSPHSolver::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> DivergenceFreeSPHSolver::exportData() {
			return mapEntityProperties(this);
		}

		bool DivergenceFreeSPHSolver::isExportable() {
			return true;
		}

		void *DivergenceFreeSPHSolver::creator(DfxGlobalEntityName uniquename) {
			return new DivergenceFreeSPHSolver(uniquename);
		}

		void DivergenceFreeSPHSolver::init() {

		}

		void DivergenceFreeSPHSolver::postInit() {

			using namespace DfxEntityPropertyTypes;

			DfxGlobal::info::message("Initializing DFSPH Solver...");

			UID *uid = (UID*)getProperty(outSolverID);
			uid->clearChildren();

			if (propertyHasBinding(inRef)) {
				uid->addChild((UID*)getProperty(inRef));
			}

			auto rigids = findRigidObjects();

			auto bulletRigids = findBulletRigidObjects();

			auto liquids = findLiquidObjects();

			auto emitters = findEmitters();

			nbparticles = 0;

			for (auto elem : rigids) {
				((SPHRigid*)elem)->pingParents();
				nbparticles += ((SPHRigid*)elem)->particleCount();

			}

			for (auto elem : bulletRigids) {
				for (auto handle : ((SPHBulletRigid*)elem)->getRigidHandles()) {
					nbparticles += handle->particleCount();
				}
			}

			for (auto elem : liquids) {
				((SPHLiquid*)elem)->pingParents();
				nbparticles += ((SPHLiquid*)elem)->particleCount();
			}

			for (auto elem : emitters) {
				((SPHShapeEmitter*)elem)->pingParents();
				nbparticles += ((SPHShapeEmitter*)elem)->particleCount();
			}

			pingParents(false);
			syncSettings();
			

			if (nbparticles > 0) {	
				//loadProperties();
				if (pradius <= 0) {
					validparticleRadius = false;
					DfxGlobal::console::println("DFSPH Solver: Invalid Particle Radius");
				}
				else {
					validparticleRadius = true;
				}

				if (validparticleRadius) {
					Global::getInstance()->reset(h, Global::CUBIC_KERNEL);

					particles = new Particle*[nbparticles];

					bmasses = new float[nbparticles]();
					falpha = new float[nbparticles]();
					fbeta = new float[nbparticles]();
					rhoi = new float[nbparticles]();
					rho_grad = new float[nbparticles]();
					lastX = new glm::vec3[nbparticles]();
					vi = new glm::vec3[nbparticles]();
					forces = new glm::vec3[nbparticles]();
					psiForce = new glm::vec3[nbparticles]();

					int counter = 0;
					for (auto elem : rigids) {

						SPHRigid *_elem = (SPHRigid*)elem;

						for (int inx = 0; inx < _elem->particleCount(); inx++) {
							_elem->getParticle(inx)->setIndex(counter);
							particles[counter] = _elem->getParticle(inx);
							counter++;
						}

					}

					for (auto elem : bulletRigids) {

						for (auto handle : ((SPHBulletRigid*)elem)->getRigidHandles()) {

							for (size_t inx = 0; inx < handle->particleCount(); inx++) {
								handle->getParticle(inx)->setIndex(counter);
								particles[counter] = handle->getParticle(inx);
								counter++;
							}
						}
					}

					for (auto elem : liquids) {
						SPHLiquid *_elem = (SPHLiquid*)elem;

						for (int inx = 0; inx < _elem->particleCount(); inx++) {
							_elem->getParticle(inx)->setIndex(counter);
							particles[counter] = _elem->getParticle(inx);
							counter++;
						}
					}

					for (auto elem : emitters) {
						SPHShapeEmitter *_elem = (SPHShapeEmitter*)elem;

						for (int inx = 0; inx < _elem->particleCount(); inx++) {
							_elem->getParticle(inx)->setIndex(counter);
							particles[counter] = _elem->getParticle(inx);
							counter++;
						}
					}

					compactSearch.findNeighbours(particles, nbparticles, h, pradius);

					//Calculate Densities and Alpha
					SPHComponents::calc_boundary_masses(particles, nbparticles, bmasses, rdensity);

					SPHComponents::calc_densities(particles, nbparticles, bmasses, mass);
					SPHComponents::calc_alpha(particles, nbparticles, bmasses, falpha, timestep, mass);
				}
			}

			DfxGlobal::info::message("");
		}

		void DivergenceFreeSPHSolver::done() {

			delete[] particles;

			delete[] bmasses;
			delete[] falpha;
			delete[] fbeta;
			delete[] rhoi;
			delete[] lastX;
			delete[] rho_grad;
			delete[] vi;
			delete[] forces;
			delete[] psiForce;

			particles = nullptr;
			nbparticles = 0;

			bmasses = nullptr;
			falpha	= nullptr;
			fbeta	= nullptr;
			rhoi	= nullptr;
			lastX   = nullptr;
			rho_grad = nullptr;
			vi		= nullptr;
			forces	= nullptr;
			psiForce = nullptr;
		}

		void DivergenceFreeSPHSolver::clear() {

		}

		void DivergenceFreeSPHSolver::load(std::ifstream &) {
			*((size_t*)getProperty(outSolverID)) = getInstanceID();
		}

		void DivergenceFreeSPHSolver::compute() {

			if (DfxSimulationCoreGlobal::getInstance()->getSimulationMode() == DfxSimulationMode::ACTIVE_MODE) {

				if (particles != nullptr && nbparticles > 0) {
					syncSettings();
				
					if (validparticleRadius) {
						Global::getInstance()->reset(h, Global::CUBIC_KERNEL);

						std::string tailmsg = "DFSPH Solver: Step(";

						auto emitters = findEmitters();
						
						auto bulletRigids = findBulletRigidObjects();

						auto bulletSolvers = findBulletSolvers();

						for (int step = 0; step < substep; step++) {
							std::string introStr = tailmsg + std::to_string(step + 1) + "/" + std::to_string(substep) + "): ";

							resetForces();
							addUniformForce();
							applyForces();

							SPHComponents::advance_velocities(particles, nbparticles, forces, vi, timestep);

							DfxGlobal::info::message(introStr + "Correcting Density Error");
							correct_density_error(particles, nbparticles, vi, bmasses, rho_grad, falpha, rhoi, psiForce);

							for (int ins = 0; ins < nbparticles; ins++) lastX[ins] = particles[ins]->x;

							SPHComponents::advance_positions(particles, nbparticles, forces, vi, timestep);

							resolveMeshCollisions(particles, nbparticles, lastX);

							for (auto emt : emitters) {
								((SPHShapeEmitter*)emt)->step(timestep, vi);
							}

							for (auto solv : bulletSolvers) {
								((bullet::DfxCBulletSolver*)solv)->stepSolver(timestep);
							}

							for (auto brgd : bulletRigids) {
								for (auto handle : ((SPHBulletRigid*)brgd)->getRigidHandles()) {
									handle->updateBulletTransform();
								}
							}

							DfxGlobal::info::message(introStr + "Finding Neighbours");
							compactSearch.findNeighbours(particles, nbparticles, h, pradius);

							SPHComponents::calc_boundary_masses(particles, nbparticles, bmasses, rdensity);

							SPHComponents::calc_densities(particles, nbparticles, bmasses, mass);

							SPHComponents::calc_alpha(particles, nbparticles, bmasses, falpha, timestep, mass);

							if (diveregenceFree) {
								DfxGlobal::info::message(introStr + "Correcting Divergence Error");
								correct_divergence_error(particles, nbparticles, vi, rho_grad, rhoi, falpha, bmasses, psiForce);
							}

							SPHComponents::damp_velocities(particles, nbparticles, vi, velocityDampening);

							for (auto brgd : bulletRigids) {
								updateBulletDynamics(((SPHBulletRigid*)brgd)->getRigidHandles(), psiForce);
							}

						}
					}
					else {
						DfxGlobal::info::message("DFSPH Solver: Invalid Particle Radius");
					}

					makePropertyBindingDirty(outSolverID);
					makePropertyBindingDirty(outTrigger);
					getDataInstance()->makeClean();
					makeChildrenDirty();

					DfxGlobal::info::message("");
				}		

			}
		}

		void DivergenceFreeSPHSolver::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void DivergenceFreeSPHSolver::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outSolverID, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(outTrigger, "execute", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inTrigger, "trigger", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inParticleRadius, "particle_radius", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inTimestep, "timestep", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inSubstep, "substep", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inDivergenceFree, "divergence_free", new DfxEntityPropertyTypes::DfxEnumPropertyType);
			DfxEntityDB::createProperty(inDensityError, "density_error", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inDivergenceError, "divergence_error", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inPressureRelaxation, "pressure_relaxation", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inRigidCouplingStiffness, "coupling_stiffness", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inVelocityDampening, "velocity_dampening", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inViscosity, "viscosity", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inForce, "uniform_force", new DfxEntityPropertyTypes::Float3PropertyType());
		}

		std::vector<DfxCSimulationEntity*> DivergenceFreeSPHSolver::findRigidObjects() {
			using namespace DfxEntityPropertyTypes;

			std::vector<DfxCSimulationEntity*> robjects;

			if (propertyHasBinding(inRef)) {

				UID *uid = (UID*)getProperty(inRef);

				auto eobjects = dfxutil::traitUIDSearch(*uid, sph::SPHRigid_Identifier);

				for (auto eobj : eobjects) {
					robjects.push_back((DfxCSimulationEntity*)eobj);
				}
			}

			return robjects;
		}

		std::vector<DfxCSimulationEntity*> DivergenceFreeSPHSolver::findBulletRigidObjects() {
			using namespace DfxEntityPropertyTypes;

			std::vector<DfxCSimulationEntity*> robjects;

			if (propertyHasBinding(inRef)) {

				UID *uid = (UID*)getProperty(inRef);

				auto eobjects = dfxutil::traitUIDSearch(*uid, sph::SPHBulletRigid_Identifier);

				for (auto eobj : eobjects) {
					robjects.push_back((DfxCSimulationEntity*)eobj);
				}
			}

			return robjects;
		}

		std::vector<DfxCSimulationEntity*> DivergenceFreeSPHSolver::findLiquidObjects() {
			using namespace DfxEntityPropertyTypes;

			std::vector<DfxCSimulationEntity*> robjects;

			if (propertyHasBinding(inRef)) {

				UID *uid = (UID*)getProperty(inRef);

				auto eobjects = dfxutil::traitUIDSearch(*uid, sph::SPHLiquid_Identifier);

				for (auto eobj : eobjects) {
					robjects.push_back((DfxCSimulationEntity*)eobj);
				}
			}

			return robjects;
		}

		std::vector<DfxCSimulationEntity*> DivergenceFreeSPHSolver::findEmitters() {
			using namespace DfxEntityPropertyTypes;

			std::vector<DfxCSimulationEntity*> robjects;

			if (propertyHasBinding(inRef)) {

				UID *uid = (UID*)getProperty(inRef);

				auto eobjects = dfxutil::traitUIDSearch(*uid, sph::SPHShapeEmitter_Identifier);

				for (auto eobj : eobjects) {
					robjects.push_back((DfxCSimulationEntity*)eobj);
				}
			}

			return robjects;
		}

		Particle *DivergenceFreeSPHSolver::getParticle(size_t inx) {
			return particles[inx];
		}

		Particle **DivergenceFreeSPHSolver::getParticles() {
			return particles;
		}

		float DivergenceFreeSPHSolver::getKernelSpacing() {
			return h;
		}

		glm::vec3 *DivergenceFreeSPHSolver::getForceChannel() {
			return forces;
		}

		size_t  DivergenceFreeSPHSolver::getParticleCount() {
			return nbparticles;
		}

		float *DivergenceFreeSPHSolver::getBoundaryMassChannel() {
			return bmasses;
		}

		float DivergenceFreeSPHSolver::getMass() {
			return mass;
		}

		void DivergenceFreeSPHSolver::calc_density_change(Particle **particles, int nbParticles, glm::vec3 *vi, float *rho_grad, 
			float *bmass) {

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != FLUID_PARTICLE)
						continue;

					float rhograd(0.0f);

					for (int j = 0; j < particles[i]->nbCount; j++) {
						Particle *np = particles[particles[i]->nbIndices[j]];

						if (!np->isAlive()) continue;

						glm::vec3 grad = Global::getInstance()->conduit().gradW(particles[i]->x - np->x);

						glm::vec3 vij = vi[i] - vi[np->getIndex()];

						if (np->getType() == FLUID_PARTICLE)
							rhograd += mass * glm::dot(vij, grad);

					}

					for (int j = 0; j < particles[i]->nbCount; j++) {
						Particle *np = particles[particles[i]->nbIndices[j]];

						if (!np->isAlive()) continue;

						glm::vec3 grad = Global::getInstance()->conduit().gradW(particles[i]->x - np->x);

						glm::vec3 vij = vi[i] - vi[np->getIndex()];

						if (np->getType() != FLUID_PARTICLE)
							rhograd += bmass[np->getIndex()] * glm::dot(vij, grad);

					}

					/*float dtGrad = (dt * rhograd);
					float rhoAdv = particles[i]->density + dtGrad;
					rhoAdv = std::max(rhoAdv, rdensity);
					rho_grad[i] = (rhoAdv - rdensity) * (1.0f/dt);*/
					float densityAdv = std::max(densityAdv, 0.0f);

					if (particles[i]->nbCount < 20)
						densityAdv = 0.0;

					rho_grad[i] = densityAdv;
				}
			}
		}

		void DivergenceFreeSPHSolver::correct_divergence_error( Particle **particles, int nbParticles, glm::vec3 *vi, 
			float *rho_grad, float *rhoi, float *alpha, float *bmass, glm::vec3 *psiForce ) {

			float dt = timestep;
			int iter = 0;
			float eta = (1.0 / dt) * divergenceError * 0.01f * rdensity;
			float rateAvg = 1e100;
			int pcount = 0;
			float _invdt = 1.f / dt;

			calc_density_change(particles, nbParticles, vi, rho_grad, bmass);

			while (((rateAvg > eta) || (iter < 2)) && (iter < 50)) {
				rateAvg = 0.0f;
				pcount = 0;

				//Adapt Velocities
				#pragma omp parallel default(shared)
				{
					#pragma omp for schedule(static)
					for (int i = 0; i < nbParticles; i++) {
						if (!particles[i]->isAlive()) continue;

						if (particles[i]->getType() != FLUID_PARTICLE)
							continue;

						float ki = rho_grad[i] * alpha[i];// * _invdt;
						float ki_rho = ki;//particles[i]->density;

						glm::vec3 pressureForce(0.0f);
						glm::vec3 boundaryPressureForce(0.0f);

						for (int j = 0; j < particles[i]->nbCount; j++) {
							Particle *np = particles[particles[i]->nbIndices[j]];

							if (!np->isAlive()) continue;

							if (np->getType() == FLUID_PARTICLE) {
								glm::vec3 grad = Global::getInstance()->conduit().gradW(particles[i]->x - np->x);

								float kj = rho_grad[np->getIndex()] * alpha[np->getIndex()]; //* _invdt;
								float kj_rho = kj;//np->density;	

								float ksum = ki_rho + kj_rho;

								if (ksum > min_e) {
									pressureForce += mass * ksum * grad;
								}
							}
						}

						for (int j = 0; j < particles[i]->nbCount; j++) {
							Particle *np = particles[particles[i]->nbIndices[j]];;

							if (!np->isAlive()) continue;

							if (np->getType() != FLUID_PARTICLE) {

								glm::vec3 grad = Global::getInstance()->conduit().gradW(particles[i]->x - np->x);
								if (ki_rho > min_e) {
									glm::vec3 _tempForce = bmass[np->getIndex()] * (ki_rho)* grad;
									pressureForce -= _tempForce;
									boundaryPressureForce -= _tempForce;
								}
							}
						}

						psiForce[i] += boundaryPressureForce;
						vi[i] -= dt * pressureForce;
					}
				}

				calc_density_change(particles, nbParticles, vi, rho_grad, bmass);

				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != FLUID_PARTICLE) continue;

					rateAvg += rho_grad[i];

					++pcount;
				}

				rateAvg = rateAvg / pcount;
				iter++;
			}
		}

		void DivergenceFreeSPHSolver::calc_density_advance(Particle **particles, int nbParticles, glm::vec3 *vi, float *rhoi, float *boundary_masses) {

			float dt = timestep;
			//Calculate Density Rate of change
			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {

					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != FLUID_PARTICLE)
						continue;

					float rhoadv(0.0f);

					for (int j = 0; j < particles[i]->nbCount; j++) {
						Particle *np = particles[particles[i]->nbIndices[j]];

						if (!np->isAlive()) continue;

						glm::vec3 grad = Global::getInstance()->conduit().gradW(particles[i]->x - np->x);

						glm::vec3 vij = vi[i] - vi[np->getIndex()];

						if (np->getType() == FLUID_PARTICLE)
							rhoadv += mass * glm::dot(grad, vij);
						else
							rhoadv += boundary_masses[np->getIndex()] * glm::dot(grad, vij);

					}

					rhoi[i] = particles[i]->density + dt*rhoadv;
					rhoi[i] = std::max(rhoi[i], rdensity);

				}
			}

		}

		//Correct Density Error
		void DivergenceFreeSPHSolver::correct_density_error(Particle **particles, int nbParticles, glm::vec3 *vi, 
			float *boundary_masses, float *rho_grad, float *alpha, float *rhoi, glm::vec3 *psiForce) {

			int iter = 0;
			//int _density_solver_iter = iter;
			float delta = 0.01f * rdensity;
			float avgComp = 0.0f;
			float eta = densityError * 0.01f * rdensity;  // maxError is given in percent
			int   pcounter = 0;
			float error = 1e100;

			float dt = timestep;
			float _dt2 = dt * dt;
			float _invdt2 = 1.f / _dt2;

			calc_density_advance(particles, nbParticles, vi, rhoi, boundary_masses);

			while (((error > eta) || (iter < 2)) && (iter < 50)) {
				//	for(int adv=0; adv < 10; adv++){
				if (iter > 100) break;

				//Reset Compression Error Variables
				pcounter = 0;
				error = 0.0f;
				avgComp = 0.0f;

				////////////////////Adapt Velocities////////////////////////
				#pragma omp parallel default(shared)
				{
					#pragma omp for schedule(static)
					for (int i = 0; i < nbParticles; i++) {
						if (!particles[i]->isAlive()) continue;

						if (particles[i]->getType() != FLUID_PARTICLE)
							continue;

						float diff = std::max(0.f, rhoi[i] - rdensity);
						float ki = diff * _invdt2 * alpha[i];
						float ki_rho = ki;//particles[i]->density;
										  //float ki_rho = std::min(ki, 0.3f);

										  /*glm::vec3 pressureForce(0.0f);*/
						glm::vec3 boundaryPressureForce(0.0f);

						for (int j = 0; j < particles[i]->nbCount; j++) {
							Particle *np = particles[particles[i]->nbIndices[j]];

							if (!np->isAlive()) continue;

							if (np->getType() == FLUID_PARTICLE) {
								glm::vec3 pressureForce(0.0f);

								glm::vec3 grad = Global::getInstance()->conduit().gradW(particles[i]->x - np->x);

								float diff2 = std::max(0.f, rhoi[np->getIndex()] - rdensity);
								float kj = diff2 * _invdt2 *   alpha[np->getIndex()];
								float kj_rho = kj;///np->density;
												  ///float kj_rho = std::min(kj, 0.3f);

								float ksum = (ki_rho + kj_rho);

								if (ksum  > (float)min_e) {
									pressureForce = pressureRelaxation * mass * (ksum)* grad;
									vi[i] -= dt * pressureForce;
								}

							}
						}

						// 
						for (int j = 0; j < particles[i]->nbCount; j++) {
							Particle *np = particles[particles[i]->nbIndices[j]];

							if (!np->isAlive()) continue;

							if (np->getType() != FLUID_PARTICLE) {
								if (ki_rho  >(float)min_e) {
									glm::vec3 pressureForce(0.0f);

									glm::vec3 grad = Global::getInstance()->conduit().gradW(particles[i]->x - np->x);

									pressureForce = pressureRelaxation * boundary_masses[np->getIndex()] * (ki_rho)* grad;
									boundaryPressureForce += pressureForce;
									//boundaryPressureForce += boundary_masses[np->getIndex()] * pressureForce * invh;


									vi[i] += -dt * pressureForce;
								}
							}
						}

						psiForce[i] += boundaryPressureForce;

					}
				}

				calc_density_advance(particles, nbParticles, vi, rhoi, boundary_masses);

				//#pragma omp for reduction(+:avgComp) schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != FLUID_PARTICLE)
						continue;

					float temp_avg = rhoi[i] - rdensity;
					avgComp += temp_avg;

					++pcounter;
				}

				error = avgComp / pcounter;

				iter++;
			}
		}

		void DivergenceFreeSPHSolver::defaultSettings() {

			using namespace DfxEntityPropertyTypes;

			*(float*)getProperty(inParticleRadius) = 0.02f;
			*(float*)getProperty(inTimestep) = 0.001f;
			*(float*)getProperty(inSubstep) = 6.f;
			((DfxEnum*)getProperty(inDivergenceFree))->setCurrentIndex(0);
			*(float*)getProperty(inDensityError) = 0.01f;
			*(float*)getProperty(inDivergenceError) = 0.1f;
			*(float*)getProperty(inPressureRelaxation) = 0.25f;
			*(float*)getProperty(inRigidCouplingStiffness) = 0.05f;
			*(float*)getProperty(inVelocityDampening) = 0.002f;
			*(float*)getProperty(inViscosity) = 0.01f;

			((Float3*)getProperty(inForce))->x = 0.f;
			((Float3*)getProperty(inForce))->y = -9.81f;
			((Float3*)getProperty(inForce))->z = 0.f;

		}

		void DivergenceFreeSPHSolver::updateBulletDynamics(std::vector<DfxCSPHBulletRigidHandle*> &handles, glm::vec3 *psiForces) {
			for (auto handle: handles) {

				if (handle->isDynamic()) {

					btVector3 _com = handle->getBulletHandle()->getBody()->getCenterOfMassPosition();
					glm::vec3 centerOfMass = glm::vec3(float(_com.getX()), float(_com.getY()), float(_com.getZ()));

					glm::vec3 *bPsiTotal = new glm::vec3[handle->particleCount()];
					glm::vec3 psiTotal = glm::vec3(0);
					glm::vec3 torque = glm::vec3(0);

					#pragma omp parallel default(shared)
					{
						#pragma omp for schedule(static)
						for (int i = 0; i < handle->particleCount(); i++) {

							Particle *particle = handle->getParticle(i);
							glm::vec3 psi_temp = glm::vec3(0);
							for (int j = 0; j < particle->nbCount; j++) {
								Particle *np = particles[particle->nbIndices[j]];

								if (np->getType() != FLUID_PARTICLE) continue;
								if (!np->isAlive()) continue;

								psi_temp += rigidCouplingStiffness * psiForces[np->getIndex()];
								//psi_temp += psiForces[np->getIndex()];
							}

							bPsiTotal[i] = psi_temp;
						}
					}

					#pragma omp parallel default(shared)
					{
						#pragma omp for schedule(static)
						for (int i = 0; i < handle->particleCount(); i++) {
							Particle *particle = handle->getParticle(i);

							psiTotal += bPsiTotal[i];
							torque += glm::cross((particle->x - (centerOfMass)), bPsiTotal[i]);

						}
					}

					handle->getBulletHandle()->getBody()->applyCentralForce(btVector3(psiTotal.x, psiTotal.y, psiTotal.z));
					handle->getBulletHandle()->getBody()->applyTorque(btVector3(torque.x, torque.y, torque.z));

					delete[] bPsiTotal;
				}
				
			}
		}

		void DivergenceFreeSPHSolver::addUniformForce() {

			using namespace DfxEntityPropertyTypes;		

			for (int i = 0; i < nbparticles; i++) {

				forces[i] += glm::vec3(((Float3*)getProperty(inForce))->x,
					((Float3*)getProperty(inForce))->y,
					((Float3*)getProperty(inForce))->z);
			}
	
		}

		void DivergenceFreeSPHSolver::resetForces() {
			
			using namespace DfxEntityPropertyTypes;

			for ( int i = 0; i < nbparticles; i++ ) {

				forces[i] = glm::vec3(0.0);
				psiForce[i] = glm::vec3(0.0);
			}
		}

		std::vector<AbstractForce*> DivergenceFreeSPHSolver::findForces() {
			using namespace DfxEntityPropertyTypes;

			std::vector<AbstractForce*> fobjects;

			if (propertyHasBinding(inRef)) {

				UID *uid = (UID*)getProperty(inRef);

				auto objects01 = dfxutil::traitUIDSearch(*uid, sph::SPHViscosity_Identifier);
				auto objects02 = dfxutil::traitUIDSearch(*uid, sph::SPHVorticity_Identifier);
				auto objects03 = dfxutil::traitUIDSearch(*uid, sph::SPHAdhesion_Identifier);


				for (auto eobj : objects01) {
					fobjects.push_back((AbstractForce*)((SPHViscosity*)eobj));
				}

				for (auto eobj : objects02) {
					fobjects.push_back((AbstractForce*)((SPHViscosity*)eobj));
				}

				for (auto eobj : objects03) {
					fobjects.push_back((AbstractForce*)((SPHViscosity*)eobj));
				}
			}

			return fobjects;
		}

		std::vector<DfxCSimulationEntity*> DivergenceFreeSPHSolver::findBulletSolvers() {
			using namespace DfxEntityPropertyTypes;

			std::vector<DfxCSimulationEntity*> objects;

			if (propertyHasBinding(inRef)) {

				UID *uid = (UID*)getProperty(inRef);
				auto objs = dfxutil::traitUIDSearch(*uid, bullet::DfxCBulletSolver_Identifier);

				for (auto obj : objs) {
					objects.push_back((DfxCSimulationEntity*)obj);
				}
			}

			return objects;
		}

		void DivergenceFreeSPHSolver::applyForces() {

			std::vector<AbstractForce*> _forces = findForces();

			for (int inx = 0; inx < _forces.size(); inx++) {

				_forces[inx]->setKernelSpacing(h);
				_forces[inx]->setForces(forces);
				_forces[inx]->setMass(mass);
				_forces[inx]->setParticles(particles, nbparticles);
				_forces[inx]->setBoundaryMasses(bmasses);

				_forces[inx]->addForce();
			}
		}

		std::vector<DfxCSimulationEntity*> DivergenceFreeSPHSolver::findMeshRigidObjects() {
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

		void DivergenceFreeSPHSolver::resolveMeshCollisions(Particle **particles, int nbParticles, glm::vec3 *lastPos) {

			auto robjects = findMeshRigidObjects();

			for (auto robj : robjects) {

				#pragma omp parallel default(shared)
				{
					#pragma omp for schedule(static)
					for (long int inj = 0; inj < nbparticles; inj++) {
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
		}

	}
}