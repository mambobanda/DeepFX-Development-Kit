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

#include "DfxLxEnvironmentLightHandler.h"
#include "DfxItemWidgetType.h"

std::string DfxLxEnvironmentLightHandler::model_id = "/LIGHT/LUX_ENVIRONMENT_LIGHT_NODE_INFO";
size_t DfxLxEnvironmentLightHandler::ID_GEN = 1;

DfxLxEnvironmentLightHandler::DfxLxEnvironmentLightHandler() {
	_dataObject = nullptr;
}

DfxLxEnvironmentLightHandler::~DfxLxEnvironmentLightHandler() {

}

fl::Any *DfxLxEnvironmentLightHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	auto object = fl::DataObject::Builder()
		.setName("lux_environment_light")
		.setLabel("Lux Environment Light" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "item_type", "Item Type", DfxItemWidgetType::ENVIRONMENT_LIGHT)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", DfxRenderTarget::LUXCORE)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "image", "Image", std::string(""))
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gamma", "Gamma", 1.f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "diffuse", "Diffuse", true)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "glossy", "Glossy", true)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "specular", "Specular", true)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "upper_hemisphere", "Use Upper Hemisphere Only", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gain", "Gain", 1.f)
		.build(space);

	return object;
}

bool DfxLxEnvironmentLightHandler::create() {
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

deepsg::SgShapeNode *DfxLxEnvironmentLightHandler::createShapeNode() {
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

bool DfxLxEnvironmentLightHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {

		auto shapeNodeAny = _dataObject->data<fl::DataObject>().property("sg_shape_node");
		DfxLxEnvironmentLightWidget *shapeNode = (DfxLxEnvironmentLightWidget*)_dataObject->data<fl::DataObject>().property("sg_shape_node")->rawData<deepsg::SgShapeNode>();

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

deepsg::SgShapeNode *DfxLxEnvironmentLightHandler::shapeNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_shape_node");

	if (any != nullptr) {
		return any->rawData<deepsg::SgShapeNode>();
	}
	else {
		return nullptr;
	}
}

bool DfxLxEnvironmentLightHandler::clearAllEnvironmentLights() {
	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto object_branch = space->findAbsolute("~/LIGS");

	/// Proceed only if it exists
	if (object_branch != nullptr) {
		for (auto any : object_branch->data<fl::DataObject>().properties()) {

			auto lt = any->data<fl::DataObject>().property("item_type")->data<unsigned int>();

			if (lt == DfxItemWidgetType::ENVIRONMENT_LIGHT) {
				DfxLxEnvironmentLightHandler handler;
				bool rc = handler.fetch(space->absolutePath(any));

				if (rc) {
					if (!handler.destroy()) {
						DfxGlobal::console::println("Environment Light Handler: Clear all failed to destroy Environment light.");
					}
				}
				else {
					DfxGlobal::console::println("Environment Light Handler: Clear all failed to fetch Environment light.");
				}
			}
		}
	}
	else {
		DfxGlobal::console::println("Environment Light Handler: Clear all failed to find branch.");
		return false;
	}

	return true;
}

deepsg::SgNode* DfxLxEnvironmentLightHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_shape_node");
		auto xform = any->data<fl::DataObject>().property("xform");

		deepsg::SgShapeNode *gnode = new DfxLxEnvironmentLightWidget;

		/// Store Scene graph node
		scenegraph_node->newData<deepsg::SgShapeNode>(DfxFluentTypes::SG_SHAPE_NODE, gnode);

		/// Finish setting up scene graph node
		gnode->setXFormProperties(xform);
		gnode->setReferencePath(referencePath);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxLxEnvironmentLightHandler::deleteNode(deepsg::SgNode *node) {
	DfxLxEnvironmentLightHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}