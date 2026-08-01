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

#include <RtxCommon.h>
#include "DfxRiptideStageNode.h"
#include "DfxRiptideActorNode.h"
#include "RtxSPHSystem.h"
#include "DfxRiptideHeaterPropNode.h"
#include "DfxRiptideStageNode.h"

std::string DfxRiptideHeaterPropNode::_uniqueName = "UID_RIPTIDE_HEATER_PROP_NODE";

DfxRiptideHeaterPropNode::DfxRiptideHeaterPropNode(deepsg::SgStageNode *stage) : DfxSimulationStagePropBase(stage, riptide::type::HEATER) {
	radiusAny = nullptr;
	_riptideHeaterID = -1;
}

DfxRiptideHeaterPropNode::~DfxRiptideHeaterPropNode() {

}

std::string DfxRiptideHeaterPropNode::uniqueName() {
	return _uniqueName;
}

std::string DfxRiptideHeaterPropNode::getUniqueName() {
	return _uniqueName;
}

size_t DfxRiptideHeaterPropNode::propState() {
	return _propState;
}

void DfxRiptideHeaterPropNode::setPropState(size_t ps) {
	_propState = ps;
}

int DfxRiptideHeaterPropNode::riptideHeaterID() {
	return _riptideHeaterID;
}

void DfxRiptideHeaterPropNode::riptideHeaterID(int index) {
	_riptideHeaterID = index;
}

deepsg::SgShape *DfxRiptideHeaterPropNode::shape() {

	float radius = 0.5;

	if (_shape != nullptr) {
		delete _shape;
		_shape = nullptr;
	}

	_shape = new deepsg::SgShape;
	_shape->setScale(1.f);

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {

		if (any->type() == fl::type::DATA_OBJECT) {
			auto dao = any->rawData<fl::DataObject>();

			auto radius = std::max(0.0001f, dao->property("radius")->data<float>());

			return drawHeater(radius);
		}

	}

	return nullptr;
}

void DfxRiptideHeaterPropNode::spawn() {

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	auto riptideStage = (DfxRiptideStageNode*)stage();
	auto pradius = riptideStage->particleRadius();
	auto _h = pradius * 4.f;

	if (any != nullptr) {

		auto dao = any->rawData<fl::DataObject>();

		_riptideHeaterID = riptideStage->sphSystem()->newHeater();
	}
}

deepsg::SgShape *DfxRiptideHeaterPropNode::drawHeater(float radius) {

	deepsg::SgShapeBuffer shBuffer;
	shBuffer.setColor(153, 255, 192);

	double x_circ[65];
	double y_circ[65];

	float custom_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	/*access->setColor(access->dispData, custom_color);*/

	for (int i = 0; i <= 64; i++) {
		double angle = 2 * RTX_PI * i / 64.0;
		double x = cos(angle)*radius;
		double y = sin(angle)*radius;

		x_circ[i] = x;
		y_circ[i] = y;
	}

	for (int i = 0; i < 64; i++) {

		double pb1[] = { x_circ[i],   0.0, y_circ[i] };
		double pb2[] = { x_circ[i + 1], 0.0, y_circ[i + 1] };

		shBuffer.drawStippledLine(glm::vec3(pb1[0], pb1[1], pb1[2]), glm::vec3(pb2[0], pb2[1], pb2[2]));
		shBuffer.drawLine(glm::vec3(pb1[0], pb1[1], pb1[2]) * 1.05f, glm::vec3(pb2[0], pb2[1], pb2[2]) * 1.05f);
	}

	for (int i = 0; i < 64; i++) {

		double pb1[] = { 0.0, y_circ[i], x_circ[i] };
		double pb2[] = { 0.0, y_circ[i + 1], x_circ[i + 1] };

		shBuffer.drawStippledLine(glm::vec3(pb1[0], pb1[1], pb1[2]), glm::vec3(pb2[0], pb2[1], pb2[2]));
		shBuffer.drawLine(glm::vec3(pb1[0], pb1[1], pb1[2]) * 1.05f, glm::vec3(pb2[0], pb2[1], pb2[2]) * 1.05f);
	}

	for (int i = 0; i < 64; i++) {

		double pb1[] = { x_circ[i], y_circ[i], 0.0 };
		double pb2[] = { x_circ[i + 1], y_circ[i + 1], 0.0 };

		shBuffer.drawStippledLine(glm::vec3(pb1[0], pb1[1], pb1[2]), glm::vec3(pb2[0], pb2[1], pb2[2]));
		shBuffer.drawLine(glm::vec3(pb1[0], pb1[1], pb1[2]) * 1.05f, glm::vec3(pb2[0], pb2[1], pb2[2]) * 1.05f);
	}

	_shape->shapes().push_back(shBuffer);

	return _shape;
}

void DfxRiptideHeaterPropNode::updateHeaterState() {
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());
	auto riptideStage = (DfxRiptideStageNode*)stage();

	if (any != nullptr) {
		if (any->type() == fl::type::DATA_OBJECT) {

			auto dao = any->rawData<fl::DataObject>();

			auto radius = std::max(0.0001f, dao->property("radius")->data<float>());

			riptideStage->sphSystem()->heaters()[_riptideHeaterID].setPosition(position());
			riptideStage->sphSystem()->heaters()[_riptideHeaterID].setRadius(radius);
		}
	}
}

QWidget *DfxRiptideHeaterPropNode::properties() {
	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto generalControlPanel = new DfxControlPanel();

	DfxQControlArray *generalControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// General
	auto radius = new DfxQDecimalSpinnerControl(path + "radius", "", false);

	generalControlArray->addControl("Radius", radius);

	generalControlPanel->setTitle("Properties");
	generalControlPanel->setWidget(generalControlArray);

	main_layout->addWidget(generalControlPanel);

	widget->setLayout(main_layout);

	return widget;
}

std::vector<std::pair<QWidget*, QString>> DfxRiptideHeaterPropNode::propertyForms() {
	std::pair<QWidget*, QString> props(properties(), "General");
	return std::vector<std::pair<QWidget*, QString>> { props };
}