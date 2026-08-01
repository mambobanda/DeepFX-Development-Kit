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

#include "DfxPointCloudMesherHandler.h"

std::string DfxPointCloudMesherHandler::model_id = "/POINT_CLOUD/POINT_CLOUD_MESHER_INFO";
size_t DfxPointCloudMesherHandler::ID_GEN = 1;

DfxPointCloudMesherHandler::DfxPointCloudMesherHandler() {

}

DfxPointCloudMesherHandler::~DfxPointCloudMesherHandler() {

}

fl::Any *DfxPointCloudMesherHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	auto object = fl::DataObject::Builder()
		.setName("point_cloud_mesher")
		.setLabel("Point Cloud Mesher" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "point_cloud", "Point Cloud", std::string(""))
		.addProperty(space, fl::type::INT, fl::role::NONE, "group_id", "Group ID", -1)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "units", "Units", 0)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "resolution", "Resolution", 15)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "grid_scaling", "Grid Scaling", 0)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "grid_size", "Grid Size", 0.02f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "point_diameter", "Point Diameter", 0.04f)

		.addProperty(space, fl::type::BOOL, fl::role::NONE, "auto_generate_mesh", "Auto Generate Mesh", false)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "dilate_filter_offset", "Dilate Filter Offset", 0.f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "erode_filter_offset", "Erode Filter Offset", 0.f)

		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "smoothing_filter", "Smoothing Filter", 1)

		.addProperty(space, fl::type::INT, fl::role::NONE, "mean_filter_width", "Mean Filter Width", 2)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "mean_filter_iterations", "Mean Filter Iterations", 1)

		.addProperty(space, fl::type::INT, fl::role::NONE, "median_filter_width", "Median Filter Width", 2)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "median_filter_iterations", "Median Filter Iterations", 1)

		.addProperty(space, fl::type::INT, fl::role::NONE, "gaussian_filter_width", "Gaussian Filter Width", 2)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "gaussian_filter_iterations", "Gaussian Filter Iterations", 1)

		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "laplacian_filter_iterations", "Laplacian Filter Iterations", 1)

		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "mean_curvature_filter_iterations", "Mean Curvature Filter Iterations", 1)

		.build(space);

	return object;
}

bool DfxPointCloudMesherHandler::create() {

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto temp_branch = space->findAbsolute("~/OBJS");

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

	/// Return failier
	return false;
}

bool DfxPointCloudMesherHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {

		auto ndata00 = mesherNode();

		if (ndata00 != nullptr) {
			DfxQSceneManager::getInstance()->removeNode(ndata00);
			delete ndata00;

			auto any = _dataObject->data<fl::DataObject>().property("sg_tool_node");
			any->nullify();
		}

		space->remove(_dataObject);
		_dataObject = nullptr;

		return true;
	}

	return false;
}

DfxPointCloudMesherNode *DfxPointCloudMesherHandler::mesherNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_tool_node");

		if (any != nullptr) {
			return any->rawData<DfxPointCloudMesherNode>();
		}
		else {

			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Store Scene graph node
			auto scenegraph_node = space->any("sg_tool_node", "Scene Graph Tool Node");
			space->move(scenegraph_node, _dataObject);

			return (DfxPointCloudMesherNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

deepsg::SgNode* DfxPointCloudMesherHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_tool_node");

		auto gnode = new DfxPointCloudMesherNode;

		scenegraph_node->newData<DfxPointCloudMesherNode>(DfxFluentTypes::SG_GEO_NODE, gnode);

		/// Finish setting up scene graph node
		gnode->setReferencePath(referencePath);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxPointCloudMesherHandler::deleteNode(deepsg::SgNode *node) {
	DfxPointCloudMesherHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}