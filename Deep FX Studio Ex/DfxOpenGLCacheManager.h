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
#include <unordered_map>
#include <DfxGlobal.h>
#include <DfxMode.h>
#include "DfxObjectCache.h"
#include "DfxVolumeCache.h"

class DfxOpenGLCacheManager : public QObject {
	Q_OBJECT
private:
	static DfxOpenGLCacheManager *_instance;

	std::unordered_map<size_t, DfxObjectCache*> _objectCacheMap;
	std::unordered_map<size_t, DfxVolumeCache*> _volumeCacheMap;

	DfxOpenGLCacheManager();
public:

	~DfxOpenGLCacheManager();

	static DfxOpenGLCacheManager *getInstance();

	void addObjectCache(size_t, DfxObjectCache*);
	void removeObjectCache(size_t);
	DfxObjectCache *findObjectCache(size_t);

	void addVolumeCache(size_t, DfxVolumeCache*);
	void removeVolumeCache(size_t);
	DfxVolumeCache *findVolumeCache(size_t);

public slots:
	//void shadingModedChanged();
	void editModeChanged();
};