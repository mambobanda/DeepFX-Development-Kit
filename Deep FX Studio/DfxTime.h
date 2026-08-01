/*
* The Mailo Today Solutions Software License
*
* Copyright 2017 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/
#pragma once

#include <mutex>
#include <vector>
#include <DfxEntity.h>
#include "DfxTimeActionListener.h"
#include "DfxTimeActionInfo.h"

class DfxTime {
private:
	static DfxTime *instance;

	DfxTime();
private:
	int totalFrames;
	int currentframe;
	int fps;

	std::mutex classLock;

	std::vector<DfxTimeActionListener*> actionListnerList;
	std::vector<DfxEntity*> onActionEntityList;
public:

	~DfxTime();

	static DfxTime *getInstance();

	void setTotalFrames(int f);
	int getTotalFrames();

	void setNewFrame(int f);
	int getCurrentFrame();

	void setFramesPerSec(int);
	int getFramesPerSec();

	void addActionListener(DfxTimeActionListener*);

	void addOnActionEntity(DfxEntity *);

	void removeOnActionEntity(size_t instanceid);

	void onActionEntityCompute();

	void announce(DfxTimeActionInfo info);
};