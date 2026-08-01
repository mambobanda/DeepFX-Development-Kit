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
#include <algorithm>
#include <string>

class DfxSurfaceDictionary {
public:
	static const unsigned int ID_LIMIT = 0x3B9ACA00;
private:
	static DfxSurfaceDictionary *_instance;
	std::unordered_map<std::string, std::unordered_map<unsigned int, std::string>> _map;

	DfxSurfaceDictionary();
public:

	virtual ~DfxSurfaceDictionary();

	static DfxSurfaceDictionary *getInstance();

	unsigned int maxID(std::string path);
	std::string string(std::string path, unsigned int id);

	void create(std::string path);
	void insert(std::string path, unsigned int id, std::string string);
	std::vector<unsigned int> find(std::string path, std::string);
	void erase(std::string path, unsigned int id);
	void eraseAll(std::string path);

	std::vector<std::pair<std::string, std::unordered_map<unsigned int, std::string>>> all();

	void clear();

	static void clearCallback();
};