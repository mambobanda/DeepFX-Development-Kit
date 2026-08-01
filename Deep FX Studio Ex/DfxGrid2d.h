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

#pragma once

class DfxGrid2d {
private:
	int width;
	int height;
	int subgrid;

	float spacing;
	float originX;
	float originY;
	float originZ;
public:

	DfxGrid2d();
	virtual ~DfxGrid2d();

	int getWidth();
	void setWidth(int w);

	int getHeight();
	void setHeight(int h);

	int getSubgrid();
	void setSubgrid(int sg);

	float getSpacing();
	void setSpacing(int sp);

	float getOriginX();
	void setOriginX(float x);

	float getOriginY();
	void setOriginY(float y);

	float getOriginZ();
	void setOriginZ(float z);
};