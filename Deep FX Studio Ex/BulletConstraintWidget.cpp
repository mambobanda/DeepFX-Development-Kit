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

#include "DfxBulletConstraintWidget.h"
#include "DfxBulletAgent.h"

std::string DfxBulletConstraintWidget::_uniqueName = "UID_BULLET_CONSTRAINT_WIDGET_NODE";

DfxBulletConstraintWidget::DfxBulletConstraintWidget() {
	body_a = nullptr;
	body_b = nullptr;

	deepsg::SgShape::setScale(1.f);

	setShape(this);
	setShapeType(DfxItemWidgetType::UNDEFINED);
}

DfxBulletConstraintWidget::~DfxBulletConstraintWidget() {

}

std::string DfxBulletConstraintWidget::uniqueName() {
	return _uniqueName;
}

int DfxBulletConstraintWidget::shapeCount() {
	int count = 1;

	if (body_a != nullptr) count++;
	if (body_b != nullptr) count++;

	return count;
}

void DfxBulletConstraintWidget::setBodies(DfxBulletAgent *ba, DfxBulletAgent *bb) {

	body_a = ba;
	body_b = bb;

}

std::vector<deepsg::SgShapeBuffer> &DfxBulletConstraintWidget::shapes() {

	deepsg::SgShapeBuffer shBuffer;

	if (body_a == nullptr || body_b == nullptr) {

		shBuffer.setWireframe(false);
		shBuffer.setColor(153, 255, 192);

		float scl = 0.09f;
		std::vector<glm::vec3> points;

		// Top Cube
		points.push_back(glm::vec3(-scl, scl, scl));
		points.push_back(glm::vec3(scl, scl, scl));
		points.push_back(glm::vec3(scl, scl, -scl));
		points.push_back(glm::vec3(-scl, scl, -scl));

		shBuffer.drawPolygon(points);

		// Bottom Cube
		points.clear();
		points.push_back(glm::vec3(-scl, -scl, -scl));
		points.push_back(glm::vec3(scl, -scl, -scl));
		points.push_back(glm::vec3(scl, -scl, scl));
		points.push_back(glm::vec3(-scl, -scl, scl));

		shBuffer.drawPolygon(points);

		// Front Cube
		points.clear();
		points.push_back(glm::vec3(-scl, scl, scl));
		points.push_back(glm::vec3(scl, scl, scl));
		points.push_back(glm::vec3(scl, -scl, scl));
		points.push_back(glm::vec3(-scl, -scl, scl));

		shBuffer.drawPolygon(points);

		// Back Cube
		points.clear();
		points.push_back(glm::vec3(-scl, scl, -scl));
		points.push_back(glm::vec3(scl, scl, -scl));
		points.push_back(glm::vec3(scl, -scl, -scl));
		points.push_back(glm::vec3(-scl, -scl, -scl));

		shBuffer.drawPolygon(points);

		// left Cube
		points.clear();
		points.push_back(glm::vec3(-scl, scl, -scl));
		points.push_back(glm::vec3(-scl, scl, scl));
		points.push_back(glm::vec3(-scl, -scl, scl));
		points.push_back(glm::vec3(-scl, -scl, -scl));

		shBuffer.drawPolygon(points);

		// Right Cube
		points.clear();
		points.push_back(glm::vec3(scl, scl, -scl));
		points.push_back(glm::vec3(scl, scl, scl));
		points.push_back(glm::vec3(scl, -scl, scl));
		points.push_back(glm::vec3(scl, -scl, -scl));

		shBuffer.drawPolygon(points);

		//if (body_a != nullptr) {
		//	
		//	btTransform gt;
		//	body_a->body->toRigidBody()->getMotionState()->getWorldTransform(gt);

		//	glm::vec3 a(gt.getOrigin().x(), gt.getOrigin().y(), gt.getOrigin().z());
		//	glm::vec3 b(0.f, 0.f, 0.f);

		//	shBuffer.drawStippledLine(b, a);

		//}
	}

	return std::vector<deepsg::SgShapeBuffer> { shBuffer };
}