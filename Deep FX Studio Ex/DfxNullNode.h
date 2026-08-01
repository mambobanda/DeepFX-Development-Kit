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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <SgNullNode.h>
#include <SgShape.h>
#include "DfxNullCubeShape.h"
#include "DfxNullArrowShape.h"
#include "DfxQEmptyWidget.h"
#include "DfxQStackControl.h"
#include "DfxQControlPanel.h"
#include "DfxQControlArray.h"
#include "DfxQBoolControl.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQFluentPathControl.h"

class DfxNullNode : public deepsg::SgNullNode {
public:
	struct ShapeType { enum { CUBE = 0, ARROW = 1 }; };

private:
	std::vector<std::pair<std::string, unsigned int>> shapeTypeQControlenums;

private:
	static std::string _uniqueName;

private:
	glm::ivec3 _color;
	std::vector<deepsg::SgShape*> _shapes;
public:

	DfxNullNode();
	virtual ~DfxNullNode();

	std::string uniqueName();
	static std::string getUniqueName();

	void setColor(glm::ivec3 ivc);
	glm::ivec3 getColor();

	void addShape(deepsg::SgShape *shape);
	void removeShape(size_t inx);

	deepsg::SgShape *shape(size_t index);

	QWidget *properties();
	std::vector<std::pair<QWidget*, QString>> propertyForms();

};