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

#include "DfxLxSunSkyLightHandler.h"
#include "DfxItemWidgetType.h"

std::string DfxLxSunSkyLightHandler::model_id = "/LIGHT/LUX_SUN_SKY_LIGHT_NODE_INFO";
size_t DfxLxSunSkyLightHandler::ID_GEN = 1;

DfxLxSunSkyLightHandler::DfxLxSunSkyLightHandler() {
	_dataObject = nullptr;
}

DfxLxSunSkyLightHandler::~DfxLxSunSkyLightHandler() {

}

fl::Any *DfxLxSunSkyLightHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	auto object = fl::DataObject::Builder()
		.setName("lux_sun_sky_light")
		.setLabel("Lux Sun Sky Light" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "item_type", "Item Type", DfxItemWidgetType::SUN_SKY_LIGHT)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", DfxRenderTarget::LUXCORE)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "use_sun", "Enable Sun", true)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "use_sky", "Enable Sky", true)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "turbidity", "Turbidity", 2.2f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "dir_x", "Direction X", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "dir_y", "Direction Y", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "dir_z", "Direction Z", 1.0f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "diffuse", "Diffuse", true)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "glossy", "Glossy", true)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "specular", "Specular", true)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gain", "Gain", 0.00005f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "sky_gain", "Sky Gain", 0.00005f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "sky_ground_albedo_r", "Sky Ground Albedo Red", 0.5f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "sky_ground_albedo_g", "Sky Ground Albedo Green", 0.5f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "sky_ground_albedo_b", "Sky Ground Albedo Blue", 0.5f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "sun_relative_size", "Sun Relative Size", 1.0f)
		.build(space);

	return object;
}

bool DfxLxSunSkyLightHandler::create() {
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

deepsg::SgShapeNode *DfxLxSunSkyLightHandler::createShapeNode() {
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

bool DfxLxSunSkyLightHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {
		
		auto shapeNodeAny = _dataObject->data<fl::DataObject>().property("sg_shape_node");
		DfxLxSunSkyLightWidget *shapeNode = (DfxLxSunSkyLightWidget*)_dataObject->data<fl::DataObject>().property("sg_shape_node")->rawData<deepsg::SgShapeNode>();

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

deepsg::SgShapeNode *DfxLxSunSkyLightHandler::shapeNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_shape_node");

	if (any != nullptr) {
		return any->rawData<deepsg::SgShapeNode>();
	}
	else {
		return nullptr;
	}
}

bool DfxLxSunSkyLightHandler::clearAllSunSkyLights() {
	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto object_branch = space->findAbsolute("~/LIGS");

	/// Proceed only if it exists
	if (object_branch != nullptr) {
		for (auto any : object_branch->data<fl::DataObject>().properties()) {

			auto lt = any->data<fl::DataObject>().property("item_type")->data<unsigned int>();

			if (lt == DfxItemWidgetType::SUN_SKY_LIGHT) {
				DfxLxSunSkyLightHandler handler;
				bool rc = handler.fetch(space->absolutePath(any));

				if (rc) {
					if (!handler.destroy()) {
						DfxGlobal::console::println("Sun Sky Light Handler: Clear all failed to destroy Sun Sky light.");
					}
				}
				else {
					DfxGlobal::console::println("Sun Sky Light Handler: Clear all failed to fetch Sun Sky light.");
				}
			}
		}
	}
	else {
		DfxGlobal::console::println("Sun Sky Light Handler: Clear all failed to find branch.");
		return false;
	}

	return true;
}

deepsg::SgNode* DfxLxSunSkyLightHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		/// Master Space
		auto space = DfxSpace::getInstance();

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_shape_node");
		auto xform = any->data<fl::DataObject>().property("xform");

		deepsg::SgShapeNode *gnode = new DfxLxSunSkyLightWidget;

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

void DfxLxSunSkyLightHandler::deleteNode(deepsg::SgNode *node) {
	DfxLxSunSkyLightHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}
