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

#include "SgShapeNode.h"


namespace deepsg {

	SgShapeNode::SgShapeNode() :SgNode(SgNodeType::SHAPE) {
		_shape = nullptr;
		color = glm::vec3(0);
	}

	SgShapeNode::~SgShapeNode() {
		delete _shape;
	}

	void SgShapeNode::setShape(SgShape *sg) {
		_shape = sg;
	}

	SgShape *SgShapeNode::shape() {
		return _shape;
	}

	glm::ivec3 SgShapeNode::getColor() {
		return color;
	}

	void SgShapeNode::setColor(glm::ivec3 cl) {
		color = cl;
	}

	unsigned int SgShapeNode::shapeType() {
		return _shapeType;
	}

	void SgShapeNode::setShapeType(unsigned int type) {
		_shapeType = type;
	}

	std::string SgShapeNode::name() {
		return "Shape";
	}
}