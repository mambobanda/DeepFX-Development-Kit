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


#include "DfxBulletActorHandler.h"
#include "DfxBulletActorNode.h"

std::string DfxBulletActorHandler::model_id = "/SIMULATION/BULLET_ACTOR_NODE_INFO";
size_t DfxBulletActorHandler::ID_GEN = 1;

DfxBulletActorHandler::DfxBulletActorHandler() {
	_dataObject = nullptr;
}

DfxBulletActorHandler::~DfxBulletActorHandler() {

}

fl::Any *DfxBulletActorHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return  fl::DataObject::Builder()
		.setName("bullet_actor")
		.setLabel("Bullet Actor" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "fluent_set", "Fluent Set", std::string("Bullet Actor"))
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "source_path", "Source Path", std::string(""))
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "source_type", "Source Type", 0)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "shape_type", "Shape Type", 0)

		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "bt_object_type", "Bullet Object Type", 0)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "bt_collision_shape_type", "Bullet Collision Shape", 0)

		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "target", "Target", 0)

		// Source Shape Reference Path
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "shape_reference_path", "Shape Reference Path", std::string(""))
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "target_reference_path", "Target Reference Path", std::string(""))

		// Rigid Body Properties
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "rigid_mass", "Rigid Body Mass", 10.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "rigid_friction", "Rigid Body Friction", 0.5f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "rigid_rolling_friction", "Rigid Body Rolling Friction", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "rigid_spinning_friction", "Rigid Body Spinning Friction", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "rigid_bounciness", "Rigid Body Bounciness", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "rigid_linear_dampening", "Rigid Body Linear Dampening", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "rigid_angular_dampening", "Rigid Body Angular Dampening", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "rigid_linear_sleeping", "Rigid Body Linear Sleeping Threshold", 0.8f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "rigid_angular_sleeping", "Rigid Body Angular Sleeping Threshold", 1.f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "rigid_initial_vel_x", "Rigid Body Initial Velocity X", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "rigid_initial_vel_y", "Rigid Body Initial Velocity Y", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "rigid_initial_vel_z", "Rigid Body Initial Velocity Z", 0.f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "rigid_initial_ang_vel_x", "Rigid Body Initial Angular Velocity X", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "rigid_initial_ang_vel_y", "Rigid Body Initial Angular Velocity Y", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "rigid_initial_ang_vel_z", "Rigid Body Initial Angular Velocity Z", 0.f)

		// Static Rigid Object
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "static_friction", "Static Body Friction", 0.5f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "static_bounciness", "Static Body Bounciness", 0.f)

		// Kinematic Rigid Object
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "kinematic_friction", "Kinematic Body Friction", 0.5f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "kinematic_bounciness", "Kinematic Body Bounciness", 0.f)

		// Soft Body Properties
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_mass", "Mass", 0.5f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_velocity_correction", "Velocity Correction", 2.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_volume_conservation", "Volume Conservation", 1.5f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_shape_matching", "Shape Matching", 0.5f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_damping", "Damping", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_drag", "Drag", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_lift", "Lift", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_pressure", "Pressure", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_friction", "Friction", 0.0f)
		.addProperty(space, fl::type::INT, fl::role::NONE, "soft_clusters", "Clusters", 1)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_collision_margin", "Collision Margin", 0.05f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_linear_stiffness", "Linear Stiffness", 1.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_angular_stiffness", "Angular Stiffness", 0.5f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_volume_stiffness", "Volume Stiffness", 0.0f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "soft_use_volume_pressure", "Use Volume Pressure", false)


		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_bounciness", "soft_bounciness", 0.0f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_initial_vel_x", "Soft Body Initial Velocity X", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_initial_vel_y", "Soft Body Initial Velocity Y", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "soft_initial_vel_z", "Soft Body Initial Velocity Z", 0.f)

		.build(space);
}

bool DfxBulletActorHandler::create() {
	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto temp_branch = space->findAbsolute("~/STAGE_ACTORS");

	/// Proceed only if it exists
	if (temp_branch != nullptr) {

		/// Object Reference
		auto object = createModel(space);

		/// Arrange properties into the right order
		space->move(object, temp_branch);

		/// Store reference for further use
		_dataObject = object;

		/// Return success
		return true;
	}

	/// Return failure
	return false;
}

DfxBulletActorNode *DfxBulletActorHandler::actorNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_stage_actor_node");

	if (any != nullptr) {
		return any->rawData<DfxBulletActorNode>();
	}
	else {
		return nullptr;
	}
}

DfxBulletActorNode *DfxBulletActorHandler::createActorNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_stage_actor_node");

		if (any != nullptr) {
			return any->rawData<DfxBulletActorNode>();
		}
		else {
			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Create Data Slot
			auto scenegraph_node = space->any("sg_stage_actor_node", "Scene Graph Bullet Stage Actor Node");
			space->move(scenegraph_node, _dataObject);

			return (DfxBulletActorNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;

}

bool DfxBulletActorHandler::destroy() {

	if (_dataObject != nullptr) {

		auto space = DfxSpace::getInstance();

		auto node = actorNode();
		if (node != nullptr) {
			((DfxSimulationStageBase*)node->stage())->removeActor(node->getID());
			delete node;

			auto any = _dataObject->data<fl::DataObject>().property("sg_stage_actor_node");
			any->nullify();

		}
		else {
			DfxGlobal::console::println("Bullet Actor Handler: Destroy failed to clear node.");
		}

		space->remove(_dataObject);

		return true;
	}

	return false;

}

deepsg::SgNode* DfxBulletActorHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_stage_actor_node");

		auto snode = new DfxBulletActorNode(nullptr);

		scenegraph_node->newData<DfxBulletActorNode>(DfxFluentTypes::SG_STAGE_ACTOR_NODE, snode);

		snode->setReferencePath(referencePath);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(snode, parent);
		}

		return snode;
	}

	return nullptr;
}

void DfxBulletActorHandler::deleteNode(deepsg::SgNode *node) {
	DfxBulletActorHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}