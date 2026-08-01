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
#include <FlFlow.h>

class DfxFlowDB {
private:
	static size_t ID_GEN;

private:
	static DfxFlowDB *_instance;

	std::unordered_map<size_t, fl::Flow*> _map;

	DfxFlowDB();
public:

	virtual ~DfxFlowDB();

	static DfxFlowDB *getInstance();

	void addFlow(size_t id, fl::Flow *flow);
	void removeFlow(size_t id);
	fl::Flow *findFlow(size_t id);

	std::vector<std::pair<size_t, fl::Flow*>> flowList();

	void resetMaxID();
	size_t newID();
	void setID(size_t);

	void reset();
	static void resetCallback();

};