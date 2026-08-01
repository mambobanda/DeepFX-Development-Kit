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

#include "DfxLxCameraHandler.h"
#include "DfxItemWidgetType.h"

std::string DfxLxCameraHandler::model_id = "/CAMERA/LUX_CAMERA_NODE_INFO";
size_t DfxLxCameraHandler::ID_GEN = 1;

DfxLxCameraHandler::DfxLxCameraHandler() {
	_dataObject = nullptr;
}

DfxLxCameraHandler::~DfxLxCameraHandler() {

}

fl::Any *DfxLxCameraHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	auto object = fl::DataObject::Builder()
		.setName("lux_camera")
		.setLabel("Lux Camera" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "item_type", "Item Type", DfxItemWidgetType::CAMERA)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", DfxRenderTarget::LUXCORE)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "width", "Width", 640)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "height", "Height", 480)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "field_of_view", "Field Of View", 60.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "lens_size", "Lens Size", 31.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "lens_radius", "Lens Radius", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "focal_distance", "Focal Distance", 5.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "shutter_open", "Shutter Open", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "shutter_close", "Shutter Clode", 0.f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "use_dof", "Enable Depth Pf Field", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "dof_fstop", "F-Stop", 2.8f)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "dof_helper", "DOF Helper", std::string(""))

		.build(space);

	return object;
}

bool DfxLxCameraHandler::create() {
	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Camera Branch
	auto temp_branch = space->findAbsolute("~/CAMS");

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

deepsg::SgShapeNode *DfxLxCameraHandler::createShapeNode() {
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

bool DfxLxCameraHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {

		auto shapeNodeAny = _dataObject->data<fl::DataObject>().property("sg_shape_node");
		DfxLxCameraWidget *shapeNode = (DfxLxCameraWidget*)_dataObject->data<fl::DataObject>().property("sg_shape_node")->rawData<deepsg::SgShapeNode>();

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

deepsg::SgShapeNode *DfxLxCameraHandler::shapeNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_shape_node");
	
	if (any != nullptr) {
		return any->rawData<deepsg::SgShapeNode>();
	}
	else {
		return nullptr;
	}
}

std::vector<DfxLxCameraHandler>  DfxLxCameraHandler::allCameras() {

	std::vector<DfxLxCameraHandler> cameras;
	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Camera Branch
	auto object_branch = space->findAbsolute("~/CAMS");

	/// Proceed only if it exists
	if (object_branch != nullptr) {
		for (auto any : object_branch->data<fl::DataObject>().properties()) {

			auto lt = any->data<fl::DataObject>().property("item_type")->data<unsigned int>();

			if (lt == DfxItemWidgetType::CAMERA) {
				DfxLxCameraHandler handler;
				bool rc = handler.fetch(space->absolutePath(any));

				if (rc) {
					cameras.push_back(handler);
				}
			}
		}
	}

	return cameras;
}

int DfxLxCameraHandler::width() {
	return _dataObject->data<fl::DataObject>().property("width")->data<unsigned int>();
}

int DfxLxCameraHandler::height() {
	return _dataObject->data<fl::DataObject>().property("height")->data<unsigned int>();
}

bool DfxLxCameraHandler::clearAllCameras() {

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Camera Branch
	auto object_branch = space->findAbsolute("~/CAMS");

	/// Proceed only if it exists
	if (object_branch != nullptr) {
		for (auto any : object_branch->data<fl::DataObject>().properties()) {

			auto lt = any->data<fl::DataObject>().property("item_type")->data<unsigned int>();

			if (lt == DfxItemWidgetType::CAMERA) {
				DfxLxCameraHandler handler;
				bool rc = handler.fetch(space->absolutePath(any));

				if (rc) {
					if (!handler.destroy()) {
						DfxGlobal::console::println("Camera Handler: Clear all failed to destroy camera.");
					}
				}
				else {
					DfxGlobal::console::println("Camera Handler: Clear all failed to fetch camera.");
				}
			}
		}
	}
	else {
		DfxGlobal::console::println("Camera Handler: Clear all failed to find branch.");
		return false;
	}

	return true;
}

deepsg::SgNode* DfxLxCameraHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_shape_node");
		auto xform = any->data<fl::DataObject>().property("xform");

		deepsg::SgShapeNode *gnode = new DfxLxCameraWidget;

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

void DfxLxCameraHandler::deleteNode(deepsg::SgNode *node) {
	DfxLxCameraHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}