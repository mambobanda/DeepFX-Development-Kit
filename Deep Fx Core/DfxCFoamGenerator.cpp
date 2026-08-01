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

#include "DfxConcurrentVector.h"
#include "DfxCFoamGenerator.h"

namespace dfxcore {


	FoamGenerator::FoamGenerator(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename){
		setLogicalName("Foam Generator");

		setPropertyState(outID, DfxEntityPropertyState::OUTPUT_STATE);
		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inTrigger, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inMode, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inTimestep, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inSubstep, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inMinLife, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inMaxLife, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inMass, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inCollisionMargin, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inBuoyancy, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inDrag, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inParticleSize, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inMaxAirThreshold, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inMinAirThreshold, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inMaxWaveThreshold, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inMinWaveThreshold, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inMaxEnergyThreshold, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inMinEnergyThreshold, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inMaxAirParticlesPerSec, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inMaxWaveParticlesPerSec, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inGravity, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPositions, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inVelocities, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outPositions, DfxEntityPropertyState::OUTPUT_STATE);
		setPropertyState(outVelocities, DfxEntityPropertyState::OUTPUT_STATE);

		foamParticles = nullptr;
		nbFoamParticles = 0;

		((DfxEnum*)getProperty(inMode))->add("Generate Foam");
		((DfxEnum*)getProperty(inMode))->add("Analyse Kinetic Energy");

		defaultSettings();

		dbg_index = 0;
		dbg_particle_count = 0;
		dbg_culled_particle_count = 0;

		dbg_minAirPotential = std::numeric_limits<float>::infinity();
		dbg_minAirPotentialClamped = std::numeric_limits<float>::infinity();
		dbg_maxAirPotential = -1.f;
		dbg_maxAirPotentialClamped = -1.f;

		dbg_minWavePotential = std::numeric_limits<float>::infinity();;
		dbg_minWavePotentialClamped = std::numeric_limits<float>::infinity();
		dbg_maxWavePotential = -1.f;
		dbg_maxWavePotentialClamped = -1.f;

		dbg_minEnergyPotential = std::numeric_limits<float>::infinity();;
		dbg_minEnergyPotentialClamped = std::numeric_limits<float>::infinity();
		dbg_maxEnergyPotential = -1.f;
		dbg_maxEnergyPotentialClamped = -1.f;
	}

	FoamGenerator::~FoamGenerator() {

	}

	void FoamGenerator::compute() {
		using namespace DfxEntityPropertyTypes;

		int mode = ((DfxEnum*)getProperty(inMode))->getCurrentIndex();

		if (DfxSimulationCoreGlobal::getInstance()->getSimulationMode() == DfxSimulationMode::ACTIVE_MODE) {

			if (propertyHasBinding(inPositions) && propertyHasBinding(inVelocities)) {

				Float3Array *_positions = (Float3Array*)getProperty(inPositions);
				Float3Array *_velocities = (Float3Array*)getProperty(inVelocities);

				if (_positions->size() > 0 && _velocities->size() > 0) {

					if (_positions->size() == _velocities->size()) {
						
						if (mode == 0) {
							foamGenerationMode();
						}
						else {
							kineticAnalysisMode();
						}
					}
					else {
						DfxGlobal::console::println("Foam Generator ERROR: Position or Veolocity array different sizes.");
					}
				}
				else {
					DfxGlobal::console::println("Foam Generator ERROR: Position or Veolocity arrays empty.");
				}
			}
		}

		DfxGlobal::info::message("");
		getDataInstance()->makeClean();
	}

	void FoamGenerator::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void FoamGenerator::init() {

	}

	void FoamGenerator::clear() {

	}

	void FoamGenerator::done() {
		using namespace DfxEntityPropertyTypes;

		Float3Array *out_positions  = (Float3Array*)getProperty(outPositions);
		Float3Array *out_velocities = (Float3Array*)getProperty(outVelocities);

		if (foamParticles != nullptr) {
			delete[] foamParticles;
			foamParticles = nullptr;
			nbFoamParticles = 0;
		}

		out_positions->deleteArray();
		out_velocities->deleteArray();

		dbg_minAirPotential				= 0.f;
		dbg_minAirPotentialClamped		= 0.f;
		dbg_maxAirPotential				= 0.f;
		dbg_maxAirPotentialClamped		= 0.f;
		dbg_minWavePotential			= 0.f;
		dbg_minWavePotentialClamped		= 0.f;
		dbg_maxWavePotential			= 0.f;
		dbg_maxWavePotentialClamped		= 0.f;

		dbg_index						= 0;
		dbg_particle_count				= 0;
		dbg_culled_particle_count		= 0;

		dbg_minEnergyPotential			= 0.f;
		dbg_minEnergyPotentialClamped	= 0.f;
		dbg_maxEnergyPotential			= 0.f;
		dbg_maxEnergyPotentialClamped	= 0.f;
	}

	void FoamGenerator::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> FoamGenerator::exportData() {
		return mapEntityProperties(this);
	}

	bool FoamGenerator::isExportable() {
		return true;
	}

	void *FoamGenerator::creator(DfxGlobalEntityName uniquename) {
		return new FoamGenerator(uniquename);
	}

	void FoamGenerator::initialize() {
		DfxCSimulationEntity::initialize();

		DfxEntityDB::createProperty(outID, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inTrigger, "trigger", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inMode, "mode", new DfxEntityPropertyTypes::DfxEnumPropertyType());
		DfxEntityDB::createProperty(inTimestep, "timestep", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inSubstep, "substep", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inMinLife, "min_life", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inMaxLife, "max_life", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inMass, "mass", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inCollisionMargin, "collision_margin", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inBuoyancy, "buoyancy", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inDrag, "drag", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inParticleSize, "particle_size", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inMaxAirThreshold, "max_air_potential", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inMinAirThreshold, "min_air_potential", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inMaxWaveThreshold, "max_wave_potential", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inMinWaveThreshold, "min_wave_potential", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inMaxEnergyThreshold, "max_energy_potential", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inMinEnergyThreshold, "min_energy_potential", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inMaxAirParticlesPerSec, "air_particles_per_sec", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inMaxWaveParticlesPerSec, "wave_particles_per_sec", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inGravity, "gravity", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inPositions, "in_positions", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
		DfxEntityDB::createProperty(inVelocities, "in_velocities", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
		DfxEntityDB::createProperty(outPositions, "out_positions", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
		DfxEntityDB::createProperty(outVelocities , "out_velocities", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
	}

	void FoamGenerator::defaultSettings() {
		using namespace DfxEntityPropertyTypes;

		*(float*)getProperty(inTimestep)		= 0.0166f;
		*(float*)getProperty(inSubstep)			= 1.0f;
		*(float*)getProperty(inMass)			= 1.f;
		*(float*)getProperty(inMinLife)			= 0.5f;
		*(float*)getProperty(inMaxLife)			= 1.f;
		*(float*)getProperty(inCollisionMargin) = 0.04f;
		*(float*)getProperty(inBuoyancy)		= 0.75f;
		*(float*)getProperty(inDrag)			= 0.55f;
		*(float*)getProperty(inParticleSize)	= 0.02f;
		*(float*)getProperty(inMaxAirThreshold) = 80.f;
		*(float*)getProperty(inMinAirThreshold) = 5.f;
		*(float*)getProperty(inMaxWaveThreshold) = 8.f;
		*(float*)getProperty(inMinWaveThreshold) = 2.f;
		*(float*)getProperty(inMaxEnergyThreshold) = 5.f;
		*(float*)getProperty(inMinEnergyThreshold) = 1.f;
		*(float*)getProperty(inMaxAirParticlesPerSec) = 2000.f;
		*(float*)getProperty(inMaxWaveParticlesPerSec) = 2000.f;
		((Float3*)getProperty(inGravity))->x = 0.f;
		((Float3*)getProperty(inGravity))->y = -9.83f;
		((Float3*)getProperty(inGravity))->z = 0.f;
	}

	float FoamGenerator::trappedAirKernel(glm::vec3 xij, float h) {
		float xijLength = glm::length(xij);

		if (xijLength > h || xijLength == 0.f) return 0.f;

		return (1 - xijLength / h);
	}

	float FoamGenerator::clampPotential(float potential, float min, float max) {
		return (std::min(potential, max) - std::min(potential, min)) / (max - min);
	}

	float FoamGenerator::calcAirPotential(sph::Particle **particles, sph::Particle *particle, float h) {

		float velDiff = 0.f;

		for (int j = 0; j < particle->nbCount; j++) {
			sph::Particle *np = particles[particle->nbIndices[j]];;

			if (!np->isAlive()) continue;

			glm::vec3 xij = particle->x - np->x;
			glm::vec3 vij = particle->v - np->v;

			glm::vec3 xijnorm(0);
			glm::vec3 vijnorm(0);

			if (glm::length(xij) != 0.f) xijnorm = glm::normalize(xij);

			if (glm::length(vij) != 0.f) vijnorm = glm::normalize(vij);

			float wt = trappedAirKernel(xij, h);

			velDiff += glm::length(vij) * (1.f - glm::dot(vijnorm, xijnorm)) * wt;

		}
		

		return velDiff;
	}

	float FoamGenerator::calcWavePotential(sph::Particle **particles, sph::Particle *particle, float mass, float h, glm::vec3 *normals) {

		float ki = 0.f;
		float direction = 0.0f;

		if (glm::length(normals[particle->getIndex()]) == 0.f) return 0.0f;

		glm::vec3 _ni = glm::normalize(normals[particle->getIndex()]);

		for (int j = 0; j < particle->nbCount; j++) {
			sph::Particle *np = particles[particle->nbIndices[j]];

			if (!np->isAlive()) continue;

			if (glm::length(normals[np->getIndex()]) == 0.f) continue;

			glm::vec3 xij = particle->x - np->x;

			glm::vec3 _nj = glm::normalize(normals[np->getIndex()]);

			if (glm::dot(xij, normals[particle->getIndex()]) < 0.f) {

				//ki += (1 - glm::dot(_ni, _nj)) * sph::cubicSplineW(xij);
				ki += (1 - glm::dot(_ni, _nj)) * trappedAirKernel(xij, h);;
			}
		}

		glm::vec3 _vi = glm::normalize(particle->v);

		if (glm::dot(_vi, _ni) >= 0.6) {
			direction = 1.f;
		}

		return ki * direction;
	}

	void FoamGenerator::foamGenerationMode() {
		using namespace DfxEntityPropertyTypes;

		Float3Array *_positions = (Float3Array*)getProperty(inPositions);
		Float3Array *_velocities = (Float3Array*)getProperty(inVelocities);

		DfxGlobal::info::message("Foam Generator...");

		sph::Particle **particles = new sph::Particle*[_positions->size()];
		size_t nbParticles = _positions->size();

		for (size_t i = 0; i < nbParticles; i++) {

			glm::vec3 _pos((*_positions)[i].x, (*_positions)[i].y, (*_positions)[i].z);
			glm::vec3 _vel((*_velocities)[i].x, (*_velocities)[i].y, (*_velocities)[i].z);

			sph::Particle *p = new sph::Particle(sph::FLUID_PARTICLE);
			p->x = _pos;
			p->v = _vel;
			p->setAlive(true);
			p->setIndex(i);

			particles[i] = p;
		}

		FoamParticleArray farray;

		DfxGlobal::info::message("Calaculating foam potential...");
		generateFoam(particles, nbParticles, farray);

		appendFoam(farray);
		farray.deleteArray();

		DfxGlobal::info::message("Collision resolution...");
		resolveCollisionsBVH();

		DfxGlobal::info::message("Advecting foam...");
		advectFoam(particles, nbParticles);

		DfxGlobal::info::message("Culling foam...");
		cullFoam();

		DfxGlobal::info::message("Collision resolution...");
		resolveCollisionsBVH();

		DfxGlobal::info::message("Packing particles...");
		for (size_t i = 0; i < nbParticles; i++) delete particles[i];
		delete[] particles;

		printDebugInfo();

		if (nbFoamParticles > 0) {
			Float3Array *out_positions = (Float3Array*)getProperty(outPositions);
			Float3Array *out_velocities = (Float3Array*)getProperty(outVelocities);

			out_positions->deleteArray();
			out_velocities->deleteArray();

			out_positions->resize(nbFoamParticles);
			out_velocities->resize(nbFoamParticles);

			for (size_t i = 0; i < nbFoamParticles; i++) {

				(*out_positions)[i].x = foamParticles[i].pos.x;
				(*out_positions)[i].y = foamParticles[i].pos.y;
				(*out_positions)[i].z = foamParticles[i].pos.z;

				(*out_velocities)[i].x = foamParticles[i].vel.x;
				(*out_velocities)[i].y = foamParticles[i].vel.y;
				(*out_velocities)[i].z = foamParticles[i].vel.z;
			}

			//makePropertyBindingDirty(outTrigger);
			makePropertyBindingDirty(outPositions);
			makePropertyBindingDirty(outVelocities);
			makeChildrenDirty();
		}
	}

	void FoamGenerator::kineticAnalysisMode() {
		using namespace DfxEntityPropertyTypes;

		Float3Array *_positions = (Float3Array*)getProperty(inPositions);
		Float3Array *_velocities = (Float3Array*)getProperty(inVelocities);

		DfxGlobal::info::message("Foam Generator...");

		sph::Particle **particles = new sph::Particle*[_positions->size()];
		size_t nbParticles = _positions->size();

		for (size_t i = 0; i < nbParticles; i++) {

			glm::vec3 _pos((*_positions)[i].x, (*_positions)[i].y, (*_positions)[i].z);
			glm::vec3 _vel((*_velocities)[i].x, (*_velocities)[i].y, (*_velocities)[i].z);

			sph::Particle *p = new sph::Particle(sph::FLUID_PARTICLE);
			p->x = _pos;
			p->v = _vel;
			p->setAlive(true);
			p->setIndex(i);

			particles[i] = p;
		}

		FoamGenerator::analyseKineticEnergy(particles, nbParticles);
		
		printKineticEnergyDebugInfo();

		for (size_t i = 0; i < nbParticles; i++) delete particles[i];
		delete[] particles;
	}

	void FoamGenerator::generateFoam(sph::Particle **particles, size_t nbparticles, FoamParticleArray &result) {

		size_t *foamCount = new size_t[nbparticles]();
		float *velDiffArr = new float[nbparticles]();
		float *kiArr = new float[nbparticles]();
		float *kEArr = new float[nbparticles]();

		float *iAirArr = new float[nbparticles]();
		float *iWaveArr = new float[nbparticles]();
		float *iEnergyArr = new float[nbparticles]();

		float psize		= *(float*)getProperty(inParticleSize);
		float rdensity	= 1000.f;
		float diam		= (psize* 2.f);
		float mass		= 0.8f * diam * diam * diam * rdensity;
		float h			= 4.0f * psize;

		DfxConcurrentVector<FoamParticle> _tempArray;

		glm::vec3 *normals = new glm::vec3[nbparticles];

		DfxGlobal::info::message("SPH Search...");
		sph::CompactSearch compactSearch;
		compactSearch.findNeighbours(particles, nbparticles, h, psize);

		float dt = *(float*)getProperty(inTimestep);
		float minAir = *(float*)getProperty(inMinAirThreshold);
		float maxAir = *(float*)getProperty(inMaxAirThreshold);
		float minWave = *(float*)getProperty(inMinWaveThreshold);
		float maxWave = *(float*)getProperty(inMaxWaveThreshold);
		float maxEnergy = *(float*)getProperty(inMaxEnergyThreshold);
		float minEnergy = *(float*)getProperty(inMinEnergyThreshold);
		float airParticlesPerSec = *(float*)getProperty(inMaxAirParticlesPerSec);
		float waveParticlesPerSec = *(float*)getProperty(inMaxWaveParticlesPerSec);

		sph::Global::getInstance()->reset(h, sph::Global::CUBIC_KERNEL);
		
		DfxGlobal::info::message("Calc Densities...");
		sph::SPHComponents::calc_densities(particles, nbparticles, mass);

		DfxGlobal::info::message("Calc Normals...");
		sph::SPHComponents::calc_normals(particles, nbparticles, mass, h, normals);

		sph::initCubicSplineKernel(h);
		
		#pragma omp parallel default(shared)
		{
			#pragma omp for schedule(static)
			for (int i = 0; i < nbparticles; i++) {
				if (!particles[i]->isAlive()) continue;
		
				float velDiff = calcAirPotential(particles, particles[i], h);
				float ki = calcWavePotential(particles, particles[i], mass, h, normals);

				float speed = glm::length(particles[i]->v);
				float kEnergy = 0.5f * mass * pow2(speed);
				
				float iAir = clampPotential(velDiff, minAir, maxAir);
				float iWave = clampPotential(ki, minWave, maxWave);
				float iEnergy = clampPotential(kEnergy, minEnergy, maxEnergy);

				//setAirDebug(velDiff, iAir);
				//setWaveDebug(ki, iWave);
				//setEnergyDebug(kEnergy, iEnergy);

				velDiffArr[i]	= velDiff;
				kiArr[i]		= ki;
				kEArr[i]		= kEnergy;

				iAirArr[i]		= iAir;
				iWaveArr[i]		= iWave;
				iEnergyArr[i]	= iEnergy;

				//size_t fcount = iEnergy * (iAir * particlesPerSec) * dt;
				size_t fcount = iEnergy * (iAir * airParticlesPerSec + iWave * waveParticlesPerSec);

				if (fcount > 0) {

					std::default_random_engine engine;
					engine.seed(1234);
					std::uniform_real_distribution<float> random(0.f, 1.f);

					foamCount[i] = fcount;
					//increaseParticleCount(fcount);
					
					for (size_t k = 0; k < fcount; k++) {

						float xr = random(engine);
						float x_theta = random(engine);
						float xh = random(engine);

						glm::vec3 x = particles[i]->x;
						glm::vec3 v = particles[i]->v;

						glm::vec3 x_out(0.0f);
						glm::vec3 v_out(0.0f);

						generateDiffuseParticle(0.02f, xr, x_theta, xh, x, v, dt, x_out, v_out);

						FoamParticle fp;
						fp.pos = x_out;
						fp.vel = v_out;
						fp.last_pos = particles[i]->x;

						_tempArray.push_back(fp);
					}
				}
			}
		}

		for (size_t i = 0; i < nbparticles; i++) {
			setAirDebug(velDiffArr[i], iAirArr[i]);
			setWaveDebug(kiArr[i], iWaveArr[i]);
			setEnergyDebug(kEArr[i], iEnergyArr[i]);

			increaseParticleCount(foamCount[i]);
		}

		if (_tempArray.size() > 0) {
			result.resize(_tempArray.size());

			for (size_t i = 0; i < _tempArray.size(); i++) {
				result[i] = _tempArray.at(i);
			}
		}

		delete[] normals;

		delete[] foamCount;
		delete[] velDiffArr;
		delete[] kiArr;
		delete[] kEArr;
		delete[] iAirArr;
		delete[] iWaveArr;
		delete[] iEnergyArr;
	}

	void FoamGenerator::analyseKineticEnergy(sph::Particle **particles, size_t nbparticles) {

		float psize = *(float*)getProperty(inParticleSize);
		float rdensity = 1000.f;
		float diam = (psize* 2.f);
		float mass = 0.8f * diam * diam * diam * rdensity;

		float maxEnergy = *(float*)getProperty(inMaxEnergyThreshold);
		float minEnergy = *(float*)getProperty(inMinEnergyThreshold);
		
		#pragma omp parallel default(shared)
		{
			#pragma omp for schedule(static)
			for (int i = 0; i < nbparticles; i++) {
				if (!particles[i]->isAlive()) continue;

				float speed = glm::length(particles[i]->v);
				float kEnergy = 0.5f * mass * pow2(speed);

				float iEnergy = clampPotential(kEnergy, minEnergy, maxEnergy);

				setEnergyDebug(kEnergy, iEnergy);
			}
		}
	}

	void FoamGenerator::generateDiffuseParticle(float rv, float xr, float x_theta, float xh, glm::vec3 pos, glm::vec3 vel, float timestep, glm::vec3 &out_pos, glm::vec3 &out_vel) {

		//(vel.y, -vel.x, 0), (vel.y, -vel.x, 0), (-vel.z, 0, vel.x),  (0, vel.z, -vel.y)
		glm::vec3 w = glm::vec3(vel.y, -vel.x, 0);

		glm::vec3 e1 = glm::normalize(glm::cross(vel, w));
		glm::vec3 e2 = glm::normalize(glm::cross(vel, e1));

		glm::vec3 dt_vel = vel * timestep;

		float r = rv * sqrt(xr);
		float azimuth = x_theta * 2.0f * xPI;
		float h = xh * glm::length(dt_vel);

		glm::vec3 xd = pos + r*cos(azimuth)*e1 + r*sin(azimuth)*e2 + h*glm::normalize(vel);
		glm::vec3 vd = r*cos(azimuth)*e1 + r*sin(azimuth)*e2 + vel;

		out_pos = xd;
		out_vel = vd;
	}

	void FoamGenerator::appendFoam(FoamParticleArray &result) {

		float minFl = *(float*)getProperty(inMinLife);
		float maxFl = *(float*)getProperty(inMaxLife);

		std::time_t time = std::time(nullptr);
		std::uniform_real_distribution<> distLife(minFl, maxFl);

		std::default_random_engine genx(time);

		if (foamParticles == nullptr) {
			
			if (!result.isNull() && result.size() > 0) {
				foamParticles = new FoamParticle[result.size()];
				nbFoamParticles = result.size();

				for (size_t i = 0; i < result.size(); i++) {
					foamParticles[i] = result[i];
					foamParticles[i].life = (float)distLife(genx);
				}
			}
		}
		else {
			
			if (!result.isNull() && result.size() > 0) {
				size_t new_size = nbFoamParticles + result.size();
				FoamParticle *new_array = new FoamParticle[new_size];

				size_t counter = 0;

				for (size_t i = 0; i < nbFoamParticles; i++) {
					new_array[counter] = foamParticles[i];
					counter++;
				}

				for (size_t i = 0; i < result.size(); i++) {
					new_array[counter] = result[i];
					new_array[counter].life = (float)distLife(genx);
					counter++;
				}

				
				delete[] foamParticles;
				foamParticles = new_array;
				nbFoamParticles = new_size;
			}
		}
	}

	void FoamGenerator::advectFoam(sph::Particle **particles, size_t nbparticles) {
		using namespace DfxEntityPropertyTypes;

		float psize = *(float*)getProperty(inParticleSize);
		float h		= 4.0f * psize;
		float dt	= *(float*)getProperty(inTimestep);
		float mass	= *(float*)getProperty(inMass);
		float buoyancy = *(float*)getProperty(inBuoyancy);
		float drag	   = *(float*)getProperty(inDrag);

		sph::initCubicSplineKernel(h);

		glm::vec3 gravity(
						 ((Float3*)getProperty(inGravity))->x,
						 ((Float3*)getProperty(inGravity))->y,
						 ((Float3*)getProperty(inGravity))->z
						 );

		if (nbFoamParticles > 0 && nbparticles > 0) {
			FoamCompactSearch fsearch;
			fsearch.findNeighbours(foamParticles, nbFoamParticles, particles, nbparticles, h, psize);

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbFoamParticles; i++) {

					/*DfxGlobal::console::println(std::string("Index: ") + std::to_string(i) + std::string(", ") + std::to_string(foamParticles[i].nbs.size()));*/

					if (foamParticles[i].nbs.size() < 6) {
						// Spray Particles
						foamParticles[i].vel = foamParticles[i].vel + (dt*gravity);

						foamParticles[i].last_pos = foamParticles[i].pos;
						glm::vec3 new_pos = foamParticles[i].pos + (dt*foamParticles[i].vel);
						foamParticles[i].pos = foamParticles[i].pos + (dt*foamParticles[i].vel);
					}
					else if (foamParticles[i].nbs.size() > 20) {
						// Bubble Particles
						glm::vec3 top(0.f);
						float bottom = 0.f;

						for (auto j : foamParticles[i].nbs) {
							sph::Particle *np = particles[j];

							float w = sph::cubicSplineW(foamParticles[i].pos - np->x);

							top += np->v * w;
							bottom += w;
						}

						glm::vec3 divisor = top / bottom;
						glm::vec3 diff = divisor - foamParticles[i].vel;
						foamParticles[i].vel = foamParticles[i].vel + (dt * (((-buoyancy) * gravity) + (drag * (diff / dt))));

						foamParticles[i].last_pos = foamParticles[i].pos;
						foamParticles[i].pos = foamParticles[i].pos + (dt*foamParticles[i].vel);
					}
					else {
						// Foam Particles
						glm::vec3 top(0.f);
						float bottom = 0.f;

						for (auto j : foamParticles[i].nbs) {
							sph::Particle *np = particles[j];

							float w = sph::cubicSplineW(foamParticles[i].pos - np->x);

							top += np->v * w;
							bottom += w;
						}

						glm::vec3 divisor = top / bottom;

						foamParticles[i].last_pos = foamParticles[i].pos;
						foamParticles[i].pos = foamParticles[i].pos + (dt*divisor);
					}
				}
			}
		}

	}

	void FoamGenerator::cullFoam() {

		float dt = *(float*)getProperty(inTimestep);

		std::vector<FoamParticle> temp;
			
		for (size_t inj = 0; inj < nbFoamParticles; inj++) {
			foamParticles[inj].life -= dt;

			if (foamParticles[inj].life > 0.f) {
				temp.push_back(foamParticles[inj]);
			}
		}

		delete[] foamParticles;
		foamParticles	= new FoamParticle[temp.size()];
		nbFoamParticles = temp.size();

		for (size_t inj = 0; inj < nbFoamParticles; inj++) {
			foamParticles[inj] = temp[inj];
		}
	}

	glm::vec3 FoamGenerator::transformPoint(glm::mat4 m, glm::vec3 point) {
		return glm::vec3(m * glm::vec4(point, 1.f));
	}

	void FoamGenerator::resolveCollisionsBVH() {
	
		auto robjects = findRigidObjects();

		for (auto robj : robjects) {
			for (size_t inj = 0; inj < nbFoamParticles; inj++) {
				auto _robj = (ParticleRigidObject*)robj;

				if (_robj->inBounds(foamParticles[inj].pos)) {

					glm::vec3 newPos;
					glm::vec3 newVel;

					_robj->resolveCollison(foamParticles[inj].pos,
						foamParticles[inj].last_pos,
						foamParticles[inj].vel,
						newPos,
						newVel);

					foamParticles[inj].pos = newPos;
					foamParticles[inj].vel = newVel;
				}
			}
		}
	}

	void FoamGenerator::resolveCollisionsBruteForce() {

		/*auto rigids = DfxSimulationCoreGlobal::getInstance()->findSimulationEntitiesOfType(ParticleRigidObject_Identifier);
		float collisionMargin = *(float*)getProperty(inCollisionMargin);

		for (auto rg : rigids) {

			Dfx3dMeshData *mesh = ((ParticleRigidObject*)rg)->getMeshData();
			glm::mat4 trans = ((ParticleRigidObject*)rg)->finalTransform4x4();
			bool flipNormals = ((ParticleRigidObject*)rg)->flippedNormals();		

			if (mesh != nullptr) {

				if (mesh->getVertexCount() > 3 && ((int)(mesh->getIndexCount() / 3)) >= 1) {

					for (size_t i = 0; i < mesh->getIndexCount() / 3; i++) {

						int counter = i * 3;

						glm::vec3 _a = transformPoint(trans, glm::vec3(
							mesh->getVertex(mesh->getIndex(counter)).x,
							mesh->getVertex(mesh->getIndex(counter)).y,
							mesh->getVertex(mesh->getIndex(counter)).z));

						glm::vec3 _b = transformPoint(trans, glm::vec3(
							mesh->getVertex(mesh->getIndex(counter + 1)).x,
							mesh->getVertex(mesh->getIndex(counter + 1)).y,
							mesh->getVertex(mesh->getIndex(counter + 1)).z));

						glm::vec3 _c = transformPoint(trans, glm::vec3(
							mesh->getVertex(mesh->getIndex(counter + 2)).x,
							mesh->getVertex(mesh->getIndex(counter + 2)).y,
							mesh->getVertex(mesh->getIndex(counter + 2)).z));

						for (size_t j = 0; j < nbFoamParticles; j++) {

							math::vec p1(_a.x, _a.y, _a.z);
							math::vec p2(_b.x, _b.y, _b.z);
							math::vec p3(_c.x, _c.y, _c.z);

							const math::Triangle triangle(p1, p2, p3);

							math::vec a(foamParticles[j].pos.x, foamParticles[j].pos.y, foamParticles[j].pos.z);
							math::vec b(foamParticles[j].last_pos.x, foamParticles[j].last_pos.y, foamParticles[j].last_pos.z);

							LineSegment lg(b, a);
							
							math::float3 point;

							if (lg.Intersects(triangle, nullptr, &point)) {
								glm::vec3 norm;

								if (flipNormals) {
									math::vec _norm = triangle.NormalCW();
									norm = glm::vec3(_norm.x, _norm.y, _norm.z);
								}
								else {
									math::vec _norm = triangle.NormalCCW();
									norm = glm::vec3(_norm.x, _norm.y, _norm.z);
								}
		
								foamParticles[j].pos = glm::vec3(point.x, point.y, point.z)  + (collisionMargin * norm);
								foamParticles[j].vel = glm::vec3(0);
							}
						}
					}

				}
			}
		}*/
	}

	void FoamGenerator::setAirDebug(float potential, float clamped) {

		dbg_minAirPotential = std::min(potential, dbg_minAirPotential);
		dbg_minAirPotentialClamped = std::min(clamped, dbg_minAirPotentialClamped);

		dbg_maxAirPotential = std::max(potential, dbg_maxAirPotential);
		dbg_maxAirPotentialClamped = std::max(clamped, dbg_maxAirPotentialClamped);

	}

	void FoamGenerator::setWaveDebug(float potential, float clamped) {

		dbg_minWavePotential = std::min(potential, dbg_minWavePotential);
		dbg_minWavePotentialClamped = std::min(clamped, dbg_minWavePotentialClamped);

		dbg_maxWavePotential = std::max(potential, dbg_maxWavePotential);
		dbg_maxWavePotentialClamped = std::max(clamped, dbg_maxWavePotentialClamped);

	}

	void FoamGenerator::setEnergyDebug(float potential, float clamped) {

		dbg_minEnergyPotential = std::min(potential, dbg_minEnergyPotential);
		dbg_minEnergyPotentialClamped = std::min(clamped, dbg_minEnergyPotentialClamped);

		dbg_maxEnergyPotential = std::max(potential, dbg_maxEnergyPotential);
		dbg_maxEnergyPotentialClamped = std::max(clamped, dbg_maxEnergyPotentialClamped);

	}

	void FoamGenerator::printDebugInfo() {
		std::stringstream ss;

		ss << "------------------------------------\n";
		ss << "Index: " << dbg_index << std::endl;
		ss << "Total Generated Foam Particles: " << dbg_particle_count << std::endl;
		// ss << "Total Culled Foam Particles: " << dbg_culled_particle_count << std::endl;
		ss << "Foam Buffer Size: " << nbFoamParticles << std::endl;
		ss << "Min Air: " << dbg_minAirPotential << ", Min Clamped: " << dbg_minAirPotentialClamped << ", Max Air: " << dbg_maxAirPotential << ", Max Clamped: " << dbg_maxAirPotentialClamped << std::endl;
		ss << "Min Wave: " << dbg_minWavePotential << ", Min Clamped: " << dbg_minWavePotentialClamped << ", Max Wave: " << dbg_maxWavePotential << ", Max Clamped: " << dbg_maxWavePotentialClamped << std::endl;
		ss << "Min Energy: " << dbg_minEnergyPotential << ", Min Clamped: " << dbg_minEnergyPotentialClamped << ", Max Energy: " << dbg_maxEnergyPotential << ", Max Clamped: " << dbg_maxEnergyPotentialClamped << std::endl;
		ss << "------------------------------------\n\n\n";

		DfxGlobal::console::println(ss.str());

		dbg_index++;
	}

	void FoamGenerator::printKineticEnergyDebugInfo() {
		std::stringstream ss;

		ss << "------------------------------------\n";
		ss << "Index: " << dbg_index << std::endl;
		ss << "Min Energy: " << dbg_minEnergyPotential << ", Min Clamped: " << dbg_minEnergyPotentialClamped << ", Max Energy: " << dbg_maxEnergyPotential << ", Max Clamped: " << dbg_maxEnergyPotentialClamped << std::endl;
		ss << "------------------------------------\n\n\n";

		DfxGlobal::console::println(ss.str());

		dbg_index++;
	}

	void FoamGenerator::increaseParticleCount(int count) {
		dbg_particle_count += count;
	}

	std::vector<DfxCSimulationEntity*> FoamGenerator::findRigidObjects() {
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
}