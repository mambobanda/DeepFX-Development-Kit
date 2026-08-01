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

#include "DfxBaseHandler.h"


DfxBaseHandler::DfxBaseHandler() {
	_dataObject = nullptr;
}

DfxBaseHandler::~DfxBaseHandler() {

}

bool DfxBaseHandler::fetch(std::string path) {

	auto space = DfxSpace::getInstance();

	auto any = space->findAbsolute(path);

	if (any != nullptr) {
		_dataObject = any;
		return true;
	}
	else {
		return false;
	}
}

fl::Any *DfxBaseHandler::dataObject() {
	return _dataObject;
}

fl::Any *DfxBaseHandler::newXForm() {
	auto space = DfxSpace::getInstance();

	auto any = fl::DataObject::Builder()
		.setName("xform")
		.setLabel("Transform")
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "rotation_type", "Rotation Type", 0)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "x_pos", "X-Position", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "y_pos", "Y-Position", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "z_pos", "Z-Position", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "x_scl", "X-Scale", 1.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "y_scl", "Y-Scale", 1.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "z_scl", "Z-Scale", 1.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "x_rot", "X-Rotation", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "y_rot", "Y-Rotation", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "z_rot", "Z-Rotation", 0.f)
		.build(space);

	return any;
}


fl::Any *DfxBaseHandler::xform() {
	auto any = _dataObject->data<fl::DataObject>().property("xform");

	if (any != nullptr) {
		return any;
	}
	else {
		return nullptr;
	}
}

bool DfxBaseHandler::isNull() {
	return _dataObject == nullptr;
}