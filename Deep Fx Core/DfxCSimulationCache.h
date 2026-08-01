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

#pragma once

#include <glm\vec3.hpp>
#include <unordered_map>
#include <string>
#include <vector>

namespace dfxcore {

	class SimulationFrame {
	private:
		glm::vec3 *particles;
		glm::vec3 *velocities;
		float *speeds;
		int  particleCount;
		bool cached;
		bool frameHasVelocity;
		bool frameHasSpeed;
	public:
		SimulationFrame();

		void clear();
		void initFrame(int count);

		glm::vec3  *getParticles();
		glm::vec3  *getVelocities();
		float *getSpeeds();
		int getParticleCount();
		bool isCached();
		void setIsCached(bool);

		void setHasSpeed(bool);
		void setHasVelocity(bool);
		bool hasSpeed();
		bool hasVelocity();
	};

	class SimulationCache {
	private:
		std::unordered_map< std::string, SimulationFrame*> frames;
	public:
		~SimulationCache();
		int getFrameCount();
		void insertFrame(int frame, SimulationFrame *);
		//string indexToKey(int index);
		SimulationFrame* getFrame(int frame);
		void clear();
	};
}
