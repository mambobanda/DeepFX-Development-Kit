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
#include <DfxMode.h>
#include <glm/gtx/matrix_decompose.hpp>
#include "DfxTimelineManager.h"
#include "DfxRiptideCommon.h"
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


class DfxRiptideHeaterPropNode : public DfxSimulationStagePropBase {
public:

	struct PropState { enum { NONE = 0, SPAWN = 1 }; };
	
private:
	size_t _propState;

	// std::vector<std::pair<std::string, unsigned int>> emitterTypeQControlenums;

private:
	fl::Any *radiusAny;

private:
	static std::string _uniqueName;
	int _riptideHeaterID;
public:

	DfxRiptideHeaterPropNode(deepsg::SgStageNode *stage);
	virtual ~DfxRiptideHeaterPropNode();

	std::string uniqueName();
	static std::string getUniqueName();

	size_t propState();
	void setPropState(size_t);

	deepsg::SgShape *shape() override;

	void spawn();

	deepsg::SgShape *drawHeater(float radius);

	int riptideHeaterID();

	void riptideHeaterID(int );

	void updateHeaterState();

	QWidget *properties();

	std::vector<std::pair<QWidget*, QString>> propertyForms();

};