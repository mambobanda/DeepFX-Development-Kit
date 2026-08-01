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

#include "DfxLxEnvironmentLightWidget.h"

std::string DfxLxEnvironmentLightWidget::_uniqueName = "UID_ENVIROMENT_LIGHT_WIDGET";

DfxLxEnvironmentLightWidget::DfxLxEnvironmentLightWidget() {
	deepsg::SgShape::setScale(0.2f);
	
	initShapes();
	setShapeType(DfxItemWidgetType::ENVIRONMENT_LIGHT);
	setShape(this);
}

DfxLxEnvironmentLightWidget::~DfxLxEnvironmentLightWidget() {

}

std::string DfxLxEnvironmentLightWidget::uniqueName() {
	return _uniqueName;
}

std::string DfxLxEnvironmentLightWidget::getUniqueName() {
	return _uniqueName;
}

void DfxLxEnvironmentLightWidget::initShapes() {
	std::vector<glm::vec3> points;
	deepsg::SgShapeBuffer shBuffer;
	deepsg::SgShapeBuffer shBuffer00;

	shBuffer.setWireframe(true);
	shBuffer.setColor(255, 200, 100);
	shBuffer00.setColor(255, 200, 100);

	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(4.f, 0.f, 0.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(-4.f, 0.f, 0.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(0.f, 4.f, 0.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(0.f, 0.f, 4.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(0.f, 0.f, -4.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(4.f, 4.f, 4.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(4.f, 4.f, -4.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(-4.f, 4.f, 4.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(-4.f, 4.f, -4.f));

	points.clear();
	points.push_back(glm::vec3(0.213919, 1.42911, -0.444206));
	points.push_back(glm::vec3(0, 1.42911, -0.493032));
	points.push_back(glm::vec3(0, 1.5, 1.07157e-16));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.385468, 1.42911, -0.3074));
	points.push_back(glm::vec3(0.213919, 1.42911, -0.444206));
	points.push_back(glm::vec3(0, 1.5, 1.07157e-16));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.480671, 1.42911, -0.10971));
	points.push_back(glm::vec3(0.385468, 1.42911, -0.3074));
	points.push_back(glm::vec3(0, 1.5, 1.07157e-16));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.480671, 1.42911, -0.10971));
	points.push_back(glm::vec3(0, 1.5, 1.07157e-16));
	points.push_back(glm::vec3(0.480671, 1.42911, 0.10971));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.480671, 1.42911, 0.10971));
	points.push_back(glm::vec3(0, 1.5, 1.07157e-16));
	points.push_back(glm::vec3(0.385468, 1.42911, 0.3074));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.385468, 1.42911, 0.3074));
	points.push_back(glm::vec3(0, 1.5, 1.07157e-16));
	points.push_back(glm::vec3(0.213919, 1.42911, 0.444206));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.213919, 1.42911, 0.444206));
	points.push_back(glm::vec3(0, 1.5, 1.07157e-16));
	points.push_back(glm::vec3(0, 1.42911, 0.493032));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 1.5, 1.07157e-16));
	points.push_back(glm::vec3(-0.213919, 1.42911, 0.444206));
	points.push_back(glm::vec3(0, 1.42911, 0.493032));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 1.5, 1.07157e-16));
	points.push_back(glm::vec3(-0.385468, 1.42911, 0.3074));
	points.push_back(glm::vec3(-0.213919, 1.42911, 0.444206));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 1.5, 1.07157e-16));
	points.push_back(glm::vec3(-0.480671, 1.42911, 0.10971));
	points.push_back(glm::vec3(-0.385468, 1.42911, 0.3074));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 1.5, 1.07157e-16));
	points.push_back(glm::vec3(-0.480671, 1.42911, -0.10971));
	points.push_back(glm::vec3(-0.480671, 1.42911, 0.10971));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.385468, 1.42911, -0.3074));
	points.push_back(glm::vec3(-0.480671, 1.42911, -0.10971));
	points.push_back(glm::vec3(0, 1.5, 1.07157e-16));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.213919, 1.42911, -0.444206));
	points.push_back(glm::vec3(-0.385468, 1.42911, -0.3074));
	points.push_back(glm::vec3(0, 1.5, 1.07157e-16));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 1.42911, -0.493032));
	points.push_back(glm::vec3(-0.213919, 1.42911, -0.444206));
	points.push_back(glm::vec3(0, 1.5, 1.07157e-16));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.410507, 1.22219, -0.852426));
	points.push_back(glm::vec3(0, 1.22219, -0.946121));
	points.push_back(glm::vec3(0, 1.42911, -0.493032));
	points.push_back(glm::vec3(0.213919, 1.42911, -0.444206));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.739708, 1.22219, -0.589897));
	points.push_back(glm::vec3(0.410507, 1.22219, -0.852426));
	points.push_back(glm::vec3(0.213919, 1.42911, -0.444206));
	points.push_back(glm::vec3(0.385468, 1.42911, -0.3074));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.9224, 1.22219, -0.210532));
	points.push_back(glm::vec3(0.739708, 1.22219, -0.589897));
	points.push_back(glm::vec3(0.385468, 1.42911, -0.3074));
	points.push_back(glm::vec3(0.480671, 1.42911, -0.10971));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.9224, 1.22219, -0.210532));
	points.push_back(glm::vec3(0.480671, 1.42911, -0.10971));
	points.push_back(glm::vec3(0.480671, 1.42911, 0.10971));
	points.push_back(glm::vec3(0.9224, 1.22219, 0.210532));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.9224, 1.22219, 0.210532));
	points.push_back(glm::vec3(0.480671, 1.42911, 0.10971));
	points.push_back(glm::vec3(0.385468, 1.42911, 0.3074));
	points.push_back(glm::vec3(0.739708, 1.22219, 0.589897));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.739708, 1.22219, 0.589897));
	points.push_back(glm::vec3(0.385468, 1.42911, 0.3074));
	points.push_back(glm::vec3(0.213919, 1.42911, 0.444206));
	points.push_back(glm::vec3(0.410507, 1.22219, 0.852426));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.410507, 1.22219, 0.852426));
	points.push_back(glm::vec3(0.213919, 1.42911, 0.444206));
	points.push_back(glm::vec3(0, 1.42911, 0.493032));
	points.push_back(glm::vec3(0, 1.22219, 0.946121));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 1.42911, 0.493032));
	points.push_back(glm::vec3(-0.213919, 1.42911, 0.444206));
	points.push_back(glm::vec3(-0.410507, 1.22219, 0.852426));
	points.push_back(glm::vec3(0, 1.22219, 0.946121));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.213919, 1.42911, 0.444206));
	points.push_back(glm::vec3(-0.385468, 1.42911, 0.3074));
	points.push_back(glm::vec3(-0.739708, 1.22219, 0.589897));
	points.push_back(glm::vec3(-0.410507, 1.22219, 0.852426));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.385468, 1.42911, 0.3074));
	points.push_back(glm::vec3(-0.480671, 1.42911, 0.10971));
	points.push_back(glm::vec3(-0.9224, 1.22219, 0.210532));
	points.push_back(glm::vec3(-0.739708, 1.22219, 0.589897));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.480671, 1.42911, 0.10971));
	points.push_back(glm::vec3(-0.480671, 1.42911, -0.10971));
	points.push_back(glm::vec3(-0.9224, 1.22219, -0.210532));
	points.push_back(glm::vec3(-0.9224, 1.22219, 0.210532));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.385468, 1.42911, -0.3074));
	points.push_back(glm::vec3(-0.739708, 1.22219, -0.589897));
	points.push_back(glm::vec3(-0.9224, 1.22219, -0.210532));
	points.push_back(glm::vec3(-0.480671, 1.42911, -0.10971));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.213919, 1.42911, -0.444206));
	points.push_back(glm::vec3(-0.410507, 1.22219, -0.852426));
	points.push_back(glm::vec3(-0.739708, 1.22219, -0.589897));
	points.push_back(glm::vec3(-0.385468, 1.42911, -0.3074));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 1.42911, -0.493032));
	points.push_back(glm::vec3(0, 1.22219, -0.946121));
	points.push_back(glm::vec3(-0.410507, 1.22219, -0.852426));
	points.push_back(glm::vec3(-0.213919, 1.42911, -0.444206));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.573838, 0.896006, -1.19159));
	points.push_back(glm::vec3(0, 0.896006, -1.32256));
	points.push_back(glm::vec3(0, 1.22219, -0.946121));
	points.push_back(glm::vec3(0.410507, 1.22219, -0.852426));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.03402, 0.896006, -0.824604));
	points.push_back(glm::vec3(0.573838, 0.896006, -1.19159));
	points.push_back(glm::vec3(0.410507, 1.22219, -0.852426));
	points.push_back(glm::vec3(0.739708, 1.22219, -0.589897));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.2894, 0.896006, -0.294298));
	points.push_back(glm::vec3(1.03402, 0.896006, -0.824604));
	points.push_back(glm::vec3(0.739708, 1.22219, -0.589897));
	points.push_back(glm::vec3(0.9224, 1.22219, -0.210532));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.2894, 0.896006, -0.294298));
	points.push_back(glm::vec3(0.9224, 1.22219, -0.210532));
	points.push_back(glm::vec3(0.9224, 1.22219, 0.210532));
	points.push_back(glm::vec3(1.2894, 0.896006, 0.294298));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.2894, 0.896006, 0.294298));
	points.push_back(glm::vec3(0.9224, 1.22219, 0.210532));
	points.push_back(glm::vec3(0.739708, 1.22219, 0.589897));
	points.push_back(glm::vec3(1.03402, 0.896006, 0.824604));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.03402, 0.896006, 0.824604));
	points.push_back(glm::vec3(0.739708, 1.22219, 0.589897));
	points.push_back(glm::vec3(0.410507, 1.22219, 0.852426));
	points.push_back(glm::vec3(0.573838, 0.896006, 1.19159));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.573838, 0.896006, 1.19159));
	points.push_back(glm::vec3(0.410507, 1.22219, 0.852426));
	points.push_back(glm::vec3(0, 1.22219, 0.946121));
	points.push_back(glm::vec3(0, 0.896006, 1.32256));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 1.22219, 0.946121));
	points.push_back(glm::vec3(-0.410507, 1.22219, 0.852426));
	points.push_back(glm::vec3(-0.573838, 0.896006, 1.19159));
	points.push_back(glm::vec3(0, 0.896006, 1.32256));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.410507, 1.22219, 0.852426));
	points.push_back(glm::vec3(-0.739708, 1.22219, 0.589897));
	points.push_back(glm::vec3(-1.03402, 0.896006, 0.824604));
	points.push_back(glm::vec3(-0.573838, 0.896006, 1.19159));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.739708, 1.22219, 0.589897));
	points.push_back(glm::vec3(-0.9224, 1.22219, 0.210532));
	points.push_back(glm::vec3(-1.2894, 0.896006, 0.294298));
	points.push_back(glm::vec3(-1.03402, 0.896006, 0.824604));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.9224, 1.22219, 0.210532));
	points.push_back(glm::vec3(-0.9224, 1.22219, -0.210532));
	points.push_back(glm::vec3(-1.2894, 0.896006, -0.294298));
	points.push_back(glm::vec3(-1.2894, 0.896006, 0.294298));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.739708, 1.22219, -0.589897));
	points.push_back(glm::vec3(-1.03402, 0.896006, -0.824604));
	points.push_back(glm::vec3(-1.2894, 0.896006, -0.294298));
	points.push_back(glm::vec3(-0.9224, 1.22219, -0.210532));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.410507, 1.22219, -0.852426));
	points.push_back(glm::vec3(-0.573838, 0.896006, -1.19159));
	points.push_back(glm::vec3(-1.03402, 0.896006, -0.824604));
	points.push_back(glm::vec3(-0.739708, 1.22219, -0.589897));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 1.22219, -0.946121));
	points.push_back(glm::vec3(0, 0.896006, -1.32256));
	points.push_back(glm::vec3(-0.573838, 0.896006, -1.19159));
	points.push_back(glm::vec3(-0.410507, 1.22219, -0.852426));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.69068, 0.476976, -1.43421));
	points.push_back(glm::vec3(0, 0.476976, -1.59186));
	points.push_back(glm::vec3(0, 0.896006, -1.32256));
	points.push_back(glm::vec3(0.573838, 0.896006, -1.19159));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.24456, 0.476976, -0.992506));
	points.push_back(glm::vec3(0.69068, 0.476976, -1.43421));
	points.push_back(glm::vec3(0.573838, 0.896006, -1.19159));
	points.push_back(glm::vec3(1.03402, 0.896006, -0.824604));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.55194, 0.476976, -0.354221));
	points.push_back(glm::vec3(1.24456, 0.476976, -0.992506));
	points.push_back(glm::vec3(1.03402, 0.896006, -0.824604));
	points.push_back(glm::vec3(1.2894, 0.896006, -0.294298));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.55194, 0.476976, -0.354221));
	points.push_back(glm::vec3(1.2894, 0.896006, -0.294298));
	points.push_back(glm::vec3(1.2894, 0.896006, 0.294298));
	points.push_back(glm::vec3(1.55194, 0.476976, 0.354221));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.55194, 0.476976, 0.354221));
	points.push_back(glm::vec3(1.2894, 0.896006, 0.294298));
	points.push_back(glm::vec3(1.03402, 0.896006, 0.824604));
	points.push_back(glm::vec3(1.24456, 0.476976, 0.992506));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.24456, 0.476976, 0.992506));
	points.push_back(glm::vec3(1.03402, 0.896006, 0.824604));
	points.push_back(glm::vec3(0.573838, 0.896006, 1.19159));
	points.push_back(glm::vec3(0.69068, 0.476976, 1.43421));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.69068, 0.476976, 1.43421));
	points.push_back(glm::vec3(0.573838, 0.896006, 1.19159));
	points.push_back(glm::vec3(0, 0.896006, 1.32256));
	points.push_back(glm::vec3(0, 0.476976, 1.59186));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 0.896006, 1.32256));
	points.push_back(glm::vec3(-0.573838, 0.896006, 1.19159));
	points.push_back(glm::vec3(-0.69068, 0.476976, 1.43421));
	points.push_back(glm::vec3(0, 0.476976, 1.59186));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.573838, 0.896006, 1.19159));
	points.push_back(glm::vec3(-1.03402, 0.896006, 0.824604));
	points.push_back(glm::vec3(-1.24456, 0.476976, 0.992506));
	points.push_back(glm::vec3(-0.69068, 0.476976, 1.43421));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.03402, 0.896006, 0.824604));
	points.push_back(glm::vec3(-1.2894, 0.896006, 0.294298));
	points.push_back(glm::vec3(-1.55194, 0.476976, 0.354221));
	points.push_back(glm::vec3(-1.24456, 0.476976, 0.992506));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.2894, 0.896006, 0.294298));
	points.push_back(glm::vec3(-1.2894, 0.896006, -0.294298));
	points.push_back(glm::vec3(-1.55194, 0.476976, -0.354221));
	points.push_back(glm::vec3(-1.55194, 0.476976, 0.354221));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.03402, 0.896006, -0.824604));
	points.push_back(glm::vec3(-1.24456, 0.476976, -0.992506));
	points.push_back(glm::vec3(-1.55194, 0.476976, -0.354221));
	points.push_back(glm::vec3(-1.2894, 0.896006, -0.294298));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.573838, 0.896006, -1.19159));
	points.push_back(glm::vec3(-0.69068, 0.476976, -1.43421));
	points.push_back(glm::vec3(-1.24456, 0.476976, -0.992506));
	points.push_back(glm::vec3(-1.03402, 0.896006, -0.824604));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 0.896006, -1.32256));
	points.push_back(glm::vec3(0, 0.476976, -1.59186));
	points.push_back(glm::vec3(-0.69068, 0.476976, -1.43421));
	points.push_back(glm::vec3(-0.573838, 0.896006, -1.19159));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.751568, -0.000949115, -1.56065));
	points.push_back(glm::vec3(0, -0.000949115, -1.73219));
	points.push_back(glm::vec3(0, 0.476976, -1.59186));
	points.push_back(glm::vec3(0.69068, 0.476976, -1.43421));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.35428, -0.000949115, -1.08));
	points.push_back(glm::vec3(0.751568, -0.000949115, -1.56065));
	points.push_back(glm::vec3(0.69068, 0.476976, -1.43421));
	points.push_back(glm::vec3(1.24456, 0.476976, -0.992506));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.68876, -0.000949115, -0.385448));
	points.push_back(glm::vec3(1.35428, -0.000949115, -1.08));
	points.push_back(glm::vec3(1.24456, 0.476976, -0.992506));
	points.push_back(glm::vec3(1.55194, 0.476976, -0.354221));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.68876, -0.000949115, -0.385448));
	points.push_back(glm::vec3(1.55194, 0.476976, -0.354221));
	points.push_back(glm::vec3(1.55194, 0.476976, 0.354221));
	points.push_back(glm::vec3(1.68876, -0.000949115, 0.385448));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.68876, -0.000949115, 0.385448));
	points.push_back(glm::vec3(1.55194, 0.476976, 0.354221));
	points.push_back(glm::vec3(1.24456, 0.476976, 0.992506));
	points.push_back(glm::vec3(1.35428, -0.000949115, 1.08));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.35428, -0.000949115, 1.08));
	points.push_back(glm::vec3(1.24456, 0.476976, 0.992506));
	points.push_back(glm::vec3(0.69068, 0.476976, 1.43421));
	points.push_back(glm::vec3(0.751568, -0.000949115, 1.56065));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.751568, -0.000949115, 1.56065));
	points.push_back(glm::vec3(0.69068, 0.476976, 1.43421));
	points.push_back(glm::vec3(0, 0.476976, 1.59186));
	points.push_back(glm::vec3(0, -0.000949115, 1.73219));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 0.476976, 1.59186));
	points.push_back(glm::vec3(-0.69068, 0.476976, 1.43421));
	points.push_back(glm::vec3(-0.751568, -0.000949115, 1.56065));
	points.push_back(glm::vec3(0, -0.000949115, 1.73219));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.69068, 0.476976, 1.43421));
	points.push_back(glm::vec3(-1.24456, 0.476976, 0.992506));
	points.push_back(glm::vec3(-1.35428, -0.000949115, 1.08));
	points.push_back(glm::vec3(-0.751568, -0.000949115, 1.56065));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.24456, 0.476976, 0.992506));
	points.push_back(glm::vec3(-1.55194, 0.476976, 0.354221));
	points.push_back(glm::vec3(-1.68876, -0.000949115, 0.385448));
	points.push_back(glm::vec3(-1.35428, -0.000949115, 1.08));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.55194, 0.476976, 0.354221));
	points.push_back(glm::vec3(-1.55194, 0.476976, -0.354221));
	points.push_back(glm::vec3(-1.68876, -0.000949115, -0.385448));
	points.push_back(glm::vec3(-1.68876, -0.000949115, 0.385448));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.24456, 0.476976, -0.992506));
	points.push_back(glm::vec3(-1.35428, -0.000949115, -1.08));
	points.push_back(glm::vec3(-1.68876, -0.000949115, -0.385448));
	points.push_back(glm::vec3(-1.55194, 0.476976, -0.354221));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.69068, 0.476976, -1.43421));
	points.push_back(glm::vec3(-0.751568, -0.000949115, -1.56065));
	points.push_back(glm::vec3(-1.35428, -0.000949115, -1.08));
	points.push_back(glm::vec3(-1.24456, 0.476976, -0.992506));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 0.476976, -1.59186));
	points.push_back(glm::vec3(0, -0.000949115, -1.73219));
	points.push_back(glm::vec3(-0.751568, -0.000949115, -1.56065));
	points.push_back(glm::vec3(-0.69068, 0.476976, -1.43421));

	shBuffer.drawPolygon(points);

	_shapes.push_back(shBuffer);
	_shapes.push_back(shBuffer00);
}

std::vector<std::pair<QWidget*, QString>> DfxLxEnvironmentLightWidget::propertyForms() {
	std::pair<QWidget*, QString> props(properties(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> { props };
}

QWidget *DfxLxEnvironmentLightWidget::properties() {

	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	auto generalControlPanel = new DfxControlPanel();

	DfxQControlArray *generalControlArray = new DfxQControlArray();

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = referencePath() + "/";

	// Basic Controls
	auto image_path = new DfxQFilePathControl(path + "image", true, false, "Image File", "c:/", "(*.png *.jpeg *.tiff *.hdr)", "", false);
	auto gamma = new DfxQDecimalSpinnerControl(path + "gamma", "", false);
	auto diffuse = new DfxQCheckBoxControl(path + "diffuse", "", false);
	auto glossy = new DfxQCheckBoxControl(path + "glossy", "", false);
	auto specular = new DfxQCheckBoxControl(path + "specular", "", false);
	auto upper_hemisphere = new DfxQCheckBoxControl(path + "upper_hemisphere", "", false);
	auto gain = new DfxQDecimalSpinnerControl(path + "gain", "", false);

	generalControlArray->addControl("Image", image_path);
	generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("Gamma", gamma);
	generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("Intensity", gain);
	generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("Affect Diffuse", diffuse);
	generalControlArray->addControl("Affect Glossy", glossy);
	generalControlArray->addControl("Affect Specular", specular);
	generalControlArray->addControl("Only Upper Hemisphere", upper_hemisphere);

	generalControlPanel->setTitle("General");
	generalControlPanel->setWidget(generalControlArray);

	main_layout->addWidget(generalControlPanel);

	widget->setLayout(main_layout);

	return widget;
}