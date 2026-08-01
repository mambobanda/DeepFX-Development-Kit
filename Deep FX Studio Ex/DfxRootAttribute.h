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

#include <FlAny.h>
#include "DfxAttribute.h"

class DfxRootAttribute: public DfxAttribute {
private:
	std::string _name;

public:
	DfxRootAttribute();
	~DfxRootAttribute();
	
	void setName(std::string);
	std::string name() override;
	unsigned int type() override;

	bool isReadable() override;
	bool isWritable() override;
	bool isArray() override;

	fl::Any *getValue() override;
	// std::unique_ptr<fl::Any> getValueAt(size_t) override;
	size_t getValueCount() override;

	void setValue(fl::Any *val) override;
	// void setValueAt(size_t, std::unique_ptr<fl::Any> val) override;
};