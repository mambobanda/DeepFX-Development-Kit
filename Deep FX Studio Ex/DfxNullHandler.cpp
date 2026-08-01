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

#include "DfxNullHandler.h"

std::string DfxNullHandler::model_id = "/NULL/NULL_INFO";
size_t DfxNullHandler::ID_GEN = 1;

DfxNullHandler::DfxNullHandler() {

}

DfxNullHandler::~DfxNullHandler() {

}

fl::Any *DfxNullHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	auto object = fl::DataObject::Builder()
		.setName("null")
		.setLabel("Null" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "current_shape", "Current Shape", (unsigned int)0)
		.build(space);

	return object;
}

bool DfxNullHandler::create() {

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto temp_branch = space->findAbsolute("~/NULLS");

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

bool DfxNullHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {

		auto ndata00 = nullNode();

		if (ndata00 != nullptr) {
			DfxQSceneManager::getInstance()->removeNode(ndata00);
			delete ndata00;

			auto any = _dataObject->data<fl::DataObject>().property("sg_null_node");
			any->nullify();
		}

		space->remove(_dataObject);
		_dataObject = nullptr;

		return true;
	}

	return false;
}

DfxNullNode *DfxNullHandler::nullNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_null_node");

		if (any != nullptr) {
			return any->rawData<DfxNullNode>();
		}
		else {

			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Store Scene graph node
			auto scenegraph_node = space->any("sg_null_node", "Scene Graph Null Node");
			space->move(scenegraph_node, _dataObject);

			return (DfxNullNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

unsigned int DfxNullHandler::currentShapeIndex() {

	if (_dataObject != nullptr) {
		auto any = _dataObject->data<fl::DataObject>().property("current_shape");

		if (any != nullptr) {
			return any->data<unsigned int>();
		}
		else {
			return 0;
		}
	}

	return 0;
}

deepsg::SgNode* DfxNullHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_null_node");
		auto xform = any->data<fl::DataObject>().property("xform");

		auto gnode = new DfxNullNode;

		scenegraph_node->newData<DfxNullNode>(DfxFluentTypes::SG_NULL_NODE, gnode);

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

void DfxNullHandler::deleteNode(deepsg::SgNode *node) {
	DfxNullHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}