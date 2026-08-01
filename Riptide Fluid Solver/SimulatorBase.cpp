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

#include "SimulatorBase.h"

namespace rtx {

	SimulatorBase::SimulatorBase() {
		_sphSystem = new SPHSystem;
	}

	void SimulatorBase::createObstacle3D(vec3 pos, int width, int height, int depth, float radius, float spacing) {
		float diameter = spacing;

		float a = pos.x;
		float b = pos.y;
		float c = pos.z;

		// Set material
		for (int u = 0; u < width; u++) {
			for (int v = 0; v < height; v++) {
				for (int w = 0; w < depth; w++) {

					_positions.push_back(vec3(a, b, c));
					_types.push_back(BOUNDARY_PARTICLE);

					c += diameter;
				}
				c = pos.z;
				b += diameter;
			}

			c = pos.z;
			b = pos.y;
			a += diameter;
		}
	}

	void SimulatorBase::createDamBreakParticleSystem3D(vec3 pos, int width, int height, int depth, float radius, float spacing) {
		float diameter = spacing;

		float a = pos.x;
		float b = pos.y;
		float c = pos.z;

		// Set material
		for (int u = 0; u < width; u++) {
			for (int v = 0; v < height; v++) {
				for (int w = 0; w < depth; w++) {

					_positions.push_back(vec3(a, b, c));
					_types.push_back(FLUID_PARTICLE);

					c += diameter;
				}
				c = pos.z;
				b += diameter;
			}

			c = pos.z;
			b = pos.y;
			a += diameter;
		}
	}

	void SimulatorBase::createBoundaryBox3D(float radius, float width, float height, float depth, int top) {

		float diameter = radius + radius;
		int xcount = (int)(width / (diameter)+1.f);
		int ycount = (int)(height / (diameter)+1.f);
		int zcount = (int)(depth / (diameter)+1.f);

		float a = width * -0.5f;
		float b = height * -0.5f;

		// back
		for (int u = 0; u < xcount; u++) {
			for (int v = 0; v < ycount; v++) {

				_positions.push_back(vec3(a, b, -(0.5f*depth)));
				_types.push_back(BOUNDARY_PARTICLE);

				b += diameter;
			}

			b = height * -0.5f;
			a += diameter;
		}

		a = width * -0.5f;
		b = height * -0.5f;

		// front
		for (int u = 0; u < xcount; u++) {
			for (int v = 0; v < ycount; v++) {

				_positions.push_back(vec3(a, b, (0.5f*depth)));
				_types.push_back(BOUNDARY_PARTICLE);

				b += diameter;
			}

			b = height * -0.5f;
			a += diameter;
		}


		a = depth * -0.5f;
		b = height * -0.5f;

		//left
		for (int u = 0; u < zcount; u++) {
			for (int v = 0; v < ycount; v++) {

				_positions.push_back(vec3(-(0.5f*width), b, a));
				_types.push_back(BOUNDARY_PARTICLE);

				b += diameter;
			}

			b = height * -0.5f;
			a += diameter;
		}

		a = depth * -0.5f;
		b = height * -0.5f;

		//right
		for (int u = 0; u < zcount; u++) {
			for (int v = 0; v < ycount; v++) {

				_positions.push_back(vec3((0.5f*width), b, a));
				_types.push_back(BOUNDARY_PARTICLE);

				b += diameter;
			}

			b = height * -0.5f;
			a += diameter;
		}

		a = width * -0.5f;
		b = depth * -0.5f;

		//bottom
		for (int u = 0; u < xcount; u++) {
			for (int v = 0; v < zcount; v++) {

				_positions.push_back(vec3(a, -(0.5f*height), b));
				_types.push_back(BOUNDARY_PARTICLE);

				b += diameter;
			}

			b = depth * -0.5f;
			a += diameter;
		}

		a = width * -0.5f;
		b = depth * -0.5f;

		if (top) {
			for (int u = 0; u < xcount; u++) {
				for (int v = 0; v < zcount; v++) {

					_positions.push_back(vec3(a, (0.5f*height), b));
					_types.push_back(BOUNDARY_PARTICLE);

					b += diameter;
				}

				b = depth * -0.5f;
				a += diameter;
			}
		}
	}

	void SimulatorBase::createParticleSystem() {
		_sphSystem->clearResize(_positions.size());

		for (int i = 0; i < _positions.size(); i++) {
			_sphSystem->positions()[i]	= _positions[i];
			_sphSystem->type()[i]		= _types[i];
			_sphSystem->alive()[i]		= 1;
		}
	}

	SPHSystem *SimulatorBase::sphSystem() {
		return _sphSystem;
	}

	vector<glm::vec3> &SimulatorBase::positions() {
		return _positions;
	}
}