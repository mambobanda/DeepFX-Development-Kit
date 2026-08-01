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

#include "DfxOspRayCameraWidget.h"

std::string DfxOSPRayCameraWidget::_uniqueName = "UID_OSPRAY_CAMERA_WIDGET";

DfxOSPRayCameraWidget::DfxOSPRayCameraWidget() {
	deepsg::SgShape::setScale(0.4f);

	initShapes();
	setShapeType(DfxItemWidgetType::CAMERA);
	setShape(this);
}

DfxOSPRayCameraWidget::~DfxOSPRayCameraWidget() {

}

std::string DfxOSPRayCameraWidget::uniqueName() {
	return _uniqueName;
}

std::string DfxOSPRayCameraWidget::getUniqueName() {
	return _uniqueName;
}

void DfxOSPRayCameraWidget::initShapes() {

	std::vector<glm::vec3> points;
	deepsg::SgShapeBuffer shBuffer;
	deepsg::SgShapeBuffer shBuffer00;

	shBuffer.setWireframe(true);
	shBuffer.setColor(255, 200, 100);
	shBuffer00.setColor(255, 200, 100);

	shBuffer00.drawStippledLine(glm::vec3(0.f, 0.0f, -0.5f), glm::vec3(0.f, 0.0f, -5.f));

	points.clear();
	points.push_back(glm::vec3(1.72553, 1.01474, -0.373518));
	points.push_back(glm::vec3(-1.7037, 1.01839, -0.373518));
	points.push_back(glm::vec3(-1.7037, 1.01839, 0.326482));
	points.push_back(glm::vec3(1.72553, 1.01474, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.7037, 1.01839, 0.326482));
	points.push_back(glm::vec3(-1.7037, 1.01839, -0.373518));
	points.push_back(glm::vec3(-1.79981, 0.989254, -0.373518));
	points.push_back(glm::vec3(-1.79981, 0.989254, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.79981, 0.989254, 0.326482));
	points.push_back(glm::vec3(-1.79981, 0.989254, -0.373518));
	points.push_back(glm::vec3(-1.88389, 0.934318, -0.373518));
	points.push_back(glm::vec3(-1.88389, 0.934318, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.88389, 0.934318, 0.326482));
	points.push_back(glm::vec3(-1.88389, 0.934318, -0.373518));
	points.push_back(glm::vec3(-1.95498, 0.863377, -0.373518));
	points.push_back(glm::vec3(-1.95498, 0.863377, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.95498, 0.863377, 0.326482));
	points.push_back(glm::vec3(-1.95498, 0.863377, -0.373518));
	points.push_back(glm::vec3(-2.0101, 0.77942, -0.373518));
	points.push_back(glm::vec3(-2.0101, 0.77942, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-2.0101, 0.77942, 0.326482));
	points.push_back(glm::vec3(-2.0101, 0.77942, -0.373518));
	points.push_back(glm::vec3(-2.03944, 0.68337, -0.373518));
	points.push_back(glm::vec3(-2.03944, 0.68337, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-2.03944, 0.68337, 0.326482));
	points.push_back(glm::vec3(-2.03944, 0.68337, -0.373518));
	points.push_back(glm::vec3(-2.04091, -0.695868, -0.373518));
	points.push_back(glm::vec3(-2.04091, -0.695868, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-2.04091, -0.695868, 0.326482));
	points.push_back(glm::vec3(-2.04091, -0.695868, -0.373518));
	points.push_back(glm::vec3(-2.01177, -0.79198, -0.373518));
	points.push_back(glm::vec3(-2.01177, -0.79198, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-2.01177, -0.79198, 0.326482));
	points.push_back(glm::vec3(-2.01177, -0.79198, -0.373518));
	points.push_back(glm::vec3(-1.95683, -0.876055, -0.373518));
	points.push_back(glm::vec3(-1.95683, -0.876055, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.95683, -0.876055, 0.326482));
	points.push_back(glm::vec3(-1.95683, -0.876055, -0.373518));
	points.push_back(glm::vec3(-1.88589, -0.947147, -0.373518));
	points.push_back(glm::vec3(-1.88589, -0.947147, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.88589, -0.947147, 0.326482));
	points.push_back(glm::vec3(-1.88589, -0.947147, -0.373518));
	points.push_back(glm::vec3(-1.80194, -1.00226, -0.373518));
	points.push_back(glm::vec3(-1.80194, -1.00226, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.80194, -1.00226, 0.326482));
	points.push_back(glm::vec3(-1.80194, -1.00226, -0.373518));
	points.push_back(glm::vec3(-1.70589, -1.03161, -0.373518));
	points.push_back(glm::vec3(-1.70589, -1.03161, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.70589, -1.03161, 0.326482));
	points.push_back(glm::vec3(-1.70589, -1.03161, -0.373518));
	points.push_back(glm::vec3(1.72335, -1.03526, -0.373518));
	points.push_back(glm::vec3(1.72335, -1.03526, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.72335, -1.03526, -0.373518));
	points.push_back(glm::vec3(1.81946, -1.00612, -0.373518));
	points.push_back(glm::vec3(1.81946, -1.00612, 0.326482));
	points.push_back(glm::vec3(1.72335, -1.03526, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.81946, -1.00612, -0.373518));
	points.push_back(glm::vec3(1.90354, -0.951184, -0.373518));
	points.push_back(glm::vec3(1.90354, -0.951184, 0.326482));
	points.push_back(glm::vec3(1.81946, -1.00612, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.90354, -0.951184, -0.373518));
	points.push_back(glm::vec3(1.97463, -0.880244, -0.373518));
	points.push_back(glm::vec3(1.97463, -0.880244, 0.326482));
	points.push_back(glm::vec3(1.90354, -0.951184, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.97463, -0.880244, -0.373518));
	points.push_back(glm::vec3(2.02974, -0.796286, -0.373518));
	points.push_back(glm::vec3(2.02974, -0.796286, 0.326482));
	points.push_back(glm::vec3(1.97463, -0.880244, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(2.02974, -0.796286, -0.373518));
	points.push_back(glm::vec3(2.05909, -0.700236, -0.373518));
	points.push_back(glm::vec3(2.05909, -0.700236, 0.326482));
	points.push_back(glm::vec3(2.02974, -0.796286, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(2.05909, -0.700236, -0.373518));
	points.push_back(glm::vec3(2.06056, 0.679001, -0.373518));
	points.push_back(glm::vec3(2.06056, 0.679001, 0.326482));
	points.push_back(glm::vec3(2.05909, -0.700236, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(2.06056, 0.679001, -0.373518));
	points.push_back(glm::vec3(2.03142, 0.775114, -0.373518));
	points.push_back(glm::vec3(2.03142, 0.775114, 0.326482));
	points.push_back(glm::vec3(2.06056, 0.679001, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(2.03142, 0.775114, -0.373518));
	points.push_back(glm::vec3(1.97648, 0.859188, -0.373518));
	points.push_back(glm::vec3(1.97648, 0.859188, 0.326482));
	points.push_back(glm::vec3(2.03142, 0.775114, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.97648, 0.859188, -0.373518));
	points.push_back(glm::vec3(1.90554, 0.930279, -0.373518));
	points.push_back(glm::vec3(1.90554, 0.930279, 0.326482));
	points.push_back(glm::vec3(1.97648, 0.859188, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.90554, 0.930279, -0.373518));
	points.push_back(glm::vec3(1.82158, 0.985396, -0.373518));
	points.push_back(glm::vec3(1.82158, 0.985396, 0.326482));
	points.push_back(glm::vec3(1.90554, 0.930279, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.82158, 0.985396, -0.373518));
	points.push_back(glm::vec3(1.72553, 1.01474, -0.373518));
	points.push_back(glm::vec3(1.72553, 1.01474, 0.326482));
	points.push_back(glm::vec3(1.82158, 0.985396, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.82158, 0.985396, -0.373518));
	points.push_back(glm::vec3(1.90554, 0.930279, -0.373518));
	points.push_back(glm::vec3(1.97648, 0.859188, -0.373518));
	points.push_back(glm::vec3(2.03142, 0.775114, -0.373518));
	points.push_back(glm::vec3(2.06056, 0.679001, -0.373518));
	points.push_back(glm::vec3(2.05909, -0.700236, -0.373518));
	points.push_back(glm::vec3(2.02974, -0.796286, -0.373518));
	points.push_back(glm::vec3(1.97463, -0.880244, -0.373518));
	points.push_back(glm::vec3(1.90354, -0.951184, -0.373518));
	points.push_back(glm::vec3(1.81946, -1.00612, -0.373518));
	points.push_back(glm::vec3(1.72335, -1.03526, -0.373518));
	points.push_back(glm::vec3(-1.70589, -1.03161, -0.373518));
	points.push_back(glm::vec3(-1.80194, -1.00226, -0.373518));
	points.push_back(glm::vec3(-1.88589, -0.947147, -0.373518));
	points.push_back(glm::vec3(-1.95683, -0.876055, -0.373518));
	points.push_back(glm::vec3(-2.01177, -0.79198, -0.373518));
	points.push_back(glm::vec3(-2.04091, -0.695868, -0.373518));
	points.push_back(glm::vec3(-2.03944, 0.68337, -0.373518));
	points.push_back(glm::vec3(-2.0101, 0.77942, -0.373518));
	points.push_back(glm::vec3(-1.95498, 0.863377, -0.373518));
	points.push_back(glm::vec3(-1.88389, 0.934318, -0.373518));
	points.push_back(glm::vec3(-1.79981, 0.989254, -0.373518));
	points.push_back(glm::vec3(-1.7037, 1.01839, -0.373518));
	points.push_back(glm::vec3(1.72553, 1.01474, -0.373518));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.79981, 0.989254, 0.326482));
	points.push_back(glm::vec3(-1.88389, 0.934318, 0.326482));
	points.push_back(glm::vec3(-1.95498, 0.863377, 0.326482));
	points.push_back(glm::vec3(-2.0101, 0.77942, 0.326482));
	points.push_back(glm::vec3(-2.03944, 0.68337, 0.326482));
	points.push_back(glm::vec3(-2.04091, -0.695868, 0.326482));
	points.push_back(glm::vec3(-2.01177, -0.79198, 0.326482));
	points.push_back(glm::vec3(-1.95683, -0.876055, 0.326482));
	points.push_back(glm::vec3(-1.88589, -0.947147, 0.326482));
	points.push_back(glm::vec3(-1.80194, -1.00226, 0.326482));
	points.push_back(glm::vec3(-1.70589, -1.03161, 0.326482));
	points.push_back(glm::vec3(1.72335, -1.03526, 0.326482));
	points.push_back(glm::vec3(1.81946, -1.00612, 0.326482));
	points.push_back(glm::vec3(1.90354, -0.951184, 0.326482));
	points.push_back(glm::vec3(1.97463, -0.880244, 0.326482));
	points.push_back(glm::vec3(2.02974, -0.796286, 0.326482));
	points.push_back(glm::vec3(2.05909, -0.700236, 0.326482));
	points.push_back(glm::vec3(2.06056, 0.679001, 0.326482));
	points.push_back(glm::vec3(2.03142, 0.775114, 0.326482));
	points.push_back(glm::vec3(1.97648, 0.859188, 0.326482));
	points.push_back(glm::vec3(1.90554, 0.930279, 0.326482));
	points.push_back(glm::vec3(1.82158, 0.985396, 0.326482));
	points.push_back(glm::vec3(1.72553, 1.01474, 0.326482));
	points.push_back(glm::vec3(-1.7037, 1.01839, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.672417, 1.01729, 0.326482));
	points.push_back(glm::vec3(-0.672417, 1.01729, -0.373518));
	points.push_back(glm::vec3(0.694248, 1.01584, -0.373518));
	points.push_back(glm::vec3(0.694248, 1.01584, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.671762, 1.63191, 0.326482));
	points.push_back(glm::vec3(-0.671762, 1.63191, -0.373518));
	points.push_back(glm::vec3(-0.672417, 1.01729, -0.373518));
	points.push_back(glm::vec3(-0.672417, 1.01729, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.642419, 1.72796, 0.326482));
	points.push_back(glm::vec3(-0.642419, 1.72796, -0.373518));
	points.push_back(glm::vec3(-0.671762, 1.63191, -0.373518));
	points.push_back(glm::vec3(-0.671762, 1.63191, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.587302, 1.81192, 0.326482));
	points.push_back(glm::vec3(-0.587302, 1.81192, -0.373518));
	points.push_back(glm::vec3(-0.642419, 1.72796, -0.373518));
	points.push_back(glm::vec3(-0.642419, 1.72796, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.516211, 1.88286, 0.326482));
	points.push_back(glm::vec3(-0.516211, 1.88286, -0.373518));
	points.push_back(glm::vec3(-0.587302, 1.81192, -0.373518));
	points.push_back(glm::vec3(-0.587302, 1.81192, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.432137, 1.9378, 0.326482));
	points.push_back(glm::vec3(-0.432137, 1.9378, -0.373518));
	points.push_back(glm::vec3(-0.516211, 1.88286, -0.373518));
	points.push_back(glm::vec3(-0.516211, 1.88286, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.336024, 1.96694, 0.326482));
	points.push_back(glm::vec3(-0.336024, 1.96694, -0.373518));
	points.push_back(glm::vec3(-0.432137, 1.9378, -0.373518));
	points.push_back(glm::vec3(-0.432137, 1.9378, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.35988, 1.96619, -0.373518));
	points.push_back(glm::vec3(-0.336024, 1.96694, -0.373518));
	points.push_back(glm::vec3(-0.336024, 1.96694, 0.326482));
	points.push_back(glm::vec3(0.35988, 1.96619, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.45593, 1.93685, -0.373518));
	points.push_back(glm::vec3(0.35988, 1.96619, -0.373518));
	points.push_back(glm::vec3(0.35988, 1.96619, 0.326482));
	points.push_back(glm::vec3(0.45593, 1.93685, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.539887, 1.88174, -0.373518));
	points.push_back(glm::vec3(0.45593, 1.93685, -0.373518));
	points.push_back(glm::vec3(0.45593, 1.93685, 0.326482));
	points.push_back(glm::vec3(0.539887, 1.88174, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.610827, 1.81064, -0.373518));
	points.push_back(glm::vec3(0.539887, 1.88174, -0.373518));
	points.push_back(glm::vec3(0.539887, 1.88174, 0.326482));
	points.push_back(glm::vec3(0.610827, 1.81064, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.665765, 1.72657, -0.373518));
	points.push_back(glm::vec3(0.610827, 1.81064, -0.373518));
	points.push_back(glm::vec3(0.610827, 1.81064, 0.326482));
	points.push_back(glm::vec3(0.665765, 1.72657, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.694903, 1.63046, -0.373518));
	points.push_back(glm::vec3(0.665765, 1.72657, -0.373518));
	points.push_back(glm::vec3(0.665765, 1.72657, 0.326482));
	points.push_back(glm::vec3(0.694903, 1.63046, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.694248, 1.01584, -0.373518));
	points.push_back(glm::vec3(0.694903, 1.63046, -0.373518));
	points.push_back(glm::vec3(0.694903, 1.63046, 0.326482));
	points.push_back(glm::vec3(0.694248, 1.01584, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.694248, 1.01584, 0.326482));
	points.push_back(glm::vec3(0.694903, 1.63046, 0.326482));
	points.push_back(glm::vec3(0.665765, 1.72657, 0.326482));
	points.push_back(glm::vec3(0.610827, 1.81064, 0.326482));
	points.push_back(glm::vec3(0.539887, 1.88174, 0.326482));
	points.push_back(glm::vec3(0.45593, 1.93685, 0.326482));
	points.push_back(glm::vec3(0.35988, 1.96619, 0.326482));
	points.push_back(glm::vec3(-0.336024, 1.96694, 0.326482));
	points.push_back(glm::vec3(-0.432137, 1.9378, 0.326482));
	points.push_back(glm::vec3(-0.516211, 1.88286, 0.326482));
	points.push_back(glm::vec3(-0.587302, 1.81192, 0.326482));
	points.push_back(glm::vec3(-0.642419, 1.72796, 0.326482));
	points.push_back(glm::vec3(-0.671762, 1.63191, 0.326482));
	points.push_back(glm::vec3(-0.672417, 1.01729, 0.326482));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.672417, 1.01729, -0.373518));
	points.push_back(glm::vec3(-0.671762, 1.63191, -0.373518));
	points.push_back(glm::vec3(-0.642419, 1.72796, -0.373518));
	points.push_back(glm::vec3(-0.587302, 1.81192, -0.373518));
	points.push_back(glm::vec3(-0.516211, 1.88286, -0.373518));
	points.push_back(glm::vec3(-0.432137, 1.9378, -0.373518));
	points.push_back(glm::vec3(-0.336024, 1.96694, -0.373518));
	points.push_back(glm::vec3(0.35988, 1.96619, -0.373518));
	points.push_back(glm::vec3(0.45593, 1.93685, -0.373518));
	points.push_back(glm::vec3(0.539887, 1.88174, -0.373518));
	points.push_back(glm::vec3(0.610827, 1.81064, -0.373518));
	points.push_back(glm::vec3(0.665765, 1.72657, -0.373518));
	points.push_back(glm::vec3(0.694903, 1.63046, -0.373518));
	points.push_back(glm::vec3(0.694248, 1.01584, -0.373518));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.00252755, 0.717686, -0.379809));
	points.push_back(glm::vec3(-0.00252755, 0.717686, -1.7552));
	points.push_back(glm::vec3(-0.136704, 0.704616, -1.7552));
	points.push_back(glm::vec3(-0.136704, 0.704616, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.136704, 0.704616, -0.379809));
	points.push_back(glm::vec3(-0.136704, 0.704616, -1.7552));
	points.push_back(glm::vec3(-0.265753, 0.665619, -1.7552));
	points.push_back(glm::vec3(-0.265753, 0.665619, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.265753, 0.665619, -0.379809));
	points.push_back(glm::vec3(-0.265753, 0.665619, -1.7552));
	points.push_back(glm::vec3(-0.384713, 0.602196, -1.7552));
	points.push_back(glm::vec3(-0.384713, 0.602196, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.384713, 0.602196, -0.379809));
	points.push_back(glm::vec3(-0.384713, 0.602196, -1.7552));
	points.push_back(glm::vec3(-0.489014, 0.516784, -1.7552));
	points.push_back(glm::vec3(-0.489014, 0.516784, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.489014, 0.516784, -0.379809));
	points.push_back(glm::vec3(-0.489014, 0.516784, -1.7552));
	points.push_back(glm::vec3(-0.574649, 0.412664, -1.7552));
	points.push_back(glm::vec3(-0.574649, 0.412664, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.574649, 0.412664, -0.379809));
	points.push_back(glm::vec3(-0.574649, 0.412664, -1.7552));
	points.push_back(glm::vec3(-0.638326, 0.293839, -1.7552));
	points.push_back(glm::vec3(-0.638326, 0.293839, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.638326, 0.293839, -0.379809));
	points.push_back(glm::vec3(-0.638326, 0.293839, -1.7552));
	points.push_back(glm::vec3(-0.677597, 0.164874, -1.7552));
	points.push_back(glm::vec3(-0.677597, 0.164874, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.677597, 0.164874, -0.379809));
	points.push_back(glm::vec3(-0.677597, 0.164874, -1.7552));
	points.push_back(glm::vec3(-0.690953, 0.0307261, -1.7552));
	points.push_back(glm::vec3(-0.690953, 0.0307261, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.690953, 0.0307261, -0.379809));
	points.push_back(glm::vec3(-0.690953, 0.0307261, -1.7552));
	points.push_back(glm::vec3(-0.677883, -0.10345, -1.7552));
	points.push_back(glm::vec3(-0.677883, -0.10345, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.677883, -0.10345, -0.379809));
	points.push_back(glm::vec3(-0.677883, -0.10345, -1.7552));
	points.push_back(glm::vec3(-0.638886, -0.232498, -1.7552));
	points.push_back(glm::vec3(-0.638886, -0.232498, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.638886, -0.232498, -0.379809));
	points.push_back(glm::vec3(-0.638886, -0.232498, -1.7552));
	points.push_back(glm::vec3(-0.575464, -0.351459, -1.7552));
	points.push_back(glm::vec3(-0.575464, -0.351459, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.575464, -0.351459, -0.379809));
	points.push_back(glm::vec3(-0.575464, -0.351459, -1.7552));
	points.push_back(glm::vec3(-0.490051, -0.455761, -1.7552));
	points.push_back(glm::vec3(-0.490051, -0.455761, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.490051, -0.455761, -0.379809));
	points.push_back(glm::vec3(-0.490051, -0.455761, -1.7552));
	points.push_back(glm::vec3(-0.385932, -0.541395, -1.7552));
	points.push_back(glm::vec3(-0.385932, -0.541395, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.385932, -0.541395, -0.379809));
	points.push_back(glm::vec3(-0.385932, -0.541395, -1.7552));
	points.push_back(glm::vec3(-0.267106, -0.605072, -1.7552));
	points.push_back(glm::vec3(-0.267106, -0.605072, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.267106, -0.605072, -0.379809));
	points.push_back(glm::vec3(-0.267106, -0.605072, -1.7552));
	points.push_back(glm::vec3(-0.138141, -0.644343, -1.7552));
	points.push_back(glm::vec3(-0.138141, -0.644343, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.138141, -0.644343, -0.379809));
	points.push_back(glm::vec3(-0.138141, -0.644343, -1.7552));
	points.push_back(glm::vec3(-0.00399317, -0.6577, -1.7552));
	points.push_back(glm::vec3(-0.00399317, -0.6577, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.00399317, -0.6577, -0.379809));
	points.push_back(glm::vec3(-0.00399317, -0.6577, -1.7552));
	points.push_back(glm::vec3(0.130183, -0.644629, -1.7552));
	points.push_back(glm::vec3(0.130183, -0.644629, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.130183, -0.644629, -0.379809));
	points.push_back(glm::vec3(0.130183, -0.644629, -1.7552));
	points.push_back(glm::vec3(0.259231, -0.605633, -1.7552));
	points.push_back(glm::vec3(0.259231, -0.605633, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.259231, -0.605633, -0.379809));
	points.push_back(glm::vec3(0.259231, -0.605633, -1.7552));
	points.push_back(glm::vec3(0.378192, -0.542209, -1.7552));
	points.push_back(glm::vec3(0.378192, -0.542209, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.378192, -0.542209, -0.379809));
	points.push_back(glm::vec3(0.378192, -0.542209, -1.7552));
	points.push_back(glm::vec3(0.482494, -0.456797, -1.7552));
	points.push_back(glm::vec3(0.482494, -0.456797, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.482494, -0.456797, -0.379809));
	points.push_back(glm::vec3(0.482494, -0.456797, -1.7552));
	points.push_back(glm::vec3(0.568128, -0.352677, -1.7552));
	points.push_back(glm::vec3(0.568128, -0.352677, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.568128, -0.352677, -0.379809));
	points.push_back(glm::vec3(0.568128, -0.352677, -1.7552));
	points.push_back(glm::vec3(0.631805, -0.233853, -1.7552));
	points.push_back(glm::vec3(0.631805, -0.233853, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.631805, -0.233853, -0.379809));
	points.push_back(glm::vec3(0.631805, -0.233853, -1.7552));
	points.push_back(glm::vec3(0.671075, -0.104887, -1.7552));
	points.push_back(glm::vec3(0.671075, -0.104887, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.671075, -0.104887, -0.379809));
	points.push_back(glm::vec3(0.671075, -0.104887, -1.7552));
	points.push_back(glm::vec3(0.684433, 0.0292615, -1.7552));
	points.push_back(glm::vec3(0.684433, 0.0292615, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.684433, 0.0292615, -0.379809));
	points.push_back(glm::vec3(0.684433, 0.0292615, -1.7552));
	points.push_back(glm::vec3(0.671361, 0.163437, -1.7552));
	points.push_back(glm::vec3(0.671361, 0.163437, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.671361, 0.163437, -0.379809));
	points.push_back(glm::vec3(0.671361, 0.163437, -1.7552));
	points.push_back(glm::vec3(0.632364, 0.292486, -1.7552));
	points.push_back(glm::vec3(0.632364, 0.292486, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.632364, 0.292486, -0.379809));
	points.push_back(glm::vec3(0.632364, 0.292486, -1.7552));
	points.push_back(glm::vec3(0.568942, 0.411447, -1.7552));
	points.push_back(glm::vec3(0.568942, 0.411447, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.568942, 0.411447, -0.379809));
	points.push_back(glm::vec3(0.568942, 0.411447, -1.7552));
	points.push_back(glm::vec3(0.483529, 0.515748, -1.7552));
	points.push_back(glm::vec3(0.483529, 0.515748, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.483529, 0.515748, -0.379809));
	points.push_back(glm::vec3(0.483529, 0.515748, -1.7552));
	points.push_back(glm::vec3(0.37941, 0.601383, -1.7552));
	points.push_back(glm::vec3(0.37941, 0.601383, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.37941, 0.601383, -0.379809));
	points.push_back(glm::vec3(0.37941, 0.601383, -1.7552));
	points.push_back(glm::vec3(0.260584, 0.665059, -1.7552));
	points.push_back(glm::vec3(0.260584, 0.665059, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.260584, 0.665059, -0.379809));
	points.push_back(glm::vec3(0.260584, 0.665059, -1.7552));
	points.push_back(glm::vec3(0.131619, 0.70433, -1.7552));
	points.push_back(glm::vec3(0.131619, 0.70433, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.131619, 0.70433, -0.379809));
	points.push_back(glm::vec3(0.131619, 0.70433, -1.7552));
	points.push_back(glm::vec3(-0.00252755, 0.717686, -1.7552));
	points.push_back(glm::vec3(-0.00252755, 0.717686, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.131619, 0.70433, -0.379809));
	points.push_back(glm::vec3(-0.00252755, 0.717686, -0.379809));
	points.push_back(glm::vec3(-0.136704, 0.704616, -0.379809));
	points.push_back(glm::vec3(-0.265753, 0.665619, -0.379809));
	points.push_back(glm::vec3(-0.384713, 0.602196, -0.379809));
	points.push_back(glm::vec3(-0.489014, 0.516784, -0.379809));
	points.push_back(glm::vec3(-0.574649, 0.412664, -0.379809));
	points.push_back(glm::vec3(-0.638326, 0.293839, -0.379809));
	points.push_back(glm::vec3(-0.677597, 0.164874, -0.379809));
	points.push_back(glm::vec3(-0.690953, 0.0307261, -0.379809));
	points.push_back(glm::vec3(-0.677883, -0.10345, -0.379809));
	points.push_back(glm::vec3(-0.638886, -0.232498, -0.379809));
	points.push_back(glm::vec3(-0.575464, -0.351459, -0.379809));
	points.push_back(glm::vec3(-0.490051, -0.455761, -0.379809));
	points.push_back(glm::vec3(-0.385932, -0.541395, -0.379809));
	points.push_back(glm::vec3(-0.267106, -0.605072, -0.379809));
	points.push_back(glm::vec3(-0.138141, -0.644343, -0.379809));
	points.push_back(glm::vec3(-0.00399317, -0.6577, -0.379809));
	points.push_back(glm::vec3(0.130183, -0.644629, -0.379809));
	points.push_back(glm::vec3(0.259231, -0.605633, -0.379809));
	points.push_back(glm::vec3(0.378192, -0.542209, -0.379809));
	points.push_back(glm::vec3(0.482494, -0.456797, -0.379809));
	points.push_back(glm::vec3(0.568128, -0.352677, -0.379809));
	points.push_back(glm::vec3(0.631805, -0.233853, -0.379809));
	points.push_back(glm::vec3(0.671075, -0.104887, -0.379809));
	points.push_back(glm::vec3(0.684433, 0.0292615, -0.379809));
	points.push_back(glm::vec3(0.671361, 0.163437, -0.379809));
	points.push_back(glm::vec3(0.632364, 0.292486, -0.379809));
	points.push_back(glm::vec3(0.568942, 0.411447, -0.379809));
	points.push_back(glm::vec3(0.483529, 0.515748, -0.379809));
	points.push_back(glm::vec3(0.37941, 0.601383, -0.379809));
	points.push_back(glm::vec3(0.260584, 0.665059, -0.379809));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.260584, 0.665059, -1.7552));
	points.push_back(glm::vec3(0.37941, 0.601383, -1.7552));
	points.push_back(glm::vec3(0.483529, 0.515748, -1.7552));
	points.push_back(glm::vec3(0.568942, 0.411447, -1.7552));
	points.push_back(glm::vec3(0.632364, 0.292486, -1.7552));
	points.push_back(glm::vec3(0.671361, 0.163437, -1.7552));
	points.push_back(glm::vec3(0.684433, 0.0292615, -1.7552));
	points.push_back(glm::vec3(0.671075, -0.104887, -1.7552));
	points.push_back(glm::vec3(0.631805, -0.233853, -1.7552));
	points.push_back(glm::vec3(0.568128, -0.352677, -1.7552));
	points.push_back(glm::vec3(0.482494, -0.456797, -1.7552));
	points.push_back(glm::vec3(0.378192, -0.542209, -1.7552));
	points.push_back(glm::vec3(0.259231, -0.605633, -1.7552));
	points.push_back(glm::vec3(0.130183, -0.644629, -1.7552));
	points.push_back(glm::vec3(-0.00399317, -0.6577, -1.7552));
	points.push_back(glm::vec3(-0.138141, -0.644343, -1.7552));
	points.push_back(glm::vec3(-0.267106, -0.605072, -1.7552));
	points.push_back(glm::vec3(-0.385932, -0.541395, -1.7552));
	points.push_back(glm::vec3(-0.490051, -0.455761, -1.7552));
	points.push_back(glm::vec3(-0.575464, -0.351459, -1.7552));
	points.push_back(glm::vec3(-0.638886, -0.232498, -1.7552));
	points.push_back(glm::vec3(-0.677883, -0.10345, -1.7552));
	points.push_back(glm::vec3(-0.690953, 0.0307261, -1.7552));
	points.push_back(glm::vec3(-0.677597, 0.164874, -1.7552));
	points.push_back(glm::vec3(-0.638326, 0.293839, -1.7552));
	points.push_back(glm::vec3(-0.574649, 0.412664, -1.7552));
	points.push_back(glm::vec3(-0.489014, 0.516784, -1.7552));
	points.push_back(glm::vec3(-0.384713, 0.602196, -1.7552));
	points.push_back(glm::vec3(-0.265753, 0.665619, -1.7552));
	points.push_back(glm::vec3(-0.136704, 0.704616, -1.7552));
	points.push_back(glm::vec3(-0.00252755, 0.717686, -1.7552));
	points.push_back(glm::vec3(0.131619, 0.70433, -1.7552));

	shBuffer.drawPolygon(points);

	_shapes.push_back(shBuffer);
	_shapes.push_back(shBuffer00);
}

std::vector<std::pair<QWidget*, QString>> DfxOSPRayCameraWidget::propertyForms() {
	std::pair<QWidget*, QString> props(properties(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> { props };
}

QWidget *DfxOSPRayCameraWidget::properties() {

	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto resolutionControlPanel = new DfxControlPanel();
	auto lensControlPanel = new DfxControlPanel();
	auto dofControlPanel = new DfxControlPanel();

	DfxQControlArray *resolutionControlArray = new DfxQControlArray();
	DfxQControlArray *lensControlArray = new DfxQControlArray();
	DfxQControlArray *dofControlArray = new DfxQControlArray();

	DfxQComboBoxControl *resolutionPreset = new DfxQComboBoxControl("");
	resolutionPreset->addItem(0, "480 VGA(640 x 480)");
	resolutionPreset->addItem(1, "600 SVGA(800 x 600)");
	resolutionPreset->addItem(2, "768 XGA(1024 x 768)");
	resolutionPreset->addItem(3, "720 HD(1280 x 720)");
	resolutionPreset->addItem(4, "1080 Full HD(1920 x 1080)");
	resolutionPreset->addItem(5, "1200 WUXGA(1920 x 1200)");
	resolutionPreset->addItem(6, "1440 2K(2560 x 1440)");
	resolutionPreset->addItem(7, "2160 UHD(3840 x 2160)");
	resolutionPreset->addItem(8, "2160 4k(4096 x 2160)");
	resolutionPreset->addItem(9, "2880 5K(5120 x 2880)");
	resolutionPreset->addItem(10, "4320 8K UHD(7680 x 4320)");

	std::string path = referencePath() + "/";

	auto width = new DfxQIntegerSpinnerControl(path + "width", "Width", false);
	auto height = new DfxQIntegerSpinnerControl(path + "height", "Height", false);

	auto fov = new DfxQDecimalSpinnerControl(path + "field_of_view", "", false);

	auto aperture_radius = new DfxQDecimalSpinnerControl(path + "aperture_radius", "", false);
	auto architectural = new DfxQCheckBoxControl(path + "architectural", "", false);

	auto use_dof = new DfxQCheckBoxControl(path + "use_dof", "", false);
	auto focal_distance = new DfxQDecimalSpinnerControl(path + "focal_distance", "", false);
	auto dof_helper = new DfxQFluentPathControl(path + "dof_helper", "", false);

	resolutionControlArray->addControl("", resolutionPreset);
	resolutionControlArray->addSpacer(0, 10);
	resolutionControlArray->addControl("Width", width);
	resolutionControlArray->addControl("Height", height);

	lensControlArray->addControl("Lens Size", aperture_radius);
	lensControlArray->addControl("Field Of View", fov);
	resolutionControlArray->addSpacer(0, 10);
	lensControlArray->addControl("Architectural", architectural);

	/*lensControlArray->addControl("Lens Radius", lens_radius);
	lensControlArray->addControl("Focal Distance", focal_distance);
	lensControlArray->addControl("Shutter Open", shutter_open);
	lensControlArray->addControl("Shutter Close", shutter_close);*/

	dofControlArray->addControl("Enable Dof", use_dof);
	dofControlArray->addControl("Focal Distance", focal_distance);
	dofControlArray->addControl("Helper", dof_helper);

	resolutionControlPanel->setTitle("Resolution");
	resolutionControlPanel->setWidget(resolutionControlArray);

	lensControlPanel->setTitle("Lens");
	lensControlPanel->setWidget(lensControlArray);

	dofControlPanel->setTitle("Depth Of Field");
	dofControlPanel->setWidget(dofControlArray);

	main_layout->addWidget(resolutionControlPanel);
	main_layout->addWidget(lensControlPanel);
	main_layout->addWidget(dofControlPanel);

	widget->setLayout(main_layout);

	QObject::connect(resolutionPreset->comboBox(), qOverload<int>(&QComboBox::currentIndexChanged), [resolutionPreset, width, height](int value) {
		switch (value)
		{
		case 0:
			width->setValue(640); height->setValue(480);
			break;
		case 1:
			width->setValue(800); height->setValue(600);
			break;
		case 2:
			width->setValue(1024); height->setValue(768);
			break;
		case 3:
			width->setValue(1280); height->setValue(720);
			break;
		case 4:
			width->setValue(1920); height->setValue(1080);
			break;
		case 5:
			width->setValue(1920); height->setValue(1200);
			break;
		case 6:
			width->setValue(2560); height->setValue(1440);
			break;
		case 7:
			width->setValue(3840); height->setValue(2160);
			break;
		case 8:
			width->setValue(4096); height->setValue(2160);
			break;
		case 9:
			width->setValue(5120); height->setValue(2880);
			break;
		case 10:
			width->setValue(7680); height->setValue(4320);
			break;
		default:
			break;
		}
	});

	return widget;

}