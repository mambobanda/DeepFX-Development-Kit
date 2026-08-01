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
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftBodySolvers.h>
#include <BulletSoftBody/btDefaultSoftBodySolver.h>
#include <LinearMath/btPoolAllocator.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcherMt.h>
#include <BulletDynamics/Dynamics/btSimulationIslandManagerMt.h>  // for setSplitIslands()
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorldMt.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolverMt.h>
#include <FlDispatchManager.h>
#include <SgPointCloudNode.h>
#include <glm/gtx/matrix_decompose.hpp>
#include "DfxFluentSimplexObject.h"
#include "DfxFluentTypes.h"
#include "DfxQSceneManager.h"
#include "DfxSimulationStageBase.h"
#include "DfxBulletActorHandler.h"
#include "DfxBulletFlDispatcher.h"
#include "DfxBulletAgent.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxFluentSimplexCollection.h"
#include "DfxFluentSetManager.h"
#include "DfxComputeUtility.h"
#include "DfxUtil.h"

class DfxBulletStageNode : public QObject, public DfxSimulationStageBase {
	Q_OBJECT
private:
	bool _simIsDirty;
private:

	static std::string _uniqueName;

	btSoftBodyWorldInfo *softBodyWorldInfo;
	btSoftBodyRigidBodyCollisionConfiguration *collisionConfiguration;
	btCollisionDispatcher *dispatcher;
	btBroadphaseInterface *broadphase;
	btConstraintSolver *rigid_body_solver;
	btSoftBodySolver *softbodySolver;
	btDiscreteDynamicsWorld *dynamicsWorld;

	fl::Dispatcher *_flDispatcher;

private:
	std::unordered_map<size_t, DfxBulletAgent*> _fluentSimplexAgents;
	std::unordered_map<size_t, DfxBulletAgent*> _fluentStreamAgents;
private:
	QWidget *propertiesForm();
private:
	void initDispatcher();
public:

	DfxBulletStageNode();
	virtual ~DfxBulletStageNode();

	std::string uniqueName();
	static std::string getUniqueName();

	size_t addActor(fl::Any *) override;
	void removeActor(size_t uid) override;
	void restoreActor(deepsg::SgStageActorNode *);

	size_t addProp(fl::Any *) override;
	void removeProp(size_t uid) override;
	void restoreProp(deepsg::SgStagePropNode *);

	DfxBulletAgent *addFluentSimplex(size_t index, size_t type, size_t collision_shape);
	DfxBulletAgent *addFluentStream(size_t index, size_t type, size_t collision_shape);

	std::vector<DfxBulletAgent*> addFluentStream(DfxFluentSet *set, std::vector<size_t> indices, size_t type, size_t collisionShape, std::vector<deepsg::SgNode*> shapeNodes);
	void removeFluentAgent(std::unordered_map<size_t, DfxBulletAgent*> &map, size_t index);
	void removeFluentSimplex(size_t index);
	void removeFluentStream(size_t index);

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

	void initBullet();
	void initBulletDynamicsWorld();
	void clearBullet();
	void clearBulletDynamicsWorld();

public:
	void cacheFluentAgents();

	bool hasFluentSimplexAgent(size_t uid);

	bool hasFluentStreamAgent(size_t uid);

	std::unordered_map<size_t, DfxBulletAgent*> &fluentSimplexAgents();
	std::unordered_map<size_t, DfxBulletAgent*> &fluentStreamAgents();

	DfxBulletAgent *findFluentSimplexAgent(size_t);
	DfxBulletAgent *findFluentStreamAgent(size_t);

	void updateFluentRigidBodyProperties();

	void setFluentSimplexAgentActivationState(std::vector<size_t> &indices, int state, std::string alias = "");
	void setFluentStreamAgentActivationState(std::vector<size_t> &indices, int state, std::string alias = "");
public slots:
	void dirtySimulation();
};