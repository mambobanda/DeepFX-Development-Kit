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

#include "DfxRiptideEmitterPropHandler.h"

std::string DfxRiptideEmitterPropHandler::model_id = "/RIPTIDE/EMITTER_PROP";
size_t DfxRiptideEmitterPropHandler::ID_GEN = 1;

DfxRiptideEmitterPropHandler::DfxRiptideEmitterPropHandler() {
	_dataObject = nullptr;
}

DfxRiptideEmitterPropHandler::~DfxRiptideEmitterPropHandler() {

}

fl::Any *DfxRiptideEmitterPropHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return fl::DataObject::Builder()
		.setName("riptide_emitter_prop")
		.setLabel("Riptide Emitter" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "fluent_set", "Fluent Set", std::string("Riptide Prop"))
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "type", "Emitter Type", 0)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "particle_limit", "Particle Limit", (unsigned int)5000)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "strength", "strength", 10.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "radius", "radius", 0.25f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "height", "height", 0.25f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "width", "width", 0.25f)
		.addProperty(space, fl::type::INT, fl::role::NONE, "group_id", "Group ID", -1)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "u_displacement", "U Displacement", 0.0001f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "v_displacement", "V Displacement", 0.0001f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "color_r", "Color Red", .0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "color_g", "Color Green", .0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "color_b", "Color Blue", 1.0f)

		.addProperty(space, fl::type::BOOL, fl::role::NONE, "use_viscosity", "Use Viscosity", true)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE,"viscosity", "Viscosity", .01f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "use_vorticity", "Use Vorticity", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "vorticity", "Vorticity", .05f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "vorticity_viscosity", "Vorticity Angular Viscosity", .1f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "vorticity_inertia", "Vorticity Inverse Inertia", .5f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "use_surface_tension", "Use Surface Tension", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "surface_tension", "Surface Tension", .05f)

		.build(space);
}

bool DfxRiptideEmitterPropHandler::create() {
	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto temp_branch = space->findAbsolute("~/STAGE_PROPS");

	/// Proceed only if it exists
	if (temp_branch != nullptr) {

		/// Object Reference
		auto object = createModel(space);

		/// Object XForm 
		auto xform = newXForm();

		/// Arrange properties into the right order
		space->move(object, temp_branch);
		space->move(xform, object);

		/// Store reference for further use
		_dataObject = object;

		/// Return success
		return true;
	}

	/// Return failure
	return false;
}

DfxRiptideEmitterPropNode *DfxRiptideEmitterPropHandler::propNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");

	if (any != nullptr) {
		return (DfxRiptideEmitterPropNode*)any->rawData<DfxSimulationStagePropBase>();
	}
	else {
		return nullptr;
	}
}

bool DfxRiptideEmitterPropHandler::destroy() {

	if (_dataObject != nullptr) {
		auto space = DfxSpace::getInstance();

		auto ndata00 = propNode();

		if (ndata00 != nullptr) {

			DfxQSceneManager::getInstance()->removeNode(ndata00);
			delete ndata00;

			auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");
			any->nullify();

		}
		else {
			DfxGlobal::console::println("Riptide Emitter Handler: Destroy failed to clear node.");
		}

		space->remove(_dataObject);

		return true;
	}

	return false;
}

DfxRiptideEmitterPropNode *DfxRiptideEmitterPropHandler::createEmitterNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");

		if (any != nullptr) {
			return (DfxRiptideEmitterPropNode*)any->rawData<DfxSimulationStagePropBase>();
		}
		else {
			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Create Data Slot
			auto scenegraph_node = space->any("sg_stage_prop_node", "Scene Graph Riptide Emitter Prop Node");
			space->move(scenegraph_node, _dataObject);

			return (DfxRiptideEmitterPropNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

deepsg::SgNode* DfxRiptideEmitterPropHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_stage_prop_node");
		auto xform = any->data<fl::DataObject>().property("xform");

		DfxSimulationStagePropBase *gnode = new DfxRiptideEmitterPropNode(nullptr);
		gnode->setReferencePath(referencePath);
		gnode->setXFormProperties(xform);

		scenegraph_node->newData<DfxSimulationStagePropBase>(DfxFluentTypes::SG_STAGE_PROP_NODE, gnode);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxRiptideEmitterPropHandler::deleteNode(deepsg::SgNode *node) {

	DfxRiptideEmitterPropHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}