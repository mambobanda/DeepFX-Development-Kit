/*
* The Mailo Today Solutions Software License
*
* Copyright 2017 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#include "DfxCBulletRigidArray.h"

namespace dfxcore {

	namespace bullet {

		glm::mat4 RigidCompilation::transform() {

			glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, pos.z));

			trans *= glm::rotate(glm::mat4(), glm::radians(rot.x), glm::vec3(1.f, .0f, .0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(rot.y), glm::vec3(0.f, 1.f, .0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(rot.z), glm::vec3(0.f, .0f, 1.f));

			return trans;
		}

		DfxCBulletRigidArray::DfxCBulletRigidArray(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {

			setLogicalName("Bullet Rigid Array");

			setPropertyState(outID, DfxEntityPropertyState::OUTPUT_STATE);
			setPropertyState(outTransforms, DfxEntityPropertyState::OUTPUT_STATE);
			setPropertyState(inProxyMeshArray, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inShapeType, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inMass, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inMassArray, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inFriction, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inFrictionArray, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inBounciness, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inBouncinessArray, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inLinearDampening, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inLinearDampeningArray, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inAngularDampening, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inAngularDampeningArray, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inInitialLinearVelocity, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inInitialLinearVelocityArray, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inInitialAngularVelocity, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inInitialAngularVelocityArray, DfxEntityPropertyState::INPUT_STATE);

			defaultSettings();
		}

		DfxCBulletRigidArray::~DfxCBulletRigidArray() {

		}


		void DfxCBulletRigidArray::compute() {

			using namespace DfxEntityPropertyTypes;

			if (propertyHasBinding(inProxyMeshArray)) {

				ProxyObjectArray *proxies = (ProxyObjectArray*)getProperty(inProxyMeshArray);
				Matrix4x4Array *mat = (Matrix4x4Array*)getProperty(outTransforms);

				mat->deleteArray();

				if (!proxies->isNull()) {

					if (proxies->size() > 0) {

						mat->resize(proxies->size());

						for (int inx = 0; inx < proxies->size(); inx++) {
							buildShape(inx, (*proxies)[inx]);
						}
					}
				}

				makePropertyBindingDirty(outTransforms);
				makeChildrenDirty();
			}
			else {
				Matrix4x4Array *mat= (Matrix4x4Array*)getProperty(outTransforms);
				mat->deleteArray();
			}

			getDataInstance()->makeClean();
		}

		void DfxCBulletRigidArray::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void DfxCBulletRigidArray::init() {

		}

		void DfxCBulletRigidArray::clear() {

		}

		void DfxCBulletRigidArray::done() {

		}

		void DfxCBulletRigidArray::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> DfxCBulletRigidArray::exportData() {
			return mapEntityProperties(this);
		}

		bool DfxCBulletRigidArray::isExportable() {
			return true;
		}

		void *DfxCBulletRigidArray::creator(DfxGlobalEntityName uniquename) {
			return new DfxCBulletRigidArray(uniquename);
		}

		void DfxCBulletRigidArray::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outID, "solver_id", new DfxEntityPropertyTypes::Size_TPropertyType);
			DfxEntityDB::createProperty(outTransforms, "transforms4x4", new DfxEntityPropertyTypes::DfxMatrix4x4ArrayPropertyType);
			DfxEntityDB::createProperty(inProxyMeshArray, "proxy_array", new DfxEntityPropertyTypes::ProxyObjectArrayPropertyType);
			DfxEntityDB::createProperty(inShapeType, "shape_type", new DfxEntityPropertyTypes::DfxEnumPropertyType);
			DfxEntityDB::createProperty(inMass, "mass", new DfxEntityPropertyTypes::FloatPropertyType);
			DfxEntityDB::createProperty(inMassArray, "mass_array", new DfxEntityPropertyTypes::FloatArrayPropertyType);
			DfxEntityDB::createProperty(inFriction, "friction", new DfxEntityPropertyTypes::FloatPropertyType);
			DfxEntityDB::createProperty(inFrictionArray, "friction_array", new DfxEntityPropertyTypes::FloatArrayPropertyType);
			DfxEntityDB::createProperty(inBounciness, "bounciness", new DfxEntityPropertyTypes::FloatPropertyType);
			DfxEntityDB::createProperty(inBouncinessArray,"bounciness_array", new DfxEntityPropertyTypes::FloatArrayPropertyType);
			DfxEntityDB::createProperty(inLinearDampening, "linear_dampening", new DfxEntityPropertyTypes::FloatPropertyType);
			DfxEntityDB::createProperty(inLinearDampeningArray,"linear_dampening_array", new DfxEntityPropertyTypes::FloatArrayPropertyType);
			DfxEntityDB::createProperty(inAngularDampening, "angular_dampening", new DfxEntityPropertyTypes::FloatPropertyType);
			DfxEntityDB::createProperty(inAngularDampeningArray,"angular_dampening_array", new DfxEntityPropertyTypes::FloatArrayPropertyType);
			DfxEntityDB::createProperty(inInitialLinearVelocity, "initial_linear_velocity", new DfxEntityPropertyTypes::Float3PropertyType);
			DfxEntityDB::createProperty(inInitialLinearVelocityArray,"initial_linear_velocity_array", new DfxEntityPropertyTypes::Float3ArrayPropertyType);
			DfxEntityDB::createProperty(inInitialAngularVelocity,"initial_angular_velocity", new DfxEntityPropertyTypes::Float3PropertyType);
			DfxEntityDB::createProperty(inInitialAngularVelocityArray,"initial_angular_velocity_array", new DfxEntityPropertyTypes::Float3ArrayPropertyType);
		}

		void DfxCBulletRigidArray::defaultSettings() {
			*(float*)getProperty(inMass) = 10.f;
			*(float*)getProperty(inFriction) = 0.5f;
			*(float*)getProperty(inBounciness) = 0.5f;
			*(float*)getProperty(inLinearDampening) = 0.f;
			*(float*)getProperty(inAngularDampening) = 0.f;
		}

		void DfxCBulletRigidArray::buildShape(size_t index, DfxEntityPropertyTypes::ProxyObject &po) {
			using namespace DfxEntityPropertyTypes;

			if (po.meshData.getVertexCount() > 3 && ((int)(po.meshData.getIndexCount() / 3)) >= 1) {

				btTriangleMesh *mesh = new btTriangleMesh();

				for (int i = 0; i < po.meshData.getIndexCount() / 3; i++) {
					int counter = i * 3;

					btVector3 vert0(po.meshData.getVertex(po.meshData.getIndex(counter)).x,
						po.meshData.getVertex(po.meshData.getIndex(counter)).y,
						po.meshData.getVertex(po.meshData.getIndex(counter)).z);

					btVector3 vert1(po.meshData.getVertex(po.meshData.getIndex(counter + 1)).x,
						po.meshData.getVertex(po.meshData.getIndex(counter + 1)).y,
						po.meshData.getVertex(po.meshData.getIndex(counter + 1)).z);

					btVector3 vert2(po.meshData.getVertex(po.meshData.getIndex(counter + 2)).x,
						po.meshData.getVertex(po.meshData.getIndex(counter + 2)).y,
						po.meshData.getVertex(po.meshData.getIndex(counter + 2)).z);

					mesh->addTriangle(vert0, vert1, vert2);
				}

				DfxEntityPropertyTypes::Float3 scale = po.scale;
				btVector3 scl(scale.x, scale.y, scale.z);

				btGImpactMeshShape *concaveShape = new btGImpactMeshShape(mesh);
				concaveShape->setLocalScaling(scl);
				concaveShape->postUpdate();
				concaveShape->updateBound();
				
				//shapes.push_back(std::pair<int, btCollisionShape*>(0, concaveShape));
				RigidCompilation *comp = new RigidCompilation;
				comp->index = index;
				comp->shape = concaveShape;
				comp->pos = float3ToVec3(po.position);
				comp->rot = float3ToVec3(po.rotation);
				comp->scl = float3ToVec3(po.scale);
				comp->ref = this;

				shapes.push_back(comp);
			}
		}

		std::vector<RigidCompilation*> DfxCBulletRigidArray::getAllShapes() {
			return shapes;
		}

		void  DfxCBulletRigidArray::setTransform(size_t inx, DfxEntityPropertyTypes::Matrix4x4 inmat) {
			using namespace DfxEntityPropertyTypes;

			Matrix4x4Array *mat = (Matrix4x4Array*)getProperty(outTransforms);

			if (!mat->isNull() && mat->size() > 0) {
				if (inx >= 0 && inx < mat->size()) {
					(*mat)[inx].copy(inmat);
				}
			}	
		}

		DfxEntityPropertyTypes::Matrix4x4  DfxCBulletRigidArray::getTransform(size_t inx) {
			using namespace DfxEntityPropertyTypes;

			Matrix4x4Array *mat = (Matrix4x4Array*)getProperty(outTransforms);

			if (!mat->isNull() && mat->size() > 0) {
				if (inx >= 0 && inx < mat->size()) {
					return (*mat)[inx];
				}
			}
			
			return Matrix4x4();
		}
	}
}