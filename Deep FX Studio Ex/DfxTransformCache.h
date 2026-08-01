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

#include <unordered_map>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "DfxSimulationCacheBase.h"
#include "DfxCacheUtils.h"


class DfxTransformFrame {
private:
	glm::mat4 _xform;
	bool cached;
public:
	DfxTransformFrame();
	DfxTransformFrame(glm::mat4);

	glm::mat4 xform();
	void setXForm(glm::mat4);

	void clear();
	
	bool isCached();
	void setIsCached(bool);
};

class DfxTransformCache: public DfxSimulationCacheBase {
private:
	std::unordered_map< std::string, DfxTransformFrame*> frames;
public:
	DfxTransformCache();
	virtual ~DfxTransformCache();

	void insertFrame(int frame, DfxTransformFrame *);
	DfxTransformFrame* getFrame(int frame);

	bool hasFrame(int ) override;
	void clear() override;
};
