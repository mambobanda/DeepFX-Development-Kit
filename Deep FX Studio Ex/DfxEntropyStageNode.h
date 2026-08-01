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


#include <QThread>
#include <FlDispatchManager.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <Entropy.h>
#include <EntropyCore.h>
#include "DfxEntropyCommon.h"
#include "DfxFluentSimplexObject.h"
#include "DfxFluentTypes.h"
#include "DfxQSceneManager.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxFluentSimplexCollection.h"
#include "DfxQComboBoxControl.h"
#include "DfxQFluentPathControl.h"
#include "DfxQFilePathControl.h"
#include "DfxQLineEditControl.h"
#include "DfxQEmptyWidget.h"
#include "DfxQStackControl.h"
#include "DfxQCheckBoxControl.h"
#include "DfxQPushButtonStyles.h"
#include "DfxFluentSetManager.h"
#include "DfxComputeUtility.h"
#include "DfxUtil.h"
#include "DfxEntropyAgent.h"

class DfxEntropyStageNode : public QObject, public DfxSimulationStageBase {
	Q_OBJECT
private:
	bool _simIsDirty;

private:

	static std::string _uniqueName;

	fl::Dispatcher *_flDispatcher;

private:
	std::unordered_map<size_t, DfxEntropyAgent*> _agents;

private:
	std::vector<std::pair<std::string, unsigned int>> advectionQControlenums;

	struct Advection { enum { STAM_ADVECTON = 0, MAC_CORMACK_ADVECTION = 1 }; };
private:
	QWidget *propertiesForm();

private:
	void initDispatcher();

private:
	boom::Entropy *_entropy_ptr;

private:

	float timestep;
	int substeps;

public:

	DfxEntropyStageNode();
	virtual ~DfxEntropyStageNode();

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
	//void makeSimulationDirty() override;

	bool isSimulationEnabled() override;

	bool doStep(DfxStageEvaluationInfo info) override;

	void beginStep(DfxStageEvaluationInfo info) override;
	void step(DfxStageEvaluationInfo info) override;
	void endStep(DfxStageEvaluationInfo info) override;

	bool hasFluentSimplexObject(size_t uid) override;
	bool hasFluentStreamObject(size_t uid) override;
	void clearFluent() override;

	std::vector<DfxTime> subFrames() override;

	std::vector<std::pair<QWidget*, QString>> propertyForms() override;

	fl::Dispatcher *fluentDispacther();

	boom::Entropy *entropy();

	void updateEntropySettings();

	bool entropyIsReady();

	void clearEntropy();

	void newEntropyInstance(int width, int height, int depth);

public slots:
	void dirtySimulation();
};