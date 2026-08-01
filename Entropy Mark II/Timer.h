/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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