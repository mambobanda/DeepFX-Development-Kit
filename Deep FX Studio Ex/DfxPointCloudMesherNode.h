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

#include <QObject>
#include <DfxGeometryNode.h>
#include <QPushButton>
#include <SgGeoAdapterInterface.h>
#include "DfxDynaMesh.h"
#include "DfxUpdater.h"
#include "DfxToolFlDispatcher.h"
#include "DfxToolNode.h"
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
#include "DfxQCheckBoxControl.h"
#include "DfxQPushButtonStyles.h"

class DfxPointCloudMesherNode : public QObject, public DfxToolNode {
	Q_OBJECT
private:
	static std::string _uniqueName;

private:
	QWidget *propertyForm();

	fl::Dispatcher *_dispatcher;
	deepsg::SgGeoNode *_parent;
	bool _active;
	bool _generate;
protected:
	virtual void onSetReferencePath(std::string path) override;

private:
	std::vector<std::pair<std::string, unsigned int>> unitTypeQControlenums;
	std::vector<std::pair<std::string, unsigned int>> smoothingTypeQControlenums;
	std::vector<std::pair<std::string, unsigned int>> gridScalingTypeQControlenums;

private:
	float gridScalingFraction(int scaling);

public:
	struct Units { enum { RESOLUTION = 0, DISCREET = 1 }; };
	struct Smoothing { enum { NONE = 0, MEAN = 1, MEDIAN = 2, GAUSSIAN = 3, LAPLACIAN = 4, MEAN_CURVATURE = 5 }; };
	struct GridSpacing { enum { _1_1 = 0, _1_2 = 1, _1_3 = 2, _1_4 = 3, _1_5 = 4, _1_6 = 5, _1_7 = 6, _1_8 = 7, _1_9 = 8, _1_10 = 9, _1_11 = 10, _1_12 = 11, _1_13 = 12,
								_1_14 = 13, _1_15 = 14 }; };

public:

	DfxPointCloudMesherNode();
	virtual ~DfxPointCloudMesherNode();

	virtual std::string uniqueName() override;
	static std::string getUniqueName();

	void processSignal(size_t signal) override;
	
	bool isReady() override;
	bool evaluateOnUpdate() override;
	bool evaluateOnNewFrame() override;
	bool evaluateOnValueChanged() override;
	void evaluate(SgNode *parent) override;
	void create(SgNode *parent) override;
	void destroy() override;

	std::vector<std::pair<QWidget*, QString>> propertyForms() override;
	
public slots:
	void generateMesh();
	void pcloudPathUpdated(std::string);
};