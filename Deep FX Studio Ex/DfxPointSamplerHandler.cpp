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

#include "DfxPointSamplerHandler.h"

std::string DfxPointSamplerHandler::model_id = "/POINT_CLOUD/POINTS_SAMPLER_INFO";
size_t DfxPointSamplerHandler::ID_GEN = 1;

DfxPointSamplerHandler::DfxPointSamplerHandler() {

}

DfxPointSamplerHandler::~DfxPointSamplerHandler() {

}

fl::Any *DfxPointSamplerHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return fl::DataObject::Builder()
		.setName("points_sampler")
		.setLabel("Points Sampler" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)

		/// Global Properties
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "type", "Type", (size_t)DfxPointSamplerTool::Type::SURFACE)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "auto_sample_mesh", "Auto Sample Mesh", false)

		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "surface_sampler", "Surface Sampler", (size_t)DfxPointSamplerTool::SurfaceSamplingType::TRIANGLE)
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "volume_sampler", "Volume Sampler", (size_t)DfxPointSamplerTool::VolumeSamplingType::VOXEL)

		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "object", "Object", std::string(""))

		// Voxel Sampling
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "voxel_radius", "Voxel Radius / Resolution", 0.2f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "voxel_use_resolution", "Voxel Use Resolution", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "voxel_resolution", "Voxel Use Resolution", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "voxel_apply_transform", "Voxel Apply Transform", false)

		// Uniform Random
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "uniform_random_seed", "Uniform Random Seed", 57786)
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "uniform_random_count", "Uniform Random Count", 50)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "uniform_random_apply_transform", "Uniform Random Apply Transform", false)

		// Sphere
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "sphere_random_seed", "Sphere Random Seed", 57786)
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "sphere_count", "Sphere Count", 50)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "sphere_radius", "Sphere Radius", 1.f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "sphere_apply_transform", "Sphere Apply Transform", false)

		// Regular Sampling
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "regular_radius", "regular Radius / Resolution", 0.2f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "regular_use_resolution", "Regular Use Resolution", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "regular_packed_resolution", "Regular Packed Resolution", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "regular_apply_transform", "Regular Apply Transform", false)

		// Poisson Sampling
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "poisson_radius", "Poisson Radius / Resolution", 0.2f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "poisson_use_resolution", "Poisson Use Resolution", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "poisson_packed_resolution", "Poisson Packed Resolution", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "poisson_apply_transform", "Poisson Apply Transform", false)

		// Traingle Sampling
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "triangle_radius", "Triangle Radius / Resolution", 0.2f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "triangle_use_resolution", "Triangle Use Resolution", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "triangle_apply_transform", "Triangle Apply Transform", false)

		.build(space);
}

bool DfxPointSamplerHandler::create() {
	// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Objects Branch
	auto object_branch = space->findAbsolute("~/TOOLS");

	/// Proceed only if it exists
	if (object_branch != nullptr) {

		auto object = createModel(space);

		/// Arrange properties into the right order
		space->move(object, object_branch);

		/// Store reference for further use
		_dataObject = object;

		/// Return success
		return true;
	}
	else {
		DfxGlobal::console::println("Point Sampler Handler: Create failed to find branch.");
	}

	return false;
}

deepsg::SgToolNode *DfxPointSamplerHandler::createToolNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_tool_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgToolNode>();
		}
		else {

			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Store Scene graph node
			auto scenegraph_node = space->any("sg_tool_node", "Scene Graph Tool Node");
			space->move(scenegraph_node, _dataObject);

			return (deepsg::SgToolNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

bool DfxPointSamplerHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {

		auto nodeAny = _dataObject->data<fl::DataObject>().property("sg_tool_node");
		auto node = toolNode();

		if (node != nullptr) {
			DfxQSceneManager::getInstance()->removeNode(node);
			delete node;
			nodeAny->nullify();
		}

		space->remove(_dataObject);
		_dataObject = nullptr;

		return true;
	}

	return false;
}

deepsg::SgToolNode *DfxPointSamplerHandler::toolNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_tool_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgToolNode>();
		}
	}

	return nullptr;
}

deepsg::SgNode* DfxPointSamplerHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {
	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		/// Master Space
		auto space = DfxSpace::getInstance();

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_tool_node");

		deepsg::SgToolNode *gnode = new DfxPointSamplerTool;

		/// Store Scene graph node
		scenegraph_node->newData<deepsg::SgToolNode>(DfxFluentTypes::SG_TOOL_NODE, gnode);

		/// Finish setting up scene graph node
		gnode->setReferencePath(referencePath);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxPointSamplerHandler::deleteNode(deepsg::SgNode *node) {
	DfxPointSamplerHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}