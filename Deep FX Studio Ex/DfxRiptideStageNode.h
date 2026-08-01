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

#include <QThread>
#include <queue>
#include <QInputDialog>
#include <FlDispatchManager.h>
#include <RtxCommon.h>
#include <RtxSimulationConfig.h>
#include <DfxPointCache.h>
#include "DfxStageType.h"
#include "DfxRiptideCommon.h"
#include "DfxRiptideAgent.h"
#include "DfxFluentTypes.h"
#include "DfxRiptideDefaultAttributes.h"
#include "DfxQSceneManager.h"
#include "DfxSimulationStageBase.h"
#include "DfxSimulationManager.h"
#include "DfxQIntegerSpinnerControl.h"
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

class DfxRiptideStageNode : public QObject, public DfxSimulationStageBase {
	Q_OBJECT
private:
	bool _simIsDirty;

private:

	static std::string _uniqueName;

	fl::Dispatcher *_flDispatcher;

	int _solverType;
	float _current_timestep;
	int _current_substeps;

	rtx::SimulationConfig _simulationConfig;
private:
	rtx::SPHSystem *_sphSystem; 
	std::vector<DfxRiptideAgent*>  _fluentSimplexAgentQueue;
	std::vector<DfxRiptideAgent*> _fluentSimplexAgents;
	std::vector<DfxRiptideAgent*>  _fluentStreamAgentQueue;
	std::vector<DfxRiptideAgent*> _fluentStreamAgents;
private:

	std::vector<std::pair<std::string, unsigned int>> devicesQControlenums;
	std::vector<std::pair<std::string, unsigned int>> timingQControlenums;
	std::vector<std::pair<std::string, unsigned int>> searchQControlenums;

public:

	struct Devices { enum { CPU = 0 }; };
	struct Search { enum { COMPACT = 0, COMPACT_PLUS = 1}; };
	struct Timing  { enum { ADAPTIVE = 0, FPS = 1, TIMESTEP = 2 }; };

public:

	DfxRiptideStageNode();
	virtual ~DfxRiptideStageNode();

	std::string uniqueName();
	static std::string getUniqueName();

	size_t addActor(fl::Any *) override;
	void removeActor(size_t uid) override;
	void restoreActor(deepsg::SgStageActorNode *);

	size_t addProp(fl::Any *) override;
	void removeProp(size_t uid) override;
	void restoreProp(deepsg::SgStagePropNode *);

	void restoreAll() override;

	void reset() override;
	bool needsReset() override;

	void dirtySimulation();

	bool isSimulationEnabled() override;

	bool doStep(DfxStageEvaluationInfo info) override;

	void beginStep(DfxStageEvaluationInfo info) override;
	void step(DfxStageEvaluationInfo info) override;
	void endStep(DfxStageEvaluationInfo info) override;

	std::vector<DfxTime> subFrames() override;

	QWidget *form();
	std::vector<std::pair<QWidget*, QString>> propertyForms() override;

	void queueSimplexAgent(DfxRiptideAgent *agent);
	void queueStreamAgent(DfxRiptideAgent *agent);

	bool hasFluentSimplexObject(size_t uid) override;
	bool hasFluentStreamObject(size_t uid) override;
	void clearFluent() override;

	rtx::SPHSystem *sphSystem();
	void applySphSystemSettings();

	DfxPointFrame *getSimulationFrame();
	void updateStream();

	DfxRiptideDefaultAttributes fetchDefaultStreamAttributes(bool create = false);

	void updateSPHSystem();

	static void stepUpdateCallback(int step, float timestep, rtx::SPHSystem*, void *userdata);

	static void stepLogCallback(const char* msg);

	float particleRadius();

	void processAgentQueue(std::vector<DfxRiptideAgent*> &queue, size_t type);
	/*
	float particleRadius();
	int solver();
	std::pair<float, int> timestepSubsteps();
	int fpsToSubsteps(float dt, int fps);

	static float particleRadiusFromResolution(int in_resolution);
	static void progress(char *str);

	void setTimestepSubsteps();

	void freeSolver();
	void initSolver();

	void updateCacheInfo(DfxSimulationStagePropBase *cache);

	void clearPreviewFrame();
	*/

// public slots:
	//void dirtySimulation();
	//void resampleDomain();

};