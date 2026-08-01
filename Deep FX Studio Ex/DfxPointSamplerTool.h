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
#include "DfxQCheckBoxControl.h"
#include "DfxToolFlDispatcher.h"

class DfxPointSamplerTool :public QObject, public DfxToolNode {
	Q_OBJECT
private:

	QWidget *form();
	fl::Dispatcher *_dispatcher;

	std::vector<std::pair<std::string, unsigned int>> typeQControlenums;
	std::vector<std::pair<std::string, unsigned int>> surfaceSamplingQControlenums;
	std::vector<std::pair<std::string, unsigned int>> volumeSamplingTypeQControlenums;

	static std::string _uniqueName;

	bool _generate;
protected:
	virtual void onSetReferencePath(std::string path) override;

public:

	struct Type { enum { SURFACE = 0, VOLUME = 1 }; };
	struct SurfaceSamplingType { enum { REGULAR = 0, POISSON = 1, TRIANGLE = 2 }; };
	struct VolumeSamplingType { enum { VOXEL = 0, UNIFORM_RANDOM = 1, SPHERE = 2}; };

public:

	DfxPointSamplerTool();
	virtual ~DfxPointSamplerTool();

	std::string uniqueName() override;
	static std::string getUniqueName();

	void sampleMesh();

	bool isReady() override;
	bool evaluateOnUpdate() override;
	bool evaluateOnNewFrame() override;
	bool evaluateOnValueChanged() override;
	void evaluate(SgNode *parent) override;
	void create(SgNode *parent) override;
	void destroy() override;

	std::vector<std::pair<QWidget*, QString>> propertyForms() override;
};