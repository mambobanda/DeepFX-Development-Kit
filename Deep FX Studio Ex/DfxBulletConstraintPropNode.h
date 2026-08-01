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

#include <SgShapeBuffer.h>
#include <btBulletDynamicsCommon.h>
#include "DfxBulletCommon.h"
#include "DfxSimulationStagePropBase.h"
#include "DfxQEmptyWidget.h"
#include "DfxQStackControl.h"
#include "DfxQControlPanel.h"
#include "DfxQControlArray.h"
#include "DfxQBoolControl.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQFluentPathControl.h"
#include "DfxQCheckBoxControl.h"
#include "DfxNullHandler.h"


class DfxBulletConstraintPropNode : public DfxSimulationStagePropBase {
public:
	
	struct Type { enum { POINT_TO_POINT = 0, HINGE = 1, SLIDER = 2, CONE_TWIST = 3, SIX_DOF = 4 }; };

	struct PropState { enum { NONE = 0, SPAWN = 2 }; };

private:

	std::vector<std::pair<std::string, unsigned int>> typeQControlenums;
private:
	btDiscreteDynamicsWorld *_dynamicsWorld;
	btTypedConstraint *_constraint;
	DfxBulletAgent *agentA;
	DfxBulletAgent *agentB;

	size_t _propState;
	int _constraint_type;
private:

	std::vector<DfxBulletAgent*> findRigidBodyAgents(std::string path);
	deepsg::SgNode *findNode(std::string path);

	static std::string _uniqueName;
public:

	DfxBulletConstraintPropNode(deepsg::SgStageNode *stage);
	virtual ~DfxBulletConstraintPropNode();

	std::string uniqueName();
	static std::string getUniqueName();

	void spawn();

	void preStep();

	void postStep();

	void updatePivot();

	void clear();

	size_t propState();
	void setPropState(size_t);

	QWidget *properties();
	std::vector<std::pair<QWidget*, QString>> propertyForms();

	btDiscreteDynamicsWorld *dynamicsWorld();
	void setDynamicsWorld(btDiscreteDynamicsWorld *btWorld);
};