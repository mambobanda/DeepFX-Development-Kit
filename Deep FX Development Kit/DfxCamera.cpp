/*
* The Mailo Today Solutions Software License
*
* Copyright 2016 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#include <glm/gtc/quaternion.hpp>
#include "Quaternion_Util.h"
#include "DfxScenePropRegistry.h"
#include "DfxCamera.h"


DfxCamera::DfxCamera(DfxGlobalEntityName uniquename) : DfxViewItem(DFX_CUSTOM_ITEM, uniquename) {
	using namespace DfxEntityPropertyTypes;

	setLogicalName("Camera");

	setPropertyState(inTarget, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inWidth, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inHeight, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inHoriziontalFOV, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inVerticalFOV, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inFocalLength, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(position, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(rotation, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(rotationOrder, DfxEntityPropertyState::INPUT_STATE);

	defaultSettings();

	glInitialized = false;

	getDataInstance()->makeClean();

	size = .5f;

	DfxScenePropRegistry::getIntstance()->addProp(getInstanceID(), this, DfxScenePropRegistry::CAMERA);
}

DfxCamera::~DfxCamera() {
	DfxScenePropRegistry::getIntstance()->removeProp(getInstanceID());
}

void DfxCamera::draw() {
	initializeOpenGLFunctions();

	using namespace DfxEntityPropertyTypes;

	glm::vec3 a;
	glm::vec3 b;

	glm::mat4 transMat = prefferedTransform();

	glDisable(GL_LIGHTING);
	glLineWidth(3.f);
	glColor3f(1.f, 0.6f, 0.f);

	glBegin(GL_LINES);

	// Left Frame
	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), -size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), -size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f) , (size*.2f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// Right Frame
	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), -size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), -size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// Frame Links
	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), -size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), -size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// Fin
	a = glm::vec3(transMat * glm::vec4(glm::vec3( 0, (size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3( 0, (size), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3( 0, (size), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3( 0, (size), (size*.5f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3( 0, (size), (size*.5f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size*.5f), 0), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size*.5f), 0), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size*.5f), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// Lens Base
	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f) , -(size*.4f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.2f) ), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// Lens Front
	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.8f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.8f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// Lens Links
	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	glEnd();

	glLineWidth(1.0f);

	glEnable(GL_LIGHTING);

}

void DfxCamera::drawSelection() {
	using namespace DfxEntityPropertyTypes;

	glDisable(GL_LIGHTING);
	glDisable(GL_DITHER);
	glShadeModel(GL_FLAT);

	glm::vec3 a;
	glm::vec3 b;
	auto transMat = prefferedTransform();

	glLineWidth(20.f);
	glColor3f(colorID.redf(), colorID.greenf(), colorID.bluef());

	glBegin(GL_LINES);

	// Left Frame
	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), -size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), -size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// Right Frame
	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), -size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), -size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// Frame Links
	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), -size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), -size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// Fin
	a = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size), (size*.5f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size), (size*.5f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size*.5f), 0), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size*.5f), 0), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size*.5f), size), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// Lens Base
	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// Lens Front
	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.8f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.8f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// Lens Links
	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.2f)), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	glEnd();

	glLineWidth(1.0f);

	glEnable(GL_LIGHTING);
	glEnable(GL_DITHER);
	glShadeModel(GL_FLAT);
}

DfxBounds DfxCamera::bounds() {

	DfxBounds bounds;
	bounds.empty();
	
	glm::vec3 a;
	glm::vec3 b;
	auto transMat = prefferedTransform();

	// Left Frame
	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), -size), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), -size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), size), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), size), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	// Right Frame
	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), -size), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), -size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), size), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), size), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	// Frame Links
	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), size), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.5f), -size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.5f), -size), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), (size*.2f), -size - (size*.2f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), -size - (size*.2f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.5f), -(size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.5f), -(size*.5f), size), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	// Fin
	a = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size*.5f), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size), size), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size), size), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size), (size*.5f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size), (size*.5f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size*.5f), 0), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size*.5f), 0), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(0, (size*.5f), size), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	// Lens Base
	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.2f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.2f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.2f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	// Lens Front
	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.8f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.8f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	// Lens Links
	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), 0, -size - (size*.2f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-(size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), -(size*.4f), -size - (size*.2f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.8f)), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3((size*.25f), 0, -size - (size*.2f)), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	return bounds;
}

bool DfxCamera::selectable() {
	return true;
}

void DfxCamera::compute() {
	using namespace DfxEntityPropertyTypes;

	
	makeChildrenDirty();
	getDataInstance()->makeClean();
}

void DfxCamera::calculateBounds() {
	// Do nothing
}

bool DfxCamera::isExportable() {
	return true;
}

void *DfxCamera::creator(DfxGlobalEntityName uniquename) {
	return new DfxCamera(uniquename);
}

void DfxCamera::importData(std::vector<std::pair<std::string, std::string>> data) {

}

std::vector<std::pair<std::string, std::string>> DfxCamera::exportData() {
	return mapEntityProperties(this);
}

void DfxCamera::load(std::ifstream &) {
	using namespace DfxEntityPropertyTypes;

	
}

void DfxCamera::initialize() {

	DfxViewItem::initialize();

	DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
	DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
	DfxEntityDB::createProperty(inTarget, "in_target", new DfxEntityPropertyTypes::Float3PropertyType());
	DfxEntityDB::createProperty(inWidth,  "in_width", new DfxEntityPropertyTypes::FloatPropertyType());
	DfxEntityDB::createProperty(inHeight, "in_height", new DfxEntityPropertyTypes::FloatPropertyType());
	DfxEntityDB::createProperty(inHoriziontalFOV, "in_horizontal_fov", new DfxEntityPropertyTypes::FloatPropertyType());
	DfxEntityDB::createProperty(inVerticalFOV, "in_vertical_fov", new DfxEntityPropertyTypes::FloatPropertyType());
	DfxEntityDB::createProperty(inFocalLength, "in_focal_length", new DfxEntityPropertyTypes::FloatPropertyType());
}

bool DfxCamera::hasTarget() {

	if (propertyHasBinding(inTarget)) {
		return true;
	}

	return false;
}


glm::mat4 DfxCamera::lookAtTranform() {
	using namespace DfxEntityPropertyTypes;

	pingParents(false);

	glm::vec3 focus(((DfxEntityPropertyTypes::Float3*)getProperty(inTarget))->x,
					((DfxEntityPropertyTypes::Float3*)getProperty(inTarget))->y,
					((DfxEntityPropertyTypes::Float3*)getProperty(inTarget))->z);

	glm::vec3 pos(((DfxEntityPropertyTypes::Float3*)getProperty(position))->x,
				  ((DfxEntityPropertyTypes::Float3*)getProperty(position))->y,
				  ((DfxEntityPropertyTypes::Float3*)getProperty(position))->z);

	glm::vec3 lookAtVec = glm::normalize(focus - pos);
	 
	glm::quat rotquat = RotationBetweenVectors(glm::vec3(0.0, 0.0, -1.0), lookAtVec);

	glm::mat4 rotmat = glm::toMat4(rotquat);

	glm::mat4 transMat = glm::translate(glm::mat4(1.f), glm::vec3(getPosition().x, getPosition().y, getPosition().z));
	transMat *= rotmat;

	return transMat;

}

glm::vec3 DfxCamera::getTarget() {
	return glm::vec3(((DfxEntityPropertyTypes::Float3*)getProperty(inTarget))->x,
					((DfxEntityPropertyTypes::Float3*)getProperty(inTarget))->y,
					((DfxEntityPropertyTypes::Float3*)getProperty(inTarget))->z);
}

glm::mat4 DfxCamera::prefferedTransform() {
	if (hasTarget()) {
		return lookAtTranform();
	}
	else {
		return transformationMatrix();
	}
}

void DfxCamera::defaultSettings() {

	*(float*)getProperty(inWidth)  = 640.f;
	*(float*)getProperty(inHeight) = 480.f;
	*(float*)getProperty(inHoriziontalFOV) = 36.f;
	*(float*)getProperty(inVerticalFOV)	   = 24.f;
	*(float*)getProperty(inFocalLength)    = 45.f;
}