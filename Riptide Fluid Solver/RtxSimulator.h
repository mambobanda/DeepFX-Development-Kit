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

#include <functional>
#include "RtxCommon.h"
#include "RtxSPHFunctions.h"
#include "RtxSimulationConfig.h"
#include "RtxCompactSearch.h"

namespace rtx {

	using SimulationCallback = void(*)(int step, float timestep, rtx::SPHSystem*, void *userdata);
	using LoggingCallback = void(*)(const char *);
	class Simulator {
	public:
		static void init(rtx::SPHSystem *system, SimulationConfig config);
		static void step(rtx::SPHSystem *system, SimulationConfig config, SimulationCallback callback = nullptr, LoggingCallback log = nullptr, void *userdata = nullptr);
	};
}