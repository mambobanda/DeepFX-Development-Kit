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

#include "SgStagePropNode.h"

namespace deepsg {

	SgStagePropNode::SgStagePropNode(SgStageNode *stg, size_t ptype): SgNode(SgNodeType::STAGE_PROP) {
		_stage = stg;
		_type = ptype;
		_shape = nullptr;
	}

	SgStagePropNode::~SgStagePropNode() {

	}

	SgStageNode *SgStagePropNode::stage() {
		return _stage;
	}

	void SgStagePropNode::setStage(SgStageNode *node) {
		_stage = node;
	}

	size_t SgStagePropNode::propType() {
		return _type;
	}

	void SgStagePropNode::addShape(SgShape *shp) {
		_shape = shp;
	}

	SgShape* SgStagePropNode::shape() {
		return _shape;
	}

	void SgStagePropNode::setColor(glm::ivec3 ivc) {
		_color = ivc;
	}

	glm::ivec3 SgStagePropNode::getColor() {
		return _color;
	}

};