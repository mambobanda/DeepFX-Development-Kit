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

#include <DfxNullArrowShape.h>

DfxNullArrowShape::DfxNullArrowShape() {
	build();
}

DfxNullArrowShape::~DfxNullArrowShape() {

}

void DfxNullArrowShape::build() {

	deepsg::SgShapeBuffer shBuffer;

	shBuffer.setWireframe(true);
	shBuffer.setColor(153, 255, 192);

	float hHeight = 1.8f;
	float hTHeight = 0.2f;
	float hWidthDepth = 0.1f;

	std::vector<glm::vec3> points;

	// Arrow Base
	points.push_back(glm::vec3(-hWidthDepth, hHeight, hWidthDepth));
	points.push_back(glm::vec3(hWidthDepth, hHeight, hWidthDepth));
	points.push_back(glm::vec3(hWidthDepth, hHeight, -hWidthDepth));
	points.push_back(glm::vec3(-hWidthDepth, hHeight, -hWidthDepth));

	shBuffer.drawQuad(points[0], points[1], points[2], points[3]);
	shBuffer.drawQuad(points[3], points[2], points[1], points[0]);

	shBuffer.drawLine(glm::vec3(-hWidthDepth, hHeight, hWidthDepth), glm::vec3(0.f, hHeight + hTHeight, 0.f));
	shBuffer.drawLine(glm::vec3(hWidthDepth, hHeight, hWidthDepth), glm::vec3(0.f, hHeight + hTHeight, 0.f));
	shBuffer.drawLine(glm::vec3(hWidthDepth, hHeight, -hWidthDepth), glm::vec3(0.f, hHeight + hTHeight, 0.f));
	shBuffer.drawLine(glm::vec3(-hWidthDepth, hHeight, -hWidthDepth), glm::vec3(0.f, hHeight + hTHeight, 0.f));

	shBuffer.drawStippledLine(glm::vec3(0.f), glm::vec3(0.f, hHeight, 0.f));

	_shapes.push_back(shBuffer);
}