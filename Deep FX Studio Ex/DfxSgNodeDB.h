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

#include <SgNode.h>
#include <unordered_map>
#include "DfxSgNodeCommon.h"

class DfxSqNodeDB {
private:
	static DfxSqNodeDB *_instance;
	std::unordered_map<std::string, node::SgNodeLoadCallback> _loadCallbackMap;
	std::unordered_map<std::string, node::SgNodeDeleteCallback> _deleteCallbackMap;

	DfxSqNodeDB();
public:

	virtual ~DfxSqNodeDB();

	static DfxSqNodeDB *getInstance();

	void addLoadCallback(std::string key, node::SgNodeLoadCallback cb);
	node::SgNodeLoadCallback loadCallback(std::string key);

	void addDeleteCallback(std::string key, node::SgNodeDeleteCallback cb);
	node::SgNodeDeleteCallback deleteCallback(std::string key);
};