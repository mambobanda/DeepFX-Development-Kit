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

#include "DfxLxSunSkyLightWidget.h"

std::string DfxLxSunSkyLightWidget::_uniqueName = "UID_SUN_SKY_LIGHT_WIDGET";

DfxLxSunSkyLightWidget::DfxLxSunSkyLightWidget() {
	deepsg::SgShape::setScale(0.4f);
	
	initShapes();
	setShapeType(DfxItemWidgetType::SUN_SKY_LIGHT);
	setShape(this);
}

DfxLxSunSkyLightWidget::~DfxLxSunSkyLightWidget() {

}

std::string DfxLxSunSkyLightWidget::uniqueName() {
	return _uniqueName;
}

std::string DfxLxSunSkyLightWidget::getUniqueName() {
	return _uniqueName;
}

void DfxLxSunSkyLightWidget::initShapes() {
	
	// Sun
	std::vector<glm::vec3> points;
	deepsg::SgShapeBuffer shBuffer;

	shBuffer.setWireframe(true);
	shBuffer.setColor(255, 200, 100);

	points.clear();
	points.push_back(glm::vec3(-0.668466, 0.533084, 0.168547));
	points.push_back(glm::vec3(-0.833563, 0.190255, 0.168547));
	points.push_back(glm::vec3(-0.833563, -0.190255, 0.168547));
	points.push_back(glm::vec3(-0.668466, -0.533084, 0.168547));
	points.push_back(glm::vec3(-0.370971, -0.770328, 0.168547));
	points.push_back(glm::vec3(0, -0.855, 0.168547));
	points.push_back(glm::vec3(0.370971, -0.770328, 0.168547));
	points.push_back(glm::vec3(0.668466, -0.533084, 0.168547));
	points.push_back(glm::vec3(0.833563, -0.190255, 0.168547));
	points.push_back(glm::vec3(0.833563, 0.190255, 0.168547));
	points.push_back(glm::vec3(0.668466, 0.533084, 0.168547));
	points.push_back(glm::vec3(0.370971, 0.770328, 0.168547));
	points.push_back(glm::vec3(0, 0.855, 0.168547));
	points.push_back(glm::vec3(-0.370971, 0.770328, 0.168547));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.370971, 0.770328, -0.191453));
	points.push_back(glm::vec3(0.668466, 0.533084, -0.191453));
	points.push_back(glm::vec3(0.833563, 0.190255, -0.191453));
	points.push_back(glm::vec3(0.833563, -0.190255, -0.191453));
	points.push_back(glm::vec3(0.668466, -0.533084, -0.191453));
	points.push_back(glm::vec3(0.370971, -0.770328, -0.191453));
	points.push_back(glm::vec3(0, -0.855, -0.191453));
	points.push_back(glm::vec3(-0.370971, -0.770328, -0.191453));
	points.push_back(glm::vec3(-0.668466, -0.533084, -0.191453));
	points.push_back(glm::vec3(-0.833563, -0.190255, -0.191453));
	points.push_back(glm::vec3(-0.833563, 0.190255, -0.191453));
	points.push_back(glm::vec3(-0.668466, 0.533084, -0.191453));
	points.push_back(glm::vec3(-0.370971, 0.770328, -0.191453));
	points.push_back(glm::vec3(0, 0.855, -0.191453));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.668466, 0.533084, 0.168547));
	points.push_back(glm::vec3(-0.74274, 0.592315, -0.0114528));
	points.push_back(glm::vec3(-0.926181, 0.211395, -0.0114528));
	points.push_back(glm::vec3(-0.833563, 0.190255, 0.168547));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.74274, 0.592315, -0.0114528));
	points.push_back(glm::vec3(-0.668466, 0.533084, -0.191453));
	points.push_back(glm::vec3(-0.833563, 0.190255, -0.191453));
	points.push_back(glm::vec3(-0.926181, 0.211395, -0.0114528));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.370971, 0.770328, 0.168547));
	points.push_back(glm::vec3(-0.41219, 0.85592, -0.0114528));
	points.push_back(glm::vec3(-0.74274, 0.592315, -0.0114528));
	points.push_back(glm::vec3(-0.668466, 0.533084, 0.168547));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.41219, 0.85592, -0.0114528));
	points.push_back(glm::vec3(-0.370971, 0.770328, -0.191453));
	points.push_back(glm::vec3(-0.668466, 0.533084, -0.191453));
	points.push_back(glm::vec3(-0.74274, 0.592315, -0.0114528));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 0.855, 0.168547));
	points.push_back(glm::vec3(0, 0.95, -0.0114528));
	points.push_back(glm::vec3(-0.41219, 0.85592, -0.0114528));
	points.push_back(glm::vec3(-0.370971, 0.770328, 0.168547));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 0.95, -0.0114528));
	points.push_back(glm::vec3(0, 0.855, -0.191453));
	points.push_back(glm::vec3(-0.370971, 0.770328, -0.191453));
	points.push_back(glm::vec3(-0.41219, 0.85592, -0.0114528));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.370971, 0.770328, 0.168547));
	points.push_back(glm::vec3(0.41219, 0.85592, -0.0114528));
	points.push_back(glm::vec3(0, 0.95, -0.0114528));
	points.push_back(glm::vec3(0, 0.855, 0.168547));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.41219, 0.85592, -0.0114528));
	points.push_back(glm::vec3(0.370971, 0.770328, -0.191453));
	points.push_back(glm::vec3(0, 0.855, -0.191453));
	points.push_back(glm::vec3(0, 0.95, -0.0114528));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.668466, 0.533084, 0.168547));
	points.push_back(glm::vec3(0.74274, 0.592315, -0.0114528));
	points.push_back(glm::vec3(0.41219, 0.85592, -0.0114528));
	points.push_back(glm::vec3(0.370971, 0.770328, 0.168547));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.74274, 0.592315, -0.0114528));
	points.push_back(glm::vec3(0.668466, 0.533084, -0.191453));
	points.push_back(glm::vec3(0.370971, 0.770328, -0.191453));
	points.push_back(glm::vec3(0.41219, 0.85592, -0.0114528));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.833563, 0.190255, 0.168547));
	points.push_back(glm::vec3(0.926181, 0.211395, -0.0114528));
	points.push_back(glm::vec3(0.74274, 0.592315, -0.0114528));
	points.push_back(glm::vec3(0.668466, 0.533084, 0.168547));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.926181, 0.211395, -0.0114528));
	points.push_back(glm::vec3(0.833563, 0.190255, -0.191453));
	points.push_back(glm::vec3(0.668466, 0.533084, -0.191453));
	points.push_back(glm::vec3(0.74274, 0.592315, -0.0114528));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.833563, -0.190255, 0.168547));
	points.push_back(glm::vec3(0.926181, -0.211395, -0.0114528));
	points.push_back(glm::vec3(0.926181, 0.211395, -0.0114528));
	points.push_back(glm::vec3(0.833563, 0.190255, 0.168547));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.926181, -0.211395, -0.0114528));
	points.push_back(glm::vec3(0.833563, -0.190255, -0.191453));
	points.push_back(glm::vec3(0.833563, 0.190255, -0.191453));
	points.push_back(glm::vec3(0.926181, 0.211395, -0.0114528));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.668466, -0.533084, 0.168547));
	points.push_back(glm::vec3(0.74274, -0.592315, -0.0114528));
	points.push_back(glm::vec3(0.926181, -0.211395, -0.0114528));
	points.push_back(glm::vec3(0.833563, -0.190255, 0.168547));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.74274, -0.592315, -0.0114528));
	points.push_back(glm::vec3(0.668466, -0.533084, -0.191453));
	points.push_back(glm::vec3(0.833563, -0.190255, -0.191453));
	points.push_back(glm::vec3(0.926181, -0.211395, -0.0114528));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.370971, -0.770328, 0.168547));
	points.push_back(glm::vec3(0.41219, -0.85592, -0.0114528));
	points.push_back(glm::vec3(0.74274, -0.592315, -0.0114528));
	points.push_back(glm::vec3(0.668466, -0.533084, 0.168547));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.41219, -0.85592, -0.0114528));
	points.push_back(glm::vec3(0.370971, -0.770328, -0.191453));
	points.push_back(glm::vec3(0.668466, -0.533084, -0.191453));
	points.push_back(glm::vec3(0.74274, -0.592315, -0.0114528));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, -0.855, 0.168547));
	points.push_back(glm::vec3(0, -0.95, -0.0114528));
	points.push_back(glm::vec3(0.41219, -0.85592, -0.0114528));
	points.push_back(glm::vec3(0.370971, -0.770328, 0.168547));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, -0.95, -0.0114528));
	points.push_back(glm::vec3(0, -0.855, -0.191453));
	points.push_back(glm::vec3(0.370971, -0.770328, -0.191453));
	points.push_back(glm::vec3(0.41219, -0.85592, -0.0114528));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.370971, -0.770328, 0.168547));
	points.push_back(glm::vec3(-0.41219, -0.85592, -0.0114528));
	points.push_back(glm::vec3(0, -0.95, -0.0114528));
	points.push_back(glm::vec3(0, -0.855, 0.168547));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.41219, -0.85592, -0.0114528));
	points.push_back(glm::vec3(-0.370971, -0.770328, -0.191453));
	points.push_back(glm::vec3(0, -0.855, -0.191453));
	points.push_back(glm::vec3(0, -0.95, -0.0114528));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.668466, -0.533084, 0.168547));
	points.push_back(glm::vec3(-0.74274, -0.592315, -0.0114528));
	points.push_back(glm::vec3(-0.41219, -0.85592, -0.0114528));
	points.push_back(glm::vec3(-0.370971, -0.770328, 0.168547));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.74274, -0.592315, -0.0114528));
	points.push_back(glm::vec3(-0.668466, -0.533084, -0.191453));
	points.push_back(glm::vec3(-0.370971, -0.770328, -0.191453));
	points.push_back(glm::vec3(-0.41219, -0.85592, -0.0114528));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.833563, -0.190255, 0.168547));
	points.push_back(glm::vec3(-0.926181, -0.211395, -0.0114528));
	points.push_back(glm::vec3(-0.74274, -0.592315, -0.0114528));
	points.push_back(glm::vec3(-0.668466, -0.533084, 0.168547));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.926181, -0.211395, -0.0114528));
	points.push_back(glm::vec3(-0.833563, -0.190255, -0.191453));
	points.push_back(glm::vec3(-0.668466, -0.533084, -0.191453));
	points.push_back(glm::vec3(-0.74274, -0.592315, -0.0114528));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.833563, 0.190255, 0.168547));
	points.push_back(glm::vec3(-0.926181, 0.211395, -0.0114528));
	points.push_back(glm::vec3(-0.926181, -0.211395, -0.0114528));
	points.push_back(glm::vec3(-0.833563, -0.190255, 0.168547));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.926181, 0.211395, -0.0114528));
	points.push_back(glm::vec3(-0.833563, 0.190255, -0.191453));
	points.push_back(glm::vec3(-0.833563, -0.190255, -0.191453));
	points.push_back(glm::vec3(-0.926181, -0.211395, -0.0114528));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-6.85453e-09, 1.6985, 0.056275));
	points.push_back(glm::vec3(-0.345, 1.196, 0.104275));
	points.push_back(glm::vec3(0.4025, 1.196, 0.104275));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-6.85453e-09, 1.6985, -0.079725));
	points.push_back(glm::vec3(0.4025, 1.196, -0.127725));
	points.push_back(glm::vec3(-0.345, 1.196, -0.127725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.345, 1.196, -0.011725));
	points.push_back(glm::vec3(-0.345, 1.196, -0.127725));
	points.push_back(glm::vec3(0.4025, 1.196, -0.127725));
	points.push_back(glm::vec3(0.4025, 1.196, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.4025, 1.196, -0.011725));
	points.push_back(glm::vec3(0.4025, 1.196, 0.104275));
	points.push_back(glm::vec3(-0.345, 1.196, 0.104275));
	points.push_back(glm::vec3(-0.345, 1.196, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.345, 1.196, -0.011725));
	points.push_back(glm::vec3(-0.345, 1.196, 0.104275));
	points.push_back(glm::vec3(-6.85453e-09, 1.6985, 0.056275));
	points.push_back(glm::vec3(-6.85453e-09, 1.8285, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-6.85453e-09, 1.8285, -0.011725));
	points.push_back(glm::vec3(-6.85453e-09, 1.6985, -0.079725));
	points.push_back(glm::vec3(-0.345, 1.196, -0.127725));
	points.push_back(glm::vec3(-0.345, 1.196, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.4025, 1.196, -0.011725));
	points.push_back(glm::vec3(0.4025, 1.196, -0.127725));
	points.push_back(glm::vec3(-6.85453e-09, 1.6985, -0.079725));
	points.push_back(glm::vec3(-6.85453e-09, 1.8285, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-6.85453e-09, 1.8285, -0.011725));
	points.push_back(glm::vec3(-6.85453e-09, 1.6985, 0.056275));
	points.push_back(glm::vec3(0.4025, 1.196, 0.104275));
	points.push_back(glm::vec3(0.4025, 1.196, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.30113, 1.09177, 0.056275));
	points.push_back(glm::vec3(0.694427, 1.03306, 0.104275));
	points.push_back(glm::vec3(1.17491, 0.460441, 0.104275));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.30113, 1.09177, -0.079725));
	points.push_back(glm::vec3(1.17491, 0.460441, -0.127725));
	points.push_back(glm::vec3(0.694427, 1.03306, -0.127725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.694427, 1.03306, -0.011725));
	points.push_back(glm::vec3(0.694427, 1.03306, -0.127725));
	points.push_back(glm::vec3(1.17491, 0.460441, -0.127725));
	points.push_back(glm::vec3(1.17491, 0.460441, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.17491, 0.460441, -0.011725));
	points.push_back(glm::vec3(1.17491, 0.460441, 0.104275));
	points.push_back(glm::vec3(0.694427, 1.03306, 0.104275));
	points.push_back(glm::vec3(0.694427, 1.03306, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.694427, 1.03306, -0.011725));
	points.push_back(glm::vec3(0.694427, 1.03306, 0.104275));
	points.push_back(glm::vec3(1.30113, 1.09177, 0.056275));
	points.push_back(glm::vec3(1.40071, 1.17534, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.40071, 1.17534, -0.011725));
	points.push_back(glm::vec3(1.30113, 1.09177, -0.079725));
	points.push_back(glm::vec3(0.694427, 1.03306, -0.127725));
	points.push_back(glm::vec3(0.694427, 1.03306, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.17491, 0.460441, -0.011725));
	points.push_back(glm::vec3(1.17491, 0.460441, -0.127725));
	points.push_back(glm::vec3(1.30113, 1.09177, -0.079725));
	points.push_back(glm::vec3(1.40071, 1.17534, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.40071, 1.17534, -0.011725));
	points.push_back(glm::vec3(1.30113, 1.09177, 0.056275));
	points.push_back(glm::vec3(1.17491, 0.460441, 0.104275));
	points.push_back(glm::vec3(1.17491, 0.460441, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.6985, 5.49349e-08, 0.056275));
	points.push_back(glm::vec3(1.196, 0.345, 0.104275));
	points.push_back(glm::vec3(1.196, -0.4025, 0.104275));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.6985, 5.49349e-08, -0.079725));
	points.push_back(glm::vec3(1.196, -0.4025, -0.127725));
	points.push_back(glm::vec3(1.196, 0.345, -0.127725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.196, 0.345, -0.011725));
	points.push_back(glm::vec3(1.196, 0.345, -0.127725));
	points.push_back(glm::vec3(1.196, -0.4025, -0.127725));
	points.push_back(glm::vec3(1.196, -0.4025, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.196, -0.4025, -0.011725));
	points.push_back(glm::vec3(1.196, -0.4025, 0.104275));
	points.push_back(glm::vec3(1.196, 0.345, 0.104275));
	points.push_back(glm::vec3(1.196, 0.345, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.196, 0.345, -0.011725));
	points.push_back(glm::vec3(1.196, 0.345, 0.104275));
	points.push_back(glm::vec3(1.6985, 5.49349e-08, 0.056275));
	points.push_back(glm::vec3(1.8285, 4.72192e-08, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.8285, 4.72192e-08, -0.011725));
	points.push_back(glm::vec3(1.6985, 5.49349e-08, -0.079725));
	points.push_back(glm::vec3(1.196, 0.345, -0.127725));
	points.push_back(glm::vec3(1.196, 0.345, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.196, -0.4025, -0.011725));
	points.push_back(glm::vec3(1.196, -0.4025, -0.127725));
	points.push_back(glm::vec3(1.6985, 5.49349e-08, -0.079725));
	points.push_back(glm::vec3(1.8285, 4.72192e-08, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.8285, 4.72192e-08, -0.011725));
	points.push_back(glm::vec3(1.6985, 5.49349e-08, 0.056275));
	points.push_back(glm::vec3(1.196, -0.4025, 0.104275));
	points.push_back(glm::vec3(1.196, -0.4025, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.15837, -1.2422, 0.056275));
	points.push_back(glm::vec3(1.06799, -0.63941, 0.104275));
	points.push_back(glm::vec3(0.5213, -1.1492, 0.104275));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.15837, -1.2422, -0.079725));
	points.push_back(glm::vec3(0.5213, -1.1492, -0.127725));
	points.push_back(glm::vec3(1.06799, -0.63941, -0.127725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.06799, -0.63941, -0.011725));
	points.push_back(glm::vec3(1.06799, -0.63941, -0.127725));
	points.push_back(glm::vec3(0.5213, -1.1492, -0.127725));
	points.push_back(glm::vec3(0.5213, -1.1492, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.5213, -1.1492, -0.011725));
	points.push_back(glm::vec3(0.5213, -1.1492, 0.104275));
	points.push_back(glm::vec3(1.06799, -0.63941, 0.104275));
	points.push_back(glm::vec3(1.06799, -0.63941, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.06799, -0.63941, -0.011725));
	points.push_back(glm::vec3(1.06799, -0.63941, 0.104275));
	points.push_back(glm::vec3(1.15837, -1.2422, 0.056275));
	points.push_back(glm::vec3(1.24703, -1.33728, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.24703, -1.33728, -0.011725));
	points.push_back(glm::vec3(1.15837, -1.2422, -0.079725));
	points.push_back(glm::vec3(1.06799, -0.63941, -0.127725));
	points.push_back(glm::vec3(1.06799, -0.63941, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.5213, -1.1492, -0.011725));
	points.push_back(glm::vec3(0.5213, -1.1492, -0.127725));
	points.push_back(glm::vec3(1.15837, -1.2422, -0.079725));
	points.push_back(glm::vec3(1.24703, -1.33728, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.24703, -1.33728, -0.011725));
	points.push_back(glm::vec3(1.15837, -1.2422, 0.056275));
	points.push_back(glm::vec3(0.5213, -1.1492, 0.104275));
	points.push_back(glm::vec3(0.5213, -1.1492, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.0296429, -1.69824, 0.056275));
	points.push_back(glm::vec3(0.324074, -1.20184, 0.104275));
	points.push_back(glm::vec3(-0.423312, -1.18879, 0.104275));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.0296429, -1.69824, -0.079725));
	points.push_back(glm::vec3(-0.423312, -1.18879, -0.127725));
	points.push_back(glm::vec3(0.324074, -1.20184, -0.127725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.324074, -1.20184, -0.011725));
	points.push_back(glm::vec3(0.324074, -1.20184, -0.127725));
	points.push_back(glm::vec3(-0.423312, -1.18879, -0.127725));
	points.push_back(glm::vec3(-0.423312, -1.18879, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.423312, -1.18879, -0.011725));
	points.push_back(glm::vec3(-0.423312, -1.18879, 0.104275));
	points.push_back(glm::vec3(0.324074, -1.20184, 0.104275));
	points.push_back(glm::vec3(0.324074, -1.20184, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.324074, -1.20184, -0.011725));
	points.push_back(glm::vec3(0.324074, -1.20184, 0.104275));
	points.push_back(glm::vec3(-0.0296429, -1.69824, 0.056275));
	points.push_back(glm::vec3(-0.0319117, -1.82822, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.0319117, -1.82822, -0.011725));
	points.push_back(glm::vec3(-0.0296429, -1.69824, -0.079725));
	points.push_back(glm::vec3(0.324074, -1.20184, -0.127725));
	points.push_back(glm::vec3(0.324074, -1.20184, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.423312, -1.18879, -0.011725));
	points.push_back(glm::vec3(-0.423312, -1.18879, -0.127725));
	points.push_back(glm::vec3(-0.0296429, -1.69824, -0.079725));
	points.push_back(glm::vec3(-0.0319117, -1.82822, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.0319117, -1.82822, -0.011725));
	points.push_back(glm::vec3(-0.0296429, -1.69824, 0.056275));
	points.push_back(glm::vec3(-0.423312, -1.18879, 0.104275));
	points.push_back(glm::vec3(-0.423312, -1.18879, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.2218, -1.17988, 0.056275));
	points.push_back(glm::vec3(-0.620673, -1.07898, 0.104275));
	points.push_back(glm::vec3(-1.13993, -0.541277, 0.104275));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.2218, -1.17988, -0.079725));
	points.push_back(glm::vec3(-1.13993, -0.541277, -0.127725));
	points.push_back(glm::vec3(-0.620673, -1.07898, -0.127725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.620673, -1.07898, -0.011725));
	points.push_back(glm::vec3(-0.620673, -1.07898, -0.127725));
	points.push_back(glm::vec3(-1.13993, -0.541277, -0.127725));
	points.push_back(glm::vec3(-1.13993, -0.541277, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.13993, -0.541277, -0.011725));
	points.push_back(glm::vec3(-1.13993, -0.541277, 0.104275));
	points.push_back(glm::vec3(-0.620673, -1.07898, 0.104275));
	points.push_back(glm::vec3(-0.620673, -1.07898, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.620673, -1.07898, -0.011725));
	points.push_back(glm::vec3(-0.620673, -1.07898, 0.104275));
	points.push_back(glm::vec3(-1.2218, -1.17988, 0.056275));
	points.push_back(glm::vec3(-1.31531, -1.27018, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.31531, -1.27018, -0.011725));
	points.push_back(glm::vec3(-1.2218, -1.17988, -0.079725));
	points.push_back(glm::vec3(-0.620673, -1.07898, -0.127725));
	points.push_back(glm::vec3(-0.620673, -1.07898, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.13993, -0.541277, -0.011725));
	points.push_back(glm::vec3(-1.13993, -0.541277, -0.127725));
	points.push_back(glm::vec3(-1.2218, -1.17988, -0.079725));
	points.push_back(glm::vec3(-1.31531, -1.27018, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.31531, -1.27018, -0.011725));
	points.push_back(glm::vec3(-1.2218, -1.17988, 0.056275));
	points.push_back(glm::vec3(-1.13993, -0.541277, 0.104275));
	points.push_back(glm::vec3(-1.13993, -0.541277, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.6985, -2.28216e-08, 0.056275));
	points.push_back(glm::vec3(-1.196, -0.345, 0.104275));
	points.push_back(glm::vec3(-1.196, 0.4025, 0.104275));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.6985, -2.28216e-08, -0.079725));
	points.push_back(glm::vec3(-1.196, 0.4025, -0.127725));
	points.push_back(glm::vec3(-1.196, -0.345, -0.127725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.196, -0.345, -0.011725));
	points.push_back(glm::vec3(-1.196, -0.345, -0.127725));
	points.push_back(glm::vec3(-1.196, 0.4025, -0.127725));
	points.push_back(glm::vec3(-1.196, 0.4025, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.196, 0.4025, -0.011725));
	points.push_back(glm::vec3(-1.196, 0.4025, 0.104275));
	points.push_back(glm::vec3(-1.196, -0.345, 0.104275));
	points.push_back(glm::vec3(-1.196, -0.345, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.196, -0.345, -0.011725));
	points.push_back(glm::vec3(-1.196, -0.345, 0.104275));
	points.push_back(glm::vec3(-1.6985, -2.28216e-08, 0.056275));
	points.push_back(glm::vec3(-1.8285, 1.33788e-08, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.8285, 1.33788e-08, -0.011725));
	points.push_back(glm::vec3(-1.6985, -2.28216e-08, -0.079725));
	points.push_back(glm::vec3(-1.196, -0.345, -0.127725));
	points.push_back(glm::vec3(-1.196, -0.345, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.196, 0.4025, -0.011725));
	points.push_back(glm::vec3(-1.196, 0.4025, -0.127725));
	points.push_back(glm::vec3(-1.6985, -2.28216e-08, -0.079725));
	points.push_back(glm::vec3(-1.8285, 1.33788e-08, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.8285, 1.33788e-08, -0.011725));
	points.push_back(glm::vec3(-1.6985, -2.28216e-08, 0.056275));
	points.push_back(glm::vec3(-1.196, 0.4025, 0.104275));
	points.push_back(glm::vec3(-1.196, 0.4025, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.2422, 1.15837, 0.056275));
	points.push_back(glm::vec3(-1.10999, 0.563353, 0.104275));
	points.push_back(glm::vec3(-0.600195, 1.11004, 0.104275));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.2422, 1.15837, -0.079725));
	points.push_back(glm::vec3(-0.600195, 1.11004, -0.127725));
	points.push_back(glm::vec3(-1.10999, 0.563353, -0.127725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.10999, 0.563353, -0.011725));
	points.push_back(glm::vec3(-1.10999, 0.563353, -0.127725));
	points.push_back(glm::vec3(-0.600195, 1.11004, -0.127725));
	points.push_back(glm::vec3(-0.600195, 1.11004, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.600195, 1.11004, -0.011725));
	points.push_back(glm::vec3(-0.600195, 1.11004, 0.104275));
	points.push_back(glm::vec3(-1.10999, 0.563353, 0.104275));
	points.push_back(glm::vec3(-1.10999, 0.563353, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.10999, 0.563353, -0.011725));
	points.push_back(glm::vec3(-1.10999, 0.563353, 0.104275));
	points.push_back(glm::vec3(-1.2422, 1.15837, 0.056275));
	points.push_back(glm::vec3(-1.33728, 1.24703, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.33728, 1.24703, -0.011725));
	points.push_back(glm::vec3(-1.2422, 1.15837, -0.079725));
	points.push_back(glm::vec3(-1.10999, 0.563353, -0.127725));
	points.push_back(glm::vec3(-1.10999, 0.563353, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.600195, 1.11004, -0.011725));
	points.push_back(glm::vec3(-0.600195, 1.11004, -0.127725));
	points.push_back(glm::vec3(-1.2422, 1.15837, -0.079725));
	points.push_back(glm::vec3(-1.33728, 1.24703, -0.011725));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.33728, 1.24703, -0.011725));
	points.push_back(glm::vec3(-1.2422, 1.15837, 0.056275));
	points.push_back(glm::vec3(-0.600195, 1.11004, 0.104275));
	points.push_back(glm::vec3(-0.600195, 1.11004, -0.011725));

	shBuffer.drawPolygon(points);

	_shapes.push_back(shBuffer);


	// Arrow
	deepsg::SgShapeBuffer shBuffer00;
	shBuffer00.setWireframe(true);
	shBuffer00.setColor(255, 200, 100);

	float hHeight = -3.6f;
	float hTHeight = -0.2f;
	float hWidthDepth = 0.1f;
	float xshift = -0.03f;

	std::vector<glm::vec3> points00;

	points00.push_back(glm::vec3(-hWidthDepth + xshift, hHeight, hWidthDepth));
	points00.push_back(glm::vec3(hWidthDepth + xshift, hHeight, hWidthDepth));
	points00.push_back(glm::vec3(hWidthDepth + xshift, hHeight, -hWidthDepth));
	points00.push_back(glm::vec3(-hWidthDepth + xshift, hHeight, -hWidthDepth));

	shBuffer00.drawQuad(points00[0], points00[1], points00[2], points00[3]);
	shBuffer00.drawQuad(points00[3], points00[2], points00[1], points00[0]);

	shBuffer00.drawLine(glm::vec3(-hWidthDepth + xshift, hHeight, hWidthDepth), glm::vec3(xshift, hHeight + hTHeight, 0.f));
	shBuffer00.drawLine(glm::vec3(hWidthDepth + xshift, hHeight, hWidthDepth), glm::vec3(xshift, hHeight + hTHeight, 0.f));
	shBuffer00.drawLine(glm::vec3(hWidthDepth + xshift, hHeight, -hWidthDepth), glm::vec3(xshift, hHeight + hTHeight, 0.f));
	shBuffer00.drawLine(glm::vec3(-hWidthDepth + xshift, hHeight, -hWidthDepth), glm::vec3(xshift, hHeight + hTHeight, 0.f));

	shBuffer00.drawStippledLine(glm::vec3(xshift, -2.f, 0.0f), glm::vec3(xshift, hHeight, 0.f));

	_shapes.push_back(shBuffer00);
}

std::vector<std::pair<QWidget*, QString>> DfxLxSunSkyLightWidget::propertyForms() {
	std::pair<QWidget*, QString> props(properties(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> { props };
}

QWidget *DfxLxSunSkyLightWidget::properties() {
	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	auto generalControlPanel = new DfxControlPanel();
	auto sunControlPanel = new DfxControlPanel();
	auto skyControlPanel = new DfxControlPanel();

	DfxQControlArray *generalControlArray = new DfxQControlArray();
	DfxQControlArray *sunControlArray = new DfxQControlArray();
	DfxQControlArray *skyControlArray = new DfxQControlArray();

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = referencePath() + "/";

	// Basic Controls
	auto turbidity = new DfxQDecimalSpinnerControl(path + "turbidity", "", false);
	auto diffuse = new DfxQCheckBoxControl(path + "diffuse", "", false);
	auto glossy = new DfxQCheckBoxControl(path + "glossy", "", false);
	auto specular = new DfxQCheckBoxControl(path + "specular", "", false);
	auto gain = new DfxQDecimalSpinnerControl(path + "gain", "", false);

	// Sun Controls
	auto use_sun = new DfxQCheckBoxControl(path + "use_sun", "", false);
	auto sun_size = new DfxQDecimalSpinnerControl(path + "sun_relative_size", "", false);
	auto dir_x = new DfxQDecimalSpinnerControl(path + "dir_x", "", false);
	auto dir_y = new DfxQDecimalSpinnerControl(path + "dir_y", "", false);
	auto dir_z = new DfxQDecimalSpinnerControl(path + "dir_z", "", false);

	//Sky Controls
	auto use_sky = new DfxQCheckBoxControl(path + "use_sky", "", false);
	auto sky_gain = new DfxQDecimalSpinnerControl(path + "sky_gain", "", false);
	auto albedo = new DfxQColorControl(path + "sky_ground_albedo_r", path + "sky_ground_albedo_g", path + "sky_ground_albedo_b", "", false);


	generalControlArray->addControl("Turbidity", turbidity);
	generalControlArray->addSpacer(0, 5);
	generalControlArray->addControl("Affect Diffuse", diffuse);
	generalControlArray->addControl("Affect Glossy", glossy);
	generalControlArray->addControl("Affect Specular", specular);

	sunControlArray->addControl("Enable Sun", use_sun);
	sunControlArray->addSpacer(0, 5);
	sunControlArray->addControl("Relative Size", sun_size);
	sunControlArray->addSpacer(0, 5);
	sunControlArray->addControl("Intensity", gain);

	/*sunControlArray->addSpacer(0, 5);
	sunControlArray->addControl("Direction X", dir_x);
	sunControlArray->addControl("Y", dir_y);
	sunControlArray->addControl("X", dir_z);*/

	skyControlArray->addControl("Enable Sky", use_sky);
	skyControlArray->addSpacer(0, 5);
	skyControlArray->addControl("Ground Albedo", albedo);
	skyControlArray->addSpacer(0, 5);
	skyControlArray->addControl("Intensity", sky_gain);


	generalControlPanel->setTitle("General");
	generalControlPanel->setWidget(generalControlArray);

	sunControlPanel->setTitle("Sun");
	sunControlPanel->setWidget(sunControlArray);

	skyControlPanel->setTitle("Sky");
	skyControlPanel->setWidget(skyControlArray);

	main_layout->addWidget(generalControlPanel);
	main_layout->addWidget(sunControlPanel);
	main_layout->addWidget(skyControlPanel);

	widget->setLayout(main_layout);

	return widget;
}