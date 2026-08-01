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

#undef Q_FOREACH
#include <openvdb/openvdb.h>
#define foreach Q_FOREACH

#include <SgShapeBuffer.h>
#include <DfxMode.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <SgGridAdapterInterface.h>
#include <glm/vec3.hpp>
#include <Entropy.h>
#include <EntropyCore.h>
#include <DfxBounds.h>
#include "DfxTimelineManager.h"
#include "DfxEntropyCommon.h"
#include "DfxSimulationStagePropBase.h"
#include "DfxQEmptyWidget.h"
#include "DfxQStackControl.h"
#include "DfxQControlPanel.h"
#include "DfxQControlArray.h"
#include "DfxQBoolControl.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQFluentPathControl.h"
#include "DfxQLineEditControl.h"
#include "DfxQFilePathControl.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxQColorControl.h"
#include "DfxQPushButtonStyles.h"
#include "DfxQColorControl.h"


class DfxEntropyDomainPropNode : public DfxSimulationStagePropBase {
private:
	static std::string _uniqueName;

private:
	deepsg::SgShape *drawDomain(fl::DataObject *dao);

private:
	size_t _propState;

private:
	openvdb::FloatGrid::Ptr _sourceGrid;
	openvdb::FloatGrid::Ptr _obstacleGrid;

public:

	struct SourceInfo {

		bool constant_inflow;
		bool constant_temperature;
		bool constant_velocity;

		int inflow_rate_type;
		float inflow_rate;

		float inflow_density;
		float inflow_temperature;

		glm::vec3 inflow_velocity;

	};

public:

	struct PropState { enum { NONE = 0, SPAWN = 1 }; };

	DfxEntropyDomainPropNode(deepsg::SgStageNode *stage);
	virtual ~DfxEntropyDomainPropNode();

	std::string uniqueName();
	static std::string getUniqueName();

	void clear();

	deepsg::SgShape *shape() override;

	QWidget *properties();
	std::vector<std::pair<QWidget*, QString>> propertyForms() override;

	void spawn();
	void reset();

	void addSource(SourceInfo info, deepsg::SgGridAdapterInterface *adapter, boom::Entropy *entropy, glm::mat4 xform);
	void addObstacle(deepsg::SgGridAdapterInterface *adapter, boom::Entropy *entropy, glm::mat4 xform);

	DfxBounds bounds();

	glm::ivec3 dimensions();
	float voxelSize();

	size_t propState();
	void setPropState(size_t);
};