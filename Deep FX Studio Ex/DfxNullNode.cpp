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

#include "DfxNullNode.h"

std::string DfxNullNode::_uniqueName = "UID_NULL_NODE";

DfxNullNode::DfxNullNode() {
	_color = glm::ivec3(0.f);

	_shapes.push_back(new DfxNullCubeShape);
	_shapes.push_back(new DfxNullArrowShape);

	shapeTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Cube", ShapeType::CUBE));
	shapeTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Arrow", ShapeType::ARROW));
}

DfxNullNode::~DfxNullNode() {
	for (auto sh : _shapes) delete sh;
	_shapes.clear();
}

std::string DfxNullNode::uniqueName() {
	return _uniqueName;
}

std::string DfxNullNode::getUniqueName() {
	return _uniqueName;
}

void DfxNullNode::setColor(glm::ivec3 ivc) {
	_color = ivc;
}

glm::ivec3 DfxNullNode::getColor() {
	return _color;
}

void DfxNullNode::addShape(deepsg::SgShape *shape) {
	return _shapes.push_back(shape);
}

void DfxNullNode::removeShape(size_t inx) {
	_shapes.erase(_shapes.begin() + inx);
}

deepsg::SgShape *DfxNullNode::shape(size_t index) {
	return _shapes[index];
}

std::vector<std::pair<QWidget*, QString>> DfxNullNode::propertyForms() {
	std::pair<QWidget*, QString> propertiesForm(properties(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> { propertiesForm };
}


QWidget *DfxNullNode::properties() {

	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto propertiesControlPanel = new DfxControlPanel();

	DfxQControlArray *propertiesControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// Type Controls
	auto stype = new DfxQComboBoxControl(path + "current_shape", "", shapeTypeQControlenums, false);

	// Type
	propertiesControlArray->addControl("Shape", stype);

	propertiesControlPanel->setTitle("Properties");
	propertiesControlPanel->setWidget(propertiesControlArray);

	main_layout->addWidget(propertiesControlPanel);
	widget->setLayout(main_layout);

	return widget;
}