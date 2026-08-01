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

#include "DfxEntropyCommon.h"
#include "DfxSimulationStageActorBase.h"
#include "DfxQControlPanel.h"
#include "DfxQControlArray.h"
#include "DfxQBoolControl.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQFluentPathControl.h"
#include "DfxQFilePathControl.h"
#include "DfxQLineEditControl.h"
#include "DfxQEmptyWidget.h"
#include "DfxQStackControl.h"
#include "DfxQCheckBoxControl.h"
#include "DfxQPushButtonStyles.h"
#include "DfxQColorControl.h"


class DfxEntropyActorNode : public DfxSimulationStageActorBase {
private:
	static std::string _uniqueName;

private:
	size_t _actorState;

	struct Source { enum { SOURCE = 0, OBSTACLE = 1 }; };

	std::vector<std::pair<std::string, unsigned int>> sourceQControlenums;

public:

	struct ActorState { enum { NONE = 0, SPAWN = 1, UPDATING = 2}; };

	DfxEntropyActorNode(deepsg::SgStageNode *st);
	virtual ~DfxEntropyActorNode();

	std::string uniqueName();
	static std::string getUniqueName();

	bool isReady() override;

	std::string sourcePath() override;
	void setSourcePath(std::string ref) override;

	size_t actorState();
	void setActorState(size_t);

	std::vector<std::pair<QWidget*, QString>> propertyForms() override;
	QWidget *actorPropertiesForm();
};