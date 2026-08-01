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

#include "DfxPointCloudHandler.h"

std::string DfxPointCloudHandler::model_id = "/POINT_CLOUD/POINT_CLOUD_INFO";
size_t DfxPointCloudHandler::ID_GEN = 1;

DfxPointCloudHandler::DfxPointCloudHandler() {

}

DfxPointCloudHandler::~DfxPointCloudHandler() {

}

fl::Any *DfxPointCloudHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return  fl::DataObject::Builder()
		.setName("point_cloud")
		.setLabel("Point Cloud" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)

		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "cache_id", "Cache Identifier", (size_t)0)
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "attribute_id", "Attribute Identifier", (size_t)0)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "parenting", "Use Parent Transformation", true)
		.build(space);
}

bool DfxPointCloudHandler::create() {
	// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Objects Branch
	auto object_branch = space->findAbsolute("~/PTCS");

	/// Proceed only if it exists
	if (object_branch != nullptr) {

		auto object = createModel(space);

		/// Object XForm 
		auto xform = newXForm();

		/// Arrange properties into the right order
		space->move(object, object_branch);
		space->move(xform, object);

		/// Store reference for further use
		_dataObject = object;

		/// Return success
		return true;
	}
	else {
		DfxGlobal::console::println("Point Cloud Handler: Create failed to find branch.");
	}

	return false;
}

deepsg::SgPointCloudNode *DfxPointCloudHandler::pointCloudNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_point_cloud_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgPointCloudNode>();
		}
	}

	return nullptr;
}

deepsg::SgPointCloudNode *DfxPointCloudHandler::createPointCloudNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_point_cloud_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgPointCloudNode>();
		}
		else {

			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Store Scene graph node
			auto scenegraph_node = space->any("sg_point_cloud_node", "Scene Graph Point Cloud Node");
			space->move(scenegraph_node, _dataObject);

			return (deepsg::SgPointCloudNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

DfxAttribute *DfxPointCloudHandler::createAttributes() {
	return nullptr;
}

bool DfxPointCloudHandler::destroy() {

	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {

		auto mdata00 = dsPointCloudData();
		auto ndata00 = pointCloudNode();
		auto pcloudAny = _dataObject->data<fl::DataObject>().property("sg_point_cloud_node");

		if (mdata00 != nullptr) {
			mdata00->clear();
		}
		else {
			DfxGlobal::console::println("Point Cloud Handler: Destroy failed to clear mesh.");
		}

		if (ndata00 != nullptr) {
			DfxQSceneManager::getInstance()->removeNode(ndata00);
			delete ndata00;
			pcloudAny->nullify();
		}

		space->remove(_dataObject);
		_dataObject = nullptr;

		return true;
	}

	return false;
}

ds::PointCloud *DfxPointCloudHandler::dsPointCloudData() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_point_cloud_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgPointCloudNode>()->geo();
		}
	}

	return nullptr;
}


deepsg::SgNode* DfxPointCloudHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {
	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		/// Master Space
		auto space = DfxSpace::getInstance();

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_point_cloud_node");
		auto xform = any->data<fl::DataObject>().property("xform");

		deepsg::SgPointCloudNode *gnode = new DfxPointCloudNode;
		gnode->setReferencePath(referencePath);
		gnode->setXFormProperties(xform);

		/// Store Scene graph node
		scenegraph_node->newData<deepsg::SgPointCloudNode>(DfxFluentTypes::SG_POINT_CLOUD_NODE, gnode);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxPointCloudHandler::deleteNode(deepsg::SgNode *node) {
	DfxPointCloudHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}