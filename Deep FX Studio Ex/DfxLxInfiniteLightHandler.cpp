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

#include "DfxLxInfiniteLightHandler.h"
#include "DfxItemWidgetType.h"

std::string DfxLxInfiniteLightHandler::model_id = "/LIGHT/LUX_INFINITE_LIGHT_NODE_INFO";
size_t DfxLxInfiniteLightHandler::ID_GEN = 1;

DfxLxInfiniteLightHandler::DfxLxInfiniteLightHandler() {
	_dataObject = nullptr;
}

DfxLxInfiniteLightHandler::~DfxLxInfiniteLightHandler() {

}

fl::Any *DfxLxInfiniteLightHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	auto object = fl::DataObject::Builder()
		.setName("lux_infinite_light")
		.setLabel("Lux Infinite Light" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "item_type", "Item Type", DfxItemWidgetType::INFINITE_LIGHT)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", DfxRenderTarget::LUXCORE)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gain", "Gain", 1.f)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "image_path", "Image Path", std::string(""))
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gamma", "Gamma", 1.f)
		.build(space);

	return object;
}

bool DfxLxInfiniteLightHandler::create() {

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto temp_branch = space->findAbsolute("~/LIGS");

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

	/// Return failier
	return false;
}

deepsg::SgShapeNode *DfxLxInfiniteLightHandler::createShapeNode() {
	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_shape_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgShapeNode>();
		}
		else {

			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Store Scene graph node
			auto scenegraph_node = space->any("sg_shape_node", "Scene Graph Shape Node");
			space->move(scenegraph_node, _dataObject);

			return (deepsg::SgShapeNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

bool DfxLxInfiniteLightHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {

		auto shapeNodeAny = _dataObject->data<fl::DataObject>().property("sg_shape_node");
		DfxLxInfiniteLightWidget *shapeNode = (DfxLxInfiniteLightWidget*)_dataObject->data<fl::DataObject>().property("sg_shape_node")->rawData<deepsg::SgShapeNode>();

		if (shapeNode != nullptr) {
			DfxQSceneManager::getInstance()->removeNode(shapeNode);
			shapeNode->setShape(nullptr);
			delete shapeNode;
			shapeNodeAny->nullify();
		}

		space->remove(_dataObject);
		_dataObject = nullptr;

		return true;
	}

	return false;
}

deepsg::SgShapeNode *DfxLxInfiniteLightHandler::shapeNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_shape_node");

	if (any != nullptr) {
		return any->rawData<deepsg::SgShapeNode>();
	}
	else {
		return nullptr;
	}
}

bool DfxLxInfiniteLightHandler::clearAllInfiniteLights() {
	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto object_branch = space->findAbsolute("~/LIGS");

	/// Proceed only if it exists
	if (object_branch != nullptr) {
		for (auto any : object_branch->data<fl::DataObject>().properties()) {

			auto lt = any->data<fl::DataObject>().property("item_type")->data<unsigned int>();

			if (lt == DfxItemWidgetType::INFINITE_LIGHT) {
				DfxLxInfiniteLightHandler handler;
				bool rc = handler.fetch(space->absolutePath(any));

				if (rc) {
					if (!handler.destroy()) {
						DfxGlobal::console::println("Infinite Light Handler: Clear all failed to destroy infinite light.");
					}
				}
				else {
					DfxGlobal::console::println("Infinite Light Handler: Clear all failed to fetch infinite light.");
				}
			}
		}
	}
	else {
		DfxGlobal::console::println("Infinite Light Handler: Clear all failed to find branch.");
		return false;
	}

	return true;
}

deepsg::SgNode* DfxLxInfiniteLightHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		/// Master Space
		auto space = DfxSpace::getInstance();

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_shape_node");
		auto xform = any->data<fl::DataObject>().property("xform");

		deepsg::SgShapeNode *gnode = new DfxLxInfiniteLightWidget;

		/// Store Scene graph node
		scenegraph_node->newData<deepsg::SgShapeNode>(DfxFluentTypes::SG_SHAPE_NODE, gnode);

		/// Finish setting up scene graph node
		gnode->setXFormProperties(xform);
		gnode->setReferencePath(space->absolutePath(any));

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxLxInfiniteLightHandler::deleteNode(deepsg::SgNode *node) {
	DfxLxInfiniteLightHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}