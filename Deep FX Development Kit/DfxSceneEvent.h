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

#include "DfxKeyLog.h"
#include "DfxRay.h"
#include "DfxColorID.h"
#include "DfxProjectionType.h"

class DfxSceneEvent {
private:
	bool rightMouseButton;
	bool middleMouseButton;
	bool leftMouseButton;

	DfxKeyLog keyLog;

	uint eventCode;

	DfxRay screenRay;

	DfxColorID colorID;

	int screenX;
	int screenY;

	glm::vec3 scenePos;

	DfxProjectionType projectionType;
	DfxViewportView view;
public:
	DfxSceneEvent();
	~DfxSceneEvent();

	void setRightMouseButton(bool);
	bool getRightMouseButton();

	void setMiddleMouseButton(bool);
	bool getMiddleMouseButton();

	void setLeftMouseButton(bool);
	bool getLeftMouseButton();

	void setEventCode(uint );
	uint getEventCode();

	void setKeyLog(DfxKeyLog);
	DfxKeyLog getKeyLog();

	void setScreenRay(DfxRay);
	DfxRay getScreenRay();

	void setScreenX(int val);
	int getScreenX();

	void setScreenY(int val);
	int getScreenY();

	DfxColorID getColorID();
	void setColorID(DfxColorID);

	void setProjectionType(DfxProjectionType proj);
	DfxProjectionType getProjectionType();

	glm::vec3 getScenePos();
	void setScenePos(glm::vec3 pos);

	DfxViewportView getView();
	void setView(DfxViewportView view);
public:
	enum {
		MOUSE_PRESS=0xff11,
		MOUSE_RELEASE,
		MOUSE_MOVE,
		KEY_PRESS,
		KEY_RELEASE,
	};
};
