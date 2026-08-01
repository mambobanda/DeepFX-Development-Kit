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

#include <DfxGlobal.h>
#include <DfxToolNode.h>
#include "DfxQStackControl.h"
#include "DfxQControlPanel.h"
#include "DfxQControlArray.h"
#include "DfxQBoolControl.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQFluentPathControl.h"
#include "DfxToolFlDispatcher.h"

class DfxGeneratePointsTool: public DfxToolNode {
private:

	QWidget *form();
	fl::Dispatcher *_dispatcher;
	std::vector<std::pair<std::string, unsigned int>> typeQControlenums;

	static std::string _uniqueName;
protected:
	virtual void onSetReferencePath(std::string path) override;

public:

	enum { GRID = 0, CIRCULAR = 1, BRICK = 2, HELIX = 3 };

	DfxGeneratePointsTool();
	virtual ~DfxGeneratePointsTool();

	std::string uniqueName() override;
	static std::string getUniqueName();

	bool isReady() override;
	bool evaluateOnUpdate() override;
	bool evaluateOnNewFrame() override;
	bool evaluateOnValueChanged() override;
	void evaluate(SgNode *parent) override;
	void create(SgNode *parent) override;
	void destroy() override;

	std::vector<std::pair<QWidget*, QString>> propertyForms() override;
};