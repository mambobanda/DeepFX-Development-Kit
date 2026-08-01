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

#include <functional>
#include "DfxEntityDataEvent.h"

static size_t ENTITY_DATA_LISTENER_ID_GEN = 0x101;

class DfxEntityDataListener{
private:
	size_t id;
	std::function<void(DfxEntityDataEvent)> functor;
public:
	DfxEntityDataListener(std::function<void(DfxEntityDataEvent)> fc);
	virtual ~DfxEntityDataListener();
	
	size_t getID();

	void operator()(DfxEntityDataEvent  evt);
};