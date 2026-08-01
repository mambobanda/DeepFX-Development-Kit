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

#include <vector>
#include <algorithm>

class Resource {
private:
	static size_t ID_GEN;

	size_t _id;
	Resource *_parent;
	std::vector<Resource*> _children;
	std::string _name;
	int _type;
	int _role;

public:
	// Resource Properties
	std::string resourcePath;

	// Root Properties
	std::string installation_dir;
	std::string main_executable;
	std::string executable_name;
	std::string component_name;
	std::string eula;

public:
	
	enum  {
		NONE = 0, ROOT = 1, DIR = 2, FILE = 3,
		MAIN_EXE = 50, DEPENDENCY = 51,
	};

	Resource();
	virtual ~Resource();

	size_t id();

	std::vector<Resource*> &children();

	void addChild(Resource *chd);
	void removeChild(size_t id);
	Resource *findChildByIndex(int inx);
	Resource *findChildByID(size_t idn);

	Resource *parent();
	void setParent(Resource *);

	std::string name();
	void setName(std::string nm);
	void unparent();

	int type();
	void setType(int typ);

	int role();
	void setRole(int role);

	int count();
	int childCount(size_t id);
};