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

#include <string>
#include <vector>

namespace event {

	enum {

		// Key Frame Events
		INITIAL_KEYFRAME = 0,
		STARTING_KEYFRAME_UPDATE = 1,
		KEYFRAME_UPDATE_COMPLETE = 2,
		FIRST_FRAME_BEFORE_KEYFRAME_UPDATE = 3,
		FIRST_FRAME_AFTER_KEYFRAME_UPDATE  = 4,
		LAST_FRAME_BEFORE_KEYFRAME_UPDATE = 5,
		LAST_FRAME_AFTER_KEYFRAME_UPDATE  = 6,

		// Bullet Simulation Events
		BT_STARTING_SIMULATION_STEP = 500,
		BT_SIMULATION_STEP_COMPLETE = 501,

		// Deep Rising Simulation Events
		DR_STARTING_SIMULATION_STEP = 1000,
		DR_SIMULATION_STEP_COMPLETE = 1001,

	};

	std::string enumToString(unsigned int);
	unsigned int stringToEnum(std::string);

	std::vector<std::pair<unsigned int, std::string>> enumList();
}