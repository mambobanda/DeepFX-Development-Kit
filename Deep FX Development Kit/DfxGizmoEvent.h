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

#include <glm/vec3.hpp>
#include "DfxRay.h"
#include "DfxProjectionType.h"

class DfxGizmoEvent {
private:
	int screenX;
	int screenY;
	DfxRay screenRay;
	glm::vec3 scenePos;
	DfxProjectionType projectionType;
	DfxViewportView view;
public:

	void setScreenX(int val);
	int getScreenX();

	void setScreenY(int val);
	int getScreenY();

	void setScreenRay(DfxRay);
	DfxRay getScreenRay();

	void setProjectionType(DfxProjectionType typ);
	DfxProjectionType getProjectionType();

	glm::vec3 getScenePos();
	void setScenePos(glm::vec3);

	DfxViewportView getView();
	void setView(DfxViewportView view);
};
