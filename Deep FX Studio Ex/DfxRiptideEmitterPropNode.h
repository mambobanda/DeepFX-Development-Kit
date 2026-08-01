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


class DfxRiptideEmitterPropNode : public DfxSimulationStagePropBase {
public:

	struct PropState { enum { NONE = 0, SPAWN = 1 }; };
	struct EmitterShape { enum { CIRCLE = 0, SQUARE = 1 }; };

private:
	size_t _propState;

	std::vector<std::pair<std::string, unsigned int>> emitterTypeQControlenums;

private:
	fl::Any *radiusAny;

private:
	static std::string _uniqueName;
	DfxRiptideAgent *_agent;

public:

	DfxRiptideEmitterPropNode(deepsg::SgStageNode *stage);
	virtual ~DfxRiptideEmitterPropNode();

	std::string uniqueName();
	static std::string getUniqueName();

	size_t propState();
	void setPropState(size_t);

	QWidget *properties();
	deepsg::SgShape *shape() override;

	void emitParticles(glm::vec3 *vi);

	void spawn();

	deepsg::SgShape *drawCircleEmitter(fl::DataObject *);
	deepsg::SgShape *drawSquareEmitter(fl::DataObject *);

	std::vector<std::pair<QWidget*, QString>> propertyForms();

public:

	float dt;

	int emitterType;
	float strength;
	float uDisplacement;
	float vDisplacement;

	float circleRadius;

	float squareWidth;
	float squareHeight;

	int   particleLimit;

	int randomnessCounter;

	glm::vec3 position;

	glm::vec3 rotation;

	glm::quat rotationQuat;

	glm::mat4 xform;

	glm::vec3 velocity;

	double particleDiameter;

	double distanceCounter;

	std::vector<glm::vec3> emissionPositions;

	int particleType;

	bool uQuats;
	bool uXform;
public:

	void setParticleDiameter(double pd);

	double getParticleDiameter();

	void setEmissionPositions(std::vector<glm::vec3> ep);

	std::vector<glm::vec3> getEmissionPositions();

	void setStrength(float str);

	float getStrength();

	glm::vec3 getVelocity();

	void setVelocity(glm::vec3 vel);

	void intergrateDistanceCounter();

	void resetDistanceCounter();

	void setTimestep(float tmps);

	void updateWithParticleArrayIndex();

	void setRadius(float cradius);

	float getRadius();

	void setUDisplacement(float);

	float getUDisplacement();

	void setVDisplacement(float);

	float getVDisplacement();

	void setEmitterType(int);

	glm::mat4 getFinalTransformation();

	glm::vec3 getPosition();

	glm::vec3 getRotation();

	void setRotation(glm::quat rot);

	void setPosition(glm::vec3 pos);

	void setRotation(glm::vec3 rot);

	int getEmitterType();

	glm::vec3 getFinalVelocity(bool useRandomness = false);

	void setParticleLimit(int);

	int getParticleLimit();

	void generateCirclePositions();

	void generateSquarePositions();

	float getSquareWidth();

	void setSquareWidth(float);

	float getSquareHeight();

	void setSquareHeight(float);

	void emit_particles();

	int getParticleType();

	void setParticleType(int);

	bool useQuaternions();

	void setUseQuaternions(bool);

	glm::mat4 xformMatrix();

	void setXformMatrix(glm::mat4 &);

	bool useXformMatrix();

	void setUseXformMatrix(bool);

	glm::vec3 uvwRandomness(glm::vec3);

	void resetRandomnessCounter();

	DfxRiptideAgent *agent();

	void updateState();
};