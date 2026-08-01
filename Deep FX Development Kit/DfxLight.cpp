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
#include "DfxLight.h"


DfxLight::DfxLight(DfxGlobalEntityName uniquename) : DfxViewItem(DFX_CUSTOM_ITEM, uniquename) {
	using namespace DfxEntityPropertyTypes;

	setLogicalName("Light");

	setPropertyState(inTarget, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inType, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(position, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(rotation, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(scale, DfxEntityPropertyState::INPUT_STATE);

	glInitialized = false;

	getDataInstance()->makeClean();

	size = 1.5f;

	DfxScenePropRegistry::getIntstance()->addProp(getInstanceID(), this, DfxScenePropRegistry::CAMERA);
}

DfxLight::~DfxLight() {
	DfxScenePropRegistry::getIntstance()->removeProp(getInstanceID());
}

void DfxLight::draw() {
	initializeOpenGLFunctions();

	double xPI = 3.14159265358979323846;

	using namespace DfxEntityPropertyTypes;

	glm::vec3 a;
	glm::vec3 b;
	glm::vec3 c;
	glm::vec3 d;

	glm::mat4 transMat = prefferedTransform();

	double x_circ[31];
	double y_circ[31];

	for (int i = 0; i <= 30; i++) {
		double angle = 2 * xPI*i / 30;
		double x = cos(angle)*0.0625f*size;
		double y = sin(angle)*0.0625f*size;

		x_circ[i] = x;
		y_circ[i] = y;
	}

	glDisable(GL_LIGHTING);
	glLineWidth(3.f);
	glColor3f(1.f, 1.f, 0.f);

	glBegin(GL_LINES);

	for (int i = 0; i <= 29; i++) {

		a = glm::vec3(transMat * glm::vec4(glm::vec3(x_circ[i], 0.0, y_circ[i]), 1.f));
		b = glm::vec3(transMat * glm::vec4(glm::vec3(x_circ[i + 1], 0.0, y_circ[i + 1]), 1.f));
		double pb1[] = { a.x, a.y, a.z };
		double pb2[] = { b.x, b.y, b.z };

		c = glm::vec3(transMat * glm::vec4(glm::vec3(0.0, x_circ[i], y_circ[i]), 1.f));
		d = glm::vec3(transMat * glm::vec4(glm::vec3(0.0, x_circ[i + 1], y_circ[i + 1]), 1.f));
		double pb3[] = { c.x, c.y, c.z };
		double pb4[] = { d.x, d.y, d.z };

		glVertex3f(pb1[0], pb1[1], pb1[2]);
		glVertex3f(pb2[0], pb2[1], pb2[2]);

		glVertex3f(pb3[0], pb3[1], pb3[2]);
		glVertex3f(pb4[0], pb4[1], pb4[2]);
	}

	glEnd();

	float _mins = 0.2f;
	glLineStipple(1, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(size * _mins, size * _mins, size * _mins), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-size * _mins, -size * _mins, -size * _mins), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-size * _mins, size * _mins, size * _mins), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(size * _mins, -size * _mins, -size * _mins), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(size * _mins, size * _mins, -size * _mins), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-size * _mins, -size * _mins, size * _mins), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-size * _mins, size * _mins, -size * _mins), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(size * _mins, -size * _mins, size * _mins), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(0.f, size * _mins, 0.f), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(0.f, -size * _mins, 0.f), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	glEnd();
	glDisable(GL_LINE_STIPPLE);

	glLineWidth(1.0f);

	glEnable(GL_LIGHTING);

}

void DfxLight::drawSelection() {
	using namespace DfxEntityPropertyTypes;

	double xPI = 3.14159265358979323846;

	double x_circ[31];
	double y_circ[31];

	for (int i = 0; i <= 30; i++) {
		double angle = 2 * xPI*i / 30;
		double x = cos(angle)*0.0625f*size;
		double y = sin(angle)*0.0625f*size;

		x_circ[i] = x;
		y_circ[i] = y;
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_DITHER);
	glShadeModel(GL_FLAT);

	glm::vec3 a;
	glm::vec3 b;
	glm::vec3 c;
	glm::vec3 d;

	auto transMat = prefferedTransform();

	glLineWidth(20.f);
	glColor3f(colorID.redf(), colorID.greenf(), colorID.bluef());

	glBegin(GL_LINES);

	for (int i = 0; i <= 29; i++) {
		a = glm::vec3(transMat * glm::vec4(glm::vec3(x_circ[i], 0.0, y_circ[i]), 1.f));
		b = glm::vec3(transMat * glm::vec4(glm::vec3(x_circ[i + 1], 0.0, y_circ[i + 1]), 1.f));
		double pb1[] = { a.x, a.y, a.z };
		double pb2[] = { b.x, b.y, b.z };

		c = glm::vec3(transMat * glm::vec4(glm::vec3(0.0, x_circ[i], y_circ[i]), 1.f));
		d = glm::vec3(transMat * glm::vec4(glm::vec3(0.0, x_circ[i + 1], y_circ[i + 1]), 1.f));
		double pb3[] = { c.x, c.y, c.z };
		double pb4[] = { d.x, d.y, d.z };

		glVertex3f(pb1[0], pb1[1], pb1[2]);
		glVertex3f(pb2[0], pb2[1], pb2[2]);

		glVertex3f(pb3[0], pb3[1], pb3[2]);
		glVertex3f(pb4[0], pb4[1], pb4[2]);
	}

	glEnd();

	float _mins = 0.2f;
	glLineStipple(1, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(size * _mins, size * _mins, size * _mins), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-size * _mins, -size * _mins, -size * _mins), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-size * _mins, size * _mins, size * _mins), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(size * _mins, -size * _mins, -size * _mins), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(size * _mins, size * _mins, -size * _mins), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-size * _mins, -size * _mins, size * _mins), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-size * _mins, size * _mins, -size * _mins), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(size * _mins, -size * _mins, size * _mins), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(0.f, size * _mins, 0.f), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(0.f, -size * _mins, 0.f), 1.f));
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	glEnd();
	glDisable(GL_LINE_STIPPLE);

	glLineWidth(1.0f);

	glEnable(GL_LIGHTING);
	glEnable(GL_DITHER);
	glShadeModel(GL_FLAT);
}

DfxBounds DfxLight::bounds() {

	DfxBounds bounds;
	bounds.empty();

	double xPI = 3.14159265358979323846;

	glm::vec3 a;
	glm::vec3 b;
	glm::vec3 c;
	glm::vec3 d;

	auto transMat = prefferedTransform();

	double x_circ[31];
	double y_circ[31];

	for (int i = 0; i <= 30; i++) {
		double angle = 2 * xPI*i / 30;
		double x = cos(angle)*0.0625f*size;
		double y = sin(angle)*0.0625f*size;

		x_circ[i] = x;
		y_circ[i] = y;
	}

	for (int i = 0; i <= 29; i++) {
		a = glm::vec3(transMat * glm::vec4(glm::vec3(x_circ[i], 0.0, y_circ[i]), 1.f));
		b = glm::vec3(transMat * glm::vec4(glm::vec3(x_circ[i + 1], 0.0, y_circ[i + 1]), 1.f));
		double pb1[] = { a.x, a.y, a.z };
		double pb2[] = { b.x, b.y, b.z };

		c = glm::vec3(transMat * glm::vec4(glm::vec3(0.0, x_circ[i], y_circ[i]), 1.f));
		d = glm::vec3(transMat * glm::vec4(glm::vec3(0.0, x_circ[i + 1], y_circ[i + 1]), 1.f));
		double pb3[] = { c.x, c.y, c.z };
		double pb4[] = { d.x, d.y, d.z };

		bounds.add(pb1[0], pb1[1], pb1[2]);
		bounds.add(pb2[0], pb2[1], pb2[2]);

		bounds.add(pb3[0], pb3[1], pb3[2]);
		bounds.add(pb4[0], pb4[1], pb4[2]);
	}

	float _mins = 0.2f;

	a = glm::vec3(transMat * glm::vec4(glm::vec3(size * _mins, size * _mins, size * _mins), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-size * _mins, -size * _mins, -size * _mins), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-size * _mins, size * _mins, size * _mins), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(size * _mins, -size * _mins, -size * _mins), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(size * _mins, size * _mins, -size * _mins), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(-size * _mins, -size * _mins, size * _mins), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(-size * _mins, size * _mins, -size * _mins), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(size * _mins, -size * _mins, size * _mins), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	a = glm::vec3(transMat * glm::vec4(glm::vec3(0.f, size * _mins, 0.f), 1.f));
	b = glm::vec3(transMat * glm::vec4(glm::vec3(0.f, -size * _mins, 0.f), 1.f));
	bounds.add(a.x, a.y, a.z);
	bounds.add(b.x, b.y, b.z);

	return bounds;
}

bool DfxLight::selectable() {
	return true;
}

void DfxLight::compute() {
	using namespace DfxEntityPropertyTypes;


	makeChildrenDirty();
	getDataInstance()->makeClean();
}

void DfxLight::calculateBounds() {
	// Do nothing
}

bool DfxLight::isExportable() {
	return true;
}

void *DfxLight::creator(DfxGlobalEntityName uniquename) {
	return new DfxLight(uniquename);
}

void DfxLight::importData(std::vector<std::pair<std::string, std::string>> data) {

}

std::vector<std::pair<std::string, std::string>> DfxLight::exportData() {
	return mapEntityProperties(this);
}

void DfxLight::load(std::ifstream &) {
	using namespace DfxEntityPropertyTypes;


}

void DfxLight::initialize() {

	DfxViewItem::initialize();

	DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
	DfxEntityDB::createProperty(inTarget, "in_target", new DfxEntityPropertyTypes::Float3PropertyType());
	DfxEntityDB::createProperty(inType, "in_type", new DfxEntityPropertyTypes::DfxEnumPropertyType());
}

bool DfxLight::hasTarget() {

	if (propertyHasBinding(inTarget)) {
		return true;
	}

	return false;
}


glm::mat4 DfxLight::lookAtTranform() {
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

glm::mat4 DfxLight::prefferedTransform() {
	if (hasTarget()) {
		return lookAtTranform();
	}
	else {
		return transformationMatrix();
	}
}