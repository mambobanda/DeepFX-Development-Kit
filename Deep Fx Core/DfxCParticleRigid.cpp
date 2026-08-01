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

#include "DfxCParticleRigid.h"

namespace dfxcore {

	ParticleRigidObject::ParticleRigidObject(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
		setLogicalName("Particle Rigid Object");

		DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outID);
		uid->setRootID(getInstanceID());

		setPropertyState(outID, DfxEntityPropertyState::OUTPUT_STATE);
		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inParticleRadius, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inFlipNormals, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inFriction, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inBounciness, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPosition, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inRotation, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inScale, DfxEntityPropertyState::INPUT_STATE);

		((DfxEnum*)getProperty(inFlipNormals))->add("No");
		((DfxEnum*)getProperty(inFlipNormals))->add("yes");

		defaultSettings();

		mdataPtr = nullptr;
	}

	ParticleRigidObject::~ParticleRigidObject() {
	}

	bool ParticleRigidObject::isValid() {
		if (mdataPtr == nullptr) return false;
		else return true;
	}

	void ParticleRigidObject::compute() {
		using namespace DfxEntityPropertyTypes;

		mdataPtr = nullptr;

		if (propertyHasBinding(inRef)) {

			UID *uid = (UID*)getProperty(inRef);

			mdataPtr = dfxutil::findMeshData(uid);

			if (mdataPtr != nullptr) {
				calcBounds();

				collider.clear();
				collider.setMeshData(mdataPtr);
				collider.setTransform(getTransform());
				collider.initBVH();	
			}

			makeChildrenDirty();
		}
			
		getDataInstance()->makeClean();
	}

	void ParticleRigidObject::onDataChanged(DfxEntityDataInstance *instance) {
	}

	void ParticleRigidObject::preInit() {

		using namespace DfxEntityPropertyTypes;
	}

	void ParticleRigidObject::init() {

	}

	void ParticleRigidObject::clear() {
	}

	void ParticleRigidObject::done() {
	}

	void ParticleRigidObject::importData(std::vector<std::pair<std::string, std::string>> data) {
	}

	std::vector<std::pair<std::string, std::string>> ParticleRigidObject::exportData() {
		return mapEntityProperties(this);
	}

	bool ParticleRigidObject::isExportable() {
		return true;
	}

	void *ParticleRigidObject::creator(DfxGlobalEntityName uniquename) {
		return new  ParticleRigidObject(uniquename);
	}

	void ParticleRigidObject::initialize() {
		DfxCSimulationEntity::initialize();

		DfxEntityDB::createProperty(outID, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inFlipNormals, "flip_normals", new DfxEntityPropertyTypes::DfxEnumPropertyType());
		DfxEntityDB::createProperty(inParticleRadius, "particle_radius", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inFriction, "friction", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inBounciness, "bounciness", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inPosition, "position", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inRotation, "rotation", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inScale, "scale", new DfxEntityPropertyTypes::Float3PropertyType());

	}

	void ParticleRigidObject::defaultSettings() {

		*(float*)getProperty(inParticleRadius) = 0.02f;
		*(float*)getProperty(inFriction) = 0.6f;
		*(float*)getProperty(inBounciness) = 0.0f;

		((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->x = 1.0f;
		((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->y = 1.0f;
		((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->z = 1.0f;
	}

	void ParticleRigidObject::calcBounds() {
		aabb.empty();

		if (isValid()) {
				
			glm::mat4 trans = getTransform();

			for (size_t inx = 0; inx < mdataPtr->getVertexCount(); inx++) {
					
				auto p = transformPoint(trans, mdataPtr->getVertex(inx));
				aabb.add(p.x, p.y, p.z);
			}
		}
	}

	bool ParticleRigidObject::inBounds(glm::vec3 pos) {
		//return aabb.intersectsPoint(pos.x, pos.y, pos.z);
		return true;
	}

	void ParticleRigidObject::resolveCollison(glm::vec3 currentPos, glm::vec3 lastPos, glm::vec3 currentVel, glm::vec3 &newPos, glm::vec3 &newVel) {

		bool flip = (((DfxEnum*)getProperty(inFlipNormals))->getCurrentIndex() == 0) ? true: false;
		float pradius = *(float*)getProperty(inParticleRadius);
		float friction = *(float*)getProperty(inFriction);
		float bounciness = *(float*)getProperty(inBounciness);

		glm::vec3 vel = currentVel;
		glm::vec3 cpos = currentPos;

		collider.setCCWTriangles(flip);
		collider.resolveCollision(pradius, bounciness, friction, vel, cpos, lastPos, newPos, newVel);
		
 	}

	glm::mat4  ParticleRigidObject::getTransform() {
		using namespace DfxEntityPropertyTypes;

		Float3 _pos = *(Float3*)getProperty(inPosition);
		Float3 _rot = *(Float3*)getProperty(inRotation);
		Float3 _scl = *(Float3*)getProperty(inScale);

		glm::vec3 xpos(_pos.x, _pos.y, _pos.z);
		glm::vec3 xrot(_rot.x, _rot.y, _rot.z);
		glm::vec3 xscl(_scl.x, _scl.y, _scl.z);

		glm::mat4 transMat = glm::translate(glm::mat4(), xpos);

		transMat *= glm::rotate(glm::mat4(), glm::radians(xrot.x), glm::vec3(1.f, .0f, .0f));
		transMat *= glm::rotate(glm::mat4(), glm::radians(xrot.y), glm::vec3(0.f, 1.f, .0f));
		transMat *= glm::rotate(glm::mat4(), glm::radians(xrot.z), glm::vec3(0.f, .0f, 1.f));

		transMat *= glm::scale(glm::mat4(), glm::vec3(xscl.x, xscl.y, xscl.z));

		return transMat;
	}

}