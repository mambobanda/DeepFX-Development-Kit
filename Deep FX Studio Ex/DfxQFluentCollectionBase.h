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

#include <QGraphicsScene>
#include <vector>

class DfxQFluentCollectionBase : public QGraphicsScene {
public:
	virtual void clearAll() = 0;
	virtual void addExisting(std::string) = 0;
	virtual std::pair<std::string, unsigned int> typeInfo() = 0;
	//virtual void unparent() = 0;
};