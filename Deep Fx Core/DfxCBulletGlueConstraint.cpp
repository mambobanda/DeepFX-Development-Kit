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

#include "DfxCBulletRigidHandle.h"
#include "DfxCBulletGlueConstraint.h"
#include "DfxCBulletRigidObject.h"

namespace dfxcore {

	namespace bullet {

		BulletGlueConstraint::BulletGlueConstraint(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
			setLogicalName("Bullet Glue Constraint");

			DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outID);
			uid->setRootID(getInstanceID());

			setPropertyState(outID, DfxEntityPropertyState::OUTPUT_STATE);
			setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inType, DfxEntityPropertyState::INPUT_STATE);
			//setPropertyState(inMinDistance, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inLimit, DfxEntityPropertyState::INPUT_STATE);

			((DfxEnum*)getProperty(inType))->add("Fixed");
			((DfxEnum*)getProperty(inType))->add("Flexible");

			*(float*)getProperty(inLimit) = 3.f;
			*(float*)getProperty(inMinDistance) = .0001f;
		}

		BulletGlueConstraint::~BulletGlueConstraint() {

		}

		void BulletGlueConstraint::compute() {

		}

		void BulletGlueConstraint::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void BulletGlueConstraint::init() {

		}

		void BulletGlueConstraint::clear() {

		}

		void BulletGlueConstraint::done() {

		}

		void BulletGlueConstraint::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> BulletGlueConstraint::exportData() {
			return mapEntityProperties(this);
		}

		bool BulletGlueConstraint::isExportable() {
			return true;
		}

		void *BulletGlueConstraint::creator(DfxGlobalEntityName uniquename) {
			return new BulletGlueConstraint(uniquename);
		}

		void BulletGlueConstraint::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outID,  "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inRef,  "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inType, "type", new DfxEntityPropertyTypes::DfxEnumPropertyType());
			DfxEntityDB::createProperty(inMinDistance, "distance", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inLimit, "strength", new DfxEntityPropertyTypes::FloatPropertyType());
		}

		bool BulletGlueConstraint::hasReference(size_t _id_) {
			using namespace DfxEntityPropertyTypes;

			UID *uid = (UID*)getProperty(inRef);
			auto sims = findSimulationEntities(uid);

			if (sims.size() > 0) {
				for (auto sim : sims) {
					if (sim->UniqueName() == DfxCBulletRigidObject_Identifier) {
						sim->pingParents();

						DfxCBulletRigidObject *robj = (DfxCBulletRigidObject*)sim;

						if (robj->getInstanceID() == _id_) {
							return true;
						}
					}
				}
			}

			return false;
		}

		void BulletGlueConstraint::setupConstraints(btDiscreteDynamicsWorld *m_dynamicsWorld) {
			using namespace DfxEntityPropertyTypes;

			btAlignedObjectArray<btRigidBody*> bodies;

			bool useFixedConstraint	= (((DfxEnum*)getProperty(inType))->getCurrentIndex() == 0) ? false : true;
			float minDistanceLimit	= *(float*)getProperty(inMinDistance);
			float BREAKING_THRESHOLD = *(float*)getProperty(inLimit);

			if (propertyHasBinding(inRef)) {

				int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();

				for (int i = 0; i < numManifolds; i++) {

					btPersistentManifold* manifold = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
					if (!manifold->getNumContacts())
						continue;

					btScalar minDist = 1e30f;
					int minIndex = -1;
					for (int v = 0; v < manifold->getNumContacts(); v++)
					{
						if (minDist > manifold->getContactPoint(v).getDistance())
						{
							minDist = manifold->getContactPoint(v).getDistance();
							minIndex = v;
						}
					}

					/*if (minDist > minDistanceLimit)
						continue;*/

					btCollisionObject* colObj0 = (btCollisionObject*)manifold->getBody0();
					btCollisionObject* colObj1 = (btCollisionObject*)manifold->getBody1();

					auto udata00 = (DfxCBulletRigidHandle*)(DfxCBulletHandle*)colObj0->getUserPointer();
					auto udata01 = (DfxCBulletRigidHandle*)(DfxCBulletHandle*)colObj1->getUserPointer();

					if (hasReference(udata00->getUidPtr()->getRootID()) && hasReference(udata01->getUidPtr()->getRootID())) {

						btRigidBody* body0 = btRigidBody::upcast(colObj0);
						btRigidBody* body1 = btRigidBody::upcast(colObj1);

						if (bodies.findLinearSearch(body0) == bodies.size())
							bodies.push_back(body0);
						if (bodies.findLinearSearch(body1) == bodies.size())
							bodies.push_back(body1);

						if (body0 && body1)
						{
							if (!colObj0->isStaticOrKinematicObject() && !colObj1->isStaticOrKinematicObject())
							{
								if (body0->checkCollideWithOverride(body1))
								{
									{
										btTransform trA, trB;
										trA.setIdentity();
										trB.setIdentity();
										btVector3 contactPosWorld = manifold->getContactPoint(minIndex).m_positionWorldOnA;
										btTransform globalFrame;
										globalFrame.setIdentity();
										globalFrame.setOrigin(contactPosWorld);

										trA = body0->getWorldTransform().inverse()*globalFrame;
										trB = body1->getWorldTransform().inverse()*globalFrame;
										float totalMass = 1.f / body0->getInvMass() + 1.f / body1->getInvMass();

										if (useFixedConstraint) {

											btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint(*body0, *body1, trA, trB, true);
											dof6->setOverrideNumSolverIterations(30);

											dof6->setBreakingImpulseThreshold(BREAKING_THRESHOLD*totalMass);

											for (int i = 0; i < 6; i++)
												dof6->setLimit(i, 0, 0);
											m_dynamicsWorld->addConstraint(dof6, true);
										}
										else {

											btFixedConstraint* fixed = new btFixedConstraint(*body0, *body1, trA, trB);
											fixed->setBreakingImpulseThreshold(BREAKING_THRESHOLD*totalMass);
											fixed->setOverrideNumSolverIterations(30);
											m_dynamicsWorld->addConstraint(fixed, true);
										}

									}
								}
							}
						}
					}
				}

				for (int i = 0; i < bodies.size(); i++)
				{
					m_dynamicsWorld->removeRigidBody(bodies[i]);
					m_dynamicsWorld->addRigidBody(bodies[i]);
				}
			}
		}

	}
}