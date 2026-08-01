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

#include <DfxRay.h>
#include <SgNode.h>
#include <SgWidget.h>
#include <SgTransform.h>
#include "DfxMode.h"
#include "DfxWidgetManager.h"

class DfxWidgetInterface: public deepsg::SgWidget {
public:
	class OnMoveInfo {
	public:
		enum 
		{
			TOP = 1, BOTTOM = 2, LEFT = 3, RIGHT = 4, FRONT = 5, BACK = 6,
			RAY = 100, SCREEN = 101,
		};

		int type;
		int view;

		DfxRay previous;
		DfxRay current;

		int screenPosX_previous;
		int screenPosY_previous;

		int screenPosX_current;
		int screenPosY_current;

		inline OnMoveInfo() {
			type = RAY;
		}
	};

	class OnClickInfo {
	public:
		DfxRay current;

		int screenPosX;
		int screenPosY;
	};

	class OnReleaseInfo {
	public:
		DfxRay current;

		int screenPosX;
		int screenPosY;
	};

public:

	DfxWidgetInterface();
	virtual ~DfxWidgetInterface();

	virtual void onClick(int handle, DfxWidgetInterface::OnClickInfo info) = 0;
	virtual void onMove(int handle, DfxWidgetInterface::OnMoveInfo info) = 0;
	virtual void onRelease(int handle, DfxWidgetInterface::OnReleaseInfo info) = 0;

	virtual glm::mat4 handleTransform(int handle, deepsg::SgXForm parentTransform) = 0;

};