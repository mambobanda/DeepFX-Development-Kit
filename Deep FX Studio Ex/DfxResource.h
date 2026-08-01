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
#include <DfxSpace.h>
#include "DfxResourceCommon.h"

class DfxResource {
private:
	static size_t ID_GEN;
	size_t _id;

private:

	void *_resource;
private:

	DfxResource *_parent;
	std::vector<DfxResource*> _children;
	std::vector<DfxResourcePtr*> _pointers;
	size_t _shareCounter;

	resource::ReleaseCallback _releaseCallback;

	unsigned int _mode;
	unsigned int _type;
	unsigned int _format;
	std::string _path;
	std::string _name;
	bool _referenced;
	bool _embedded;

private: 
	size_t maxId(DfxResource *rsc);

public:

	DfxResource();
	DfxResource(unsigned int nType, unsigned int rType, std::string path);

	static void setBaseId(size_t uid);

	size_t maxId();

	size_t id();
	void setId(size_t uid);

	std::vector<DfxResource*> children();

	bool hasParent();
	DfxResource *parent();
	void setParent(DfxResource*);

	unsigned int mode();
	void setMode(unsigned int nt);

	unsigned int type();
	void setType(unsigned int);

	unsigned int format();
	void setFormat(unsigned int);

	std::string name();
	void setName(std::string nm);

	std::string path();
	std::string setPath(std::string nm);

	size_t childCount(size_t chd);
	size_t count();

	void setReleaseCallback(resource::ReleaseCallback cb);
	resource::ReleaseCallback releaseCallback();

	void addChild(DfxResource *rnode);
	DfxResource *getChild(size_t inx);
	DfxResource *getChildByID(size_t uid);
	void removeChild(DfxResource *rnode);
	void removeChild(size_t inx);
	void removeChildByID(size_t uid);	
	void detach();
	size_t childCount();

	void *resource();
	void setResource(void *rc);

	size_t resourceCount();

	bool isReferenced();
	void setIsReferenced(bool);

	bool isEmbedded();
	void setEmbedded(bool);

	DfxResourcePtr *newPointer();
	
	void releasePointer(DfxResourcePtr *ptr);
	static void releasePointer(DfxResource *resource, DfxResourcePtr *resourcePtr, bool autoDelete = false);
	static void releaseResource(DfxResource *resource);
};