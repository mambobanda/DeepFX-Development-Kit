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

#include "DfxCBulletRigidObject.h"

namespace dfxcore {

	namespace bullet {

		DfxCBulletRigidObject::DfxCBulletRigidObject(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
			setLogicalName("Bullet Rigid Object");

			DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outID);
			uid->setRootID(getInstanceID());

			setPropertyState(outID, DfxEntityPropertyState::OUTPUT_STATE);
			setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inInitialState, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inShapeType, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inMass, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inFriction, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inBounciness, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inLinearDampening, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inAngularDampening, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inInitialLinearVelocity, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inInitialAngularVelocity, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inPosition, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inRotation, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inScale, DfxEntityPropertyState::INPUT_STATE);

			((DfxEnum*)getProperty(inShapeType))->add("Triangles");
			((DfxEnum*)getProperty(inShapeType))->add("Convex Hull");

			((DfxEnum*)getProperty(inInitialState))->add("Normal");
			((DfxEnum*)getProperty(inInitialState))->add("Sleep");

			((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->x = 1.f;
			((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->y = 1.f;
			((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->z = 1.f;

			defaultSettings();
		}

		DfxCBulletRigidObject::~DfxCBulletRigidObject() {
			clearHandles();
		}

		void DfxCBulletRigidObject::compute() {
			using namespace DfxEntityPropertyTypes;

			makeChildrenDirty();
			getDataInstance()->makeClean();
		}

		void DfxCBulletRigidObject::onDataChanged(DfxEntityDataInstance *instance) {
		}

		void DfxCBulletRigidObject::preInit() {

			using namespace DfxEntityPropertyTypes;

			UID *uid = (UID*)getProperty(outID);
			uid->clearChildren();

			if (propertyHasBinding(inRef)) {
				uid->addChild((UID*)getProperty(inRef));
			}

			if (isPropertyBindingDirty(inRef)) {
				pingParents();

				Dfx3dMeshData *in_mesh = nullptr;
				ProxyObjectArray *in_proxies = nullptr;

				clearHandles();

				UID *uid = (UID*)getProperty(inRef);

				in_mesh = dfxutil::findMeshData(uid);

				if (in_mesh == nullptr) in_proxies = dfxutil::findProxyMeshData(uid);

				if (in_mesh != nullptr) {

					createHandlesFromMeshData(in_mesh);
				}
				else if (in_proxies != nullptr) {

					createHandlesFromProxyMeshData(in_proxies);
				}
			}
		}

		void DfxCBulletRigidObject::init() {
			
		}

		void DfxCBulletRigidObject::clear() {
		}

		void DfxCBulletRigidObject::done() {
		}

		void DfxCBulletRigidObject::importData(std::vector<std::pair<std::string, std::string>> data) {
		}

		std::vector<std::pair<std::string, std::string>> DfxCBulletRigidObject::exportData() {
			return mapEntityProperties(this);
		}

		bool DfxCBulletRigidObject::isExportable() {
			return true;
		}

		void *DfxCBulletRigidObject::creator(DfxGlobalEntityName uniquename) {
			return new  DfxCBulletRigidObject(uniquename);
		}

		void DfxCBulletRigidObject::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outID, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(outTransform, "transform4x4", new DfxEntityPropertyTypes::DfxMatrix4x4PropertyType());
			DfxEntityDB::createProperty(inRef,"in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inInitialState, "initial_state", new DfxEntityPropertyTypes::DfxEnumPropertyType());
			DfxEntityDB::createProperty(inShapeType, "shape_type", new DfxEntityPropertyTypes::DfxEnumPropertyType());
			DfxEntityDB::createProperty(inMass, "mass", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inFriction, "friction", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inBounciness, "bounciness", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inLinearDampening, "linear_dampening", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inAngularDampening, "angular_dampening", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inInitialLinearVelocity, "initial_linear_velocity", new DfxEntityPropertyTypes::Float3PropertyType());
			DfxEntityDB::createProperty(inInitialAngularVelocity, "initial_angular_velocity", new DfxEntityPropertyTypes::Float3PropertyType());
			DfxEntityDB::createProperty(inPosition, "position", new DfxEntityPropertyTypes::Float3PropertyType());
			DfxEntityDB::createProperty(inRotation, "rotation", new DfxEntityPropertyTypes::Float3PropertyType());
			DfxEntityDB::createProperty(inScale, "scale", new DfxEntityPropertyTypes::Float3PropertyType());

		}


		void DfxCBulletRigidObject::clearHandles() {

			for (size_t inx = 0; inx < rigidHandles.size(); inx++) {
				delete rigidHandles[inx];
			}

			rigidHandles.clear();
		}

		std::vector<DfxCBulletRigidHandle*> &DfxCBulletRigidObject::getRigidHandles() {
			return rigidHandles;
		}

		void DfxCBulletRigidObject::defaultSettings() {

			*(float*)getProperty(inMass) = 10.f;
			*(float*)getProperty(inFriction) = 0.5f;
			*(float*)getProperty(inBounciness) = 0.5f;
			*(float*)getProperty(inLinearDampening) = 0.f;
			*(float*)getProperty(inAngularDampening) = 0.f;

			((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->x = 1.0f;
			((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->y = 1.0f;
			((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->z = 1.0f;
		}

		void DfxCBulletRigidObject::setTransform(DfxEntityPropertyTypes::Matrix4x4 &mat) {

			((DfxEntityPropertyTypes::Matrix4x4*)getProperty(outTransform))->copy(mat);
		}

		glm::mat4 DfxCBulletRigidObject::getAbsoluteTransform() {

			DfxEntityPropertyTypes::Float3 position = *(DfxEntityPropertyTypes::Float3*)getProperty(inPosition);
			DfxEntityPropertyTypes::Float3 rotation = *(DfxEntityPropertyTypes::Float3*)getProperty(inRotation);

			glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z));
		
			trans *= glm::rotate(glm::mat4(), glm::radians(rotation.x), glm::vec3(1.f, .0f, .0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(rotation.y), glm::vec3(0.f, 1.f, .0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(rotation.z), glm::vec3(0.f, .0f, 1.f));

			return trans;
		}

		glm::vec3 DfxCBulletRigidObject::getScale() {
			DfxEntityPropertyTypes::Float3 scale = *(DfxEntityPropertyTypes::Float3*)getProperty(inScale);

			return glm::vec3(scale.x, scale.y, scale.z);
		}

		void DfxCBulletRigidObject::createHandlesFromMeshData(Dfx3dMeshData *mdata) {
			using namespace DfxEntityPropertyTypes;

			DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outID);
			int  stype = ((DfxEnum*)getProperty(inShapeType))->getCurrentIndex();

			Float3 lvel = *(Float3*)getProperty(inInitialLinearVelocity);
			Float3 avel = *(Float3*)getProperty(inInitialAngularVelocity);

			DfxEntityPropertyTypes::Float3 _pos = *(DfxEntityPropertyTypes::Float3*)getProperty(inPosition);
			DfxEntityPropertyTypes::Float3 _rot = *(DfxEntityPropertyTypes::Float3*)getProperty(inRotation);
			DfxEntityPropertyTypes::Float3 _scl = *(DfxEntityPropertyTypes::Float3*)getProperty(inScale);

			float mass = *(float*)getProperty(inMass);
			bool sleep_state = (((DfxEnum*)getProperty(inInitialState))->getCurrentIndex() == 0) ? false : true;

			glm::vec3 pos = glm::vec3(_pos.x, _pos.y, _pos.z);
			glm::vec3 rot = glm::vec3(_rot.x, _rot.y, _rot.z);
			glm::vec3 scl = glm::vec3(_scl.x, _scl.y, _scl.z);

			DfxCBulletRigidHandle *handle = new DfxCBulletRigidHandle();
			handle->setMeshDataPtr(mdata);
			handle->setUidPtr(uid);
			handle->setMass(mass);
			handle->setInitialPosition(pos);
			handle->setInitialRotation(rot);
			handle->setInitialScale(scl);
			handle->setInitialLinearVelocity(glm::vec3(lvel.x, lvel.y, lvel.z));
			handle->setInitialAngularVelocity(glm::vec3(avel.x, avel.y, avel.z));
			handle->getState()->IS_SLEEPING = sleep_state;

			if (stype == 1) handle->setShapeType(DfxBulletShapeType::CONVEX_HULL);

			handle->setupShape();
			handle->setupBody();

			rigidHandles.push_back(handle);
		}

		void DfxCBulletRigidObject::createHandlesFromProxyMeshData(DfxEntityPropertyTypes::ProxyObjectArray *pxs){
			using namespace DfxEntityPropertyTypes;

			if (!pxs->isNull()) {

				DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outID);
				int  stype = ((DfxEnum*)getProperty(inShapeType))->getCurrentIndex();

				Float3 lvel = *(Float3*)getProperty(inInitialLinearVelocity);
				Float3 avel = *(Float3*)getProperty(inInitialAngularVelocity);

				float mass = *(float*)getProperty(inMass);
				bool sleep_state = (((DfxEnum*)getProperty(inInitialState))->getCurrentIndex() == 0) ? false : true;

				for (int pinx = 0; pinx < pxs->size(); pinx++) {

					ProxyObject &proxObj = (*pxs)[pinx];

					if (proxObj.meshData.getVertexCount() > 0) {
						
						DfxEntityPropertyTypes::Float3 position = proxObj.position;
						DfxEntityPropertyTypes::Float3 rotation = proxObj.rotation;
						DfxEntityPropertyTypes::Float3 scale = proxObj.scale;

						glm::vec3 pos = glm::vec3(position.x, position.y, position.z);
						glm::vec3 rot = glm::vec3(rotation.x, rotation.y, rotation.z);
						glm::vec3 scl = glm::vec3(scale.x, scale.y, scale.z);

						DfxCBulletRigidHandle *handle = new DfxCBulletRigidHandle();
						handle->setMeshDataPtr(&proxObj.meshData);
						handle->setUidPtr(uid);
						handle->setMass(mass);
						handle->setInitialPosition(pos);
						handle->setInitialRotation(rot);
						handle->setInitialScale(scl);
						handle->setInitialLinearVelocity(glm::vec3(lvel.x, lvel.y, lvel.z));
						handle->setInitialAngularVelocity(glm::vec3(avel.x, avel.y, avel.z));
						handle->getState()->IS_SLEEPING = sleep_state;

						if (stype == 1) handle->setShapeType(DfxBulletShapeType::CONVEX_HULL);

						handle->setupShape();
						handle->setupBody();

						rigidHandles.push_back(handle);

					}
					
				}
			}
		}
	}
}