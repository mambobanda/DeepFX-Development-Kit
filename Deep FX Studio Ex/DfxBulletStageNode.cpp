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

#include "DfxBulletStageNode.h"
#include "DfxBulletStageHandler.h"
#include "DfxBulletActorNode.h"
#include "DfxBulletRigidBody.h"
#include "DfxTimelineManager.h"
#include "DfxSimulationManager.h"
#include "DfxBulletConstraintPropHandler.h"
#include "DfxBulletFluentHelper.h"
#include "DfxBulletConstraintPropNode.h"

std::string DfxBulletStageNode::_uniqueName = "UID_BULLET_STAGE_NODE";

DfxBulletStageNode::DfxBulletStageNode(): DfxSimulationStageBase(DfxStageType::BULLET_STAGE) {
	_flDispatcher = new DfxBulletFlDispatcher;

	_simIsDirty = false;

	initDispatcher();
	initBullet();

	DfxSimulationManager::getInstance()->forceReset(true);

	QObject::connect(DfxSimulationManager::getInstance(), &DfxSimulationManager::resetTriggered, this, &DfxBulletStageNode::dirtySimulation);
	//QObject::connect((DfxBulletFlDispatcher*)_flDispatcher, &DfxBulletFlDispatcher::valueHasChanged, this, &DfxBulletStageNode::dirtySimulation);
	QObject::connect((DfxBulletFlDispatcher*)_flDispatcher, &DfxBulletFlDispatcher::valueHasChanged, DfxSimulationManager::getInstance(), &DfxSimulationManager::resetSimulationState);
}

DfxBulletStageNode::~DfxBulletStageNode() {
	delete _flDispatcher;
}

std::string DfxBulletStageNode::uniqueName() {
	return _uniqueName;
}

std::string DfxBulletStageNode::getUniqueName() {
	return _uniqueName;
}

fl::Dispatcher *DfxBulletStageNode::fluentDispacther() {
	return _flDispatcher;
}

bool DfxBulletStageNode::hasFluentSimplexAgent(size_t uid) {
	auto itr = _fluentSimplexAgents.find(uid);

	if (itr != _fluentSimplexAgents.end()) return true;
	else return false;
}

bool DfxBulletStageNode::hasFluentStreamAgent(size_t uid) {
	auto itr = _fluentStreamAgents.find(uid);

	if (itr != _fluentStreamAgents.end()) return true;
	else return false;
}

std::unordered_map<size_t, DfxBulletAgent*> &DfxBulletStageNode::fluentSimplexAgents() {
	return _fluentSimplexAgents;
}

std::unordered_map<size_t, DfxBulletAgent*> &DfxBulletStageNode::fluentStreamAgents() {
	return _fluentStreamAgents;
}

DfxBulletAgent *DfxBulletStageNode::findFluentSimplexAgent(size_t uid) {
	auto itr = _fluentSimplexAgents.find(uid);

	if (itr != _fluentSimplexAgents.end()) {
		return itr->second;
	}

	return nullptr;
}

DfxBulletAgent *DfxBulletStageNode::findFluentStreamAgent(size_t uid) {
	auto itr = _fluentStreamAgents.find(uid);

	if (itr != _fluentStreamAgents.end()) {
		return itr->second;
	}

	return nullptr;
}

void DfxBulletStageNode::setFluentSimplexAgentActivationState(std::vector<size_t> &indices, int state, std::string alias) {

	if (DfxFluentSimplexCollection::getInstance()->size() > 0) {

		std::string *alias_arr = nullptr;

		if (!alias.empty()) {
			auto ref = compute::getAttribute<std::string>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@alias", DfxFluenType::STRING);

			if (ref.sameAsRequestedType && ref.exists && ref._data != nullptr) {
				alias_arr = compute::extractAttrRef<std::string>(ref);
			}
		}

		for (auto &index : indices) {

			if (alias_arr != nullptr) {
				auto _alias = alias_arr[index];
				if (_alias != alias) continue;
			}

			auto flAgent = findFluentSimplexAgent(index);

			if (flAgent != nullptr) {
				if (flAgent->rigidBody) {
					flAgent->fluentBody->rigidBody->setActivationState(state);
				}
			}
		}
	}

}

void DfxBulletStageNode::setFluentStreamAgentActivationState(std::vector<size_t> &indices, int state, std::string alias) {

	if (DfxFluentSimplexCollection::getInstance()->size() > 0) {

		std::string *alias_arr = nullptr;

		if (!alias.empty()) {
			auto ref = compute::getAttribute<std::string>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@alias", DfxFluenType::STRING);

			if (ref.sameAsRequestedType && ref.exists && ref._data != nullptr) {
				alias_arr = compute::extractAttrRef<std::string>(ref);
			}
		}

		for (auto &index : indices) {

			if (alias_arr != nullptr) {
				auto _alias = alias_arr[index];
				if (_alias != alias) continue;
			}

			auto flAgent = findFluentStreamAgent(index);

			if (flAgent != nullptr) {
				if (flAgent->rigidBody) {
					flAgent->fluentBody->rigidBody->setActivationState(state);
				}
			}
		}
	}

}

void DfxBulletStageNode::updateFluentRigidBodyProperties() {
	if (DfxFluentSimplexCollection::getInstance()->size() > 0) {

		auto massRef = compute::getAttribute<float>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@mass", DfxFluenType::FLOAT);
		auto frictionRef = compute::getAttribute<float>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@friction", DfxFluenType::FLOAT);
		auto restitutionRef = compute::getAttribute<float>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@restitution", DfxFluenType::FLOAT);
		auto lDampeningRef = compute::getAttribute<float>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@l_dampening", DfxFluenType::FLOAT);
		auto aDampeningRef = compute::getAttribute<float>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@a_dampening", DfxFluenType::FLOAT);
		auto velRef = compute::getAttribute<float>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@force", DfxFluenType::VEC3);
		auto aVelRef = compute::getAttribute<float>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@angular_force", DfxFluenType::VEC3);

		size_t *uids = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<size_t>("@uid");
		float *mass_arr = nullptr;
		float *friction_arr = nullptr;
		float *restitution_arr = nullptr;
		float *l_dampening_arr = nullptr;
		float *a_dampening_arr = nullptr;
		glm::vec3 *force_arr = nullptr;
		glm::vec3 *a_force_arr = nullptr;

		if (massRef.exists && massRef.sameAsRequestedType && massRef._data != nullptr) mass_arr = compute::extractAttrRef<float>(massRef);
		if (frictionRef.exists && frictionRef.sameAsRequestedType && frictionRef._data != nullptr) friction_arr = compute::extractAttrRef<float>(frictionRef);
		if (restitutionRef.exists && restitutionRef.sameAsRequestedType && restitutionRef._data != nullptr) restitution_arr = compute::extractAttrRef<float>(restitutionRef);
		if (lDampeningRef.exists && lDampeningRef.sameAsRequestedType && lDampeningRef._data != nullptr) l_dampening_arr = compute::extractAttrRef<float>(lDampeningRef);
		if (aDampeningRef.exists && aDampeningRef.sameAsRequestedType && aDampeningRef._data != nullptr) a_dampening_arr = compute::extractAttrRef<float>(aDampeningRef);
		if (velRef.exists && velRef.sameAsRequestedType && velRef._data != nullptr) force_arr = compute::extractAttrRef<glm::vec3>(velRef);
		if (aVelRef.exists && aVelRef.sameAsRequestedType && aVelRef._data != nullptr) a_force_arr = compute::extractAttrRef<glm::vec3>(aVelRef);

		for (unsigned long int i = 0; i < DfxFluentSimplexCollection::getInstance()->size(); i++) {
			auto flAgent = findFluentSimplexAgent(i);
			if (flAgent != nullptr) {
				if (flAgent->rigidBody) {
					
					if (mass_arr != nullptr) {
						btVector3 inertia(0.f, 0.f, 0.f);
						flAgent->fluentBody->shape->calculateLocalInertia(mass_arr[i], inertia);
						flAgent->fluentBody->rigidBody->setMassProps(mass_arr[i], inertia);	
					}

					float lDamp = std::min(0.f, flAgent->fluentBody->rigidBody->getLinearDamping());
					float aDamp = std::min(0.f, flAgent->fluentBody->rigidBody->getAngularDamping());
					
					if (friction_arr != nullptr) flAgent->fluentBody->rigidBody->setFriction(friction_arr[i]);
					if (restitution_arr != nullptr) flAgent->fluentBody->rigidBody->setRestitution(restitution_arr[i]);
					
					if (l_dampening_arr != nullptr) lDamp = l_dampening_arr[i];
					if (a_dampening_arr != nullptr) aDamp = a_dampening_arr[i];
					flAgent->fluentBody->rigidBody->setDamping(lDamp, aDamp);

					if (force_arr != nullptr) {
						auto tforce = force_arr[i];
						btVector3 vel = flAgent->fluentBody->rigidBody->getLinearVelocity() + btVector3(tforce.x, tforce.y, tforce.z);
						flAgent->fluentBody->rigidBody->setLinearVelocity(vel);
					}

					if (a_force_arr != nullptr) {
						auto tforce = a_force_arr[i];
						btVector3 vel = flAgent->fluentBody->rigidBody->getAngularVelocity() + btVector3(tforce.x, tforce.y, tforce.z);
						flAgent->fluentBody->rigidBody->setAngularVelocity(vel);
					}
				}
			}

			if (force_arr != nullptr) force_arr[i] = glm::vec3(0.f);
			if (a_force_arr != nullptr) a_force_arr[i] = glm::vec3(0.f);
		}
	}
}

void DfxBulletStageNode::initDispatcher() {
	auto space = DfxSpace::getInstance();

	_flDispatcher->attachAny(space->findAbsolute("~/TIME/frame_start"));
	_flDispatcher->attachAny(space->findAbsolute("~/TIME/frame_end"));
}

size_t DfxBulletStageNode::addActor(fl::Any *any) { 
	
	if (any->type() ==  DfxFluentTypes::SG_NODE) {
		auto snode = any->rawData<deepsg::SgNode>();
		
		DfxBulletActorNode *actor_node = nullptr;

		if (snode->getType() == deepsg::SgNodeType::GEOMETRY) {
			DfxBulletActorHandler handler;
			handler.create();

			auto dao = handler.dataObject()->rawData<fl::DataObject>();
			actor_node = handler.createActorNode();
			actor_node->setStage(this);

			dao->property("source_type")->setData<unsigned int>(DfxBulletActorNode::Source::GEOMETRY);
			dao->property("shape_type")->setData<unsigned int>(DfxBulletActorNode::Shape::SOURCE);
			dao->property("bt_object_type")->setData<unsigned int>(DfxBulletActorNode::Type::RIGID_OBJECT);
			dao->property("bt_collision_shape_type")->setData<unsigned int>(DfxBulletActorNode::BulletCollisionType::CONVEX_HULL);
			dao->property("target")->setData<unsigned int>(DfxBulletActorNode::Target::SOURCE);

			actor_node->setSourcePath(snode->referencePath());
			actor_node->setActorState(DfxBulletActorNode::ActorState::SPAWN);

			_flDispatcher->attachBranch(snode->xFormProperties());
			_flDispatcher->attachBranch(handler.dataObject());

			DfxQSceneManager::getInstance()->moveNode(actor_node, actorCollectionNode());		
		}
		else if (snode->getType() == deepsg::SgNodeType::POINT_CLOUD) {
			DfxBulletActorHandler handler;
			handler.create();

			auto dao = handler.dataObject()->rawData<fl::DataObject>();
			actor_node = handler.createActorNode();
			actor_node->setStage(this);

			dao->property("source_type")->setData<unsigned int>(DfxBulletActorNode::Source::POINT_CLOUD);
			dao->property("shape_type")->setData<unsigned int>(DfxBulletActorNode::Shape::SOURCE);
			dao->property("bt_object_type")->setData<unsigned int>(DfxBulletActorNode::Type::RIGID_OBJECT);
			dao->property("bt_collision_shape_type")->setData<unsigned int>(DfxBulletActorNode::BulletCollisionType::CONVEX_HULL);
			dao->property("target")->setData<unsigned int>(DfxBulletActorNode::Target::SOURCE);

			actor_node->setSourcePath(snode->referencePath());
			actor_node->setActorState(DfxBulletActorNode::ActorState::SPAWN);

			actor_node->setDynamicsWorld(dynamicsWorld);

			_flDispatcher->attachBranch(snode->xFormProperties());
			_flDispatcher->attachBranch(handler.dataObject());

			DfxQSceneManager::getInstance()->moveNode(actor_node, actorCollectionNode());
		}
		else {
			DfxGlobal::console::println("Bullet doesn't support data.");
		}

		if (actor_node != nullptr) {
			actor_node->setDynamicsWorld(dynamicsWorld);
			actor_node->setSoftBodyWorldInfo(softBodyWorldInfo);
		}
	}

	return 0; 
};

void DfxBulletStageNode::removeActor(size_t uid) {
	auto node = actorCollectionNode()->findChild(uid);

	if (node != nullptr) {
		auto act = (DfxBulletActorNode*)node;
		act->clearBody();
		act->clearCache();
		act->reset();
		act->clearAgents();

		DfxQSceneManager::getInstance()->removeNode(node);

		dirtySimulation();
		DfxSimulationManager::getInstance()->resetSimulationState();
	}
};

DfxBulletAgent *DfxBulletStageNode::addFluentSimplex(size_t index, size_t type, size_t collision_shape) {

	DfxBulletFluentHelper helper;
	DfxBulletAgent *agent = nullptr;

	auto simplexObj = DfxFluentSimplexCollection::getInstance()->simplexObjects()[index];
	auto xform = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<glm::mat4>("@xform")[index];
	//auto uid = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<size_t>("@uid")[index];
	auto uid = index;
	auto setName = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<std::string>("@set_name")[index];

	auto fAgentItr = _fluentSimplexAgents.find(uid);
	
	if (fAgentItr == _fluentSimplexAgents.end()) {

		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 pos;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(xform, scale, rotation, pos, skew, perspective);

		if (type == 0) {
			agent = helper.spawnRigidBodyAgent(dynamicsWorld, simplexObj, helper.bulletTransform(pos, glm::conjugate(rotation)), scale, collision_shape, setName, uid);
			if(agent != nullptr) _fluentSimplexAgents[uid] = (agent);
		}
		if (type == 1) {
			agent = helper.spawnStaticBodyAgent(dynamicsWorld, simplexObj, helper.bulletTransform(pos, glm::conjugate(rotation)), scale, collision_shape, setName, uid);
			if (agent != nullptr) _fluentSimplexAgents[uid] = (agent);
		}
		if (type == 2) {

		}

		if (agent != nullptr) {
			auto velocityRef = compute::getAttribute<float>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@velocity", DfxFluenType::VEC3);

			if (velocityRef.exists && velocityRef.sameAsRequestedType && velocityRef._data != nullptr) {
				auto vel_arr = compute::extractAttrRef<glm::vec3>(velocityRef);
				btVector3 vel(vel_arr[index].x, vel_arr[index].y, vel_arr[index].z);
				agent->fluentBody->rigidBody->setLinearVelocity(vel);
			}
		}
	}

	return agent;
}

DfxBulletAgent *DfxBulletStageNode::addFluentStream(size_t index, size_t type, size_t collision_shape) {

	DfxBulletFluentHelper helper;
	DfxBulletAgent *agent = nullptr;

	auto simplexObj = DfxFluentSimplexCollection::getInstance()->simplexObjects()[index];
	auto xform = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<glm::mat4>("@xform")[index];
	//auto uid = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<size_t>("@uid")[index];
	auto uid = index;
	auto setName = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<std::string>("@set_name")[index];

	auto fAgentItr = _fluentSimplexAgents.find(uid);

	if (fAgentItr == _fluentSimplexAgents.end()) {

		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 pos;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(xform, scale, rotation, pos, skew, perspective);

		if (type == 0) {
			agent = helper.spawnRigidBodyAgent(dynamicsWorld, simplexObj, helper.bulletTransform(pos, glm::conjugate(rotation)), scale, collision_shape, setName, uid);
			if (agent != nullptr) _fluentSimplexAgents[uid] = (agent);
		}
		if (type == 1) {
			auto agent = helper.spawnStaticBodyAgent(dynamicsWorld, simplexObj, helper.bulletTransform(pos, glm::conjugate(rotation)), scale, collision_shape, setName, uid);
			if (agent != nullptr) _fluentSimplexAgents[uid] = (agent);
		}
		if (type == 2) {

		}
	}

	return agent;
}

std::vector<DfxBulletAgent*> DfxBulletStageNode::addFluentStream(DfxFluentSet *set, std::vector<size_t> indices, size_t type, size_t collisionShape, std::vector<deepsg::SgNode*> shapeNodes) {
	std::vector<DfxBulletAgent*> output;
	
	if (DfxFluentStreamCollection::getInstance()->size() > 0 && !shapeNodes.empty()) {

		deepsg::SgGeoNode *shapeNode = nullptr;
		if (shapeNodes[0] == nullptr) return output;
		else {
			if (shapeNodes[0]->getType() == deepsg::SgNodeType::GEOMETRY) {
				shapeNode = (deepsg::SgGeoNode*)shapeNodes[0];
			}
			else {
				return output;
			}
		}

		glm::vec3 *positions = DfxFluentStreamCollection::getInstance()->atrributeMap().at<glm::vec3>("@position");

		for (int inx = 0; inx < indices.size(); inx++) {

			glm::vec3 pos;
			glm::quat quat;

			auto xform = glm::translate(glm::mat4(1.f), positions[indices[inx]]);

			bool success = false;
			auto index = util::objectToFluentSet(
				((deepsg::SgGeoNode*)shapeNode)->getGeometryAdapter(),
				set,
				set->name(),
				shapeNode->referencePath(),
				xform,
				success,
				false);

			auto agent = addFluentStream(index, type, collisionShape);
			output.push_back(agent);
		}
	}

	return output;
}

void DfxBulletStageNode::cacheFluentAgents() {
	if (DfxFluentSimplexCollection::getInstance()->size() > 0) {
		auto uidAttrArray = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<size_t>("@uid");
		auto xformAttrArray = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<glm::mat4>("@xform");
		auto xvelocity = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<glm::vec3>("@velocity");

		for (auto p : _fluentSimplexAgents) {
			auto agent = p.second;

			if (agent->fluent) {

				if (agent->rigidBody && !agent->isStatic) {
					
					btTransform trans;
					agent->fluentBody->rigidBody->getMotionState()->getWorldTransform(trans);

					glm::mat4 bulletMatrix = glm::mat4(1.0f);

					trans.getOpenGLMatrix(glm::value_ptr(bulletMatrix));

					bulletMatrix *= glm::scale(agent->scale);

					if (xvelocity != nullptr) {
						auto btVel = agent->fluentBody->rigidBody->getLinearVelocity();
						xvelocity[agent->fluentUID] = glm::vec3(btVel.x(), btVel.y(), btVel.z());
					}

					xformAttrArray[agent->fluentUID] = bulletMatrix;
						
				}
			}
		}

		for (auto p : _fluentStreamAgents) {
			auto agent = p.second;

			if (agent->fluent) {

				if (agent->rigidBody && !agent->isStatic) {

					btTransform trans;
					agent->fluentBody->rigidBody->getMotionState()->getWorldTransform(trans);

					glm::mat4 bulletMatrix = glm::mat4(1.0f);

					trans.getOpenGLMatrix(glm::value_ptr(bulletMatrix));

					bulletMatrix *= glm::scale(agent->scale);

					if (xvelocity != nullptr) {
						auto btVel = agent->fluentBody->rigidBody->getLinearVelocity();
						xvelocity[agent->fluentUID] = glm::vec3(btVel.x(), btVel.y(), btVel.z());
					}

					xformAttrArray[agent->fluentUID] = bulletMatrix;

				}
			}
		}
	}
}

void DfxBulletStageNode::removeFluentAgent(std::unordered_map<size_t, DfxBulletAgent*> &map, size_t index) {
	auto flAgent = map.find(index);

	if (flAgent != map.end()) {
		if (flAgent->second->rigidBody) {

			btRigidBody *body = btRigidBody::upcast(flAgent->second->fluentBody->rigidBody);

			if (body &&  body->getMotionState()) {

				btCollisionShape *shp = body->getCollisionShape();
				if (shp) delete shp;
				delete body->getMotionState();
			}

			dynamicsWorld->removeCollisionObject(flAgent->second->fluentBody->rigidBody);
			delete flAgent->second->fluentBody->rigidBody;

			delete flAgent->second;
			map.erase(flAgent);
			/*if (flAgent->second->fluentBody->shape != nullptr) {
				delete flAgent->second->fluentBody->shape;
				flAgent->second->fluentBody->shape = nullptr;
				delete flAgent->second->btTrimesh;
			}

			if (flAgent->second->fluentBody->rigidBody->getMotionState() != nullptr) {
				delete flAgent->second->fluentBody->rigidBody->getMotionState();
			}

			if (dynamicsWorld != nullptr) {
				if (flAgent->second->fluentBody->rigidBody->isInWorld()) {
					dynamicsWorld->removeRigidBody(flAgent->second->fluentBody->rigidBody);
					delete flAgent->second->fluentBody->rigidBody;
					flAgent->second->fluentBody->rigidBody = nullptr;
				}
			}

			delete flAgent->second;
			map.erase(flAgent);*/
		}
	}
}

void DfxBulletStageNode::removeFluentSimplex(size_t index) {
	removeFluentAgent(_fluentSimplexAgents, index);
}

void DfxBulletStageNode::removeFluentStream(size_t index) {
	removeFluentAgent(_fluentStreamAgents, index);
}

bool DfxBulletStageNode::hasFluentSimplexObject(size_t uid) {
	for (auto flobj : _fluentSimplexAgents) {
		if (flobj.second->fluentUID == uid) return true;
	}

	return false;
}

bool DfxBulletStageNode::hasFluentStreamObject(size_t uid) {
	for (auto flobj : _fluentSimplexAgents) {
		if (flobj.second->fluentUID == uid) return true;
	}

	return false;
}

void DfxBulletStageNode::restoreActor(deepsg::SgStageActorNode *node) {
	if (node->uniqueName() == DfxBulletActorNode::getUniqueName()) {
		DfxBulletActorHandler handler;

		if (handler.fetch(node->referencePath())) {
			auto actor_node = handler.actorNode();

			actor_node->setStage(this);
			actor_node->setActorState(DfxBulletActorNode::ActorState::SPAWN);
			actor_node->setDynamicsWorld(dynamicsWorld);
			actor_node->setSoftBodyWorldInfo(softBodyWorldInfo);

			_flDispatcher->attachBranch(handler.dataObject());

			auto any = DfxSpace::getInstance()->findAbsolute(actor_node->sourcePath());

			if (any != nullptr) {

				if (any->type() == fl::type::DATA_OBJECT) {
					auto dao = any->rawData<fl::DataObject>();

					if (dao->hasProperty("sg_geo_node")) {
						auto geoNode = dao->property("sg_geo_node")->rawData<deepsg::SgGeoNode>();	
						_flDispatcher->attachBranch(geoNode->xFormProperties());
					}
					else if (dao->hasProperty("sg_point_cloud_node")) {
						auto pcNode = dao->property("sg_point_cloud_node")->rawData<deepsg::SgPointCloudNode>();
						_flDispatcher->attachBranch(pcNode->xFormProperties());
					}
				}		
			}
		}
	}
}

void DfxBulletStageNode::restoreAll() {
	for (auto actor: actorCollectionNode()->children()) {
		if (actor->getType() == deepsg::SgNodeType::STAGE_ACTOR) {
			restoreActor((deepsg::SgStageActorNode*)actor);
		}
	}

	for (auto prop : propCollectionNode()->children()) {
		if (prop->getType() == deepsg::SgNodeType::STAGE_PROP) {
			restoreProp((deepsg::SgStagePropNode*)prop);
		}
	}
}

size_t DfxBulletStageNode::addProp(fl::Any *any) { 
	
	if (any->type() == DfxFluentTypes::SG_STAGE_PROP_NODE) {

		auto prop_node = any->rawData<deepsg::SgStagePropNode>();
		
		if (prop_node->propType() == bullet::CONSTRAINT_PROP) {
			auto bulletProp = (DfxBulletConstraintPropNode*)prop_node;
			bulletProp->setStage(this);
			bulletProp->setDynamicsWorld(dynamicsWorld);
			bulletProp->setPropState(DfxBulletConstraintPropNode::PropState::SPAWN);
			DfxQSceneManager::getInstance()->moveNode(prop_node, propCollectionNode());
		}
		else {
			DfxGlobal::console::println("Data not compatible with Bullet stage.");
		}

	}
	else {
		DfxGlobal::console::println("Bullet doesn't support prop.");
	}

	return 0; 
};

void DfxBulletStageNode::removeProp(size_t uid) {  
	auto node = propCollectionNode()->findChild(uid);

	if (node != nullptr) {
		if (node->uniqueName() == DfxBulletConstraintPropNode::getUniqueName()) {
			auto prop = (DfxBulletConstraintPropNode*)node;
			prop->clear();

			DfxQSceneManager::getInstance()->removeNode(node);

			dirtySimulation();
			DfxSimulationManager::getInstance()->resetSimulationState();
		}
	}
};

void DfxBulletStageNode::restoreProp(deepsg::SgStagePropNode *node) {
	if (node->uniqueName() == DfxBulletConstraintPropNode::getUniqueName()) {
		DfxBulletConstraintPropHandler handler;

		if (handler.fetch(node->referencePath())) {
			auto prop_node = handler.propNode();

			prop_node->setStage(this);
			prop_node->setPropState(DfxBulletConstraintPropNode::PropState::SPAWN);
			prop_node->setDynamicsWorld(dynamicsWorld);

			_flDispatcher->attachBranch(handler.dataObject());

		}
	}
}

bool DfxBulletStageNode::isSimulationEnabled() {
	return true;
}

bool DfxBulletStageNode::doStep(DfxStageEvaluationInfo info) {
	if (info.currentTime.frame == DfxTimelineManager::getInstance()->firstFrame()) return false;
	else return true;
}

void DfxBulletStageNode::reset() {

	for (auto actor : actors()) {
		DfxBulletActorHandler handler;
		auto bulletActor = (DfxBulletActorNode*)actor;

		if (handler.fetch(bulletActor->referencePath())) {
			bulletActor->clearBody();
			bulletActor->clearCache();
			bulletActor->reset();
			bulletActor->clearAgents();
			bulletActor->setActorState(DfxBulletActorNode::ActorState::SPAWN);
		}
	}

	for (auto prop : props()) {
		if (prop->propType() == bullet::CONSTRAINT_PROP) {
			auto bulletProp = (DfxBulletConstraintPropNode*)prop;
			bulletProp->clear();
		}
	}

	clearFluent();

	clearBulletDynamicsWorld();
	_simIsDirty = false;
};

bool DfxBulletStageNode::needsReset() { 
	return _simIsDirty;  
};

// void DfxBulletStageNode::makeSimulationDirty() {};

void DfxBulletStageNode::beginStep(DfxStageEvaluationInfo info) {

	DfxEventManager::getInstance()->announceEvent(event::BT_STARTING_SIMULATION_STEP);

	for (auto actor : actors()) {
		DfxBulletActorHandler handler;
		auto bulletActor = (DfxBulletActorNode*)actor;
		
		if (handler.fetch(bulletActor->referencePath())) {
			
			if (bulletActor->actorState() == DfxBulletActorNode::ActorState::SPAWN) {
				bulletActor->spawn();
			}
		}
	}

	for (auto prop : props()) {
		auto bulletProp = (DfxBulletConstraintPropNode*)prop;

		if (bulletProp->propState() == DfxBulletConstraintPropNode::PropState::SPAWN) {
			bulletProp->spawn();
		}
	}

	if (!_fluentSimplexAgents.empty()) {
		if (DfxFluentSimplexCollection::getInstance()->size() > 0) {
			if (!DfxFluentSimplexCollection::getInstance()->atrributeMap().hasAttribute("@velocity")) {
				DfxFluentSimplexCollection::getInstance()->atrributeMap().addAttribute<glm::vec3>("@velocity");
			}
		}
	}

	if (!_fluentStreamAgents.empty()) {
		if (DfxFluentSimplexCollection::getInstance()->size() > 0) {
			if (!DfxFluentSimplexCollection::getInstance()->atrributeMap().hasAttribute("@velocity")) {
				DfxFluentSimplexCollection::getInstance()->atrributeMap().addAttribute<glm::vec3>("@velocity");
			}
		}
	}

	updateFluentRigidBodyProperties();
};

void DfxBulletStageNode::step(DfxStageEvaluationInfo info) { 

	DfxBulletStageHandler handler;
	float dt = 0.0166;
	int substeps = 1;
	btVector3 gravity(0.f, -9.f, 0.0f);

	if (handler.fetch(referencePath())) {
		auto dao = handler.dataObject()->rawData<fl::DataObject>();

		dt = dao->property("timestep")->data<float>();
		substeps = dao->property("substeps")->data<int>();
		float gx = dao->property("gravity_x")->data<float>();
		float gy = dao->property("gravity_y")->data<float>();
		float gz = dao->property("gravity_z")->data<float>();
		gravity.setValue(gx, gy, gz);
	}

	dynamicsWorld->setGravity(gravity);

	for (int i = 0; i < substeps; i++) {
		for (auto actor : actors()) ((DfxBulletActorNode*)actor)->preStep();
		
		for (auto prop : props()) {
			if (prop->propType() == bullet::CONSTRAINT_PROP) ((DfxBulletConstraintPropNode*)prop)->preStep();
		}

		dynamicsWorld->stepSimulation(dt, 1, dt);

		for (auto actor : actors()) ((DfxBulletActorNode*)actor)->postStep();

		for (auto prop : props()) {
			if (prop->propType() == bullet::CONSTRAINT_PROP) ((DfxBulletConstraintPropNode*)prop)->postStep();
		}
	}

	for (auto actor : actors()) {
		DfxBulletActorHandler handler;
		auto bulletActor = (DfxBulletActorNode*)actor;

		if (handler.fetch(bulletActor->referencePath())) {			
			bulletActor->cacheLastStepAll(info.currentTime);
		}
	}

	cacheFluentAgents();
};

void DfxBulletStageNode::endStep(DfxStageEvaluationInfo info) {
	DfxEventManager::getInstance()->announceEvent(event::BT_SIMULATION_STEP_COMPLETE);
}

std::vector<DfxTime> DfxBulletStageNode::subFrames() { return std::vector<DfxTime>(); };

/// Bullet 
void DfxBulletStageNode::initBullet() {

#ifdef USE_BULLET_MT_FEATURES
	softBodyWorldInfo = new btSoftBodyWorldInfo;

	collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();

	dispatcher = new btCollisionDispatcherMt(collisionConfiguration);
	btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);

	broadphase = new btDbvtBroadphase();

	auto solverPool = new btConstraintSolverPoolMt(16);
	dynamicsWorld = new btDiscreteDynamicsWorldMt(dispatcher, broadphase, solverPool, rigid_body_solver, collisionConfiguration);

	btSetTaskScheduler(btCreateDefaultTaskScheduler());

	softBodyWorldInfo->m_broadphase = broadphase;
	softBodyWorldInfo->m_dispatcher = dispatcher;
	softBodyWorldInfo->m_sparsesdf.Initialize();
#else
	softBodyWorldInfo = new btSoftBodyWorldInfo;

	collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();

	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);

	broadphase = new btDbvtBroadphase();

	rigid_body_solver = new btSequentialImpulseConstraintSolver;
	softbodySolver = new btDefaultSoftBodySolver();

	dynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher, broadphase, rigid_body_solver, collisionConfiguration, softbodySolver);

	softBodyWorldInfo->m_broadphase = broadphase;
	softBodyWorldInfo->m_dispatcher = dispatcher;
	softBodyWorldInfo->m_gravity.setValue(0, -10, 0);
	softBodyWorldInfo->m_sparsesdf.Initialize();
#endif // USE_BULLET_MT_FEATURES

}

void DfxBulletStageNode::initBulletDynamicsWorld() {

	dynamicsWorld->setGravity(btVector3(0.f, -9.8f, 0.f));

}

void DfxBulletStageNode::clearBulletDynamicsWorld() {

	for (int i = dynamicsWorld->getNumConstraints() - 1; i >= 0; i--)
	{
		btTypedConstraint* constraint = dynamicsWorld->getConstraint(i);
		dynamicsWorld->removeConstraint(constraint);
		delete constraint;
	}

	for (int i = 0; i < dynamicsWorld->getNumCollisionObjects(); i++) {
		btCollisionObject *obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody *body = btRigidBody::upcast(obj);

		if (body &&  body->getMotionState()) {

			btCollisionShape *shp = body->getCollisionShape();
			if (shp) delete shp;
			delete body->getMotionState();
		}

		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}
}

void DfxBulletStageNode::clearFluent() {

	for (auto flAgent : _fluentSimplexAgents) {

		if (flAgent.second->rigidBody) {
			if (flAgent.second->fluentBody->shape != nullptr) {
				delete flAgent.second->fluentBody->shape;
				flAgent.second->fluentBody->shape = nullptr;
				delete flAgent.second->btTrimesh;
			}

			if (flAgent.second->fluentBody->rigidBody->getMotionState() != nullptr) {
				delete flAgent.second->fluentBody->rigidBody->getMotionState();
			}

			if (dynamicsWorld != nullptr) {
				if (flAgent.second->fluentBody->rigidBody->isInWorld()) {
					dynamicsWorld->removeRigidBody(flAgent.second->fluentBody->rigidBody);
					delete flAgent.second->fluentBody->rigidBody;
					flAgent.second->fluentBody->rigidBody = nullptr;
				}
			}

			delete flAgent.second;
		}

	}

	for (auto flAgent : _fluentStreamAgents) {

		if (flAgent.second->rigidBody) {
			if (flAgent.second->fluentBody->shape != nullptr) {
				delete flAgent.second->fluentBody->shape;
				flAgent.second->fluentBody->shape = nullptr;
				delete flAgent.second->btTrimesh;
			}

			if (flAgent.second->fluentBody->rigidBody->getMotionState() != nullptr) {
				delete flAgent.second->fluentBody->rigidBody->getMotionState();
			}

			if (dynamicsWorld != nullptr) {
				if (flAgent.second->fluentBody->rigidBody->isInWorld()) {
					dynamicsWorld->removeRigidBody(flAgent.second->fluentBody->rigidBody);
					delete flAgent.second->fluentBody->rigidBody;
					flAgent.second->fluentBody->rigidBody = nullptr;
				}
			}

			delete flAgent.second;
		}

	}

	_fluentSimplexAgents.clear();
	_fluentStreamAgents.clear();
}

void DfxBulletStageNode::clearBullet() {
	clearBulletDynamicsWorld();

	delete softBodyWorldInfo;
	delete dynamicsWorld;
	delete rigid_body_solver;
	delete broadphase;
	delete dispatcher;
	delete collisionConfiguration;
}

void DfxBulletStageNode::dirtySimulation() {
	_simIsDirty = true;
}

std::vector<std::pair<QWidget*, QString>> DfxBulletStageNode::propertyForms() {
	std::pair<QWidget*, QString> properties(propertiesForm(), "Stage Properties");

	return std::vector<std::pair<QWidget*, QString>> { properties };
}

QWidget *DfxBulletStageNode::propertiesForm() {
	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto propertiesControlPanel = new DfxControlPanel();

	DfxQControlArray *propertiesControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	auto timestep	= new DfxQDecimalSpinnerControl(path + "timestep", "", false);
	auto substeps   = new DfxQIntegerSpinnerControl(path + "substeps", "", false);
	auto gx			= new DfxQDecimalSpinnerControl(path + "gravity_x", "", false);
	auto gy			= new DfxQDecimalSpinnerControl(path + "gravity_y", "", false);
	auto gz			= new DfxQDecimalSpinnerControl(path + "gravity_z", "", false);

	// Source
	propertiesControlArray->addControl("Timestep", timestep);
	propertiesControlArray->addSpacer(0, 2);
	propertiesControlArray->addControl("Substeps", substeps);
	propertiesControlArray->addSpacer(0, 2);
	propertiesControlArray->addControl("Gravity X", gx);
	propertiesControlArray->addControl("Y", gy);
	propertiesControlArray->addControl("Z", gz);

	propertiesControlPanel->setTitle("Global Settings");
	propertiesControlPanel->setWidget(propertiesControlArray);

	main_layout->addWidget(propertiesControlPanel);

	widget->setLayout(main_layout);

	return widget;
}