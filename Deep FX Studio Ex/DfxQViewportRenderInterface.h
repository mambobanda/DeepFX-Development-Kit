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

#include <string>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <DfxGrid2d.h>
#include <DfxColorID.h>
#include <DfxColorField.h>
#include "DfxSelectionMarquee.h"

class DfxQViewportRenderInterface {
protected:
	DfxSelectionMarquee *selectionMarquee;
public:
	DfxSelectionMarquee *getSelectionMarquee();
	void setSelectionMarquee(DfxSelectionMarquee *);

public:
	enum class ElementType { VERTEX, EDGE, FACE, OBJECT, OBJECT_SHAPE };
	enum class ShadeMode { BOUNDING_BOX, WIREFRAME, WIREFRAME_SOLID_SHADE, SOLID_SHADE, Element };

	virtual std::string name() = 0;
	virtual ShadeMode shadingMode() = 0;
	virtual void setShadingMode(ShadeMode) = 0;
	virtual void init() = 0;
	virtual void render() = 0;
	virtual void renderColors(ElementType) = 0;
	virtual glm::ivec4 colorAtPosition(int x, int y, int canvasHeight) = 0;
	virtual std::vector<DfxColorField> colorAtPosition(int x, int y, int w, int h, int canvasHeight) = 0;
	virtual float depthAtPosition(int, int, int) = 0;
	virtual void resize(int width, int height) = 0;
	virtual void renderWidget(std::vector<DfxColorID> colors) = 0;
	//virtual void renderWidgets() = 0;
	virtual glm::mat4 getProjectionMatrix() = 0;
	virtual glm::mat4 getViewMatrix() = 0;
	virtual void setProjectionMatrix(glm::mat4) = 0;
	virtual void setViewMatrix(glm::mat4) = 0;
	virtual void setWorkGrid(DfxGrid2d) = 0;
	virtual void setCameraPosition(glm::vec3) = 0;
	virtual void setCameraPath(std::string) = 0;
	virtual void setCameraTransform(glm::mat4) = 0;
	virtual void drawWorkGrid() = 0;
	virtual void setBackgroundColor(glm::vec3) = 0;
	virtual glm::vec3 getBackgroundColor() = 0;
	virtual void drawCameraRenderRegion(int, int) = 0;

};