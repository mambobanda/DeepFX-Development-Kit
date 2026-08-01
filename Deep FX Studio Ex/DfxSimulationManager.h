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
#include <DfxGlobal.h>
#include "DfxSpace.h"
#include "DfxTimelineManager.h"
#include "DfxSimulationStageBase.h"
#include "DfxBulletStageNode.h"
#include "DfxBulletStageHandler.h"

class DfxSimulationManager: public QObject {
	Q_OBJECT
private:
	static DfxSimulationManager *_instance;

	bool _needsReset;
	int  _stepsCovered;

	DfxSimulationManager();
public:
	
	virtual ~DfxSimulationManager();

	static DfxSimulationManager *getInstance();

	void run(DfxStageEvaluationInfo &info);

	bool needsReset();
	void forceReset(bool rs);

	int  stepsCovered();
	void setStepsCovered(int);

signals:
	void resetTriggered();

public slots:
	void resetSimulationState();
	void reset();
};