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

#include <iostream>
#include <sstream>
#include "ParticleVisualizer.h"

using namespace std;

namespace rtx {

	int ParticleVisualizer::height = 400;
	int ParticleVisualizer::width = 400;

	int ParticleVisualizer::winid = 0;

	InitFunction ParticleVisualizer::ifunc = NULL;

	bool ParticleVisualizer::showNeighbourParticles = false;
	bool ParticleVisualizer::showFluidParticles = true;
	bool ParticleVisualizer::showBoundaryParticles = true;
	bool ParticleVisualizer::pause = false;
	bool ParticleVisualizer::capture = false;

	int    ParticleVisualizer::frame_counter = 0;
	
	glm::vec3 *ParticleVisualizer::_particleColors = NULL;
	int  ParticleVisualizer::selectedParticleIndex = 0;
	float ParticleVisualizer::particleRadius = 0.02f;

	std::vector<rtx::Mesh*> ParticleVisualizer::_objects;

	float ParticleVisualizer::boundaryColor[3] = { 1.0f, 1.0f, 0.0f };
	float ParticleVisualizer::granularColor[3] = { 1.0f, 0.0f, 1.0f };
	float ParticleVisualizer::selectedColor[3] = { 0.0f, 0.0f, 1.0f };
	float ParticleVisualizer::primaryColor[3] = { 1.0f, 0.0f, 0.0f };
	float ParticleVisualizer::neighbourColor[3] = { 0.0f, 1.0f, 0.0f };

	int    ParticleVisualizer::cameraType = CAMERA_PERSPECTIVE;
	double ParticleVisualizer::cameraLookAt[9] = { .4, 0.4, 0.5, 0.3, 0.3, 0, 0, 1, 0 };
	float  ParticleVisualizer::worldTranslate[3] = { 0.f, 0.6f, -1.f };
	float  ParticleVisualizer::lightDirection[3] = { -0.57735f, -0.57735f, -0.57735f };

	float ParticleVisualizer::rotx = 0.f;
	float ParticleVisualizer::roty = 0.f;
	float ParticleVisualizer::rotz = 0.f;

	int ParticleVisualizer::drawParticleType = DRAW_SPHERE_PARTICLE;

	SPHSystem *ParticleVisualizer::_sphSystem = NULL;

	void ParticleVisualizer::setShowNeighbourParticles(bool val) {
		showNeighbourParticles = val;
	}

	void ParticleVisualizer::setShowFluidParticles(bool val) {
		showFluidParticles = val;
	}

	void ParticleVisualizer::setShowBoundaryParticles(bool val) {
		showBoundaryParticles = val;
	}


	void ParticleVisualizer::initLighting() {
		float v[4];

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		v[0] = v[1] = v[2] = 1.f*0.4f; v[3] = 1.0f;
		glLightfv(GL_LIGHT0, GL_AMBIENT, v);
		v[0] = v[1] = v[2] = 1.5f*0.8f; v[3] = 1.0f;
		glLightfv(GL_LIGHT0, GL_DIFFUSE, v);
		v[0] = -lightDirection[0]; v[1] = -lightDirection[1]; v[2] = -lightDirection[2]; v[3] = 0.0f;
		glLightfv(GL_LIGHT0, GL_POSITION, v);
	}

	void ParticleVisualizer::initVisualiser() {

		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_BLEND);

		resize();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		initLighting();

		if (ifunc != NULL) ifunc();
	}

	void ParticleVisualizer::show(int argc, char **argv) {
		frame_counter = 0;

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(width, height);
		winid = glutCreateWindow("Position Based Dynamics");
		//glutIdleFunc(idle_func);
		glutKeyboardFunc(keyBoardFunc);
		glutDisplayFunc(display);

		initVisualiser();
		glutMainLoop();
	}

	void ParticleVisualizer::idle_func() {
		if (ifunc != NULL)
			ifunc();

		glutSetWindow(winid);
		glutPostRedisplay();
	}

	void ParticleVisualizer::keyBoardFunc(unsigned char key, int x, int y) {
		if (key == 'w' || key == 'W') {
			rotx += 0.8f;
		}
		else if (key == 'a' || key == 'A') {
			roty += 0.8f;
		}
		else if (key == 'd' || key == 'D') {
			roty -= 0.8f;
		}
		else if (key == 's' || key == 'S') {
			rotx -= 0.8f;
		}
		else if (key == 'b' || key == 'B') {
			showBoundaryParticles = (showBoundaryParticles) ? false : true;
		}
		else if (key == 'p' || key == 'P') {
			showFluidParticles = (showFluidParticles) ? false : true;
		}
		else if (key == 'z' || key == 'Z') {
			pause = (pause) ? false : true;
		}
		else if (key == 'g' || key == 'G') {
			selectedParticleIndex++;

			if (selectedParticleIndex >= _sphSystem->nbParticles()) selectedParticleIndex = 0;
		}
	}

	void ParticleVisualizer::timer(int) {
		idle_func();
	}

	void ParticleVisualizer::resize() {

		if (cameraType == CAMERA_ORTHO) {
			//float h = height;
			//float w = width;

			//float orthoX = 1.0f;
			//float orthoY = 1.0f;
			//float orthoZ = 1.0f;

			//GLfloat aspectRatio;
			//// Prevent a divide by zero
			//if (h == 0)
			//	h = 1;
			//// Set Viewport to window dimensions
			//glViewport(0, 0, w, h);

			//aspectRatio = (GLfloat)w / (GLfloat)h;

			//glMatrixMode(GL_PROJECTION);
			//glLoadIdentity();

			//if (w <= h) {
			//	glOrtho(-(orthoX), orthoX, -(orthoY) / aspectRatio, orthoY / aspectRatio,
			//		orthoZ, -(orthoZ));
			//}
			//else {
			//	glOrtho(-(orthoX)*aspectRatio, orthoX*aspectRatio, -(orthoY), orthoY,
			//		orthoZ, -(orthoZ));
			//}

			//glMatrixMode(GL_MODELVIEW);
			//glLoadIdentity();
		}
		else {
			glViewport(0, 0, width, height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(40, (double)width / height, 1, 20);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(cameraLookAt[0], cameraLookAt[1], cameraLookAt[2], cameraLookAt[3], cameraLookAt[4],
				cameraLookAt[5], cameraLookAt[6], cameraLookAt[7], cameraLookAt[8]);
			glTranslatef(worldTranslate[0], worldTranslate[1], worldTranslate[2]);

		}
	}

	void ParticleVisualizer::display(void) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPushMatrix();

		glRotatef(rotx, 1.0f, 0.0f, 0.0f);
		glRotatef(roty, 0.0f, 1.0f, 0.0f);
		glRotatef(rotz, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, -0.3f, 0.0f);

		glColor3f(0.f, 0.f, 0.f);

		if (_sphSystem != NULL) {
			clearParticleColors();

			if (showNeighbourParticles) {
				markParticleNeighbours();
			}

			markSelectedParticle();

			if (showFluidParticles) {
				for (int i = 0; i < _sphSystem->nbParticles(); i++) {
					if ((int)_sphSystem->type()[i] == FLUID_PARTICLE)
						drawParticle(_sphSystem->positions()[i], _particleColors[i]);
				}
			}

			if (showBoundaryParticles) {
				for (int i = 0; i < _sphSystem->nbParticles(); i++) {
					if (_sphSystem->type()[i] == BOUNDARY_PARTICLE)
						drawParticle(_sphSystem->positions()[i], _particleColors[i]);
				}
			}

			for (int i = 0; i < _objects.size(); i++) {
				renderWireframeObject(_objects[i]);
			}
			
		}

		glPopMatrix();

		/*if(capture){
			stringstream ss;
			ss << capturePath << "/output_"<< frame_counter++ <<".png";
			screenshot_png(ss.str().c_str(), width, height, &pixels, &png_bytes, &png_rows);
			free_screenshot_buffers();
		}*/

		glutSwapBuffers();
		glutTimerFunc(1000 / 30, timer, 0);
	}

	void ParticleVisualizer::setSPHSystem(SPHSystem *sys) {
		_sphSystem = sys;
	}

	void ParticleVisualizer::setInitfunction(InitFunction _ifunc) {
		ifunc = _ifunc;
	}

	void ParticleVisualizer::setWidth(int w) {
		width = w;
	}

	void ParticleVisualizer::setHeight(int h) {
		height = h;
	}

	void ParticleVisualizer::drawParticle(glm::vec3 pos, glm::vec3 color) {

		GLfloat colorAmbient[] = { 0.5, 0.5, 0.5 };
		GLfloat colorDiffuse[] = { color.x, color.y, color.z};

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colorAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorDiffuse);
		
		glPushMatrix();

		if (drawParticleType == DRAW_POINT_PARTICLE) {
			glPointSize(2.0f);
			glBegin(GL_POINTS);
			glVertex3f(pos.x, pos.y, pos.z);
			glEnd();
		}
		else if (drawParticleType == DRAW_SPHERE_PARTICLE) {
			glTranslatef(pos.x, pos.y, pos.z);
			glutSolidSphere(particleRadius, 12, 12);
		}

		glPopMatrix();

	}

	void ParticleVisualizer::initParticleColors() {
		_particleColors = new glm::vec3[_sphSystem->nbParticles()];
	}

	void ParticleVisualizer::clearParticleColors() {

		for (int i = 0; i < _sphSystem->nbParticles(); i++) {

			if (_sphSystem->type()[i] == FLUID_PARTICLE) {
				_particleColors[i].x = primaryColor[0];
				_particleColors[i].y = primaryColor[1];
				_particleColors[i].z = primaryColor[2];
			}
			else {
				_particleColors[i].x = boundaryColor[0];
				_particleColors[i].y = boundaryColor[1];
				_particleColors[i].z = boundaryColor[2];
			}

		}

	}

	void ParticleVisualizer::markParticleNeighbours() {

		//cout << "Selected Particle: " << selectedParticleIndex << endl;
		//cout << "NB Count: " << particles[selectedParticleIndex]->nbCount << endl;

		auto nbListOffset = selectedParticleIndex * _sphSystem->nbListSize();
		for (int j = 0; j < _sphSystem->nbCount()[selectedParticleIndex]; j++) {

			_particleColors[_sphSystem->neighbours()[nbListOffset + j]].x = neighbourColor[0];
			_particleColors[_sphSystem->neighbours()[nbListOffset + j]].y = neighbourColor[1];
			_particleColors[_sphSystem->neighbours()[nbListOffset + j]].z = neighbourColor[2];

		}
	}

	void ParticleVisualizer::markSelectedParticle() {

		_particleColors[selectedParticleIndex].x = selectedColor[0];
		_particleColors[selectedParticleIndex].y = selectedColor[1];
		_particleColors[selectedParticleIndex].z = selectedColor[2];

	}

	void ParticleVisualizer::setParticleRadius(float pr) {
		particleRadius = pr;
	}

	void ParticleVisualizer::addObject(rtx::Mesh *msh) {
		_objects.push_back(msh);
	}

	void ParticleVisualizer::renderWireframeObject(rtx::Mesh *msh) {

		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.f);

		glBegin(GL_TRIANGLES);
		for (size_t inx = 0; inx < msh->nbFaces; inx++) {
			size_t offset = inx * 3;
			glVertex3f(msh->verts[msh->indices[offset]].x, msh->verts[msh->indices[offset]].y, msh->verts[msh->indices[offset]].z);
			glVertex3f(msh->verts[msh->indices[offset + 1]].x, msh->verts[msh->indices[offset + 1]].y, msh->verts[msh->indices[offset + 1]].z);
			glVertex3f(msh->verts[msh->indices[offset + 2]].x, msh->verts[msh->indices[offset + 2]].y, msh->verts[msh->indices[offset + 2]].z);
		}
		glEnd();

		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		/*glDisable(GL_BLEND);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(1.f);*/

		/*initLighting();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_BLEND);*/
		//glEnable(GL_LIGHTING);
	}
}