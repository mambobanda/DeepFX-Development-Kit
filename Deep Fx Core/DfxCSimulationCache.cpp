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

#include <sstream>
#include "Util.h"
#include "DfxCSimulationCache.h"

namespace dfxcore {

	SimulationFrame::SimulationFrame() {
		particles = nullptr;
		speeds = nullptr;
		velocities = nullptr;
		cached = false;
		particleCount = 0;
		frameHasSpeed = false;
		frameHasVelocity = false;
	}

	bool SimulationFrame::isCached() {
		return cached;
	}

	void SimulationFrame::setIsCached(bool cch) {
		cached = cch;
	}

	void SimulationFrame::clear() {
		delete[] particles;
		delete[] speeds;
		delete[] velocities;
		particleCount = 0;

		particles = nullptr;
		speeds = nullptr;
		velocities = nullptr;
	}

	void SimulationFrame::initFrame(int count) {
		particleCount = count;
		particles = new glm::vec3[count];
		velocities = new glm::vec3[count];
		speeds = new float[count];
	}

	glm::vec3 *SimulationFrame::getParticles() {
		return particles;
	}

	float *SimulationFrame::getSpeeds() {
		return speeds;
	}

	glm::vec3  *SimulationFrame::getVelocities() {
		return velocities;
	}

	void SimulationFrame::setHasSpeed(bool b) {
		frameHasSpeed = b;
	}

	void SimulationFrame::setHasVelocity(bool b) {
		frameHasVelocity = b;
	}

	bool SimulationFrame::hasSpeed() {
		return frameHasSpeed;
	}

	bool SimulationFrame::hasVelocity() {
		return frameHasVelocity;
	}

	int SimulationFrame::getParticleCount() {
		return particleCount;
	}

	SimulationCache::~SimulationCache() {
		clear();
	}

	int SimulationCache::getFrameCount() {
		return frames.size();
	}

	void SimulationCache::insertFrame(int frame, SimulationFrame *sframe) {

		std::pair<std::string, SimulationFrame*> p(indexToKey(frame), sframe);
		frames.insert(p);
	}


	SimulationFrame* SimulationCache::getFrame(int index) {
		std::unordered_map<std::string, SimulationFrame*>::iterator fr = frames.find(indexToKey(index));

		if (fr == frames.end()) {
			return nullptr;
		}
		else {
			SimulationFrame* ptr = fr->second;

			return ptr;
		}

	}

	void SimulationCache::clear() {
		std::unordered_map<std::string, SimulationFrame*>::iterator it = frames.begin();

		for (; it != frames.end(); it++) {
			it->second->clear();
			delete it->second;
			it->second = nullptr;
		}

		frames.clear();
	}
}