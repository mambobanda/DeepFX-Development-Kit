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

#pragma once

#include <GL/glut.h>
#include <vector>
#include "RtxMesh.h"
#include "RtxCommon.h"
#include "RtxSPHSystem.h"

namespace rtx {

	enum { CAMERA_ORTHO, CAMERA_PERSPECTIVE, DRAW_POINT_PARTICLE, DRAW_SPHERE_PARTICLE };

	typedef void(*InitFunction)();

	class ParticleVisualizer {
	private:
		static bool showNeighbourParticles;
		static bool showFluidParticles;
		static bool showBoundaryParticles;
		static bool pause;

		static int frame_counter;

		static int winid;
		static int selectedParticleIndex;
		static int height;
		static int width;
		static float particleRadius;
		static float rotx;
		static float roty;
		static float rotz;

		static float boundaryColor[3];
		static float granularColor[3];
		static float selectedColor[3];
		static float primaryColor[3];
		static float neighbourColor[3];

		static int    drawParticleType;
		static int    cameraType;
		static double cameraLookAt[9];
		static float  worldTranslate[3];
		static float  lightDirection[3];

		static InitFunction ifunc;
		static SPHSystem *_sphSystem;
		static glm::vec3 *_particleColors;

		static std::vector<rtx::Mesh*> _objects;

		static bool capture;

		static void markParticleNeighbours();
		static void markSelectedParticle();
		static void clearParticleColors();
		static void drawParticle(glm::vec3 pos, glm::vec3 color);

		static void initLighting();
		static void initVisualiser();
	public:

		static void setInitfunction(InitFunction);
		static void setWidth(int);
		static void setHeight(int);
		static void setSPHSystem(SPHSystem *sys);

		static void show(int argc, char **argv);
		static void timer(int);

		static void idle_func();
		static void keyBoardFunc(unsigned char key, int x, int y);
		static void display(void);
		static void resize();

		static void setShowNeighbourParticles(bool val);
		static void setShowFluidParticles(bool val);
		static void setShowBoundaryParticles(bool val);
		static void initParticleColors();
		static void setParticleRadius(float particleRadius);

		static void addObject(rtx::Mesh *msh);
		static void renderWireframeObject(rtx::Mesh *msh);
	};
}