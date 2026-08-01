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

#include "DfxLxPointLightHandler.h"
#include "DfxItemWidgetType.h"

std::string DfxLxPointLightHandler::model_id = "/LIGHT/LUX_POINT_LIGHT_NODE_INFO";
size_t DfxLxPointLightHandler::ID_GEN = 1;

DfxLxPointLightHandler::DfxLxPointLightHandler() {
	_dataObject = nullptr;
}

DfxLxPointLightHandler::~DfxLxPointLightHandler() {

}

fl::Any *DfxLxPointLightHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	auto object = fl::DataObject::Builder()
		.setName("lux_point_light")
		.setLabel("Lux Point Light" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "item_type", "Item Type", DfxItemWidgetType::POINT_LIGHT)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", DfxRenderTarget::LUXCORE)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gain", "Gain", 1.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "color_r", "Color Red", 1.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "color_g", "Color Green", 1.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "color_b", "Color Blue", 1.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "power", "Power", 60.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "efficacy", "Efficacy", 17.0f)
		.build(space);

	return object;
}

bool DfxLxPointLightHandler::create() {
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

deepsg::SgShapeNode *DfxLxPointLightHandler::createShapeNode() {
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

bool DfxLxPointLightHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {

		auto shapeNodeAny = _dataObject->data<fl::DataObject>().property("sg_shape_node");
		DfxLxPointLightWidget *shapeNode = (DfxLxPointLightWidget*)_dataObject->data<fl::DataObject>().property("sg_shape_node")->rawData<deepsg::SgShapeNode>();

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

deepsg::SgShapeNode *DfxLxPointLightHandler::shapeNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_shape_node");

	if (any != nullptr) {
		return any->rawData<deepsg::SgShapeNode>();
	}
	else {
		return nullptr;
	}
}

bool DfxLxPointLightHandler::clearAllPointLights() {
	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto object_branch = space->findAbsolute("~/LIGS");

	/// Proceed only if it exists
	if (object_branch != nullptr) {
		for (auto any : object_branch->data<fl::DataObject>().properties()) {

			auto lt = any->data<fl::DataObject>().property("item_type")->data<unsigned int>();

			if (lt == DfxItemWidgetType::POINT_LIGHT) {
				DfxLxPointLightHandler handler;
				bool rc = handler.fetch(space->absolutePath(any));

				if (rc) {
					if (!handler.destroy()) {
						DfxGlobal::console::println("Point Light Handler: Clear all failed to destroy point light.");
					}
				}
				else {
					DfxGlobal::console::println("Point Light Handler: Clear all failed to fetch point light.");
				}
			}
		}
	}
	else {
		DfxGlobal::console::println("Point Light Handler: Clear all failed to find branch.");
		return false;
	}

	return true;
}

deepsg::SgNode* DfxLxPointLightHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		/// Master Space
		auto space = DfxSpace::getInstance();

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_shape_node");
		auto xform = any->data<fl::DataObject>().property("xform");

		deepsg::SgShapeNode *gnode = new DfxLxPointLightWidget;

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

void DfxLxPointLightHandler::deleteNode(deepsg::SgNode *node) {
	DfxLxPointLightHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}