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

#include "DfxGeometryNode.h"
#include "DfxDynaMesh.h"
#include "DfxSpace.h"

std::string DfxGeometryNode::_uniqueName = "UID_GEOMETRY_NODE";

std::string DfxGeometryNode::uniqueName() {
	return _uniqueName;
}

std::string DfxGeometryNode::getUniqueName() { 
	return _uniqueName; 
}

void DfxGeometryNode::beginningEvaluatingTools() {
}

void DfxGeometryNode::finishedEvaluatingTools() {
}

bool DfxGeometryNode::requestDynaMesh() {
	if (_dynaMeshRequestCounter == 0) {
		freeDynaMesh();
		_dynaMesh = new DfxDynaMesh;

		auto any = DfxSpace::getInstance()->findAbsolute(referencePath());
		if (any != nullptr) ((DfxDynaMesh*)_dynaMesh)->cache()->setAny(any);

		_dynaMeshRequestCounter++;
		return true;
	}
	else {
		_dynaMeshRequestCounter++;
		return true;
	}
}

void DfxGeometryNode::releaseDynaMesh() {
	if (_dynaMeshRequestCounter != 0) {
		_dynaMeshRequestCounter--;

		if (_dynaMeshRequestCounter == 0) {
			freeDynaMesh();
		}
	}
}

void DfxGeometryNode::freeDynaMesh() {
	if (_dynaMesh != nullptr) {
		_dynaMesh->clearAll();
		delete _dynaMesh;
		_dynaMesh = nullptr;
	}
}

std::vector<std::pair<QWidget*, QString>> DfxGeometryNode::propertyForms() {
	std::pair<QWidget*, QString> properties(renderForm(), "Render");

	return std::vector<std::pair<QWidget*, QString>> { properties };
}

QWidget *DfxGeometryNode::renderForm() {
	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	auto basicControlPanel = new DfxControlPanel();

	DfxQControlArray *lensControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	auto smooth_shading = new DfxQBoolControl(path + "smooth_shading", "", false);

	DfxQControlArray *basicControlArray = new DfxQControlArray();

	basicControlArray->addControl("Smooth Shading", smooth_shading);

	basicControlPanel->setTitle("Shading");
	basicControlPanel->setWidget(basicControlArray);

	main_layout->addWidget(basicControlPanel);

	widget->setLayout(main_layout);

	return widget;
}