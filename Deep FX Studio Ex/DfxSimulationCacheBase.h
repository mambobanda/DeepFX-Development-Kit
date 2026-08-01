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


class DfxSimulationCacheBase{
private:
	size_t _type;
public:
	enum {NONE=0, TRANSFORM_CACHE=1, POINT_CACHE=2};

	DfxSimulationCacheBase(size_t type);
	virtual ~DfxSimulationCacheBase();

	size_t type();

	virtual void clear() = 0;
	virtual bool hasFrame(int) = 0;
};