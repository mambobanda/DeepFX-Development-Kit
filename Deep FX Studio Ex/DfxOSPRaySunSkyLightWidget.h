/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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


#include <DfxShapeNode.h>
#include <SgShape.h>
#include "DfxItemWidgetType.h"
#include "DfxObjectHandler.h"
#include "DfxBaseForm.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxQCheckBoxControl.h"
#include "DfxQColorControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQControlArray.h"
#include "DfxQControlPanel.h"

class DfxOSPRaySunSkyLightWidget :public DfxShapeNode, public deepsg::SgShape {
private:
	static std::string _uniqueName;

	QWidget *properties();

public:
	DfxOSPRaySunSkyLightWidget();
	virtual ~DfxOSPRaySunSkyLightWidget();

	std::string uniqueName();
	static std::string getUniqueName();

	void initShapes();

	std::vector<std::pair<QWidget*, QString>> propertyForms() override;

};