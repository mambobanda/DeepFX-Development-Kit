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

#include <string>
#include <vector>
#include <FlAny.h>

class DfxAttribute {
private:
	static unsigned int UID_GEN;

	unsigned int _id;
	unsigned int _role;
private:
	std::string _path;
	std::vector<DfxAttribute*> _attributes;
	DfxAttribute *_parent;

public:

	enum { ROOT, SUB_ATTRIBUTE };

	DfxAttribute();
	~DfxAttribute();

	unsigned int id();

	void addAttribute(DfxAttribute*);
	void removeAttribute(DfxAttribute*);

	size_t attributeCount();
	DfxAttribute *attribute(size_t);
	std::vector<DfxAttribute*> &attributes();

	unsigned int role();
	void setRole(unsigned int);

	DfxAttribute *parent();
	void setParent(DfxAttribute *);

	unsigned int count(unsigned int);
	unsigned int count();

	std::string path();
	void setPath(std::string);

	std::string attributePath();

	void deleteChildren();
public:

	virtual void setName(std::string) = 0;
	virtual std::string name()  = 0;
	virtual unsigned int type() = 0;

	virtual bool isReadable() = 0;
	virtual bool isWritable() = 0;
	virtual bool isArray() = 0;

	virtual fl::Any *getValue() = 0;
	//virtual std::unique_ptr<fl::Any> getValueAt(size_t) = 0;
	virtual size_t getValueCount() = 0;

	virtual void setValue(fl::Any *val) = 0;
	//virtual void setValueAt(size_t, std::unique_ptr<fl::Any> val) = 0;
};