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

#include "SgShape.h"

namespace deepsg {

	SgShape::SgShape() {

	}

	SgShape::~SgShape() {

	}

	float SgShape::getScale() {
		return scale;
	}

	void SgShape::setScale(float s) {
		scale = s;
	}

	int SgShape::shapeCount() {
		return (int)_shapes.size();
	}

	std::vector<SgShapeBuffer> &SgShape::shapes() {
		return _shapes;
	}

}