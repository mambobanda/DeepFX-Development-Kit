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
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include <glm/gtx/quaternion.hpp>
#include "DfxBulletActorNode.h"
#include "DfxBulletActorHandler.h"
#include "DfxObjectHandler.h"
#include "DfxPointCloudHandler.h"
#include "DfxBulletRigidBody.h"
#include "DfxBulletKinematicRigidBody.h"
#include "DfxBulletDeformingRigidBody.h"
#include "DfxBulletStaticBody.h"
#include "DfxBulletSoftBody.h"

std::string DfxBulletActorNode::_uniqueName = "UID_BULLET_ACTOR_NODE";;

DfxBulletActorNode::DfxBulletActorNode(deepsg::SgStageNode *st): DfxSimulationStageActorBase(st, bullet::MAIN_ACTOR) {

	std::string targetPath = "";

	sourceQControlenums.push_back(std::pair<std::string, unsigned int>("None", Source::NONE));
	sourceQControlenums.push_back(std::pair<std::string, unsigned int>("Geometry", Source::GEOMETRY));
	sourceQControlenums.push_back(std::pair<std::string, unsigned int>("Point Cloud", Source::POINT_CLOUD));

	shapeQControlenums.push_back(std::pair<std::string, unsigned int>("None", Shape::NONE));
	shapeQControlenums.push_back(std::pair<std::string, unsigned int>("Source", Shape::SOURCE));
	shapeQControlenums.push_back(std::pair<std::string, unsigned int>("Reference", Shape::REFERENCE));

	typeQControlenums.push_back(std::pair<std::string, unsigned int>("None", Type::NONE));
	typeQControlenums.push_back(std::pair<std::string, unsigned int>("Static Object", Type::STATIC_OBJECT));
	typeQControlenums.push_back(std::pair<std::string, unsigned int>("Rigid Object", Type::RIGID_OBJECT));
	typeQControlenums.push_back(std::pair<std::string, unsigned int>("Soft Body Object", Type::SOFT_BODY_OBJECT));
	typeQControlenums.push_back(std::pair<std::string, unsigned int>("Kinematic Object", Type::KINEMATIC_OBJECT));
	typeQControlenums.push_back(std::pair<std::string, unsigned int>("Deforming Object", Type::DEFORMING_OBJECT));

	targetQControlenums.push_back(std::pair<std::string, unsigned int>("None", Target::NONE));
	targetQControlenums.push_back(std::pair<std::string, unsigned int>("Source", Target::SOURCE));
	targetQControlenums.push_back(std::pair<std::string, unsigned int>("Reference", Target::REFERENCE));

	bulletCollisionQControlenums.push_back(std::pair<std::string, unsigned int>("None", BulletCollisionType::NONE));
	bulletCollisionQControlenums.push_back(std::pair<std::string, unsigned int>("Triangles", BulletCollisionType::TRIANGLES));
	bulletCollisionQControlenums.push_back(std::pair<std::string, unsigned int>("Convex Hull", BulletCollisionType::CONVEX_HULL));
	bulletCollisionQControlenums.push_back(std::pair<std::string, unsigned int>("Convex Decomposition", BulletCollisionType::CONVEX_DECOMPOSITION));
	bulletCollisionQControlenums.push_back(std::pair<std::string, unsigned int>("Cube", BulletCollisionType::CUBE));
	bulletCollisionQControlenums.push_back(std::pair<std::string, unsigned int>("Sphere", BulletCollisionType::SPHERE));

}

DfxBulletActorNode::~DfxBulletActorNode() {
	clearBody();
}

std::string DfxBulletActorNode::uniqueName() {
	return _uniqueName;
}

std::string DfxBulletActorNode::getUniqueName() {
	return _uniqueName;
}

size_t DfxBulletActorNode::actorState() {
	return _actorState;
}

void DfxBulletActorNode::setActorState(size_t st) {
	_actorState = st;
}

std::string DfxBulletActorNode::sourcePath() {
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {
		return any->rawData<fl::DataObject>()->property("source_path")->data<std::string>();
	}

	return "";
}

void DfxBulletActorNode::setSourcePath(std::string ref) {
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {
		any->rawData<fl::DataObject>()->property("source_path")->setData<std::string>(ref);
	}
}

void DfxBulletActorNode::initPointCloudTarget(ds::PointCloud *pcloud) {
	ds::Channel *useTransformMatrixOverride = nullptr;
	ds::Channel *transformOverride = nullptr;
	ds::Channel *instanceShapePath = nullptr;

	useTransformMatrixOverride = pcloud->channel("@useTransformMatrixOverride");
	if (useTransformMatrixOverride == nullptr) {
		pcloud->addAttribute<int>("@useTransformMatrixOverride");
	}
	pcloud->fill<int>("@useTransformMatrixOverride", 1);

	transformOverride = pcloud->channel("@transformOverride");
	if (transformOverride == nullptr) {
		pcloud->addAttribute<glm::mat4>("@transformOverride");
	}
	pcloud->fill<glm::mat4>("@transformOverride", glm::mat4(1.f));

	instanceShapePath = pcloud->channel("@instanceShapePath");
	if (instanceShapePath == nullptr) {
		pcloud->addAttribute<std::string>("@instanceShapePath");
	}
	pcloud->fill<std::string>("@instanceShapePath", "");
}

std::vector<DfxBulletAgent*> &DfxBulletActorNode::agents() {
	return _agents;
}

void DfxBulletActorNode::spawn() {

	DfxBulletActorHandler handler;

	if (handler.fetch(referencePath())) {

		auto dao = handler.dataObject()->rawData<fl::DataObject>();
		
		auto tset = dao->property("fluent_set")->data<std::string>();
		auto source_type = dao->property("source_type")->data<unsigned int>();
		auto shape_type  = dao->property("shape_type")->data<unsigned int>();
		auto collision_shape = dao->property("bt_collision_shape_type")->data<unsigned int>();
		auto target		 = dao->property("target")->data<unsigned int>();
		auto shape_reference  = dao->property("shape_reference_path")->data<std::string>();
		auto target_reference = dao->property("target_reference_path")->data<std::string>();

		auto bt_object_type = dao->property("bt_object_type")->data<unsigned int>();

		deepsg::SgNode *sourceNode = nullptr;
		deepsg::SgNode *shapeNode  = nullptr;
		std::string    shapeNodePath = "";
		deepsg::SgNode *targetNode = nullptr;

		// Source
		if (source_type == Source::GEOMETRY) {
			DfxObjectHandler handler;

			if (handler.fetch(sourcePath())) {
				sourceNode = handler.geoNode();
			}
		}
		else if (source_type == Source::POINT_CLOUD) {
			DfxPointCloudHandler handler;

			if (handler.fetch(sourcePath())) {
				sourceNode = handler.pointCloudNode();
			}
		}

		// Shape 
		if (shape_type == Shape::SOURCE) {

			if (source_type == Source::GEOMETRY) {
				DfxObjectHandler handler;

				if (handler.fetch(sourcePath())) {
					shapeNode = handler.geoNode();
					shapeNodePath = sourcePath();
				}
			}
			else if (source_type == Source::POINT_CLOUD) {
				DfxPointCloudHandler handler;

				if (handler.fetch(sourcePath())) {
					shapeNode = handler.pointCloudNode();
					shapeNodePath = sourcePath();
				}
			}

		}
		else if (shape_type == Shape::REFERENCE) {

			auto any = DfxSpace::getInstance()->findAbsolute(shape_reference);

			if (any != nullptr) {

				if (any->type() == fl::type::DATA_OBJECT) {
					auto dao = any->rawData<fl::DataObject>();

					if (dao->hasProperty("sg_geo_node")) {
						shapeNode = dao->property("sg_geo_node")->rawData<deepsg::SgGeoNode>();
						shapeNodePath = shape_reference;
					}
					else if (dao->hasProperty("sg_point_cloud_node")) {
						shapeNode = dao->property("sg_point_cloud_node")->rawData<deepsg::SgPointCloudNode>();
						shapeNodePath = shape_reference;
					}
				}

			}
		}

		// Target 
		if (target == Target::SOURCE) {

			if (source_type == Source::GEOMETRY) {
				DfxObjectHandler handler;

				if (handler.fetch(sourcePath())) {
					targetNode = handler.geoNode();
				}
			}
			else if (source_type == Source::POINT_CLOUD) {
				DfxPointCloudHandler handler;

				if (handler.fetch(sourcePath())) {
					
					targetNode = handler.pointCloudNode();
			
					auto pcloud = ((deepsg::SgPointCloudNode*)targetNode)->geo();

					initPointCloudTarget(pcloud);
				}
			}

		}
		else if (target == Target::REFERENCE) {

			auto any = DfxSpace::getInstance()->findAbsolute(shape_reference);
			
			if (any != nullptr) {

				if (any->type() == fl::type::DATA_OBJECT) {

					auto dao = any->rawData<fl::DataObject>();

					if (dao->hasProperty("sg_geo_node")) {
						targetNode = dao->property("sg_geo_node")->rawData<deepsg::SgGeoNode>();
					}
					else if (dao->hasProperty("sg_point_cloud_node")) {

						targetNode = dao->property("sg_point_cloud_node")->rawData<deepsg::SgPointCloudNode>();
						auto pcloud = ((deepsg::SgPointCloudNode*)targetNode)->geo();
						
						initPointCloudTarget(pcloud);
					}
				}
			}
		}

		// 
		if (source_type == Source::GEOMETRY) {
			if (bt_object_type == Type::RIGID_OBJECT) {
				
				if (shapeNode != nullptr) {

					if (shapeNode->getType() == deepsg::SgNodeType::GEOMETRY) {

						auto transform = bulletTransform(sourceNode);
						auto agent = spawnRigidBodyAgent((deepsg::SgGeoNode*)shapeNode, transform, collision_shape, targetNode);
						agent->worldTransform = transform;

						if (agent != nullptr) {
							_agents.push_back(agent);
						}
					}
				}
			}
			else if (bt_object_type == Type::KINEMATIC_OBJECT) {

				if (shapeNode != nullptr) {

					if (shapeNode->getType() == deepsg::SgNodeType::GEOMETRY) {

						auto transform = bulletTransform(sourceNode);
						auto agent = spawnKinematicRigidBodyAgent((deepsg::SgGeoNode*)shapeNode, transform, collision_shape);

						if (agent != nullptr) {
							_agents.push_back(agent);
							agent->worldTransform = transform;

						}
					}
				}
			}
			else if (bt_object_type == Type::DEFORMING_OBJECT) {

				if (shapeNode != nullptr) {

					if (shapeNode->getType() == deepsg::SgNodeType::GEOMETRY) {

						auto transform = bulletTransform(sourceNode);
						auto agent = spawnDeformingRigidBodyAgent((deepsg::SgGeoNode*)shapeNode, transform);
						agent->worldTransform = transform;

						if (agent != nullptr) {
							_agents.push_back(agent);
						}
					}
				}
			}
			else if (bt_object_type == Type::STATIC_OBJECT) {

				if (shapeNode != nullptr) {

					if (shapeNode->getType() == deepsg::SgNodeType::GEOMETRY) {

						auto transform = bulletTransform(sourceNode);
						auto agent = spawnStaticBodyAgent((deepsg::SgGeoNode*)shapeNode, transform, collision_shape);
						agent->worldTransform = transform;

						if (agent != nullptr) {

							_agents.push_back(agent);
						}
					}
				}
			}
			else if (bt_object_type == Type::SOFT_BODY_OBJECT) {

				if (shapeNode != nullptr) {

					if (shapeNode->getType() == deepsg::SgNodeType::GEOMETRY) {

						float totalMass = handler.dataObject()->rawData<fl::DataObject>()->property("soft_mass")->data<float>();

						auto transform = bulletTransform(sourceNode);
						auto agent = spawnSoftBodyAgent((deepsg::SgGeoNode*)shapeNode, transform, targetNode, totalMass);

						if (agent != nullptr) {
							agent->worldTransform = transform;
							_agents.push_back(agent);
						}
					}
				}
			}
		}
		else if (source_type == Source::POINT_CLOUD) {
			auto pcloud = ((deepsg::SgPointCloudNode*)sourceNode)->geo();

			if (shapeNode != nullptr) {

				if (shapeNode->getType() == deepsg::SgNodeType::GEOMETRY && !tset.empty()) {

					auto initialRotationChannel = pcloud->channel("@btInitialRotation");
					auto initialPositionChannel = pcloud->channel("@btInitialPosition");

					auto set = DfxFluentSetManager::getInstance()->getSet(tset);
					if (set == nullptr) {
						set = new DfxFluentSet(DfxFluentSet::SIMPLEX);
						DfxFluentSetManager::getInstance()->addSet(tset, set);
					}

					if (set != nullptr) {
						if (set->type() == DfxFluentSet::SIMPLEX) {

							for (int inx = 0; inx < pcloud->size(); inx++) {

								glm::vec3 pos;
								glm::quat quat;

								if (initialPositionChannel != nullptr) {
									pos = initialPositionChannel->at<glm::vec3>()[inx];
								}
								else {
									auto position = pcloud->channel("@position");
									pos = position->at<glm::vec3>()[inx];
								}

								auto globalTrans = deepsg::SgRootNode::getInstance()->getTransformation4x4(sourceNode);
								auto finalPos = glm::vec3(globalTrans * glm::vec4(pos, 1.f));
								auto xform = glm::translate(glm::mat4(1.f), finalPos);

								bool success = false;
								auto index = util::objectToFluentSet(
									((deepsg::SgGeoNode*)shapeNode)->getGeometryAdapter(),
									set,
									tset,
									shapeNode->referencePath(),
									xform,
									success,
									false,
									shapeNode->scale());

								unsigned int ftype = 0;
								unsigned int fCollisionShape = 0;

								if (bt_object_type == 1) ftype = 1;
								else if (bt_object_type == 2) ftype = 0;
								else if (bt_object_type == 3) ftype = 2;
								else if (bt_object_type == 4) ftype = 3;

								if (collision_shape == 1) fCollisionShape = 1;

								auto  agent = ((DfxBulletStageNode*)stage())->addFluentSimplex(index, ftype, fCollisionShape);

								if (agent != nullptr) {
									applyFixedMassRigidBodyProperties(((DfxBulletFluentBody*)agent->fluentBody)->rigidBody, ((DfxBulletFluentBody*)agent->fluentBody)->shape);
									applyFixedRigidBodyProperties(((DfxBulletFluentBody*)agent->fluentBody)->rigidBody);
								}
							}
						}
					}
				}
			}
		}
	}

	setActorState(DfxBulletActorNode::ActorState::NONE);
}

void DfxBulletActorNode::clearBody() {
	for (auto agent: _agents) {
		if (agent->body != nullptr) {
			agent->body->freeBody();
			delete agent->body;
			agent->body = nullptr;
			delete agent->btTrimesh;
		}
	}
}

void DfxBulletActorNode::clearCache() {
	for (auto agent : _agents) {
		if (agent->cache != nullptr) {
			delete agent->transformOverride;
			agent->transformOverride = nullptr;

			delete agent->pointCloudCacheFilter;
			agent->pointCloudCacheFilter = nullptr;

			delete agent->pointCacheDeformer;
			agent->pointCacheDeformer = nullptr;

			agent->cache->clear();
			delete agent->cache;
			agent->cache = nullptr;
		}
	}
}

void  DfxBulletActorNode::clearAgents() {
	for (auto agent : _agents) delete agent;
	_agents.clear();
}

void DfxBulletActorNode::reset() {
	for (auto agent : _agents) agent->reset();
}

bool DfxBulletActorNode::isReady() {
	return _ready;
}

void DfxBulletActorNode::cacheLastStepToTransform(DfxTime time, DfxBulletAgent *agent) {
	
	auto bulletBody = (DfxBulletRigidBody*)agent->body;

	auto xformCache = (DfxTransformCache*)agent->cache;
	auto frame = new DfxTransformFrame(motionStateTransform(agent));
	xformCache->insertFrame(time.frame, frame);
	frame->setIsCached(true);

}

void DfxBulletActorNode::cacheLastStepToPointCache(DfxTime time, DfxBulletAgent *agent) {

	auto bulletBody = (DfxBulletSoftBody*)agent->body;

	auto pointCache = (DfxPointCache*)agent->cache;

	if (pointCache != nullptr) {
		if (bulletBody->nodeCount() > 0) {

			auto frame = new DfxPointFrame;
			frame->initFrame(bulletBody->nodeCount());

			for (int i = 0; i < bulletBody->nodeCount(); i++) {
				frame->getPositions()[i] = bulletBody->nodePosition(i);
				frame->getVelocities()[i] = bulletBody->nodePosition(i);
				frame->getSpeeds()[i] = 0.f;
			}

			pointCache->insertFrame(time.frame, frame);
		}
	}
}

void DfxBulletActorNode::cacheLastStepAll(DfxTime time) {
	
	for ( auto agent : _agents ) {

		if (agent->updateTransformCache) {
			if (agent->cache != nullptr) {
				cacheLastStepToTransform(time, agent);
			}
		}

		if (agent->updatePointCache) {
			if (agent->cache != nullptr) {
				cacheLastStepToPointCache(time, agent);
			}
		}
	
	}
	
}

void DfxBulletActorNode::updateRigidObjects() {
	for (auto agent : _agents) {
		if (agent->body->type() == DfxBulletActorNode::Type::RIGID_OBJECT) {
			applyDynamicRigidBodyProperties(((DfxBulletRigidBody*)agent->body)->body());
		}
	}
}

void DfxBulletActorNode::updateSoftbodyObjects() {
	for (auto agent : _agents) {
		if (agent->body->type() == DfxBulletActorNode::Type::SOFT_BODY_OBJECT) {
			applyDynamicSoftBodyProperties(((DfxBulletSoftBody*)agent->body)->body());
		}
	}
}

void DfxBulletActorNode::updateStaticObjects() {
	for (auto agent : _agents) {
		if (agent->body->type() == DfxBulletActorNode::Type::STATIC_OBJECT) {
			applyDynamicStaticBodyProperties(((DfxBulletStaticBody*)agent->body)->body());
		}
	}
}

void DfxBulletActorNode::updateKinematicObjects() {
	for (auto agent : _agents) {

		if (agent->kinematic) {
			DfxBulletActorHandler handler;

			if (handler.fetch(referencePath())) {

				deepsg::SgNode *sourceNode = nullptr;
				auto dao = handler.dataObject()->rawData<fl::DataObject>();

				auto source_type = dao->property("source_type")->data<unsigned int>();

				applyDynamicKinematicBodyProperties(((DfxBulletKinematicRigidBody*)agent->body)->body());

				// Source
				if (source_type == Source::GEOMETRY) {
					DfxObjectHandler handler;

					if (handler.fetch(sourcePath())) {
						sourceNode = handler.geoNode();
						auto transform = bulletTransform(sourceNode);
						((DfxBulletKinematicRigidBody*)agent->body)->motionState()->setWorldTransform(transform);
					}
				}
				else if (source_type == Source::POINT_CLOUD) {
					/*DfxPointCloudHandler handler;

					if (handler.fetch(sourcePath())) {
						sourceNode = handler.pointCloudNode();
					}*/
				}
			}
		}
	}
}

void DfxBulletActorNode::updateDeformingObjects() {
	for (auto agent : _agents) {

		if (agent->deforming) {
			DfxBulletActorHandler handler;

			if (handler.fetch(referencePath())) {

				deepsg::SgNode *sourceNode = nullptr;
				auto dao = handler.dataObject()->rawData<fl::DataObject>();

				auto source_type = dao->property("source_type")->data<unsigned int>();

				// Source
				if (source_type == Source::GEOMETRY) {
					DfxObjectHandler handler;

					if (handler.fetch(sourcePath())) {
						sourceNode = handler.geoNode();
						auto transform = bulletTransform(sourceNode);
						((DfxBulletDeformingRigidBody*)agent->body)->motionState()->setWorldTransform(transform);
						((DfxBulletDeformingRigidBody*)agent->body)->updateShape(sourceNode);
					}
				}
				else if (source_type == Source::POINT_CLOUD) {
					/*DfxPointCloudHandler handler;

					if (handler.fetch(sourcePath())) {
						sourceNode = handler.pointCloudNode();
					}*/
				}
			}
		}
	}
}

// Spawn Static Body
DfxBulletAgent *DfxBulletActorNode::spawnStaticBodyAgent(deepsg::SgNode *node, btTransform startTransform, unsigned int shapeType) {

	DfxBulletAgent *agent = nullptr;
	btCollisionShape *shape = nullptr;
	btTriangleMesh *triMesh = nullptr;

	if (node->getType() == deepsg::SgNodeType::GEOMETRY) {

		auto geoNode = (deepsg::SgGeoNode*)node;

		auto adapter = geoNode->getGeometryAdapter();

		auto p = DfxBulletShapeBuilder::bvhTirangleShapeFromGeometry(geoNode->getGeometryAdapter(), geoNode->scale());
		shape = p.first;
		triMesh = p.second;
	}

	// Add to dynamics world if valid
	if (shape != nullptr) {

		agent = new DfxBulletAgent;
		agent->body = new DfxBulletStaticBody(dynamicsWorld());
		agent->rigidBody = true;
		agent->btTrimesh = triMesh;

		((DfxBulletRigidBody*)agent->body)->setShape(shape);
		((DfxBulletRigidBody*)agent->body)->initBody(startTransform);
		((DfxBulletRigidBody*)agent->body)->insertIntoWorld();

		agent->ready = true;
	}

	return agent;
}

// Spawn Rigid Body
DfxBulletAgent *DfxBulletActorNode::spawnRigidBodyAgent(deepsg::SgNode *node, btTransform startTransform, unsigned int shapeType, deepsg::SgNode *target) {
	
	DfxBulletAgent *agent = nullptr;
	btCollisionShape *shape = nullptr;
	btTriangleMesh *triMesh = nullptr;

	glm::vec3 scale(1.f);
	bool sourceValid = false;

	if (node->getType() == deepsg::SgNodeType::GEOMETRY) {
	
		auto geoNode = (deepsg::SgGeoNode*)node;
		scale = geoNode->scale();

		auto adapter = geoNode->getGeometryAdapter();

		if (adapter->getVertexCount() > 0) {

			if (shapeType == BulletCollisionType::CONVEX_HULL) {
				shape = DfxBulletShapeBuilder::convexHullShapeFromGeometry(geoNode->getGeometryAdapter(), geoNode->scale());
			}
			else if (shapeType == BulletCollisionType::TRIANGLES) {
				auto p = DfxBulletShapeBuilder::gimpactTirangleShapeFromGeometry(geoNode->getGeometryAdapter(), geoNode->scale());
				shape = p.first;
				triMesh = p.second;
			}
			else if (shapeType == BulletCollisionType::SPHERE) {
				shape = DfxBulletShapeBuilder::sphereShapeFromGeometry(geoNode);
			}
			else if (shapeType == BulletCollisionType::CUBE) {
				shape = DfxBulletShapeBuilder::boxShapeFromGeometry(geoNode);
			}
			else if (shapeType == BulletCollisionType::CONVEX_DECOMPOSITION) {

			}	
		}
	}

	// Add to dynamics world if valid
	if (shape != nullptr) {

		bool validTarget = false;

		agent = new DfxBulletAgent;
		agent->body = new DfxBulletRigidBody(dynamicsWorld());
		agent->cache = new DfxTransformCache;
		agent->transformOverride = nullptr;
		agent->pointCloudCacheFilter  = nullptr;
		agent->updateTransformCache = true;
		agent->rigidBody = true;
		agent->scale = scale;
		agent->btTrimesh = triMesh;

		if ( target->getType() == deepsg::SgNodeType::GEOMETRY ) {
			agent->transformOverride = new DfxTransformCacheOverride((DfxTransformCache*)agent->cache);
			target->addTransformOverride(agent->transformOverride);		
		}
		else if(target->getType() == deepsg::SgNodeType::POINT_CLOUD) {
			agent->pointCloudCacheFilter = new DfxBulletPointCloudCacheFilter;
			agent->pointCloudCacheFilter->setBulletActor(this);
			target->addNodeFilter(agent->pointCloudCacheFilter);
		}

		btVector3 inertia(0.f, 0.f, 0.f);

		((DfxBulletRigidBody*)agent->body)->setShape(shape);
		((DfxBulletRigidBody*)agent->body)->initBody(startTransform);
		applyFixedMassRigidBodyProperties(((DfxBulletRigidBody*)agent->body)->body(), ((DfxBulletRigidBody*)agent->body)->shape());
		((DfxBulletRigidBody*)agent->body)->insertIntoWorld();
		applyFixedRigidBodyProperties(((DfxBulletRigidBody*)agent->body)->body());

		if(validTarget) agent->ready = true;
	}
	
	return agent;
}

DfxBulletAgent *DfxBulletActorNode::spawnKinematicRigidBodyAgent(deepsg::SgNode *node, btTransform startTransform, unsigned int shapeType) {
	DfxBulletAgent *agent = nullptr;
	btCollisionShape *shape = nullptr;
	btTriangleMesh *triMesh = nullptr;

	bool sourceValid = false;

	if (node->getType() == deepsg::SgNodeType::GEOMETRY) {

		auto geoNode = (deepsg::SgGeoNode*)node;

		auto adapter = geoNode->getGeometryAdapter();

		auto p = DfxBulletShapeBuilder::bvhTirangleShapeFromGeometry(geoNode->getGeometryAdapter(), geoNode->scale());
		shape = p.first;
		triMesh = p.second;
	}

	// Add to dynamics world if valid
	if (shape != nullptr) {

		bool validTarget = false;

		agent = new DfxBulletAgent;
		agent->body = new DfxBulletKinematicRigidBody(dynamicsWorld());
		agent->kinematic = true;
		agent->rigidBody = true;
		agent->btTrimesh = triMesh;


		btVector3 inertia(0.f, 0.f, 0.f);

		((DfxBulletKinematicRigidBody*)agent->body)->setShape(shape);
		((DfxBulletKinematicRigidBody*)agent->body)->initBody(startTransform);
		((DfxBulletKinematicRigidBody*)agent->body)->body()->setMassProps(0, inertia);
		((DfxBulletKinematicRigidBody*)agent->body)->body()->setCollisionFlags(((DfxBulletKinematicRigidBody*)agent->body)->body()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		((DfxBulletKinematicRigidBody*)agent->body)->body()->setActivationState(DISABLE_DEACTIVATION);
		((DfxBulletKinematicRigidBody*)agent->body)->body()->setUserIndex(-1);
		((DfxBulletKinematicRigidBody*)agent->body)->insertIntoWorld();

		if (validTarget) agent->ready = true;
	}

	return agent;
}

DfxBulletAgent *DfxBulletActorNode::spawnDeformingRigidBodyAgent(deepsg::SgNode *node, btTransform startTransform) {
	DfxBulletAgent *agent = nullptr;

	bool sourceValid = false;

	// Add to dynamics world if valid
	bool validTarget = false;

	agent = new DfxBulletAgent;
	agent->body = new DfxBulletDeformingRigidBody(dynamicsWorld());
	agent->deforming = true;

	((DfxBulletDeformingRigidBody*)agent->body)->initBody(node, startTransform);

	if (((DfxBulletDeformingRigidBody*)agent->body)->body() != nullptr) {

		((DfxBulletDeformingRigidBody*)agent->body)->body()->setCollisionFlags(((DfxBulletDeformingRigidBody*)agent->body)->body()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		((DfxBulletDeformingRigidBody*)agent->body)->body()->setActivationState(DISABLE_DEACTIVATION);
		((DfxBulletDeformingRigidBody*)agent->body)->body()->setUserIndex(-1);
		((DfxBulletDeformingRigidBody*)agent->body)->insertIntoWorld();

		if (validTarget) agent->ready = true;
	}
	else {
		delete agent->body;
		delete agent;
	}
	
	return agent;
}

DfxBulletAgent *DfxBulletActorNode::spawnSoftBodyAgent(deepsg::SgNode *node, btTransform startTransform, deepsg::SgNode *target, float totalMass) {

	bool validTarget = false;

	DfxBulletAgent *agent = nullptr;
	auto body  = new DfxBulletSoftBody(dynamicsWorld(), softBodyWorldInfo());

	if (body->initBody(node)) {

		agent = new DfxBulletAgent();
		agent->body = body;
		agent->cache = new DfxPointCache;
		agent->updatePointCache  = true;

		int clusters = 1;
		float collision_margin = 0.05f;
		float linear_stiffness = 1.f;
		float angular_stiffness = .5f;
		float volume_stiffness  = .0f;
		bool use_pressure_volume = false;

		DfxBulletActorHandler handler;

		if (handler.fetch(referencePath())) {

			auto dao = handler.dataObject()->rawData<fl::DataObject>();

			clusters = dao->property("soft_clusters")->data<int>();
			collision_margin = dao->property("soft_collision_margin")->data<float>();
			
			linear_stiffness  = dao->property("soft_linear_stiffness")->data<float>();
			angular_stiffness = dao->property("soft_angular_stiffness")->data<float>();
			volume_stiffness  = dao->property("soft_volume_stiffness")->data<float>();

			use_pressure_volume = dao->property("soft_use_volume_pressure")->data<bool>();
		}
		
		btSoftBody::Material* pm = ((DfxBulletSoftBody*)body)->body()->appendMaterial();
		pm->m_kLST = linear_stiffness;
		pm->m_kAST = angular_stiffness;
		pm->m_kVST = volume_stiffness;

		((DfxBulletSoftBody*)body)->body()->generateBendingConstraints(2, pm);
		((DfxBulletSoftBody*)body)->body()->getCollisionShape()->setMargin(collision_margin);
		((DfxBulletSoftBody*)body)->body()->setTotalMass(totalMass);
		
		if (!use_pressure_volume) {
			((DfxBulletSoftBody*)body)->body()->setPose(false, true);
		}
		else {
			((DfxBulletSoftBody*)body)->body()->setPose(true, false);
		}
		
		((DfxBulletSoftBody*)body)->body()->m_cfg.piterations = 10;
		((DfxBulletSoftBody*)body)->body()->transform(startTransform);
		((DfxBulletSoftBody*)body)->body()->randomizeConstraints();
		((DfxBulletSoftBody*)body)->body()->m_cfg.collisions = btSoftBody::fCollision::SDF_RS + btSoftBody::fCollision::CL_SS;
		((DfxBulletSoftBody*)body)->body()->generateClusters(clusters);
			
		((DfxBulletSoftBody*)body)->insertIntoWorld();

		if (target->getType() == deepsg::SgNodeType::GEOMETRY) {
			agent->pointCacheDeformer = new DfxPointCacheDeformer((DfxPointCache*)agent->cache);
			((deepsg::SgGeoNode*)target)->addVertexFilter(agent->pointCacheDeformer);

			agent->transformOverride = new DfxTransformIdentityOverride;
			target->addTransformOverride(agent->transformOverride);
		}
		else if (target->getType() == deepsg::SgNodeType::POINT_CLOUD) {
			/*
			agent->pointCloudCacheFilter = new DfxBulletPointCloudCacheFilter;
			agent->pointCloudCacheFilter->setBulletActor(this);
			target->addNodeFilter(agent->pointCloudCacheFilter);
			*/
		}

		agent->ready = true;
		return agent;
	}
	else {
		delete body;
	}

	return nullptr;
}

glm::mat4 DfxBulletActorNode::motionStateTransform(DfxBulletAgent *agent) {
	auto bulletBody = (DfxBulletRigidBody*)agent->body;

	btTransform trans;
	bulletBody->motionState()->getWorldTransform(trans);

	glm::mat4 bulletMatrix = glm::mat4(1.0f);

	trans.getOpenGLMatrix(glm::value_ptr(bulletMatrix));

	bulletMatrix *= glm::scale(agent->scale);

	return  bulletMatrix;
}

void DfxBulletActorNode::preStep() {
	updateRigidObjects();
	updateStaticObjects();
	updateKinematicObjects();
	updateDeformingObjects();
	updateSoftbodyObjects();
}

void DfxBulletActorNode::postStep() {

}

btDiscreteDynamicsWorld *DfxBulletActorNode::dynamicsWorld() {
	return _dynamicsWorld;
}

void DfxBulletActorNode::setDynamicsWorld(btDiscreteDynamicsWorld *btWorld) {
	_dynamicsWorld = btWorld;
}

btSoftBodyWorldInfo *DfxBulletActorNode::softBodyWorldInfo() {
	return _SoftBodyWorldInfo;
}

void DfxBulletActorNode::setSoftBodyWorldInfo(btSoftBodyWorldInfo *sbodyWorldInfo) {
	_SoftBodyWorldInfo = sbodyWorldInfo;
}


void DfxBulletActorNode::applyFixedMassRigidBodyProperties(btRigidBody *body, btCollisionShape *shape) {

	DfxBulletActorHandler handler;

	if (handler.fetch(referencePath())) {

		auto dao = handler.dataObject()->rawData<fl::DataObject>();

		auto mass = dao->property("rigid_mass")->data<float>();

		btVector3 inertia(0.f, 0.f, 0.f);

		shape->calculateLocalInertia(mass, inertia);
		body->setMassProps(mass, inertia);

	}
}

void DfxBulletActorNode::applyFixedRigidBodyProperties(btRigidBody *body) {
	
	DfxBulletActorHandler handler;

	if (handler.fetch(referencePath())) {

		auto dao = handler.dataObject()->rawData<fl::DataObject>();

		auto lvx = dao->property("rigid_initial_vel_x")->data<float>();
		auto lvy = dao->property("rigid_initial_vel_y")->data<float>();
		auto lvz = dao->property("rigid_initial_vel_z")->data<float>();
		auto avx = dao->property("rigid_initial_ang_vel_x")->data<float>();
		auto avy = dao->property("rigid_initial_ang_vel_y")->data<float>();
		auto avz = dao->property("rigid_initial_ang_vel_z")->data<float>();	

		body->setLinearVelocity(btVector3(lvx, lvy, lvz));
		body->setAngularVelocity(btVector3(avx, avx, avx));
	}
	
}

void DfxBulletActorNode::applyDynamicRigidBodyProperties(btRigidBody *body) {

	if (body->isActive()) {

		DfxBulletActorHandler handler;

		if (handler.fetch(referencePath())) {

			auto dao = handler.dataObject()->rawData<fl::DataObject>();

			auto friction = dao->property("rigid_friction")->data<float>();
			auto rfriction = dao->property("rigid_rolling_friction")->data<float>();
			auto sfriction = dao->property("rigid_spinning_friction")->data<float>();
			auto bounciness = dao->property("rigid_bounciness")->data<float>();
			auto ldampening = dao->property("rigid_linear_dampening")->data<float>();
			auto adampening = dao->property("rigid_angular_dampening")->data<float>();
			auto lsleeping = dao->property("rigid_linear_sleeping")->data<float>();
			auto asleeping = dao->property("rigid_angular_sleeping")->data<float>();

			body->setFriction(friction);
			body->setRollingFriction(rfriction);
			body->setSpinningFriction(sfriction);
			body->setRestitution(bounciness);
			body->setDamping(ldampening, adampening);
			body->setSleepingThresholds(lsleeping, asleeping);
		}
	}
}

void DfxBulletActorNode::applyDynamicSoftBodyProperties(btSoftBody *body) {

	if (body->isActive()) {

		DfxBulletActorHandler handler;

		if (handler.fetch(referencePath())) {

			auto dao = handler.dataObject()->rawData<fl::DataObject>();

			auto s_mass = dao->property("soft_mass")->data<float>();
			auto vel_correction	= dao->property("soft_velocity_correction")->data<float>();
			auto vol_correction	= dao->property("soft_volume_conservation")->data<float>();
			auto shape_matching	= dao->property("soft_shape_matching")->data<float>();
			auto damping = dao->property("soft_damping")->data<float>();
			auto drag = dao->property("soft_drag")->data<float>();
			auto lift = dao->property("soft_lift")->data<float>();
			auto pressure = dao->property("soft_pressure")->data<float>();
			auto friction = dao->property("soft_friction")->data<float>();
			auto use_pressure_volume = dao->property("soft_use_volume_pressure")->data<bool>();
			// body->setTotalMass(s_mass);
			/*
			body->m_cfg.kVC = vol_correction;
			body->m_cfg.kDG = drag;
			body->m_cfg.kLF = lift;
			body->m_cfg.kPR = pressure;
			*/

			body->m_cfg.kMT = shape_matching;
			body->m_cfg.kDP = damping;
			body->m_cfg.kVCF = vel_correction;
			body->m_cfg.kDF  = friction;

			if (use_pressure_volume) {
				body->m_cfg.kVC = vol_correction;
				body->m_cfg.kPR = pressure;
			}
			else {
				body->m_cfg.kVC = 0.f;
				body->m_cfg.kPR = 0.f;
			}
		}
	}
}

void DfxBulletActorNode::applyDynamicKinematicBodyProperties(btRigidBody *body) {

	DfxBulletActorHandler handler;

	if (handler.fetch(referencePath())) {

		auto dao = handler.dataObject()->rawData<fl::DataObject>();

		auto friction = dao->property("kinematic_friction")->data<float>();
		auto bounciness = dao->property("kinematic_bounciness")->data<float>();

		body->setFriction(friction);
		body->setRestitution(bounciness);

	}
}

void DfxBulletActorNode::applyDynamicStaticBodyProperties(btRigidBody *body) {

	DfxBulletActorHandler handler;

	if (handler.fetch(referencePath())) {

		auto dao = handler.dataObject()->rawData<fl::DataObject>();

		auto friction = dao->property("static_friction")->data<float>();
		auto bounciness = dao->property("static_bounciness")->data<float>();

		body->setFriction(friction);
		body->setRestitution(bounciness);

	}
}

std::vector<std::pair<QWidget*, QString>> DfxBulletActorNode::propertyForms() {

	std::pair<QWidget*, QString> general(generalPropertiesForm(), "General");
	std::pair<QWidget*, QString> bulletObject(bulletObjectPropertiesForm(), "Bullet Object");
	
	return std::vector<std::pair<QWidget*, QString>> { general, bulletObject };
}

QWidget *DfxBulletActorNode::generalPropertiesForm() {
	
	QWidget *widget = new QWidget;
	QWidget *shapeWidget = new QWidget;

	DfxQStackControl *shapeWidgetStack = new DfxQStackControl;

	QVBoxLayout *main_layout = new QVBoxLayout;
	QVBoxLayout *shape_widget_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	shape_widget_layout->setContentsMargins(0, 0, 0, 0);
	shape_widget_layout->setAlignment(Qt::AlignTop);

	auto sourceControlPanel = new DfxControlPanel();

	auto setControlPanel = new DfxControlPanel();
	auto shapeControlPanel = new DfxControlPanel();
	//auto shapeSourceControlPanel = new DfxControlPanel();
	auto shapeReferenceControlPanel = new DfxControlPanel();
	/*auto shapeCubeControlPanel = new DfxControlPanel();
	auto shapeSphereControlPanel = new DfxControlPanel();*/

	auto targetControlPanel = new DfxControlPanel();

	DfxQControlArray *setControlArray = new DfxQControlArray();
	DfxQControlArray *sourceControlArray = new DfxQControlArray();

	DfxQControlArray *shapeControlArray  = new DfxQControlArray();
	DfxQControlArray *shapeReferenceControlArray = new DfxQControlArray();

	DfxQControlArray *targetControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	auto fluent_set = new DfxQLineEditControl(path + "fluent_set", "", false);
	auto source_type = new DfxQComboBoxControl(path + "source_type", "", sourceQControlenums, false);

	auto shape_type  = new DfxQComboBoxControl(path + "shape_type", "", shapeQControlenums, false);
	auto shape_reference = new DfxQFluentPathControl(path + "shape_reference_path", "", false);
	
	auto target_type = new DfxQComboBoxControl(path + "target", "", targetQControlenums, false);
	// auto source_type = new DfxQComboBoxControl(path + "bt_object_type", "", sourceQControlenums, false);


	// Set
	setControlArray->addControl("Set", fluent_set);

	setControlPanel->setTitle("Fluent Set");
	setControlPanel->setWidget(setControlArray);

	// Source
	sourceControlArray->addControl("Source", source_type);

	sourceControlPanel->setTitle("Source");
	sourceControlPanel->setWidget(sourceControlArray);

	// Shape
	shapeControlArray->addControl("Shape", shape_type);

	shapeControlPanel->setTitle("Shape");
	shapeControlPanel->setWidget(shapeControlArray);

	shapeReferenceControlArray->addControl("Reference", shape_reference);

	shapeReferenceControlPanel->setTitle("Shape Reference");
	shapeReferenceControlPanel->setWidget(shapeReferenceControlArray);

	/*shapeSourceControlPanel->setTitle("Source Shape");
	shapeCubeControlPanel->setTitle("Cube Shape");
	shapeSphereControlPanel->setTitle("Shpere Shape");*/

	shapeWidgetStack->addPage(new DfxQEmptyWidget());
	shapeWidgetStack->addPage(new DfxQEmptyWidget());
	shapeWidgetStack->addPage(shapeReferenceControlPanel);
	shapeWidgetStack->addPage(new DfxQEmptyWidget());
	shapeWidgetStack->addPage(new DfxQEmptyWidget());

	shape_widget_layout->addWidget(shapeControlPanel);
	shape_widget_layout->addWidget(shapeWidgetStack);

	shapeWidget->setLayout(shape_widget_layout);

	shapeWidgetStack->changeCurrentPage(shape_type->comboBox()->currentIndex());

	QObject::connect(shape_type->comboBox(), QOverload<int>::of(&QComboBox::activated), shapeWidgetStack, &DfxQStackControl::changeCurrentPage);

	// Target
	targetControlArray->addControl("Target", target_type);

	targetControlPanel->setTitle("Target");
	targetControlPanel->setWidget(targetControlArray);

	// main_layout->addWidget(sourceControlPanel);
	main_layout->addWidget(setControlPanel);
	main_layout->addWidget(shapeWidget);
	main_layout->addWidget(targetControlPanel);

	widget->setLayout(main_layout);

	return widget;
}

QWidget *DfxBulletActorNode::bulletObjectPropertiesForm() {

	QWidget *widget = new QWidget;
	QWidget *noneWidget = new QWidget;
	QWidget *staticWidget = new QWidget;
	QWidget *rigidWidget = new QWidget;
	QWidget *softWidget = new QWidget;
	QWidget *kinematicWidget = new QWidget;
	QWidget *deformingWidget = new QWidget;
	DfxQStackControl *widgetStack = new DfxQStackControl;

	QVBoxLayout *main_layout = new QVBoxLayout;
	QVBoxLayout *none_widget_layout = new QVBoxLayout;
	QVBoxLayout *static_widget_layout = new QVBoxLayout;
	QVBoxLayout *rigid_widget_layout = new QVBoxLayout;
	QVBoxLayout *soft_widget_layout = new QVBoxLayout;
	QVBoxLayout *kinematic_widget_layout  = new QVBoxLayout;
	QVBoxLayout *deforming_widget_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	none_widget_layout->setContentsMargins(0, 0, 0, 0);

	static_widget_layout->setContentsMargins(0, 0, 0, 0);
	static_widget_layout->setAlignment(Qt::AlignTop);

	rigid_widget_layout->setContentsMargins(0, 15, 0, 0);
	rigid_widget_layout->setAlignment(Qt::AlignTop);

	soft_widget_layout->setContentsMargins(0, 15, 0, 0);
	soft_widget_layout->setAlignment(Qt::AlignTop);

	kinematic_widget_layout->setContentsMargins(0, 15, 0, 0);
	kinematic_widget_layout->setAlignment(Qt::AlignTop);

	deforming_widget_layout->setContentsMargins(0, 15, 0, 0);
	deforming_widget_layout->setAlignment(Qt::AlignTop);

	auto typeControlPanel = new DfxControlPanel();

	auto staticBodyControlPanel = new DfxControlPanel();

	auto rigidBodyControlPanel = new DfxControlPanel();
	auto rigidBodyShapeControlPanel = new DfxControlPanel();
	auto rigidBodyInitialVelocityControlPanel = new DfxControlPanel();

	auto softBodyControlPanel = new DfxControlPanel();

	auto kinematicBodyControlPanel = new DfxControlPanel();

	auto deformingBodyControlPanel = new DfxControlPanel();
	auto deformingBodyShapeControlPanel = new DfxControlPanel();

	DfxQControlArray *typeControlArray = new DfxQControlArray();

	DfxQControlArray *staticControlArray = new DfxQControlArray();

	DfxQControlArray *rigidControlArray = new DfxQControlArray();
	DfxQControlArray *rigidShapeControlArray = new DfxQControlArray();
	DfxQControlArray *rigidInitialVelocityControlArray = new DfxQControlArray();

	DfxQControlArray *softControlArray = new DfxQControlArray();

	DfxQControlArray *kinematicControlArray = new DfxQControlArray();

	DfxQControlArray *deformingControlArray = new DfxQControlArray();
	DfxQControlArray *deformingShapeControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// Type Controls
	auto type_type = new DfxQComboBoxControl(path + "bt_object_type", "", typeQControlenums, false);

	// Static Controls
	auto static_friction = new DfxQDecimalSpinnerControl(path + "static_friction", "", false);
	auto static_bounciness = new DfxQDecimalSpinnerControl(path + "static_bounciness", "", false);

	// Kinematic Controls
	auto kinematic_friction = new DfxQDecimalSpinnerControl(path + "kinematic_friction", "", false);
	auto kinematic_bounciness = new DfxQDecimalSpinnerControl(path + "kinematic_bounciness", "", false);

	// Rigid Controls
	auto rigid_collision_shp = new DfxQComboBoxControl(path + "bt_collision_shape_type", "", bulletCollisionQControlenums, false);
	
	auto rigid_bounciness = new DfxQDecimalSpinnerControl(path + "rigid_bounciness", "", false);
	auto rigid_mass = new DfxQDecimalSpinnerControl(path + "rigid_mass", "", false);

	auto rigid_friction = new DfxQDecimalSpinnerControl(path + "rigid_friction", "",  false);
	auto rigid_rolling_friction  = new DfxQDecimalSpinnerControl(path + "rigid_rolling_friction", "", false);
	auto rigid_spinning_friction = new DfxQDecimalSpinnerControl(path + "rigid_spinning_friction", "", false);

	auto rigid_linear_dampening  = new DfxQDecimalSpinnerControl(path + "rigid_linear_dampening", "", false);
	auto rigid_angular_dampening = new DfxQDecimalSpinnerControl(path + "rigid_angular_dampening", "", false);

	auto rigid_linear_sleeping  = new DfxQDecimalSpinnerControl(path + "rigid_linear_sleeping", "", false);
	auto rigid_angular_sleeping = new DfxQDecimalSpinnerControl(path + "rigid_angular_sleeping", "", false);

	auto rigid_initial_linear_velx = new DfxQDecimalSpinnerControl(path + "rigid_initial_vel_x", "", false);
	auto rigid_initial_linear_vely = new DfxQDecimalSpinnerControl(path + "rigid_initial_vel_y", "", false);
	auto rigid_initial_linear_velz = new DfxQDecimalSpinnerControl(path + "rigid_initial_vel_z", "", false);

	auto rigid_initial_angular_velx = new DfxQDecimalSpinnerControl(path + "rigid_initial_ang_vel_x", "", false);
	auto rigid_initial_angular_vely = new DfxQDecimalSpinnerControl(path + "rigid_initial_ang_vel_y", "", false);
	auto rigid_initial_angular_velz = new DfxQDecimalSpinnerControl(path + "rigid_initial_ang_vel_z", "", false);

	// Soft Controls
	auto s_mass = new DfxQDecimalSpinnerControl(path + "soft_mass", "", false);
	auto v_correction   = new DfxQDecimalSpinnerControl(path + "soft_velocity_correction", "", false);
	auto v_conservation = new DfxQDecimalSpinnerControl(path + "soft_volume_conservation", "", false);
	auto s_matching = new DfxQDecimalSpinnerControl(path + "soft_shape_matching", "", false);
	auto damping = new DfxQDecimalSpinnerControl(path + "soft_damping", "", false);
	auto drag = new DfxQDecimalSpinnerControl(path + "soft_drag", "", false);
	auto lift = new DfxQDecimalSpinnerControl(path + "soft_lift", "", false);
	auto pressure = new DfxQDecimalSpinnerControl(path + "soft_pressure", "", false);
	auto soft_friction = new DfxQDecimalSpinnerControl(path + "soft_friction", "", false);
	auto soft_clusters = new DfxQIntegerSpinnerControl(path + "soft_clusters", "", false);
	auto soft_collision_margin = new DfxQDecimalSpinnerControl(path + "soft_collision_margin", "", false);
	auto soft_linear_stiffness = new DfxQDecimalSpinnerControl(path + "soft_linear_stiffness", "", false);
	auto soft_angular_stiffness = new DfxQDecimalSpinnerControl(path + "soft_angular_stiffness", "", false);
	auto soft_volume_stiffness  = new DfxQDecimalSpinnerControl(path + "soft_volume_stiffness", "", false);
	auto soft_use_volume_pressure = new DfxQCheckBoxControl(path + "soft_use_volume_pressure", "", false);

	// None
	noneWidget->setLayout(none_widget_layout);

	// Type
	typeControlArray->addControl("Type", type_type);

	typeControlPanel->setTitle("Object Type");
	typeControlPanel->setWidget(typeControlArray);

	// Static Body
	staticControlArray->addControl("Bounciness", static_bounciness);
	staticControlArray->addSpacer(0, 5);
	staticControlArray->addControl("Friction", static_friction);

	staticBodyControlPanel->setTitle("Static Body Properties");
	staticBodyControlPanel->setWidget(staticControlArray);

	static_widget_layout->addWidget(staticBodyControlPanel);

	staticWidget->setLayout(static_widget_layout);

	// Rigid Body
	rigidShapeControlArray->addControl("Collision Type", rigid_collision_shp);

	rigidBodyShapeControlPanel->setTitle("Collision Shape");
	rigidBodyShapeControlPanel->setWidget(rigidShapeControlArray);

	rigidControlArray->addControl("Mass", rigid_mass);
	rigidControlArray->addSpacer(0, 5);
	rigidControlArray->addControl("Bounciness", rigid_bounciness);
	rigidControlArray->addSpacer(0, 5);
	rigidControlArray->addControl("Friction", rigid_friction);
	rigidControlArray->addSpacer(0, 5);
	rigidControlArray->addControl("Rolling Friction", rigid_rolling_friction);
	rigidControlArray->addSpacer(0, 5);
	rigidControlArray->addControl("Spinning Friction", rigid_spinning_friction);
	rigidControlArray->addSpacer(0, 5);
	rigidControlArray->addControl("Linear Dampening", rigid_linear_dampening);
	rigidControlArray->addSpacer(0, 5);
	rigidControlArray->addControl("Angular Dampening", rigid_angular_dampening);
	rigidControlArray->addSpacer(0, 5);
	rigidControlArray->addControl("Linear Sleeping Threshhold", rigid_linear_sleeping);
	rigidControlArray->addSpacer(0, 5);
	rigidControlArray->addControl("Angular Sleeping Threshhold", rigid_angular_sleeping);

	rigidInitialVelocityControlArray->addControl("Initial Velocity X", rigid_initial_linear_velx);
	rigidInitialVelocityControlArray->addControl("Y", rigid_initial_linear_vely);
	rigidInitialVelocityControlArray->addControl("Z", rigid_initial_linear_velz);
	rigidInitialVelocityControlArray->addSpacer(0, 5);
	rigidInitialVelocityControlArray->addControl("Initial Angular Velocity X", rigid_initial_angular_velx);
	rigidInitialVelocityControlArray->addControl("Y", rigid_initial_angular_vely);
	rigidInitialVelocityControlArray->addControl("Z", rigid_initial_angular_velz);

	rigidBodyControlPanel->setTitle("Rigid Body Properties");
	rigidBodyControlPanel->setWidget(rigidControlArray);

	rigidBodyInitialVelocityControlPanel->setTitle("Initial Velocity");
	rigidBodyInitialVelocityControlPanel->setWidget(rigidInitialVelocityControlArray);

	rigid_widget_layout->addWidget(rigidBodyShapeControlPanel);
	rigid_widget_layout->addWidget(rigidBodyControlPanel);
	rigid_widget_layout->addWidget(rigidBodyInitialVelocityControlPanel);

	rigidWidget->setLayout(rigid_widget_layout);

	// Soft Body
	softControlArray->addControl("Mass", s_mass);
	softControlArray->addSpacer(0, 3);
	softControlArray->addControl("Velocity Correction", v_correction);
	softControlArray->addSpacer(0, 3);
	softControlArray->addControl("Stiffness", s_matching);
	softControlArray->addSpacer(0, 3);
	softControlArray->addControl("Friction", soft_friction);
	softControlArray->addSpacer(0, 3);
	softControlArray->addControl("Clusters", soft_clusters);
	softControlArray->addSpacer(0, 3);
	softControlArray->addControl("Collision Margin", soft_collision_margin);
	softControlArray->addSpacer(0, 3);
	softControlArray->addControl("Linear Bend Strength", soft_linear_stiffness);
	softControlArray->addSpacer(0, 3);
	softControlArray->addControl("Angular Bend Strength", soft_angular_stiffness);
	softControlArray->addSpacer(0, 3);
	softControlArray->addControl("Volume Bend Strength", soft_volume_stiffness);
	softControlArray->addSpacer(0, 3);
	softControlArray->addControl("Volume Conservation", v_conservation);
	softControlArray->addSpacer(0, 3);
	softControlArray->addControl("Pressure", pressure);
	softControlArray->addSpacer(0, 3);
	softControlArray->addControl("Use Volume / Pressure", soft_use_volume_pressure);

	softBodyControlPanel->setTitle("Soft Body");
	softBodyControlPanel->setWidget(softControlArray);

	soft_widget_layout->addWidget(softBodyControlPanel);

	softWidget->setLayout(soft_widget_layout);

	// Kinematic
	kinematicControlArray->addControl("Bounciness", kinematic_bounciness);
	kinematicControlArray->addSpacer(0, 5);
	kinematicControlArray->addControl("Friction", kinematic_friction);

	kinematicBodyControlPanel->setTitle("Kinematic Body Properties");
	kinematicBodyControlPanel->setWidget(kinematicControlArray);

	kinematic_widget_layout->addWidget(kinematicBodyControlPanel);

	kinematicWidget->setLayout(kinematic_widget_layout);

	// Deforming
	deformingWidget->setLayout(deforming_widget_layout);

	// Stack
	widgetStack->addPage(noneWidget);
	widgetStack->addPage(staticWidget);
	widgetStack->addPage(rigidWidget);
	widgetStack->addPage(softWidget);
	widgetStack->addPage(kinematicWidget);
	widgetStack->addPage(deformingWidget);

	widgetStack->changeCurrentPage(type_type->comboBox()->currentIndex());

	// Main layout
	main_layout->addWidget(typeControlPanel);
	main_layout->addWidget(widgetStack);

	widget->setLayout(main_layout);
	
	// Signal & Slots
	QObject::connect(type_type->comboBox(), QOverload<int>::of(&QComboBox::activated), widgetStack, &DfxQStackControl::changeCurrentPage);

	return widget;
}

btTransform DfxBulletActorNode::bulletTransform(deepsg::SgNode *node) {

	auto rotMat = node->rotationMatrix();
	auto pos = node->position();

	auto quat = glm::toQuat(rotMat);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(quat.x, quat.y, quat.z, quat.w));

	startTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));

	return startTransform;
}

btTransform DfxBulletActorNode::bulletTransform(glm::vec3 pos, glm::quat quat) {

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(quat.x, quat.y, quat.z, quat.w));

	startTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));

	return startTransform;
}
