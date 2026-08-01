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

#include <QWidget>
#include <vector>
#include <functional>
#include "DfxQViewID.h"

class DfxQViewMap {
private:
	static DfxQViewMap *_instance;
	std::vector<std::pair<std::string, std::pair<DfxQViewID, std::function<QWidget*()>>>> _map;

	DfxQViewMap();
public:
	virtual ~DfxQViewMap();

	static DfxQViewMap *getInstance();

	std::vector<std::pair<std::string, std::pair<DfxQViewID, std::function<QWidget*()>>>> &entries();

	void addEntry(std::string name, DfxQViewID id, std::function<QWidget*()> func);
	std::function<QWidget*()> findEntry(DfxQViewID id, bool &found);
	std::string findEntryName(DfxQViewID id);
};