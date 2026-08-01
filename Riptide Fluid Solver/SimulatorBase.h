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


#include <glm\vec3.hpp>
#include <vector>
#include "RtxCommon.h"
#include "RtxSPHSystem.h"

using namespace std;
using namespace glm;

namespace rtx {

	class SimulatorBase {
	private:

		vector<glm::vec3> _positions;
		vector<uint8_t>   _types;
		SPHSystem *_sphSystem;

	public:
		SimulatorBase();

		void createBoundaryBox3D(float radius, float width, float height, float depth, int top);
		void createDamBreakParticleSystem3D(vec3 pos, int width, int height, int depth, float radius, float spacing);
		void createObstacle3D(vec3 pos, int width, int height, int depth, float radius, float spacing);
		void createParticleSystem();

		vector<glm::vec3> &positions();

		SPHSystem *sphSystem();
	};

}