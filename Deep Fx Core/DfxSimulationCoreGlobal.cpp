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

#include "DfxSimulationCoreGlobal.h"
#include "DfxCMeshVoxelizer.h"
#include "DfxCUniformMeshSampler.h"
#include "DfxCExplodeView.h"
#include "DfxVoronoiFracture.h"
#include "DfxCutoutFracture.h"
#include "DfxPlaneFracture.h"
#include "DfxSliceFracture.h"
#include "DfxCPointXForm.h"
#include "DfxCAlembicObjectCache.h"
#include "DfxCSPHRigid.h"
#include "DfxCSPHLiquid.h"
#include "DfxCSPHShapeEmitter.h"
#include "DfxCSPHParticleCache.h"
#include "DfxCSPHMapChannel.h"
#include "DfxCSPHViscosity.h"
#include "DfxCSPHVorticity.h"
#include "DfxCSPHAdhesion.h"
#include "DfxCSPHBulletRigid.h"
#include "DfxCDivergenceFreeSPHSolver.h"
#include "DfxCBulletRigidObject.h"
#include "DfxCBulletRigidArray.h"
#include "DfxCBulletGlueConstraint.h"
#include "DfxCBulletPointToPointConstraint.h"
#include "DfxCBulletHingeConstraint.h"
#include "DfxCBulletSolver.h"
#include "DfxPointGrid.h"
#include "DfxCFoamGenerator.h"
#include "DfxCBulletPointCache.h"
#include "DfxCParticleCacheLoader.h"
#include "DfxCParticleCache.h"
#include "DfxCParticleRigid.h"
#include "DfxCKillVolume.h"
#include "DfxCPlane.h"
#include "DfxCPBDGrainSolver.h"
#include "DfxCPBDGrainGrains.h"
#include "DfxCPBDGrainPointRigid.h"
#include "DfxCPBDGrainMapChannel.h"
#include "btDbvtTester.h"

DfxSimulationCoreGlobal *DfxSimulationCoreGlobal::instance = nullptr;

void DfxSimulationCoreGlobal::installNodes() {

	// Core Nodes
	DfxEntityDB::registerDfxEntity(dfxcore::MeshVoxelizer_Identifier,
								   dfxcore::MeshVoxelizer::initialize,
								   dfxcore::MeshVoxelizer::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::DfxAlembicObjectCache_Identifier,
								   dfxcore::DfxCAlembicObjectCache::initialize,
								   dfxcore::DfxCAlembicObjectCache::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::DfxUniformMeshSamplerNode_Identifier,
								   dfxcore::DfxUniformMeshSamplerNode::initialize,
								   dfxcore::DfxUniformMeshSamplerNode::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::PointXForm_Identifier,
								   dfxcore::PointXForm::initialize,
								   dfxcore::PointXForm::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::DfxPointGrid_Identifier,
								   dfxcore::DfxPointGrid::initialize,
								   dfxcore::DfxPointGrid::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::ParticleCacheLoader_Identifier,
								   dfxcore::ParticleCacheLoader::initialize,
								   dfxcore::ParticleCacheLoader::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::ParticleCache_Identifier,
								   dfxcore::ParticleCache::initialize,
								   dfxcore::ParticleCache::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::ParticleRigidObject_Identifier,
								   dfxcore::ParticleRigidObject::initialize,
								   dfxcore::ParticleRigidObject::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::ParticleKillVolume_Identifier,
								   dfxcore::ParticleKillVolume::initialize,
								   dfxcore::ParticleKillVolume::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::DfxCPlane_Identifier,
								   dfxcore::DfxCPlane::initialize,
								   dfxcore::DfxCPlane::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::ParticleRigidObject_Identifier,
								   dfxcore::ParticleRigidObject::initialize,
								   dfxcore::ParticleRigidObject::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::FoamGenerator_Identifier,
								   dfxcore::FoamGenerator::initialize,
								   dfxcore::FoamGenerator::creator);

	// PBD Grains Nodes
	DfxEntityDB::registerDfxEntity(dfxcore::pbd::GrainSolver_Identifier,
								   dfxcore::pbd::GrainSolver::initialize,
								   dfxcore::pbd::GrainSolver::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::pbd::PBDGrains_Identifier,
								   dfxcore::pbd::PBDGrains::initialize,
								   dfxcore::pbd::PBDGrains::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::pbd::PBDGrainMapChannel_Identifier,
								   dfxcore::pbd::PBDGrainMapChannel::initialize,
								   dfxcore::pbd::PBDGrainMapChannel::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::pbd::GrainPointRigid_Identifier,
								   dfxcore::pbd::GrainPointRigid::initialize,
								   dfxcore::pbd::GrainPointRigid::creator);


	// Fracture Nodes  
	DfxEntityDB::registerDfxEntity(dfxcore::DfxVoronoiFractureNode_Identifier,
								   dfxcore::DfxVoronoiFracture::initialize,
								   dfxcore::DfxVoronoiFracture::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::DfxCutoutFracture_Identifier,
								   dfxcore::DfxCutoutFracture::initialize,
								   dfxcore::DfxCutoutFracture::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::DfxPlaneFracture_Identifier,
								   dfxcore::DfxPlaneFracture::initialize,
								   dfxcore::DfxPlaneFracture::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::DfxSliceFracture_Identifier,
								   dfxcore::DfxSliceFracture::initialize,
								   dfxcore::DfxSliceFracture::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::DfxCExplodeView_Identifier,
								   dfxcore::DfxCExplodeView::initialize,
								   dfxcore::DfxCExplodeView::creator);

	
	// SPH Nodes
	DfxEntityDB::registerDfxEntity(dfxcore::sph::DivergenceFreeSPHSolver_Identifier,
								   dfxcore::sph::DivergenceFreeSPHSolver::initialize,
								   dfxcore::sph::DivergenceFreeSPHSolver::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::sph::SPHLiquid_Identifier,
								   dfxcore::sph::SPHLiquid::initialize,
								   dfxcore::sph::SPHLiquid::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::sph::SPHRigid_Identifier,
								   dfxcore::sph::SPHRigid::initialize,
								   dfxcore::sph::SPHRigid::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::sph::SPHBulletRigid_Identifier,
								   dfxcore::sph::SPHBulletRigid::initialize,
								   dfxcore::sph::SPHBulletRigid::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::sph::SPHParticleCache_Identifier, 
								   dfxcore::sph::SPHParticleCache::initialize,
								   dfxcore::sph::SPHParticleCache::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::sph::SPHShapeEmitter_Identifier,
								   dfxcore::sph::SPHShapeEmitter::initialize,
								   dfxcore::sph::SPHShapeEmitter::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::sph::SPHMapChannel_Identifier,
								   dfxcore::sph::SPHMapChannel::initialize,
								   dfxcore::sph::SPHMapChannel::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::sph::SPHVorticity_Identifier,
								   dfxcore::sph::SPHVorticity::initialize,
								   dfxcore::sph::SPHVorticity::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::sph::SPHAdhesion_Identifier,
								   dfxcore::sph::SPHAdhesion::initialize,
								   dfxcore::sph::SPHAdhesion::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::sph::SPHViscosity_Identifier,
								   dfxcore::sph::SPHViscosity::initialize,
								   dfxcore::sph::SPHViscosity::creator);

	// Bullet Node
	DfxEntityDB::registerDfxEntity(dfxcore::bullet::DfxCBulletSolver_Identifier,
								   dfxcore::bullet::DfxCBulletSolver::initialize,
								   dfxcore::bullet::DfxCBulletSolver::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::bullet::DfxCBulletRigidObject_Identifier,
								   dfxcore::bullet::DfxCBulletRigidObject::initialize,
								   dfxcore::bullet::DfxCBulletRigidObject::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::bullet::DfxCBulletRigidArray_Identifier,
								   dfxcore::bullet::DfxCBulletRigidArray::initialize,
								   dfxcore::bullet::DfxCBulletRigidArray::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::bullet::DfxCBulletPointCache_Identifier,
								   dfxcore::bullet::DfxCBulletPointCache::initialize,
								   dfxcore::bullet::DfxCBulletPointCache::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::bullet::BulletGlueConstraint_Identifier,
								   dfxcore::bullet::BulletGlueConstraint::initialize,
								   dfxcore::bullet::BulletGlueConstraint::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::bullet::BulletPointToPointConstraint_Identifier,
								   dfxcore::bullet::BulletPointToPointConstraint::initialize,
								   dfxcore::bullet::BulletPointToPointConstraint::creator);

	DfxEntityDB::registerDfxEntity(dfxcore::bullet::BulletHingeConstraint_Identifier,
								   dfxcore::bullet::BulletHingeConstraint::initialize,
								   dfxcore::bullet::BulletHingeConstraint::creator);

	// R&D Nodes
	/*DfxEntityDB::registerDfxEntity(dfxcore::r_n_d::btDbvtTester_Identifier,
								   dfxcore::r_n_d::btDbvtTester::initialize);*/
}

DfxSimulationCoreGlobal::DfxSimulationCoreGlobal() {

	simMode = DfxSimulationMode::INACTIVE_MODE;
	domainInitialized = false;
}

DfxSimulationCoreGlobal *DfxSimulationCoreGlobal::getInstance() {

	if (instance == nullptr) {
		instance = new DfxSimulationCoreGlobal();
	}

	return instance;
}

DfxSimulationMode DfxSimulationCoreGlobal::getSimulationMode() {
	return simMode;
}

void DfxSimulationCoreGlobal::setSimulationMode(DfxSimulationMode simm) {
	simMode = simm;
}

void DfxSimulationCoreGlobal::addSimulationEntity(dfxcore::DfxCSimulationEntity* se) {
	simEntities.push_back(se);
}

void DfxSimulationCoreGlobal::removeSimulationEntity(dfxcore::DfxCSimulationEntity* se) {

	for (int i = 0; i < simEntities.size(); i++) {

		if (simEntities[i]->getInstanceID() == se->getInstanceID()) {
			simEntities.erase(simEntities.begin() + i);
			break;
		}
	}
}

std::vector<dfxcore::DfxCSimulationEntity*> DfxSimulationCoreGlobal::findSimulationEntitiesOfType(DfxGlobalEntityName uniquename) {
	std::vector<dfxcore::DfxCSimulationEntity*> list;

	for (auto ent : simEntities) {

		if (ent->getGlobalID() == DfxEntity::hash(uniquename)) {
			list.push_back(ent);
		}
	}

	return list;
}


bool DfxSimulationCoreGlobal::simulationEntityIsOfType(size_t instance_id, DfxGlobalEntityName uniquename) {

	for (auto ent : simEntities) {

		if ( ent->getInstanceID() == instance_id ) {
			
			if ( ent->getGlobalID() == DfxEntity::hash(uniquename)) return true;
		}
	}

	return false;
}

dfxcore::DfxCSimulationEntity *DfxSimulationCoreGlobal::findSimulationEntity(size_t instance_id) {

	for (auto ent : simEntities) {

		if (ent->getInstanceID() == instance_id) {

			return ent;
		}
	}

	return nullptr;
}

bool DfxSimulationCoreGlobal::isDomainInitialized() {
	return domainInitialized;
}

void DfxSimulationCoreGlobal::initSimulationDomain() {

	if (!domainInitialized) {
		
		for (auto ent : simEntities) {
			ent->preInit();
		}

		for (auto ent : simEntities) {
			ent->init();
		}

		for (auto ent : simEntities) {
			ent->postInit();
		}

		domainInitialized = true;
	}
	
}

void DfxSimulationCoreGlobal::deinitializeSimulationDomain() {

	if (domainInitialized) {

		for (auto ent : simEntities) {
			ent->preDone();
		}

		for (auto ent : simEntities) {
			ent->done();
		}

		for (auto ent : simEntities) {
			ent->postDone();
		}

		domainInitialized = false;
	}
}