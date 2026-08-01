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

#include <cstdint>
#include <vector>
#include <glm/vec3.hpp>
#include "DfxFluentBigID.h"
#include "DfxFluentSimplexCollection.h"
#include "DfxFluentStreamCollection.h"

class DfxFluentSet {
private:
	size_t set_id;

private:
	uint8_t _type;
	uint8_t _referenceType;

	std::vector<size_t> _indices;
	bool _visible;

	glm::vec3 _color;

	std::string _name;
private:
	DfxFluentSet *_parent;
	std::vector<DfxFluentSet*> _children;

public:
	enum { EMPTY = 0, ROOT = 1, SIMPLEX = 2, STREAM = 3, 
		   NORMAL_SET = 100, SOFT_SET = 101};

	DfxFluentSet(uint8_t typ);
	virtual ~DfxFluentSet();

	size_t id();

	uint8_t type();
	void setType(uint8_t);
	
	void forceSoftSet();
	bool isSoftSet();

	bool isVisible();
	void setIsVisible(bool);

	std::vector<size_t> &indices();
	void addIndices(std::vector<size_t> &);
	void removeIndex(size_t inx);
	void removeIndices(std::vector<size_t> &);

	bool hasObjectFromScene(std::string path);

	DfxFluentSet *parent();
	void setParent(DfxFluentSet*);

	std::vector<DfxFluentSet*> &children();
	void addChild(DfxFluentSet* set);
	void removeChild(size_t uid);
	bool moveByIndex(size_t index, DfxFluentSet *);
	DfxFluentSet* findChild(size_t);

	size_t relativeIndex();

	glm::vec3 color();
	void setColor(glm::vec3 color);

	std::string name();
	void setName(std::string);

	void unparent();
};