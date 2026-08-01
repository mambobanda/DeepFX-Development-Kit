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

#include "DfxRiptideCommon.h"
#include "DfxRiptideAgent.h"
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


class DfxRiptideActorNode : public DfxSimulationStageActorBase {
private:
	std::vector<std::pair<std::string, unsigned int>> actorTypeQControlenums;

public:

	struct Source { enum { GEOMETRY = 0, }; };
	struct ActorState { enum { NONE = 0, SPAWN = 1 }; };
	struct ActorType { enum { FLUID = 0, COLLISION_OBJECT = 1, }; };

private:

	QWidget *actorPropertiesForm();

	size_t _actorState;
	bool _isReady;

	unsigned int _source;

private:
	static std::string _uniqueName;

public:

	DfxRiptideActorNode(deepsg::SgStageNode *st);
	virtual ~DfxRiptideActorNode();

	std::string uniqueName();
	static std::string getUniqueName();

	size_t actorState();
	void setActorState(size_t);

	unsigned int source();
	void setSource(unsigned int);

	void spawn();

	bool isReady() override;

	std::string sourcePath() override;
	void setSourcePath(std::string ref) override;

	std::vector<std::pair<QWidget*, QString>> propertyForms() override;
};