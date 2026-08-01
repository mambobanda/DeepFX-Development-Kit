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

#include <vector>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\intersect.hpp>
#include <QOpenGLFunctions_2_1> 
#include <functional>
#include "DfxViewItem.h"
#include "DfxGlobal.h"
#include "DfxGizmoEvent.h"


class DfxAbstractGizmo : protected QOpenGLFunctions_2_1 {
public:

	using DrawHandleCallback = std::function<void(void)>;

	class DfxGizmoHandle {
	public:
		DfxGizmoHandle();
		virtual ~DfxGizmoHandle();
		uchar id[3];
		uchar color[3];
		uchar selectionColor[3];
		DrawHandleCallback draw;
		bool selected;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	enum {
		NORMAL_MODE=0xFF,
		SELECT_MODE
	};

public:
	DfxAbstractGizmo();
	virtual ~DfxAbstractGizmo();

	virtual uint numberOfHandles() = 0;
	virtual void pressed(DfxGizmoEvent ) = 0;
	virtual void released(DfxGizmoEvent ) = 0;
	virtual void moving(DfxGizmoEvent ) = 0;
	virtual void done() = 0;
	virtual void reposition() = 0;

	static uint hashRGB(unsigned char r, unsigned char g, unsigned char b);

	void draw(int mode);

	float getGizmoScale();
	void  setGizmoScale(float);

	bool handleHit(uchar a, uchar b, uchar c);

	bool handleHit(uchar *frame, uchar *out, int &pixIndex, uint pixelCount);

	bool handleSelected();

	DfxGizmoHandle *getSelectedHandle();

	void selectHandle(uchar a, uchar b, uchar c);

	void clearSelected();

	glm::vec3 getIntersectionPoint();
	void setIntersectionPoint(glm::vec3);

	DfxViewItem *getViewItem();
	void setViewItem(DfxViewItem *vit);

	glm::vec3 getPosition();

protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	std::vector<DfxGizmoHandle*> handles;

	uint genHandlerID();
private:
	DfxViewItem *itemPtr;

	uint handlerID;

	float gizmoScale;

	bool mouse_pressed;
	bool mouse_released;
	bool mouse_moving;

	glm::vec3 intersectionPoint;
};
