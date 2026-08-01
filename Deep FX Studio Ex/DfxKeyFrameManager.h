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
#include <FlAnyMap.h>

class DfxKeyFrameManager {
private:
	static DfxKeyFrameManager *_instance;

	std::unordered_map<std::string, fl::AnyMap<int, float>*> _floatMap;
	std::unordered_map<std::string, fl::AnyMap<int, int>*> _intMap;
	std::unordered_map<std::string, fl::AnyMap<int, std::string>*> _stringMap;

	DfxKeyFrameManager();
public:
	
	~DfxKeyFrameManager();

	static DfxKeyFrameManager *getInstance();

	// Float Map
	void addFloatMap(std::string, float originalValue);
	int  floatMapSize(std::string);
	void setFloatMapValue(std::string, int frame, float value);
	float getFloatMapValue(std::string, int frame);
	void removeFloatMapValue(std::string, int frame);
	void removeFloatMap(std::string);
	fl::AnyMap<int, float> *getFloatMap(std::string);
	
	// Int Map
	void addIntMap(std::string, int originalValue);
	int  intMapSize(std::string);
	void setIntMapValue(std::string, int frame, int value);
	int getIntMapValue(std::string, int frame);
	void removeIntMapValue(std::string, int frame);
	void removeIntMap(std::string);
	fl::AnyMap<int, int> *getIntMap(std::string);

	// String Map
	void addStringMap(std::string, std::string originalValue);
	int  stringMapSize(std::string);
	void setStringMapValue(std::string, int frame, std::string value);
	std::string getStringMapValue(std::string, int frame);
	void removeStringMapValue(std::string, int frame);
	void removeStringMap(std::string);
	fl::AnyMap<int, std::string> *getStringMap(std::string);
};