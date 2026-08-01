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

#include <QObject>
#include <vector>
#include "DfxCacheTrack.h"
#include "DfxQCacheTrackModel.h"

class DfxCacheTrackManager: public QObject {
	Q_OBJECT
private:
	static DfxCacheTrackManager *_instance;

	DfxCacheTrack *_defaultTrack;
	std::vector<DfxCacheTrack*> _tracks;

	DfxQCacheTrackModel *_model;

	DfxCacheTrackManager();
public:
	
	virtual ~DfxCacheTrackManager();

	static DfxCacheTrackManager *getInstance();

	DfxCacheTrack *track(size_t index);
	std::vector<DfxCacheTrack*> &tracks();

	void addTrack(DfxCacheTrack *track);
	void deleteTrack(size_t index);

	DfxQCacheTrackModel *model();

	void clear();

	static void clearCallback();
signals:
	void newTrackInserted();
	void trackRemoved();
};