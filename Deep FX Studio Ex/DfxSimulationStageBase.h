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
#include <FlAny.h>
#include <SgStageNode.h>
#include <DfxStageType.h>
#include <DfxGlobal.h>
#include "DfxStageEvaluationInfo.h"
#include "DfxTime.h"

class DfxSimulationStageBase : public deepsg::SgStageNode {
private:
	static size_t ID_GEN;
	size_t _simID;
public:

	DfxSimulationStageBase(size_t stageT);
	virtual ~DfxSimulationStageBase();

	size_t simulationID();

	virtual size_t addActor(fl::Any *) = 0;
	virtual void removeActor(size_t uid) = 0;
	virtual void restoreAll() = 0;

	virtual size_t addProp(fl::Any *)  = 0;
	virtual void removeProp(size_t uid) = 0;

	virtual void reset() = 0;
	virtual bool needsReset() = 0;
	// virtual void makeSimulationDirty() = 0;

	virtual bool isSimulationEnabled() = 0;
	virtual bool doStep(DfxStageEvaluationInfo info) = 0;
	virtual void beginStep(DfxStageEvaluationInfo info) = 0;
	virtual void step(DfxStageEvaluationInfo info) = 0;
	virtual void endStep(DfxStageEvaluationInfo info) = 0;

	virtual std::vector<DfxTime> subFrames() = 0;

	virtual std::vector<std::pair<QWidget*, QString>> propertyForms() = 0;
	// virtual void cache(DfxStageEvaluationInfo info);

	virtual bool hasFluentSimplexObject(size_t uid) = 0;
	virtual bool hasFluentStreamObject(size_t uid) = 0;
	virtual void clearFluent() = 0;
};