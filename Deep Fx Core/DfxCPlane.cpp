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

#include <algorithm>
#include <limits>
#include <random>
#include <ctime>
#include "DfxCPlane.h"


namespace dfxcore {

	DfxCPlane::DfxCPlane(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename, DFX_CUSTOM_ITEM) {
		setLogicalName("Implicit Plane");

		setPropertyState(outNormal, DfxEntityPropertyState::OUTPUT_STATE);
		setPropertyState(outPosition, DfxEntityPropertyState::OUTPUT_STATE);

		setPropertyState(position, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(rotation, DfxEntityPropertyState::INPUT_STATE);
	}

	DfxCPlane::~DfxCPlane() {
	}

	void DfxCPlane::drawPlane() {

		float width = 1.f;
		float height = 1.f;

		float halfwidth = width;
		float halfheight = height;

		float sfactor = 0.25f;

		double verts[][3] = { { -halfwidth, 0.0f, halfheight },
		{ -halfwidth, 0.0f,-halfheight },
		{ halfwidth, 0.0f,-halfheight },
		{ halfwidth, 0.0f, halfheight } };

		glDisable(GL_LIGHTING);
		glBegin(GL_POLYGON);
		glVertex3f(verts[0][0], verts[0][1], verts[0][2]);
		glVertex3f(verts[1][0], verts[1][1], verts[1][2]);
		glVertex3f(verts[2][0], verts[2][1], verts[2][2]);
		glVertex3f(verts[3][0], verts[3][1], verts[3][2]);
		glVertex3f(verts[0][0], verts[0][1], verts[0][2]);
		glEnd();
		glLineWidth(1.f);
		glEnable(GL_LIGHTING);

		glDisable(GL_LIGHTING);
		glBegin(GL_POLYGON);
		glVertex3f(verts[0][0], verts[0][1], verts[0][2]);
		glVertex3f(verts[3][0], verts[3][1], verts[3][2]);
		glVertex3f(verts[2][0], verts[2][1], verts[2][2]);
		glVertex3f(verts[1][0], verts[1][1], verts[1][2]);
		glVertex3f(verts[0][0], verts[0][1], verts[0][2]);
		glEnd();
		glLineWidth(1.f);
		glEnable(GL_LIGHTING);

		double x_circ[31];
		double y_circ[31];

		double v1[] = { 0.0, (width + height), 0.0 };
		double v2[] = { 0.0, (width + height), 0.0 };
		double v3[] = { 0.0, (width + height), 0.0 };
		double v4[] = { 0.0, (width + height), 0.0 };

		double ln1[] = { 0.0, 0.0f, 0.0f };
		double ln2[] = { 0.0, (width + height), 0.0 };

		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		glVertex3f(ln1[0], ln1[1], ln1[2]);
		glVertex3f(ln2[0], ln2[1], ln2[2]);
		glEnd();
		glLineWidth(1.f);
		glEnable(GL_LIGHTING);

	}

	DfxBounds DfxCPlane::planeBounds() {
		DfxBounds bounds;

		glm::mat4 transMat = glm::translate(glm::mat4(), glm::vec3(getPosition().x, getPosition().y, getPosition().z));

		transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().x), glm::vec3(1.f, .0f, .0f));
		transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().y), glm::vec3(0.f, 1.f, .0f));
		transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().z), glm::vec3(0.f, .0f, 1.f));

		float width  = 1.f;
		float height = 1.f;

		float halfwidth = width;
		float halfheight = height;

		float sfactor = 0.25f;

		double verts[][3] = { { -halfwidth, 0.0f, halfheight },
		{ -halfwidth, 0.0f,-halfheight },
		{ halfwidth, 0.0f,-halfheight },
		{ halfwidth, 0.0f, halfheight } };

		bounds.add(verts[0][0], verts[0][1], verts[0][2], transMat);
		bounds.add(verts[1][0], verts[1][1], verts[1][2], transMat);
		bounds.add(verts[1][0], verts[1][1], verts[1][2], transMat);
		bounds.add(verts[2][0], verts[2][1], verts[2][2], transMat);
		bounds.add(verts[2][0], verts[2][1], verts[2][2], transMat);
		bounds.add(verts[3][0], verts[3][1], verts[3][2], transMat);
		bounds.add(verts[3][0], verts[3][1], verts[3][2], transMat);
		bounds.add(verts[0][0], verts[0][1], verts[0][2], transMat);

		double x_circ[31];
		double y_circ[31];

		double v1[] = { 0.0, (width + height), 0.0 };
		double v2[] = { 0.0, (width + height), 0.0 };
		double v3[] = { 0.0, (width + height), 0.0 };
		double v4[] = { 0.0, (width + height), 0.0 };

		double ln1[] = { 0.0, 0.0f, 0.0f };
		double ln2[] = { 0.0, (width + height), 0.0 };

		bounds.add(ln1[0], ln1[1], ln1[2], transMat);
		bounds.add(ln2[0], ln2[1], ln2[2], transMat);

		return bounds;
	}

	void DfxCPlane::draw() {
		initializeOpenGLFunctions();

		glPushMatrix();

		glm::mat4 transMat = glm::translate(glm::mat4(), glm::vec3(getPosition().x, getPosition().y, getPosition().z));

		transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().x), glm::vec3(1.f, .0f, .0f));
		transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().y), glm::vec3(0.f, 1.f, .0f));
		transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().z), glm::vec3(0.f, .0f, 1.f));

		transMat *= glm::scale(glm::mat4(1.f), glm::vec3(getScale().x, getScale().y, getScale().z));

		glMultMatrixf(&transMat[0][0]);

		glColor4f(1.f, 1.f, 0.f, .5f);
		
		drawPlane();
		
		glPopMatrix();
	}

	void DfxCPlane::drawSelection() {

		glPushMatrix();

		glm::mat4 transMat = glm::translate(glm::mat4(), glm::vec3(getPosition().x, getPosition().y, getPosition().z));

		transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().x), glm::vec3(1.f, .0f, .0f));
		transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().y), glm::vec3(0.f, 1.f, .0f));
		transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().z), glm::vec3(0.f, .0f, 1.f));

		transMat *= glm::scale(glm::mat4(1.f), glm::vec3(getScale().x, getScale().y, getScale().z));

		glMultMatrixf(&transMat[0][0]);

		glDisable(GL_LIGHTING);
		glDisable(GL_DITHER);
		glShadeModel(GL_FLAT);

		glColor3f(colorID.redf(), colorID.greenf(), colorID.bluef());
		
		drawPlane();
		
		glPopMatrix();

		glEnable(GL_LIGHTING);
		glEnable(GL_DITHER);
		glShadeModel(GL_FLAT);
	}

	DfxBounds DfxCPlane::bounds() {
		
		return planeBounds();
	}

	bool DfxCPlane::selectable() {
		return true;
	}

	void DfxCPlane::calculateBounds() {

	}

	void DfxCPlane::compute() {

		glm::vec3 new_vec = glm::vec3(0.f, 1.f, 0.f);

		glm::normalize(new_vec);

		glm::mat4 transMatrix = glm::mat4(1.0f);

		glm::mat4 rotmat;

		rotmat *= glm::rotate(glm::mat4(), glm::radians(getRotation().x), glm::vec3(1.f, .0f, .0f));
		rotmat *= glm::rotate(glm::mat4(), glm::radians(getRotation().y), glm::vec3(0.f, 1.f, .0f));
		rotmat *= glm::rotate(glm::mat4(), glm::radians(getRotation().z), glm::vec3(0.f, .0f, 1.f));

		transMatrix *= rotmat;

		new_vec = glm::vec3(transMatrix * glm::vec4(new_vec, 1.0f));

		((DfxEntityPropertyTypes::Float3*)getProperty(outNormal))->x = new_vec.x;
		((DfxEntityPropertyTypes::Float3*)getProperty(outNormal))->y = new_vec.y;
		((DfxEntityPropertyTypes::Float3*)getProperty(outNormal))->z = new_vec.z;

		((DfxEntityPropertyTypes::Float3*)getProperty(outPosition))->x = getPosition().x;
		((DfxEntityPropertyTypes::Float3*)getProperty(outPosition))->y = getPosition().y;
		((DfxEntityPropertyTypes::Float3*)getProperty(outPosition))->z = getPosition().z;

		cleanAllProperties();
		getDataInstance()->makeClean();
		makeChildrenDirty();
	}


	void DfxCPlane::init() {
	}

	void DfxCPlane::clear() {
	}

	void DfxCPlane::done() {
	}

	void DfxCPlane::load(std::ifstream &) {
	}

	void DfxCPlane::onDataChanged(DfxEntityDataInstance *instance) {
	}

	void DfxCPlane::importData(std::vector<std::pair<std::string, std::string>> data) {
	}

	std::vector<std::pair<std::string, std::string>> DfxCPlane::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxCPlane::isExportable() {
		return true;
	}

	void *DfxCPlane::creator(DfxGlobalEntityName uniquename) {
		return new DfxCPlane(uniquename);
	}

	void DfxCPlane::initialize() {
		DfxCSimulationEntity::initialize();

		DfxEntityDB::createProperty(outNormal, "out_normal", new DfxEntityPropertyTypes::Float3PropertyType);
		DfxEntityDB::createProperty(outPosition, "out_position", new DfxEntityPropertyTypes::Float3PropertyType);	
	}
}