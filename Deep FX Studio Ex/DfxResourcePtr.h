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

#include "DfxResource.h"

class DfxResourcePtr {
private:
	static size_t ID_GEN;
	size_t _id;

	DfxResource *_rptr;
	bool _ready;

	friend DfxResource;

private:
	void setIsReady(bool val);

public:

	DfxResourcePtr();
	virtual ~DfxResourcePtr();

	size_t id();
	size_t resourceId();

	void releasePointer(bool autoDelete=false);

	bool isReady();

	template<typename T>
	T *resource() {
		if(_ready) return (T*)_rptr->resource();
		return nullptr;
	}
};