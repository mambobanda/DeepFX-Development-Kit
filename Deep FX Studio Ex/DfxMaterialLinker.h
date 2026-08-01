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

#include <DfxGlobal.h>
#include "DfxMaterialIDMap.h"


class DfxMaterialLinker {
private:
	static DfxMaterialLinker *_instance;

	std::unordered_map<std::string, DfxMaterialIDMap*> _map;

	DfxMaterialLinker();
public:

	virtual ~DfxMaterialLinker();

	static DfxMaterialLinker *getInstance();

	void updateObjectLink(std::string obj_path, unsigned int id, std::string mat_path);
	void removeObjectLink(std::string obj_path, unsigned int id);
	void removeAllLinks(std::string obj_path);
	void removeAllMaterialLinks(std::string mat_path);

	std::unordered_map<std::string, DfxMaterialIDMap*> &materialLinkMap();

	bool hasObjectLink(std::string obj_path, unsigned int id);
	std::string objectLink(std::string obj_path, unsigned int id);
	std::vector<std::pair<unsigned int, std::string>> allObjectLinks(std::string obj_path);

	void clear();
	static void clearCallback();
};