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

#include <chrono>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

namespace rtx {

	class SPHSystem;
	class CompactXSearch;

	enum { 
		FLUID_PARTICLE = 10, BOUNDARY_PARTICLE = 11, MOVING_BOUNDARY_PARTICLE = 12, GRANULAR_PARTICLE = 13, 
	
		AKINCI2012 = 100, STATIC_PARTICLES = 101,
	};

	#define RTX_MIN_E 1.0e-5
	#define RTX_PI 3.14159265358979323846

	class Timer {
	private:
		std::chrono::system_clock::time_point start;
		double finalTime;

	public:

		inline void startTimer() {
			start = std::chrono::system_clock::now();
		}

		inline void stopTimer() {
			auto diff = std::chrono::system_clock::now() - start;
			finalTime = std::chrono::duration <double, std::milli>(diff).count();
		}

		inline double getTime() {
			return finalTime;
		}
	};

	inline float pow2(float val) {
		return val * val;
	}

	inline float pow3(float val) {
		return val * val * val;
	}

	inline float pow4(float val) {
		return val * val * val * val;
	}

	inline float pow5(float val) {
		return val * val * val * val * val;
	}

	inline float pow6(float val) {
		return val * val * val * val * val * val;
	}

	inline float pow7(float val) {
		return val * val * val * val * val * val * val;
	}

	inline float pow8(float val) {
		return val * val * val * val * val * val * val * val;
	}

	inline float pow9(float val) {
		return val * val * val * val * val * val * val * val * val;
	}

	inline float pow10(float val) {
		return val * val * val * val * val * val * val * val * val * val;
	}

	inline glm::vec3 checkVec3Bounds(glm::vec3 val) {

		if (glm::isnan(val.x)) {
			return glm::vec3(0.0f);
		}

		if (glm::isnan(val.y)) {
			return glm::vec3(0.0f);
		}

		if (glm::isnan(val.z)) {
			return glm::vec3(0.0f);
		}

		// Check Infinity
		if (glm::isinf(val.x)) {
			return glm::vec3(0.0f);
		}

		if (glm::isinf(val.y)) {
			return glm::vec3(0.0f);
		}

		if (glm::isinf(val.z)) {
			return glm::vec3(0.0f);
		}

		return val;
	}
}