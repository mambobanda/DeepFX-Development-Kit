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
#include "DfxCSPHShapeEmitter.h"


namespace dfxcore {

	namespace sph {

		SPHShapeEmitter::SPHShapeEmitter(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename, DFX_CUSTOM_ITEM) {
			setLogicalName("SPH Shape Emitter");

			DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outID);
			uid->setRootID(getInstanceID());

			setPropertyState(inParticleCount, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inParticleRadius, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inShape, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inStrength, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inURandomness, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inVRandomness, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inCircleRadius, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inRectWidth, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inRectHeight, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(outID, DfxEntityPropertyState::OUTPUT_STATE);

			((DfxEnum*)getProperty(inShape))->add("Circle");
			((DfxEnum*)getProperty(inShape))->add("Rectangle");

			*(size_t*)getProperty(outID) = getInstanceID();


			initialVelocity = glm::vec3(0.f, 1.f, 0.f);
			randomnessCounter = 1;

			initDefualtValues();
		}

		SPHShapeEmitter::~SPHShapeEmitter() {

		}


		void SPHShapeEmitter::drawCircleEmitter(float lnwdth) {

			float radius = *(float*)getProperty(inCircleRadius);

			double x_circ[31];
			double y_circ[31];

			double scale = 0.25 * radius;
			double halfwidth = scale * 0.5f;

			for (int i = 0; i <= 30; i++) {
				double angle = 2 * xPI*i / 30;
				double x = cos(angle)*radius;
				double y = sin(angle)*radius;

				x_circ[i] = x;
				y_circ[i] = y;
			}

			for (int i = 0; i <= 29; i++) {

				double pb1[] = { x_circ[i],   0.0, y_circ[i] };
				double pb2[] = { x_circ[i + 1], 0.0, y_circ[i + 1] };

				glDisable(GL_LIGHTING);
				glLineWidth(lnwdth);
				glBegin(GL_LINES);
				glVertex3f(pb1[0], pb1[1], pb1[2]);
				glVertex3f(pb2[0], pb2[1], pb2[2]);
				glEnd();
				glLineWidth(1.f);
				glEnable(GL_LIGHTING);
			}

			double v1[] = { 0.0, radius*2.0, 0.0 };
			double v2[] = { 0.0, radius*2.0, 0.0 };
			double v3[] = { 0.0, radius*2.0, 0.0 };
			double v4[] = { 0.0, radius*2.0, 0.0 };

			for (int i = 0; i <= 15; i++) {
				double angle = 2 * xPI*i / 15;
				double x = cos(angle)*halfwidth;
				double y = sin(angle)*halfwidth;

				x_circ[i] = x;
				y_circ[i] = y;

				if (i == 0) {
					v1[0] = x;
					v1[2] = y;
				}

				if (i == 4) {
					v2[0] = x;
					v2[2] = y;
				}

				if (i == 7) {
					v3[0] = x;
					v3[2] = y;
				}

				if (i == 12) {
					v4[0] = x;
					v4[2] = y;
				}
			}

			for (int i = 0; i <= 14; i++) {

				double pb1[] = { x_circ[i],   radius * 2, y_circ[i] };
				double pb2[] = { x_circ[i + 1], radius * 2, y_circ[i + 1] };

				glDisable(GL_LIGHTING);
				glLineWidth(lnwdth);
				glBegin(GL_LINES);
				glVertex3f(pb1[0], pb1[1], pb1[2]);
				glVertex3f(pb2[0], pb2[1], pb2[2]);
				glEnd();
				glLineWidth(1.f);
				glEnable(GL_LIGHTING);
		
			}

			double ln1[] = { 0.0, 0.0f, 0.0f };
			double ln2[] = { 0.0, radius*2.0, 0.0 };

			glDisable(GL_LIGHTING);
			glLineWidth(lnwdth);
			glBegin(GL_LINES);
			glVertex3f(ln1[0], ln1[1], ln1[2]);
			glVertex3f(ln2[0], ln2[1], ln2[2]);
			glEnd();
			glLineWidth(1.f);
			glEnable(GL_LIGHTING);


			double v5[] = { 0.0,(radius*2.0) + scale, 0.0 };

			glDisable(GL_LIGHTING);
			glLineWidth(lnwdth);
			glBegin(GL_LINES);
			glVertex3f(v1[0], v1[1], v1[2]); 
			glVertex3f(v5[0], v5[1], v5[2]);
			glVertex3f(v2[0], v2[1], v2[2]);
			glVertex3f(v5[0], v5[1], v5[2]);
			glVertex3f(v3[0], v3[1], v3[2]); 
			glVertex3f(v5[0], v5[1], v5[2]);
			glVertex3f(v4[0], v4[1], v4[2]);
			glVertex3f(v5[0], v5[1], v5[2]);
			glEnd();
			glLineWidth(1.f);
			glEnable(GL_LIGHTING);
		}

		void SPHShapeEmitter::drawRectangleEmitter(float lnwdth) {
			float width  = *(float*)getProperty(inRectWidth);
			float height = *(float*)getProperty(inRectHeight);

			float halfwidth  = width;
			float halfheight = height;

			float sfactor = 0.25f;

			double verts[][3] = { { -halfwidth, 0.0f, halfheight },
			{ -halfwidth, 0.0f,-halfheight },
			{ halfwidth, 0.0f,-halfheight },
			{ halfwidth, 0.0f, halfheight } };

			glDisable(GL_LIGHTING);
			glLineWidth(lnwdth);
			glBegin(GL_LINES);
			glVertex3f(verts[0][0], verts[0][1], verts[0][2]);
			glVertex3f(verts[1][0], verts[1][1], verts[1][2]);
			glVertex3f(verts[1][0], verts[1][1], verts[1][2]);
			glVertex3f(verts[2][0], verts[2][1], verts[2][2]);
			glVertex3f(verts[2][0], verts[2][1], verts[2][2]);
			glVertex3f(verts[3][0], verts[3][1], verts[3][2]);
			glVertex3f(verts[3][0], verts[3][1], verts[3][2]);
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

			for (int i = 0; i <= 15; i++) {
				double angle = 2 * xPI*i / 15;
				double x = cos(angle)*0.0625f*(width + height);
				double y = sin(angle)*0.0625f*(width + height);

				x_circ[i] = x;
				y_circ[i] = y;

				if (i == 0) {
					v1[0] = x;
					v1[2] = y;
				}

				if (i == 4) {
					v2[0] = x;
					v2[2] = y;
				}

				if (i == 7) {
					v3[0] = x;
					v3[2] = y;
				}

				if (i == 12) {
					v4[0] = x;
					v4[2] = y;
				}
			}

			for (int i = 0; i <= 14; i++) {

				double pb1[] = { x_circ[i],   (width + height), y_circ[i] };
				double pb2[] = { x_circ[i + 1], (width + height), y_circ[i + 1] };

				glDisable(GL_LIGHTING);
				glLineWidth(lnwdth);
				glBegin(GL_LINES);
				glVertex3f(pb1[0], pb1[1], pb1[2]);
				glVertex3f(pb2[0], pb2[1], pb2[2]);
				glEnd();
				glLineWidth(1.f);
				glEnable(GL_LIGHTING);

			}

			double ln1[] = { 0.0, 0.0f, 0.0f };
			double ln2[] = { 0.0, (width + height), 0.0 };

			glDisable(GL_LIGHTING);
			glLineWidth(lnwdth);
			glBegin(GL_LINES);
			glVertex3f(ln1[0], ln1[1], ln1[2]);
			glVertex3f(ln2[0], ln2[1], ln2[2]);
			glEnd();
			glLineWidth(1.f);
			glEnable(GL_LIGHTING);

			double v5[] = { 0.0,((width + height)) + (width + height)*0.0625f, 0.0 };

			glDisable(GL_LIGHTING);
			glLineWidth(lnwdth);
			glBegin(GL_LINES);
			glVertex3f(v1[0], v1[1], v1[2]);
			glVertex3f(v5[0], v5[1], v5[2]);
			glVertex3f(v2[0], v2[1], v2[2]);
			glVertex3f(v5[0], v5[1], v5[2]);
			glVertex3f(v3[0], v3[1], v3[2]);
			glVertex3f(v5[0], v5[1], v5[2]);
			glVertex3f(v4[0], v4[1], v4[2]);
			glVertex3f(v5[0], v5[1], v5[2]);
			glEnd();
			glLineWidth(1.f);
			glEnable(GL_LIGHTING);
		}

		DfxBounds SPHShapeEmitter::circleEmitterBounds() {
			DfxBounds bounds;

			glm::mat4 transMat = glm::translate(glm::mat4(), glm::vec3(getPosition().x, getPosition().y, getPosition().z));

			transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().x), glm::vec3(1.f, .0f, .0f));
			transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().y), glm::vec3(0.f, 1.f, .0f));
			transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().z), glm::vec3(0.f, .0f, 1.f));

			float radius = *(float*)getProperty(inCircleRadius);

			double x_circ[31];
			double y_circ[31];

			double scale = 0.25 * radius;
			double halfwidth = scale * 0.5f;

			for (int i = 0; i <= 30; i++) {
				double angle = 2 * xPI*i / 30;
				double x = cos(angle)*radius;
				double y = sin(angle)*radius;

				x_circ[i] = x;
				y_circ[i] = y;
			}

			for (int i = 0; i <= 30; i++) {

				double pb1[] = { x_circ[i],   0.0, y_circ[i] };
				double pb2[] = { x_circ[i + 1], 0.0, y_circ[i + 1] };
				
				bounds.add(pb1[0], pb1[1], pb1[2], transMat);
				bounds.add(pb2[0], pb2[1], pb2[2], transMat);

			}

			double v1[] = { 0.0, radius*2.0, 0.0 };
			double v2[] = { 0.0, radius*2.0, 0.0 };
			double v3[] = { 0.0, radius*2.0, 0.0 };
			double v4[] = { 0.0, radius*2.0, 0.0 };

			for (int i = 0; i <= 15; i++) {
				double angle = 2 * xPI*i / 15;
				double x = cos(angle)*halfwidth;
				double y = sin(angle)*halfwidth;

				x_circ[i] = x;
				y_circ[i] = y;

				if (i == 0) {
					v1[0] = x;
					v1[2] = y;
				}

				if (i == 4) {
					v2[0] = x;
					v2[2] = y;
				}

				if (i == 7) {
					v3[0] = x;
					v3[2] = y;
				}

				if (i == 12) {
					v4[0] = x;
					v4[2] = y;
				}
			}

			for (int i = 0; i <= 14; i++) {

				double pb1[] = { x_circ[i],   radius * 2, y_circ[i] };
				double pb2[] = { x_circ[i + 1], radius * 2, y_circ[i + 1] };

				bounds.add(pb1[0], pb1[1], pb1[2], transMat);
				bounds.add(pb2[0], pb2[1], pb2[2], transMat);
				
			}

			double ln1[] = { 0.0, 0.0f, 0.0f };
			double ln2[] = { 0.0, radius*2.0, 0.0 };


			bounds.add(ln1[0], ln1[1], ln1[2], transMat);
			bounds.add(ln2[0], ln2[1], ln2[2], transMat);
			
	
			double v5[] = { 0.0,(radius*2.0) + scale, 0.0 };

			bounds.add(v1[0], v1[1], v1[2], transMat);
			bounds.add(v5[0], v5[1], v5[2], transMat);
			bounds.add(v2[0], v2[1], v2[2], transMat);
			bounds.add(v5[0], v5[1], v5[2], transMat);
			bounds.add(v3[0], v3[1], v3[2], transMat);
			bounds.add(v5[0], v5[1], v5[2], transMat);
			bounds.add(v4[0], v4[1], v4[2], transMat);
			bounds.add(v5[0], v5[1], v5[2], transMat);
			
			return bounds;
		}

		DfxBounds SPHShapeEmitter::rectangleEmitterBounds() {
			DfxBounds bounds;

			glm::mat4 transMat = glm::translate(glm::mat4(), glm::vec3(getPosition().x, getPosition().y, getPosition().z));

			transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().x), glm::vec3(1.f, .0f, .0f));
			transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().y), glm::vec3(0.f, 1.f, .0f));
			transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().z), glm::vec3(0.f, .0f, 1.f));

			float width = *(float*)getProperty(inRectWidth);
			float height = *(float*)getProperty(inRectHeight);

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

			for (int i = 0; i <= 15; i++) {
				double angle = 2 * xPI*i / 15;
				double x = cos(angle)*0.0625f*(width + height);
				double y = sin(angle)*0.0625f*(width + height);

				x_circ[i] = x;
				y_circ[i] = y;

				if (i == 0) {
					v1[0] = x;
					v1[2] = y;
				}

				if (i == 4) {
					v2[0] = x;
					v2[2] = y;
				}

				if (i == 7) {
					v3[0] = x;
					v3[2] = y;
				}

				if (i == 12) {
					v4[0] = x;
					v4[2] = y;
				}
			}

			for (int i = 0; i <= 14; i++) {

				double pb1[] = { x_circ[i],   (width + height), y_circ[i] };
				double pb2[] = { x_circ[i + 1], (width + height), y_circ[i + 1] };

				bounds.add(pb1[0], pb1[1], pb1[2], transMat);
				bounds.add(pb2[0], pb2[1], pb2[2], transMat);
			
			}

			double ln1[] = { 0.0, 0.0f, 0.0f };
			double ln2[] = { 0.0, (width + height), 0.0 };

			bounds.add(ln1[0], ln1[1], ln1[2], transMat);
			bounds.add(ln2[0], ln2[1], ln2[2], transMat);

			double v5[] = { 0.0,((width + height)) + (width + height)*0.0625f, 0.0 };

			bounds.add(v1[0], v1[1], v1[2], transMat);
			bounds.add(v5[0], v5[1], v5[2], transMat);
			bounds.add(v2[0], v2[1], v2[2], transMat);
			bounds.add(v5[0], v5[1], v5[2], transMat);
			bounds.add(v3[0], v3[1], v3[2], transMat);
			bounds.add(v5[0], v5[1], v5[2], transMat);
			bounds.add(v4[0], v4[1], v4[2], transMat);
			bounds.add(v5[0], v5[1], v5[2], transMat);

			return bounds;
		}

		void SPHShapeEmitter::draw() {
			initializeOpenGLFunctions();

			int source = ((DfxEnum*)getProperty(inShape))->getCurrentIndex();

			glPushMatrix();

			glm::mat4 transMat = glm::translate(glm::mat4(), glm::vec3(getPosition().x, getPosition().y, getPosition().z));

			transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().x), glm::vec3(1.f, .0f, .0f));
			transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().y), glm::vec3(0.f, 1.f, .0f));
			transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().z), glm::vec3(0.f, .0f, 1.f));

			glMultMatrixf(&transMat[0][0]);

			glColor3f(1.f, 1.f, 1.f);
			if (source == 0) {
				drawCircleEmitter(2.f);
			}
			else if (source == 1) {
				drawRectangleEmitter(2.f);
			}
			glPopMatrix();
		}

		void SPHShapeEmitter::drawSelection() {

			int source = ((DfxEnum*)getProperty(inShape))->getCurrentIndex();

			glPushMatrix();

			glm::mat4 transMat = glm::translate(glm::mat4(), glm::vec3(getPosition().x, getPosition().y, getPosition().z));

			transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().x), glm::vec3(1.f, .0f, .0f));
			transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().y), glm::vec3(0.f, 1.f, .0f));
			transMat *= glm::rotate(glm::mat4(), glm::radians(getRotation().z), glm::vec3(0.f, .0f, 1.f));

			glMultMatrixf(&transMat[0][0]);

			glDisable(GL_LIGHTING);
			glDisable(GL_DITHER);
			glShadeModel(GL_FLAT);
			
			glColor3f(colorID.redf(), colorID.greenf(), colorID.bluef());
			if (source == 0) {
				drawCircleEmitter(15.f);
			}
			else if (source == 1) {
				drawRectangleEmitter(15.f);
			}
			glPopMatrix();

			glEnable(GL_LIGHTING);
			glEnable(GL_DITHER);
			glShadeModel(GL_FLAT);
		}

		DfxBounds SPHShapeEmitter::bounds() {
			int source = ((DfxEnum*)getProperty(inShape))->getCurrentIndex();

			glColor3f(1.f, 1.f, 1.f);
			if (source == 0) {
				return circleEmitterBounds();
			}
			else if (source == 1) {
				return rectangleEmitterBounds();
			}

			return DfxBounds();
		}

		bool SPHShapeEmitter::selectable() {
			return true;
		}

		void SPHShapeEmitter::calculateBounds() {

		}

		void SPHShapeEmitter::compute() {
			
			size_t pcount = (size_t)*(float*)getProperty(inParticleCount);

			if (pcount > 0) {

				if (particles.size() > 0)
					for (auto p : particles) delete p;

				for (size_t i = 0; i < pcount; i++) {

					Particle *p = new Particle(FLUID_PARTICLE);
					p->setAlive(false);

					particles.push_back(p);
				}

				makePropertyBindingDirty(outID);
			}

			cleanAllProperties();
			getDataInstance()->makeClean();
			makeChildrenDirty();
		}

		size_t SPHShapeEmitter::particleCount() {
			return particles.size();
		}

		Particle *SPHShapeEmitter::getParticle(size_t index) {
			return particles[index];
		}

		void SPHShapeEmitter::init() {
			distanceCounter = *(float*)getProperty(inParticleRadius) * 2.f;
			initEmissionPositions();
		}

		void SPHShapeEmitter::clear() {

		}

		void SPHShapeEmitter::done() {
			if (particles.size() > 0)
				for (auto p : particles) delete p;

			particles.clear();

			getDataInstance()->makeDirty();
		}

		void SPHShapeEmitter::load(std::ifstream &) {
			*(size_t*)getProperty(outID) = getInstanceID();
		}

		void SPHShapeEmitter::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void SPHShapeEmitter::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> SPHShapeEmitter::exportData() {
			return mapEntityProperties(this);
		}

		bool SPHShapeEmitter::isExportable() {
			return true;
		}

		void *SPHShapeEmitter::creator(DfxGlobalEntityName uniquename) {
			return new SPHShapeEmitter(uniquename);
		}

		void SPHShapeEmitter::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outID, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inParticleRadius, "particle_radius", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inParticleCount, "particle_count", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inShape, "shape", new DfxEntityPropertyTypes::DfxEnumPropertyType());
			DfxEntityDB::createProperty(inStrength, "strength", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inURandomness, "u_randomness", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inVRandomness, "v_randomness", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inCircleRadius, "circle_radius", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inRectWidth, "rectangle_width", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inRectHeight, "rectandle_height", new DfxEntityPropertyTypes::FloatPropertyType());
		}

		void SPHShapeEmitter::initEmissionPositions() {
			int source = ((DfxEnum*)getProperty(inShape))->getCurrentIndex();

			if (source == 0) {
				generateCirclePositions();
			}
			else if (source == 1) {
				generateRectanglePositions();
			}
		}

		void SPHShapeEmitter::generateCirclePositions() {
			emissionPositions.clear();

			float circleRadius = *(float*)getProperty(inCircleRadius);
			float particleDiameter = *(float*)getProperty(inParticleRadius) * 2.f;

			if (circleRadius < (particleDiameter))
				circleRadius = (particleDiameter);

			int count = (int)(circleRadius / (float)particleDiameter);
			int counter = 0;

			for (int i = 0; i < count; i++) {

				if (i == 0) {
					emissionPositions.push_back(glm::vec3(0.0f));
					continue;
				}

				float partialRadius = circleRadius / (float)count;
				float radius = partialRadius*i;

				float circum = 2.0f*xPI*radius;

				int segments = (int)(circum / (float)particleDiameter);

				float *x_circ = new float[segments];
				float *y_circ = new float[segments];

				for (int j = 0; j < segments; j++) {
					double angle = 2.0*xPI*j / segments;
					double x = cos(angle)*radius;
					double y = sin(angle)*radius;

					x_circ[j] = x;
					y_circ[j] = y;
				}

				for (int j = 0; j < segments; j++) {
					glm::vec3 pos(x_circ[j], 0.0f, y_circ[j]);
					emissionPositions.push_back(pos);
				}

				delete[] x_circ;
				delete[] y_circ;
			}
		}

		void SPHShapeEmitter::generateRectanglePositions() {
			emissionPositions.clear();

			float squareWidth  = *(float*)getProperty(inRectWidth);
			float squareHeight = *(float*)getProperty(inRectHeight);

			float particleDiameter = *(float*)getProperty(inParticleRadius) * 2.f;

			if (squareWidth < (particleDiameter))
				squareWidth = (particleDiameter);

			if (squareHeight < (particleDiameter))
				squareHeight = (particleDiameter);

			int wcount = (int)((squareWidth  * 2.0f) / (float)particleDiameter);
			int hcount = (int)((squareHeight * 2.0f) / (float)particleDiameter);

			float hwidth  = -squareWidth;
			float hheight = -squareHeight;

			for (int i = 0; i < wcount; i++) {
				for (int j = 0; j < hcount; j++) {
					emissionPositions.push_back(glm::vec3(hwidth, 0.0f, hheight));

					hheight += particleDiameter;
				}

				hheight = -squareHeight;
				hwidth += particleDiameter;
			}
		}

		glm::vec3 SPHShapeEmitter::uvwRandomness(glm::vec3 pos) {

			int source = ((DfxEnum*)getProperty(inShape))->getCurrentIndex();

			float uDisplacement = *(float*)getProperty(inURandomness);
			float vDisplacement = *(float*)getProperty(inVRandomness);

			if (source == 0 || source == 1) {

				if (uDisplacement == 0.f && vDisplacement == 0.f)
					return pos;

				std::time_t time = std::time(nullptr);
				std::uniform_real_distribution<> uDisp(-(90.0f * uDisplacement), (90.0f * uDisplacement));
				std::uniform_real_distribution<> vDisp(-(90.0f * vDisplacement), (90.0f * vDisplacement));

				unsigned int k1 = (unsigned int)(randomnessCounter * time) + 10;
				unsigned int k2 = (unsigned int)(randomnessCounter * time) + 20;

				std::default_random_engine genu(k1);
				std::default_random_engine genv(k2);

				float posu = (float)uDisp(genu);
				float posv = (float)vDisp(genv);

				glm::mat4 uRotMat = glm::rotate(posu, glm::vec3(1.0f, 0.0f, 0.0f));
				uRotMat = glm::rotate(posv, glm::vec3(0.0f, 0.0f, 1.0f));

				glm::mat4 transMatrix = glm::mat4(1.0f);
				
				transMatrix *= uRotMat;

				if (randomnessCounter == 1000000)
					randomnessCounter = 1;
				else
					randomnessCounter++;

				return glm::vec3(transMatrix * glm::vec4(pos, 1.0f));
			}

			return pos;
		}

		glm::vec3 SPHShapeEmitter::getFinalVelocity() {

			glm::vec3 new_vec = initialVelocity;

			new_vec = uvwRandomness(new_vec);

			glm::normalize(new_vec);

			new_vec *= *(float*)getProperty(inStrength);

			glm::mat4 transMatrix = glm::mat4(1.0f);

			glm::mat4 rotmat;

			rotmat *= glm::rotate(glm::mat4(), glm::radians(getRotation().x), glm::vec3(1.f, .0f, .0f));
			rotmat *= glm::rotate(glm::mat4(), glm::radians(getRotation().y), glm::vec3(0.f, 1.f, .0f));
			rotmat *= glm::rotate(glm::mat4(), glm::radians(getRotation().z), glm::vec3(0.f, .0f, 1.f));

			transMatrix *= rotmat;

			return glm::vec3(transMatrix * glm::vec4(new_vec, 1.0f));
		}

		glm::mat4 SPHShapeEmitter::getFinalTransformation() {
			return transformationMatrix();
		}

		void SPHShapeEmitter::advanceEmitter(glm::vec3 *vi) {

			float uDisplacement = *(float*)getProperty(inURandomness);
			float vDisplacement = *(float*)getProperty(inVRandomness);

			int positionCounter = 0;
			glm::vec3 finalVel = glm::vec3(0.0f);

			if (uDisplacement == 0.f && vDisplacement == 0.f)
				finalVel = getFinalVelocity();

			glm::mat4 transformation = getFinalTransformation();
			for (int i = 0; i < particles.size(); i++) {

				if (!particles[i]->isAlive()) {

					if (uDisplacement > 0.f || vDisplacement > 0.f)
						finalVel = getFinalVelocity();

					particles[i]->setAlive(true);
					particles[i]->x = glm::vec3(transformation * glm::vec4(emissionPositions[positionCounter], 1.0f));
					vi[particles[i]->getIndex()] = finalVel;
					particles[i]->v = finalVel;

					positionCounter++;
				}

				if (positionCounter == emissionPositions.size())
					break;
			}
		}

		void SPHShapeEmitter::step(float timestep, glm::vec3 *vi) {
			if (*(float*)getProperty(inParticleCount) <= 0.f) return;
			
			float particleDiameter = *(float*)getProperty(inParticleRadius) * 2.f;
			float strength = *(float*)getProperty(inStrength);

			if (distanceCounter >= particleDiameter) {

				advanceEmitter(vi);
				
				distanceCounter = 0.0;
			}
			else {
				distanceCounter += (strength * timestep);
			}
		}

		void SPHShapeEmitter::initDefualtValues() {

			*(float*)getProperty(inParticleCount)  = 10000.f;
			*(float*)getProperty(inParticleRadius) = 0.0225;
			*(float*)getProperty(inStrength)     = 10.f;
			*(float*)getProperty(inURandomness)  = 0.0f;
			*(float*)getProperty(inVRandomness)  = 0.0f;
			*(float*)getProperty(inCircleRadius) = .5f;
			*(float*)getProperty(inRectWidth)  = .5f;
			*(float*)getProperty(inRectHeight) = .5f;
		}
	}
}