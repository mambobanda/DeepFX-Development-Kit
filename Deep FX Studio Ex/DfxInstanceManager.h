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
#include "DfxInstancerBase.h"

class DfxInstanceManager {
private:
	static DfxInstanceManager *_instance;

	std::unordered_map<size_t, DfxInstancerBase*> _intancers;
public:

	DfxInstanceManager();
	virtual ~DfxInstanceManager();

	static DfxInstanceManager *getInstance();

	void addInstancer(DfxInstancerBase *inst);
	void removeInstancer(DfxInstancerBase *inst);

	std::vector<DfxInstancerBase*> intancers();
};