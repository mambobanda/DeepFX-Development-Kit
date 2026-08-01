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
#include <QString>
#include <regex>
#include "DfxFluentSet.h"

class DfxFluentSetManager {
private:
	static size_t SOFT_SET_ID_GEN;

private:
	static DfxFluentSetManager *_instance;
	std::unordered_map<std::string, DfxFluentSet*> _setMap;
	std::unordered_map<std::string, DfxFluentSet*> _softSetMap;

	DfxFluentSet *_root;

	DfxFluentSetManager();

private:

	DfxFluentSet *_pointsDeadSet;
	DfxFluentSet *_simplexDeadSet;

public:

	virtual ~DfxFluentSetManager();

	static DfxFluentSetManager *getInstance();

	bool hasSet(std::string name);
	void addSet(std::string name, DfxFluentSet*);
	void removeSet(std::string name);
	DfxFluentSet *getSet(std::string name);

	std::pair<std::string, DfxFluentSet*> createSoftSet(uint8_t type);

	DfxFluentSet *pointsDeadSet();
	DfxFluentSet *simplexDeadSet();

	static bool isSoftSetId(std::string str);

	void clearSoftSets();

	std::unordered_map<std::string, DfxFluentSet*> &setMap();

	void clear();

};