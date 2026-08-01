/*#include <DfxCKillVolume.h>
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

#include <DfxCKillVolume.h>

namespace dfxcore {

	ParticleKillVolume::ParticleKillVolume(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename){
		setLogicalName("Particle Kill Volume");

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inType, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPosition, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inRotation, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inScale, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPositionArray, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inVelocityArray, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outPositionArray, DfxEntityPropertyState::OUTPUT_STATE);
		setPropertyState(outVelocityArray, DfxEntityPropertyState::OUTPUT_STATE);

		((DfxEnum*)getProperty(inType))->add("Outside");
		((DfxEnum*)getProperty(inType))->add("Inside");
	}

	ParticleKillVolume::~ParticleKillVolume() {

	}

	void ParticleKillVolume::init() {

	}

	void ParticleKillVolume::clear() {

	}

	void ParticleKillVolume::done() {
		using namespace DfxEntityPropertyTypes;

		Float3Array *out_positions = (Float3Array*)getProperty(outPositionArray);
		Float3Array *out_velocities = (Float3Array*)getProperty(outVelocityArray);

		out_positions->deleteArray();
		out_velocities->deleteArray();
	}

	void ParticleKillVolume::compute() {
		using namespace DfxEntityPropertyTypes;
		
		Float3Array *_opositions = (Float3Array*)getProperty(outPositionArray);
		Float3Array *_ovelocities = (Float3Array*)getProperty(outVelocityArray);

		_opositions->deleteArray();
		_ovelocities->deleteArray();

		if (propertyHasBinding(inRef)) {

			UID *uid = (UID*)getProperty(inRef);
			bool outside = (((DfxEnum*)getProperty(inType))->getCurrentIndex() == 0) ? true : false;
			Float3Array *_positions = (Float3Array*)getProperty(inPositionArray);
			Float3Array *_velocities = (Float3Array*)getProperty(inVelocityArray);
			
			auto mdataPtr = dfxutil::findMeshData(uid);

			if (mdataPtr != nullptr) {

				std::vector<glm::vec3> pos;
				std::vector<glm::vec3> vel;

				DfxBounds itemBounds;
				itemBounds.empty();

				glm::mat4 transMat = transformationMatrix();

				for (size_t inx = 0; inx < mdataPtr->getVertexCount(); inx++) {
					glm::vec3 vert = glm::vec3(getTransform() * glm::vec4(mdataPtr->getVertex(inx), 1.f));
					itemBounds.add(vert.x, vert.y, vert.z);
				}
				
				if ((!_positions->isNull() && _positions->size() > 0)) {

					for (size_t inx = 0; inx < _positions->size(); inx++) {
						glm::vec3 v(0);
						
						if (!_velocities->isNull() && _velocities->size()) {

							if (inx < _velocities->size()) {
								v = glm::vec3((*_velocities)[inx].x, (*_velocities)[inx].y, (*_velocities)[inx].z);
							}
						}

						glm::vec3 p((*_positions)[inx].x, (*_positions)[inx].y, (*_positions)[inx].z);

						bool rc = itemBounds.intersectsPoint(p.x, p.y, p.z);
						
						if (outside) {

							if (rc) {
								pos.push_back(p);
								vel.push_back(v);
							}
						}
						else {

							if (!rc) {
								pos.push_back(p);
								vel.push_back(v);
							}
						}
					}

					_opositions->resize(pos.size());
					_ovelocities->resize(pos.size());

					for (size_t inx = 0; inx < pos.size(); inx++) {
						(*_opositions)[inx].x = pos[inx].x;
						(*_opositions)[inx].y = pos[inx].y;
						(*_opositions)[inx].z = pos[inx].z;

						(*_ovelocities)[inx].x = vel[inx].x;
						(*_ovelocities)[inx].y = vel[inx].y;
						(*_ovelocities)[inx].z = vel[inx].z;
					}
				}
			}
		}
	
		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void ParticleKillVolume::onDataChanged(DfxEntityDataInstance *instance) {
	
	}

	void ParticleKillVolume::draw() {


	}

	void ParticleKillVolume::load(std::ifstream &) {

	}

	void ParticleKillVolume::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> ParticleKillVolume::exportData() {
		return mapEntityProperties(this);
	}

	bool ParticleKillVolume::isExportable() {
		return true;
	}

	void *ParticleKillVolume::creator(DfxGlobalEntityName uniquename) {
		return new ParticleKillVolume(uniquename);
	}

	void ParticleKillVolume::initialize() {
		DfxCSimulationEntity::initialize();

		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inType, "type", new DfxEntityPropertyTypes::DfxEnumPropertyType());
		DfxEntityDB::createProperty(inPosition, "in_position", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inRotation, "in_rotation", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inScale, "in_scale", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inVelocityArray, "in_velocities", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
		DfxEntityDB::createProperty(inPositionArray, "in_positions", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
		DfxEntityDB::createProperty(outVelocityArray, "out_velocities", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
		DfxEntityDB::createProperty(outPositionArray, "out_positions", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
	}

	glm::mat4  ParticleKillVolume::getTransform() {
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