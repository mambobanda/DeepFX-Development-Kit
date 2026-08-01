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

#include "DfxLxPointLightWidget.h"

std::string DfxLxPointLightWidget::_uniqueName = "UID_LUX_POINT_LIGHT_WIDGET";

DfxLxPointLightWidget::DfxLxPointLightWidget() {

	deepsg::SgShape::setScale(0.1f);

	initShapes();
	setShapeType(DfxItemWidgetType::POINT_LIGHT);
	setShape(this);
}

DfxLxPointLightWidget::~DfxLxPointLightWidget() {
}

std::string DfxLxPointLightWidget::uniqueName() {
	return _uniqueName;
}

std::string DfxLxPointLightWidget::getUniqueName() {
	return _uniqueName;
}

void DfxLxPointLightWidget::initShapes() {

	std::vector<glm::vec3> points;
	deepsg::SgShapeBuffer shBuffer;
	deepsg::SgShapeBuffer shBuffer00;

	shBuffer.setWireframe(true);
	shBuffer.setColor(255, 200, 100);
	shBuffer00.setColor(255, 200, 100);

	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(10.f, 0.f, 0.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(-10.f, 0.f, 0.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(0.f, 10.f, 0.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(0.f, -10.f, 0.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(0.f, 0.f, 10.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(0.f, 0.f, -10.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(10.f, 10.f, 10.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(10.f, 10.f, -10.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(-10.f, 10.f, 10.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(-10.f, 10.f, -10.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(10.f, -10.f, 10.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(10.f, -10.f, -10.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(-10.f, -10.f, 10.f));
	shBuffer00.drawStippledLine(glm::vec3(0.f), glm::vec3(-10.f, -10.f, -10.f));

	points.clear();
	points.push_back(glm::vec3(0.7, -0.989949, -0.7));
	points.push_back(glm::vec3(0.989949, -0.989949, -0));
	points.push_back(glm::vec3(0, -1.4, -0));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, -0.989949, -0.989949));
	points.push_back(glm::vec3(0.7, -0.989949, -0.7));
	points.push_back(glm::vec3(0, -1.4, -0));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.7, -0.989949, -0.7));
	points.push_back(glm::vec3(0, -0.989949, -0.989949));
	points.push_back(glm::vec3(0, -1.4, -0));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.989949, -0.989949, -0));
	points.push_back(glm::vec3(-0.7, -0.989949, -0.7));
	points.push_back(glm::vec3(0, -1.4, -0));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.7, -0.989949, 0.7));
	points.push_back(glm::vec3(-0.989949, -0.989949, -0));
	points.push_back(glm::vec3(0, -1.4, -0));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, -0.989949, 0.989949));
	points.push_back(glm::vec3(-0.7, -0.989949, 0.7));
	points.push_back(glm::vec3(0, -1.4, -0));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.7, -0.989949, 0.7));
	points.push_back(glm::vec3(0, -0.989949, 0.989949));
	points.push_back(glm::vec3(0, -1.4, -0));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.989949, -0.989949, -0));
	points.push_back(glm::vec3(0.7, -0.989949, 0.7));
	points.push_back(glm::vec3(0, -1.4, -0));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.7, -0.989949, -0.7));
	points.push_back(glm::vec3(0.989949, 0, -0.989949));
	points.push_back(glm::vec3(1.4, 0, -0));
	points.push_back(glm::vec3(0.989949, -0.989949, -0));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, -0.989949, -0.989949));
	points.push_back(glm::vec3(0, 0, -1.4));
	points.push_back(glm::vec3(0.989949, 0, -0.989949));
	points.push_back(glm::vec3(0.7, -0.989949, -0.7));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.989949, 0, -0.989949));
	points.push_back(glm::vec3(0, 0, -1.4));
	points.push_back(glm::vec3(0, -0.989949, -0.989949));
	points.push_back(glm::vec3(-0.7, -0.989949, -0.7));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-1.4, 0, -0));
	points.push_back(glm::vec3(-0.989949, 0, -0.989949));
	points.push_back(glm::vec3(-0.7, -0.989949, -0.7));
	points.push_back(glm::vec3(-0.989949, -0.989949, -0));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.989949, 0, 0.989949));
	points.push_back(glm::vec3(-1.4, 0, -0));
	points.push_back(glm::vec3(-0.989949, -0.989949, -0));
	points.push_back(glm::vec3(-0.7, -0.989949, 0.7));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 0, 1.4));
	points.push_back(glm::vec3(-0.989949, 0, 0.989949));
	points.push_back(glm::vec3(-0.7, -0.989949, 0.7));
	points.push_back(glm::vec3(0, -0.989949, 0.989949));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.7, -0.989949, 0.7));
	points.push_back(glm::vec3(0.989949, 0, 0.989949));
	points.push_back(glm::vec3(0, 0, 1.4));
	points.push_back(glm::vec3(0, -0.989949, 0.989949));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.989949, -0.989949, -0));
	points.push_back(glm::vec3(1.4, 0, -0));
	points.push_back(glm::vec3(0.989949, 0, 0.989949));
	points.push_back(glm::vec3(0.7, -0.989949, 0.7));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.989949, 0, -0.989949));
	points.push_back(glm::vec3(0.7, 0.989949, -0.7));
	points.push_back(glm::vec3(0.989949, 0.989949, -0));
	points.push_back(glm::vec3(1.4, 0, -0));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 0, -1.4));
	points.push_back(glm::vec3(0, 0.989949, -0.989949));
	points.push_back(glm::vec3(0.7, 0.989949, -0.7));
	points.push_back(glm::vec3(0.989949, 0, -0.989949));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.7, 0.989949, -0.7));
	points.push_back(glm::vec3(0, 0.989949, -0.989949));
	points.push_back(glm::vec3(0, 0, -1.4));
	points.push_back(glm::vec3(-0.989949, 0, -0.989949));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.989949, 0.989949, -0));
	points.push_back(glm::vec3(-0.7, 0.989949, -0.7));
	points.push_back(glm::vec3(-0.989949, 0, -0.989949));
	points.push_back(glm::vec3(-1.4, 0, -0));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(-0.7, 0.989949, 0.7));
	points.push_back(glm::vec3(-0.989949, 0.989949, -0));
	points.push_back(glm::vec3(-1.4, 0, -0));
	points.push_back(glm::vec3(-0.989949, 0, 0.989949));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 0.989949, 0.989949));
	points.push_back(glm::vec3(-0.7, 0.989949, 0.7));
	points.push_back(glm::vec3(-0.989949, 0, 0.989949));
	points.push_back(glm::vec3(0, 0, 1.4));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.989949, 0, 0.989949));
	points.push_back(glm::vec3(0.7, 0.989949, 0.7));
	points.push_back(glm::vec3(0, 0.989949, 0.989949));
	points.push_back(glm::vec3(0, 0, 1.4));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(1.4, 0, -0));
	points.push_back(glm::vec3(0.989949, 0.989949, -0));
	points.push_back(glm::vec3(0.7, 0.989949, 0.7));
	points.push_back(glm::vec3(0.989949, 0, 0.989949));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.7, 0.989949, -0.7));
	points.push_back(glm::vec3(0, 1.4, -0));
	points.push_back(glm::vec3(0.989949, 0.989949, -0));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 0.989949, -0.989949));
	points.push_back(glm::vec3(0, 1.4, -0));
	points.push_back(glm::vec3(0.7, 0.989949, -0.7));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 1.4, -0));
	points.push_back(glm::vec3(0, 0.989949, -0.989949));
	points.push_back(glm::vec3(-0.7, 0.989949, -0.7));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 1.4, -0));
	points.push_back(glm::vec3(-0.7, 0.989949, -0.7));
	points.push_back(glm::vec3(-0.989949, 0.989949, -0));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 1.4, -0));
	points.push_back(glm::vec3(-0.989949, 0.989949, -0));
	points.push_back(glm::vec3(-0.7, 0.989949, 0.7));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0, 1.4, -0));
	points.push_back(glm::vec3(-0.7, 0.989949, 0.7));
	points.push_back(glm::vec3(0, 0.989949, 0.989949));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.7, 0.989949, 0.7));
	points.push_back(glm::vec3(0, 1.4, -0));
	points.push_back(glm::vec3(0, 0.989949, 0.989949));

	shBuffer.drawPolygon(points);

	points.clear();
	points.push_back(glm::vec3(0.989949, 0.989949, -0));
	points.push_back(glm::vec3(0, 1.4, -0));
	points.push_back(glm::vec3(0.7, 0.989949, 0.7));

	shBuffer.drawPolygon(points);

	_shapes.push_back(shBuffer);
	_shapes.push_back(shBuffer00);
}

std::vector<std::pair<QWidget*, QString>> DfxLxPointLightWidget::propertyForms() {
	std::pair<QWidget*, QString> props(properties(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> { props };
}

QWidget *DfxLxPointLightWidget::properties() {
	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	auto basicControlPanel = new DfxControlPanel();

	DfxQControlArray *lensControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	auto color = new DfxQColorControl(path + "color_r", path + "color_g", path + "color_b", "", false);
	auto power = new DfxQDecimalSpinnerControl(path + "power", "", false);
	auto efficacy = new DfxQDecimalSpinnerControl(path + "efficacy", "", false);
	auto gain = new DfxQDecimalSpinnerControl(path + "gain", "", false);

	DfxQControlArray *basicControlArray = new DfxQControlArray();

	basicControlArray->addControl("Color", color);
	basicControlArray->addSpacer(0, 5);
	basicControlArray->addControl("Intensity", gain);
	basicControlArray->addSpacer(0, 5);
	basicControlArray->addControl("Power", power);
	basicControlArray->addControl("Efficacy", efficacy);

	basicControlPanel->setTitle("General");
	basicControlPanel->setWidget(basicControlArray);

	main_layout->addWidget(basicControlPanel);

	widget->setLayout(main_layout);

	return widget;
}