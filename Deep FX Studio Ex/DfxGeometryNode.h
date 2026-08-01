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

#include <QWidget>
#include <SgGeoNode.h>
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxQColorControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQControlArray.h"
#include "DfxQControlPanel.h"
#include "DfxItemWidgetType.h"
#include "DfxQBoolControl.h"

class DfxGeometryNode : public deepsg::SgGeoNode {
private:
	static std::string _uniqueName;

private:
	QWidget *renderForm();

public:

	virtual std::string uniqueName();
	static std::string getUniqueName();

	void beginningEvaluatingTools() override;
	void finishedEvaluatingTools() override;
	bool requestDynaMesh() override;
	void releaseDynaMesh() override;
	void freeDynaMesh();

	virtual std::vector<std::pair<QWidget*, QString>> propertyForms();
};