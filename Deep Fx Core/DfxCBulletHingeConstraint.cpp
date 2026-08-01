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

#include "DfxCBulletHingeConstraint.h"
#include "DfxCBulletRigidHandle.h"
#include "DfxCBulletRigidObject.h"

namespace dfxcore {

	namespace bullet {

		BulletHingeConstraint::BulletHingeConstraint(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
			setLogicalName("Bullet Hinge Constraint");

			DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outID);
			uid->setRootID(getInstanceID());

			constraint = nullptr;

			setPropertyState(outID, DfxEntityPropertyState::OUTPUT_STATE);
			setPropertyState(inRigid01, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inRigid02, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inPivot01, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inPivot02, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inAxisA, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inAxisB, DfxEntityPropertyState::INPUT_STATE);
			//setPropertyState(inBreakable, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inLimit, DfxEntityPropertyState::INPUT_STATE);

			((DfxEnum*)getProperty(inBreakable))->add("No");
			((DfxEnum*)getProperty(inBreakable))->add("Yes");

			((DfxEnum*)getProperty(inAxisA))->add("X");
			((DfxEnum*)getProperty(inAxisA))->add("Y");
			((DfxEnum*)getProperty(inAxisA))->add("Z");

			((DfxEnum*)getProperty(inAxisB))->add("X");
			((DfxEnum*)getProperty(inAxisB))->add("Y");
			((DfxEnum*)getProperty(inAxisB))->add("Z");

			*(float*)getProperty(inLimit) = 1000.f;
		}

		BulletHingeConstraint::~BulletHingeConstraint() {

		}

		void BulletHingeConstraint::compute() {

		}

		void BulletHingeConstraint::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void BulletHingeConstraint::init() {
			using namespace DfxEntityPropertyTypes;

			pingParents(false);

			if (propertyHasBinding(inRigid01)) {

				DfxCBulletRigidObject *robj01 = nullptr;
				DfxCBulletRigidObject *robj02 = nullptr;

				float limit = *(float*)getProperty(inLimit);
				Float3 *pivot01 = (DfxEntityPropertyTypes::Float3*)getProperty(inPivot01);
				Float3 *pivot02 = (DfxEntityPropertyTypes::Float3*)getProperty(inPivot02);
				btVector3 axis01 = getAxisA();
				btVector3 axis02 = getAxisB();

				UID *uid01 = (UID*)getProperty(inRigid01);
				auto sims01 = findSimulationEntities(uid01);

				for (auto s : sims01) {
					if (s->UniqueName() == DfxCBulletRigidObject_Identifier) {
						robj01 = (DfxCBulletRigidObject*)s;
						break;
					}
				}

				if (!propertyHasBinding(inRigid02)) {
					if (robj01 != nullptr) {

						if (robj01->getRigidHandles().size() > 0) {

							glm::vec3 pos = robj01->getRigidHandles()[0]->getInitialPosition();
							btVector3 worldPos(pos.x, pos.y, pos.z);
							btVector3 piv(pivot01->x, pivot01->y, pivot01->z);
							piv = piv - worldPos;
							constraint = new btHingeConstraint(*robj01->getRigidHandles()[0]->getBody(), piv, axis01, true);

							if (limit > 0) constraint->setBreakingImpulseThreshold(limit);

						}
					}
				}
				else {

					UID *uid02 = (UID*)getProperty(inRigid02);
					auto sims02 = findSimulationEntities(uid02);

					for (auto s : sims02) {
						if (s->UniqueName() == DfxCBulletRigidObject_Identifier) {
							robj02 = (DfxCBulletRigidObject*)s;
							break;
						}
					}

					if (robj01 != nullptr && robj02 != nullptr) {

						if (robj01->getRigidHandles().size() > 0 && robj02->getRigidHandles().size() > 0) {

							glm::vec3 pos01 = robj01->getRigidHandles()[0]->getInitialPosition();
							btVector3 worldPos01(pos01.x, pos01.y, pos01.z);
							btVector3 piv01(pivot01->x, pivot01->y, pivot01->z);
							piv01 = piv01 - worldPos01;

							glm::vec3 pos02 = robj02->getRigidHandles()[0]->getInitialPosition();
							btVector3 worldPos02(pos02.x, pos02.y, pos02.z);
							btVector3 piv02(pivot02->x, pivot02->y, pivot02->z);
							piv02 = piv02 - worldPos02;

							constraint = new btHingeConstraint(*robj01->getRigidHandles()[0]->getBody(),
								*robj02->getRigidHandles()[0]->getBody(),
								piv01, piv02, axis01, axis02, true);

							if(limit > 0) constraint->setBreakingImpulseThreshold(limit);
						}
					}
				}
			}
		}

		void BulletHingeConstraint::clear() {

		}

		void BulletHingeConstraint::done() {

		}

		void BulletHingeConstraint::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> BulletHingeConstraint::exportData() {
			return mapEntityProperties(this);
		}

		bool BulletHingeConstraint::isExportable() {
			return true;
		}

		void *BulletHingeConstraint::creator(DfxGlobalEntityName uniquename) {
			return new BulletHingeConstraint(uniquename);
		}

		void BulletHingeConstraint::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outID, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inRigid01, "rigid_object01", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inRigid02, "rigid_object02", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inPivot01, "pivot01", new DfxEntityPropertyTypes::Float3PropertyType());
			DfxEntityDB::createProperty(inPivot02, "pivot02", new DfxEntityPropertyTypes::Float3PropertyType());
			DfxEntityDB::createProperty(inBreakable, "breakable", new DfxEntityPropertyTypes::DfxEnumPropertyType);
			DfxEntityDB::createProperty(inAxisA, "axis_01", new DfxEntityPropertyTypes::DfxEnumPropertyType);
			DfxEntityDB::createProperty(inAxisB, "axis_02", new DfxEntityPropertyTypes::DfxEnumPropertyType);
			DfxEntityDB::createProperty(inLimit, "limit", new DfxEntityPropertyTypes::FloatPropertyType());
		}

		btTypedConstraint *BulletHingeConstraint::getConstraint() {
			return constraint;
		}

		btVector3 BulletHingeConstraint::getAxisA() {
			int rval = ((DfxEnum*)getProperty(inAxisA))->getCurrentIndex();

			if (rval == 2) return btVector3(0.f, 0.f, 1.0);
			else if (rval == 1) return btVector3(0.f, 1.f, 0.0);
			else return btVector3(1.f, 0.f, 0.f);
		}

		btVector3 BulletHingeConstraint::getAxisB() {
			int rval = ((DfxEnum*)getProperty(inAxisB))->getCurrentIndex();

			if (rval == 2) return btVector3(0.f, 0.f, 1.0);
			else if (rval == 1) return btVector3(0.f, 1.f, 0.0);
			else return btVector3(1.f, 0.f, 0.f);
		}
	}
}