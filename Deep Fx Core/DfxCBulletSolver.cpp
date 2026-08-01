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

namespace dfxcore {

	namespace bullet {

		DfxCBulletSolver::DfxCBulletSolver(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
			setLogicalName("Bullet Solver");


			DfxEntityPropertyTypes::UID *uid01 = (DfxEntityPropertyTypes::UID*)getProperty(outSolverID);
			uid01->setRootID(getInstanceID());

			setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(outSolverID, DfxEntityPropertyState::OUTPUT_STATE);
			setPropertyState(outTrigger, DfxEntityPropertyState::OUTPUT_STATE);
			setPropertyState(inMasterSlave, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inTrigger, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inTimestep, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inSubstep, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inForce, DfxEntityPropertyState::INPUT_STATE);

			((DfxEnum*)getProperty(inMasterSlave))->add("Master");
			((DfxEnum*)getProperty(inMasterSlave))->add("Slave");
			
			physicsWorldInitialised = false;

			defaultSettings();
		}

		DfxCBulletSolver::~DfxCBulletSolver() {

		}

		void DfxCBulletSolver::compute() {

			bool isMasterMode = (((DfxEnum*)getProperty(inMasterSlave))->getCurrentIndex() == 0) ? true : false;
			float timestep = *(float*)getProperty(inTimestep);
			int substep = *(float*)getProperty(inSubstep);

			if (DfxSimulationCoreGlobal::getInstance()->getSimulationMode() == DfxSimulationMode::ACTIVE_MODE) {

				if (isMasterMode) {

					for (int i = 0; i < substep; i++) {
						dynamicsWorld->stepSimulation(timestep, 1, timestep);
						processCollisons();
					}

					makePropertyBindingDirty(outSolverID);
					makePropertyBindingDirty(outTrigger);
					makeChildrenDirty();
				}
			}

			getDataInstance()->makeClean();
		}

		void DfxCBulletSolver::stepSolver(float timestep) {
			dynamicsWorld->stepSimulation(timestep, 1, timestep);
		}

		bool DfxCBulletSolver::isPhysicsWorldInitialised() {
			return physicsWorldInitialised;
		}

		bool DfxCBulletSolver::isSlaveMode() {
			return (((DfxEnum*)getProperty(inMasterSlave))->getCurrentIndex() == 0) ? false : true;
		}

		void DfxCBulletSolver::onDataChanged(DfxEntityDataInstance *instance) {
		}


		void DfxCBulletSolver::init() {
			
		}

		void DfxCBulletSolver::postInit() {

			using namespace DfxEntityPropertyTypes;

			UID *uid = (UID*)getProperty(outSolverID);
			uid->clearChildren();

			collisionConfiguration = new btDefaultCollisionConfiguration();

			dispatcher = new btCollisionDispatcher(collisionConfiguration);
			btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);

			overlappingPairCache = new btDbvtBroadphase();

			solver = new btSequentialImpulseConstraintSolver;

			dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

			if (propertyHasBinding(inRef)) {
				uid->addChild((UID*)getProperty(inRef));
			}

			initialisePhysicsWorld();
		}

		void DfxCBulletSolver::clear() {

		}

		void DfxCBulletSolver::done() {
			
			for (size_t i = 0;  i < dynamicsWorld->getNumConstraints(); i++) {

				btTypedConstraint* constraint = dynamicsWorld->getConstraint(i);
				dynamicsWorld->removeConstraint(constraint);
				delete constraint;
			}

			for (unsigned int i = 0; i < dynamicsWorld->getNumCollisionObjects(); i++) {

				btCollisionObject *obj = dynamicsWorld->getCollisionObjectArray()[i];
				btRigidBody * body = btRigidBody::upcast(obj);
				if (body && body->getMotionState()) {
					delete body->getMotionState();
				}
				dynamicsWorld->removeCollisionObject(obj);
			}
		
			delete dynamicsWorld;
			delete solver;
			delete overlappingPairCache;
			delete dispatcher;
			delete collisionConfiguration;

			rigidObjects.clear();
			rigidObjectArrays.clear();

			physicsWorldInitialised = false;
		}

		void DfxCBulletSolver::importData(std::vector<std::pair<std::string, std::string>> data) {
		}

		std::vector<std::pair<std::string, std::string>> DfxCBulletSolver::exportData() {
			return mapEntityProperties(this);
		}

		bool DfxCBulletSolver::isExportable() {
			return true;
		}

		void *DfxCBulletSolver::creator(DfxGlobalEntityName uniquename) {
			return new DfxCBulletSolver(uniquename);
		}

		void DfxCBulletSolver::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outSolverID, "@ref",   new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(outTrigger, "execute",  new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inTrigger, "trigger",  new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inMasterSlave, "compute_mode", new DfxEntityPropertyTypes::DfxEnumPropertyType());
			DfxEntityDB::createProperty(inTimestep, "timestep", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inSubstep, "substep",  new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inForce, "gravity", new DfxEntityPropertyTypes::Float3PropertyType());
		}

		void DfxCBulletSolver::defaultSettings() {

			*(float*)getProperty(inTimestep) = 0.0166f;
			*(float*)getProperty(inSubstep)  = 1;
			((DfxEntityPropertyTypes::Float3*)getProperty(inForce))->x = 0.0f;
			((DfxEntityPropertyTypes::Float3*)getProperty(inForce))->y = -9.8f;
			((DfxEntityPropertyTypes::Float3*)getProperty(inForce))->z = 0.0f;
		}

		void DfxCBulletSolver::initRigidObjects(std::vector<DfxCSimulationEntity*> sims) {

			if (sims.size() > 0) {
				for (auto sim : sims) {

					if (sim->UniqueName() == DfxCBulletRigidObject_Identifier) {
						sim->pingParents();

						DfxCBulletRigidObject *robj = (DfxCBulletRigidObject*)sim;

						for(auto handle: robj->getRigidHandles()) {
							dynamicsWorld->addRigidBody(handle->getBody());
						}
					}
				}
			}
		}

		void DfxCBulletSolver::initGlueConstraints(std::vector<DfxCSimulationEntity*> sims) {
			if (sims.size() > 0) {
				for (auto sim : sims) {

					if (sim->UniqueName() == BulletGlueConstraint_Identifier) {
						sim->pingParents();

						BulletGlueConstraint *cobj = (BulletGlueConstraint*)sim;

						cobj->setupConstraints(dynamicsWorld);
					}
				}
			}
		}

		void DfxCBulletSolver::initNormalContraints(std::vector<DfxCSimulationEntity*> sims) {
			if (sims.size() > 0) {
				for (auto sim : sims) {

					if (sim->UniqueName() == BulletPointToPointConstraint_Identifier) {
						sim->pingParents();

						BulletPointToPointConstraint *cobj = (BulletPointToPointConstraint*)sim;
						if (cobj->getConstraint() != nullptr) dynamicsWorld->addConstraint(cobj->getConstraint(), true);
						
					}
					else if (sim->UniqueName() == BulletHingeConstraint_Identifier) {
						sim->pingParents();

						BulletHingeConstraint *cobj = (BulletHingeConstraint*)sim;
						if (cobj->getConstraint() != nullptr) dynamicsWorld->addConstraint(cobj->getConstraint(), true);
					}
				}
			}
		}

		void DfxCBulletSolver::initialisePhysicsWorld(){
			using namespace DfxEntityPropertyTypes;

			if (!physicsWorldInitialised) {

				UID *uid = (UID*)getProperty(outSolverID);
				auto sims = findSimulationEntities(uid);

				initRigidObjects(sims);

				dynamicsWorld->performDiscreteCollisionDetection();

				initGlueConstraints(sims);

				initNormalContraints(sims);

				btVector3 grav(((DfxEntityPropertyTypes::Float3*)getProperty(inForce))->x,
					((DfxEntityPropertyTypes::Float3*)getProperty(inForce))->y,
					((DfxEntityPropertyTypes::Float3*)getProperty(inForce))->z
				);

				dynamicsWorld->setGravity(btVector3(grav));

				physicsWorldInitialised = true;
			}
		}

		void DfxCBulletSolver::processCollisons() {

			using namespace DfxEntityPropertyTypes;

			btAlignedObjectArray<btRigidBody*> bodies;

			if (propertyHasBinding(inRef)) {

				int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();

				for (int i = 0; i < numManifolds; i++) {

					btPersistentManifold* manifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
					if (!manifold->getNumContacts())
						continue;

					btCollisionObject* colObj0 = (btCollisionObject*)manifold->getBody0();
					btCollisionObject* colObj1 = (btCollisionObject*)manifold->getBody1();

					for (size_t v = 0; v < manifold->getNumContacts(); v++) {
						
						btManifoldPoint pt = manifold->getContactPoint(v);
						if (pt.getDistance() < 0.f) {
							
							DfxCBulletCollisionPacket packet00;
							DfxCBulletCollisionPacket packet01;

							DfxCBulletHandle *udata00 = (DfxCBulletHandle*)colObj0->getUserPointer();
							DfxCBulletHandle *udata01 = (DfxCBulletHandle*)colObj1->getUserPointer();

							auto btPtA = pt.getPositionWorldOnA();
							auto btPtB = pt.getPositionWorldOnB();

							glm::vec3 cpointA(btPtA.x(), btPtA.y(), btPtA.z());
							glm::vec3 cpointB(btPtB.x(), btPtB.y(), btPtB.z());

							// Collision Packet A
							packet00.collisionPointA = cpointA;
							packet00.collisionPointB = cpointB;

							packet00.other = udata01;

							// Collision Packet B
							packet01.collisionPointA = cpointA;
							packet01.collisionPointB = cpointB;

							packet01.other =  udata00;
							
							// Collision Callback
							udata00->collisionResponse(dynamicsWorld, packet00);
							udata01->collisionResponse(dynamicsWorld, packet01);
						}
					}
				}

				for (int i = 0; i < bodies.size(); i++) {

					dynamicsWorld->removeRigidBody(bodies[i]);
					dynamicsWorld->addRigidBody(bodies[i]);
				}
			}
		}
	}
}