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
#include <QBoxLayout>
#include <QStackedWidget>
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "DfxFluentUtil.h"
#include "DfxQStackControl.h"
#include "DfxSimulationStageActorBase.h"
#include "DfxQControlPanel.h"
#include "DfxQControlArray.h"
#include "DfxQBoolControl.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQLineEditControl.h"
#include "DfxQFluentPathControl.h"
#include "DfxSimulationCacheBase.h"
#include "DfxBulletBodyBase.h"
#include "DfxBulletShapeBuilder.h"
#include "DfxTransformCache.h"
#include "DfxPointCache.h"
#include "DfxPointCacheDeformer.h"
#include "DfxQEmptyWidget.h"
#include "DfxTime.h"
#include "DfxBulletAgent.h"
#include "DfxQCheckBoxControl.h"

class DfxBulletActorNode : public DfxSimulationStageActorBase {
public:

	struct Source { enum { NONE = 0, GEOMETRY = 1, POINT_CLOUD = 2 }; };
	struct Shape  { enum { NONE = 0, SOURCE = 1, REFERENCE = 2 }; };
	struct Type   { enum { NONE = 0, STATIC_OBJECT = 1, RIGID_OBJECT = 2, KINEMATIC_OBJECT = 3, SOFT_BODY_OBJECT = 4, DEFORMING_OBJECT = 5 }; };
	struct Target { enum { NONE = 0, SOURCE = 1, REFERENCE = 2 }; };

	struct BulletCollisionType { enum { NONE = 0, TRIANGLES = 1, CONVEX_HULL = 2, CONVEX_DECOMPOSITION = 3, CUBE = 4, SPHERE = 5 }; };

	struct ActorState { enum { NONE = 0, SPAWN = 2 }; };
private:

	std::vector<std::pair<std::string, unsigned int>> sourceQControlenums;
	std::vector<std::pair<std::string, unsigned int>> shapeQControlenums;
	std::vector<std::pair<std::string, unsigned int>> typeQControlenums;
	std::vector<std::pair<std::string, unsigned int>> targetQControlenums;
	std::vector<std::pair<std::string, unsigned int>> bulletCollisionQControlenums;

private:
	std::vector<DfxBulletAgent*> _agents;

	btDiscreteDynamicsWorld *_dynamicsWorld;
	btSoftBodyWorldInfo *_SoftBodyWorldInfo;

	size_t _actorState;

	bool _ready;
private:
	DfxBulletAgent *spawnRigidBodyAgent(deepsg::SgNode *node, btTransform startTransform, unsigned int shapeType, deepsg::SgNode *target);
	DfxBulletAgent *spawnKinematicRigidBodyAgent(deepsg::SgNode *node, btTransform startTransform, unsigned int shapeType);
	DfxBulletAgent *spawnStaticBodyAgent(deepsg::SgNode *node, btTransform startTransform, unsigned int shapeType);
	DfxBulletAgent *spawnSoftBodyAgent(deepsg::SgNode *node, btTransform startTransform, deepsg::SgNode *target, float totalMass);
	DfxBulletAgent *spawnDeformingRigidBodyAgent(deepsg::SgNode *node, btTransform startTransform);

	void applyDynamicRigidBodyProperties(btRigidBody *body);
	void applyFixedRigidBodyProperties(btRigidBody *body);
	void applyFixedMassRigidBodyProperties(btRigidBody *body, btCollisionShape *shape);

	void applyDynamicKinematicBodyProperties(btRigidBody *body);

	void applyDynamicStaticBodyProperties(btRigidBody *body);

	void applyDynamicSoftBodyProperties(btSoftBody *body);

	void initPointCloudTarget(ds::PointCloud *pcloud);

private:
	QWidget *generalPropertiesForm();
	QWidget *bulletObjectPropertiesForm();

private:
	std::string targetPath;
	static std::string _uniqueName;
public:

	DfxBulletActorNode(deepsg::SgStageNode *st);
	virtual ~DfxBulletActorNode();

	std::string uniqueName();
	static std::string getUniqueName();

	std::string sourcePath() override;
	void setSourcePath(std::string ref) override;

	std::vector<std::pair<QWidget*, QString>> propertyForms() override;

	bool isReady() override;

	size_t actorState();
	void setActorState(size_t );

	std::vector<DfxBulletAgent*> &agents();

	//std::vector<BulletRefBody> &bulletReferences();

	void clearBody();

	void clearCache();

	void reset();

	void clearAgents();

	void preStep();

	void postStep();

	btDiscreteDynamicsWorld *dynamicsWorld();
	void setDynamicsWorld(btDiscreteDynamicsWorld *btWorld);

	btSoftBodyWorldInfo *softBodyWorldInfo();
	void setSoftBodyWorldInfo(btSoftBodyWorldInfo *sbodyWorldInfo);

	void spawn();

	glm::mat4 motionStateTransform(DfxBulletAgent *);

	void cacheLastStepToTransform(DfxTime time, DfxBulletAgent *agent);

	void cacheLastStepToPointCache(DfxTime time, DfxBulletAgent *agent);

	void cacheLastStepAll(DfxTime time);

	void updateKinematicObjects();

	void updateDeformingObjects();

	void updateRigidObjects();

	void updateSoftbodyObjects();

	void updateStaticObjects();

	btTransform bulletTransform(deepsg::SgNode *node);

	btTransform bulletTransform(glm::vec3 pos, glm::quat rot);

	btTransform bulletTransform(glm::vec3 mat);
};