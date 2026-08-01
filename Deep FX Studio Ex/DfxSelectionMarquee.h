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

#include <glm/vec4.hpp>

class DfxSelectionMarquee {
private:
	int _startX;
	int _startY;
	int _endX;
	int _endY;

	bool active;	

	int _canvasWidth;
	int _canvasHeight;

	glm::vec4 _fillColor;
	glm::vec4 _strokeColor;
public:

	DfxSelectionMarquee();
	virtual ~DfxSelectionMarquee();

	int startX();
	void setStartX(int val);

	int startY();
	void setStartY(int val);

	int endX();
	void setEndX(int val);
	
	int endY();
	void setEndY(int val);

	bool isActive();
	void setIsActive(bool b);

	int canvasWidth();
	void setCanvasWidth(int);
	int canvasHeight();
	void setCanvasHeight(int);

	void setFillColor(glm::ivec4);
	glm::ivec4 fillColor();
	void setStrokeColor(glm::ivec4);
	glm::ivec4 strokeColor();
};
