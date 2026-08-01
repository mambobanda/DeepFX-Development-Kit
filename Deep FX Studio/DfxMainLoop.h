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


enum class DfxEventLoopState{ IDLE_STATE, PLAY_TIMELINE_STATE, SIMULATE_STATE };

class DfxMainLoop {
private:
	DfxEventLoopState current_state;
	void *_timeline;
private:
	static DfxMainLoop *instance;

	DfxMainLoop();
public:
	~DfxMainLoop();

	static DfxMainLoop *getInstance();

	void setTimeline(void *);

	void playFunction();

	void process();

	void setState(DfxEventLoopState );

	DfxEventLoopState getState();
};

void dfxMainLoopEvent();
