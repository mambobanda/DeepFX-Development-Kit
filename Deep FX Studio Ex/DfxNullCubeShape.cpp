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

#include <DfxNullCubeShape.h>

DfxNullCubeShape::DfxNullCubeShape() {
	build(); 
}

DfxNullCubeShape::~DfxNullCubeShape() {

}

void DfxNullCubeShape::build() {

	deepsg::SgShapeBuffer shBuffer;

	shBuffer.setWireframe(true);
	shBuffer.setColor(153, 255, 192);

	float scl = 0.09f;
	std::vector<glm::vec3> points;

	// Top Cube
	points.push_back(glm::vec3(-scl, scl, scl));
	points.push_back(glm::vec3(scl, scl, scl));
	points.push_back(glm::vec3(scl, scl, -scl));
	points.push_back(glm::vec3(-scl, scl, -scl));

	shBuffer.drawQuad(points[0], points[1], points[2], points[3]);
	shBuffer.drawQuad(points[3], points[2], points[1], points[0]);

	// Bottom Cube
	points.clear();
	points.push_back(glm::vec3(-scl, -scl, -scl));
	points.push_back(glm::vec3(scl, -scl, -scl));
	points.push_back(glm::vec3(scl, -scl, scl));
	points.push_back(glm::vec3(-scl, -scl, scl));

	shBuffer.drawQuad(points[0], points[1], points[2], points[3]);
	shBuffer.drawQuad(points[3], points[2], points[1], points[0]);

	// Front Cube
	points.clear();
	points.push_back(glm::vec3(-scl, scl, scl));
	points.push_back(glm::vec3(scl, scl, scl));
	points.push_back(glm::vec3(scl, -scl, scl));
	points.push_back(glm::vec3(-scl, -scl, scl));

	shBuffer.drawQuad(points[0], points[1], points[2], points[3]);
	shBuffer.drawQuad(points[3], points[2], points[1], points[0]);

	// Back Cube
	points.clear();
	points.push_back(glm::vec3(-scl, scl, -scl));
	points.push_back(glm::vec3(scl, scl, -scl));
	points.push_back(glm::vec3(scl, -scl, -scl));
	points.push_back(glm::vec3(-scl, -scl, -scl));

	shBuffer.drawQuad(points[0], points[1], points[2], points[3]);
	shBuffer.drawQuad(points[3], points[2], points[1], points[0]);

	// left Cube
	points.clear();
	points.push_back(glm::vec3(-scl, scl, -scl));
	points.push_back(glm::vec3(-scl, scl, scl));
	points.push_back(glm::vec3(-scl, -scl, scl));
	points.push_back(glm::vec3(-scl, -scl, -scl));

	shBuffer.drawQuad(points[0], points[1], points[2], points[3]);
	shBuffer.drawQuad(points[3], points[2], points[1], points[0]);

	// Right Cube
	points.clear();
	points.push_back(glm::vec3(scl, scl, -scl));
	points.push_back(glm::vec3(scl, scl, scl));
	points.push_back(glm::vec3(scl, -scl, scl));
	points.push_back(glm::vec3(scl, -scl, -scl));

	shBuffer.drawQuad(points[0], points[1], points[2], points[3]);
	shBuffer.drawQuad(points[3], points[2], points[1], points[0]);

	_shapes.push_back(shBuffer);
}