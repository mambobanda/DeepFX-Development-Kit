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

#include "DfxWidgetHandler.h"

DfxWidgetHandler::DfxWidgetHandler() {
	_dataObject = nullptr;
}

DfxWidgetHandler::~DfxWidgetHandler() {

}

bool DfxWidgetHandler::create() {
	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Objects Branch
	auto temp_branch = space->findAbsolute("~/TEMP");

	/// Proceed only if it exists
	if (temp_branch != nullptr) {
		deepsg::SgWidgetNode *gnode = new deepsg::SgWidgetNode;

		/// Store Scene graph node
		auto scenegraph_node = space->any("sg_widget_node", "Scene Graph Widget Node");
		scenegraph_node->newData<deepsg::SgWidgetNode>(DfxFluentTypes::SG_WIDGET_NODE, gnode);

		/// Object Reference
		auto object = fl::DataObject::Builder()
			.setName("widget")
			.setLabel("Widget")
			.build(space);

		/// Object XForm 
		auto xform = newXForm();

		/// Arrange properties into the right order
		space->move(object, temp_branch);
		space->move(xform, object);
		space->move(scenegraph_node, object);

		/// Finish setting up scene graph node and move it to the master scene graph
		gnode->setXFormProperties(xform);
		gnode->setReferencePath(space->absolutePath(object));

		/// Store reference for further use
		_dataObject = object;

		/// Return success
		return true;
	}

	/// Return failier
	return false;
}

bool DfxWidgetHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {
		space->remove(_dataObject);
		_dataObject = nullptr;

		return true;
	}

	return false;
}


deepsg::SgWidgetNode *DfxWidgetHandler::widgetNode() {
	return _dataObject->data<fl::DataObject>().property("sg_widget_node")->rawData<deepsg::SgWidgetNode>();
}