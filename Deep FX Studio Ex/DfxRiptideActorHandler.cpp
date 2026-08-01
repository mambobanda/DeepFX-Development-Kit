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

#include "DfxRiptideActorHandler.h"
#include "DfxRiptideActorNode.h"

std::string DfxRiptideActorHandler::model_id = "/RIPTIDE/ACTOR";
size_t DfxRiptideActorHandler::ID_GEN = 1;

DfxRiptideActorHandler::DfxRiptideActorHandler() {

}

DfxRiptideActorHandler::~DfxRiptideActorHandler() {

}

fl::Any *DfxRiptideActorHandler::createModel(fl::Space *space) {

	auto any = fl::DataObject::Builder()
		.setName("riptide_actor")
		.setLabel("Riptide Actor")
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "source_path", "Source Path", std::string(""))
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "samples", "Samples Path", std::string(""))
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "fluent_set", "Fluent Set", std::string("Riptide Actor"))
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "use_source_transform", "Use Source Transform", true)
		.addProperty(space, fl::type::INT, fl::role::NONE, "actor_type", "Actor Type", 0)

		.addProperty(space, fl::type::INT, fl::role::NONE, "fluid_group_id", "Group ID", -1)
		.addProperty(space, fl::type::BOOL,  fl::role::NONE, "fluid_use_viscosity", "Fluid Use Viscosity", true)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "fluid_viscosity", "Fluid Viscosity", .01f)
		.addProperty(space, fl::type::BOOL,  fl::role::NONE, "fluid_use_vorticity", "Fluid Use Vorticity", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "fluid_vorticity", "Fluid Vorticity", .05f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "fluid_vorticity_viscosity", "Fluid Vorticity Angular Viscosity", .1f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "fluid_vorticity_inertia", "Fluid Vorticity Inverse Inertia", .5f)
		.addProperty(space, fl::type::BOOL,  fl::role::NONE, "fluid_use_surface_tension", "Fluid Use Surface Tension", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "fluid_surface_tension", "Fluid Surface Tension", .05f)

		.addProperty(space, fl::type::BOOL, fl::role::NONE, "fluid_freeze", "Fluid Freeze", false)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "fluid_color_r", "Color Red", .0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "fluid_color_g", "Color Green", .0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "fluid_color_b", "Color Blue", 1.0f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "fluid_initial_velocity_x", "Fluid Initial Velocity X", 0.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "fluid_initial_velocity_y", "Fluid Initial Velocity Y", 0.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "fluid_initial_velocity_z", "Fluid Initial Velocity Z", 0.0f)

		.addProperty(space, fl::type::BOOL, fl::role::NONE,  "collision_kinematic", "Collision Object Is Kinematic", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE,  "collsiion_use_viscosity", "Collision Object Use Viscosity", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "collision_viscosity", "Collision Object Viscosity", .01f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE,  "collsiion_use_surface_tension", "Collision Object Use Surface Tension", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "collision_surface_tension", "Collision Object Surface Tension", .05f)	

		.build(space);

	return any;
}

bool DfxRiptideActorHandler::create() {

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto temp_branch = space->findAbsolute("~/STAGE_ACTORS");

	/// Proceed only if it exists
	if (temp_branch != nullptr) {

		size_t id_g = ID_GEN++;
		std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

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

DfxRiptideActorNode *DfxRiptideActorHandler::createActorNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_stage_actor_node");

		if (any != nullptr) {
			return any->rawData<DfxRiptideActorNode>();
		}
		else {
			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Create Data Slot
			auto node = space->any("sg_stage_actor_node", "Riptide Actor Node");
			space->move(node, _dataObject);

			return (DfxRiptideActorNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

bool DfxRiptideActorHandler::destroy() {

	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {
		auto ndata00 = actorNode();

		if (ndata00 != nullptr) {

			((DfxSimulationStageBase*)ndata00->stage())->removeActor(ndata00->getID());
			delete ndata00;

			auto any = _dataObject->data<fl::DataObject>().property("sg_stage_actor_node");
			any->nullify();

		}

		space->remove(_dataObject);

		return true;
	}

	return false;
}

DfxRiptideActorNode *DfxRiptideActorHandler::actorNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_stage_actor_node");

	if (any != nullptr) {
		return any->rawData<DfxRiptideActorNode>();
	}
	else {
		return nullptr;
	}
}

deepsg::SgNode* DfxRiptideActorHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto snode = any->data<fl::DataObject>().property("sg_stage_actor_node");

		DfxRiptideActorNode *gnode = new DfxRiptideActorNode(nullptr);
		gnode->setReferencePath(referencePath);

		snode->newData<DfxRiptideActorNode>(DfxFluentTypes::SG_STAGE_ACTOR_NODE, gnode);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxRiptideActorHandler::deleteNode(deepsg::SgNode *node) {

	DfxRiptideActorHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}