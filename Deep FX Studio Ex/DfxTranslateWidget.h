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

#include <iostream>
#include <limits>
#include <GTE/Mathematics/DistLineLine.h>
#include <GTE/Mathematics/DistRayRay.h>
#include <SgGeoNode.h>
#include <SgShapeNode.h>
#include "DfxMathUtil.h"
#include "DfxWidgetInterface.h"
#include "DfxDeepSimplexGeoAdapter.h"



class DfxTranslateWidget: public DfxWidgetInterface {
private:
	void createXHandle();
	void createYHandle();
	void createZHandle();

	float uniformSize;

	glm::vec3 calcDelta(int handle, DfxWidgetInterface::OnMoveInfo info);
	void onMoveObjectMode(int handle, DfxWidgetInterface::OnMoveInfo info);
	void onMoveVertexMode(int handle, DfxWidgetInterface::OnMoveInfo info);
	void onMoveEdgeMode(int handle, DfxWidgetInterface::OnMoveInfo info);
	void onMoveFaceMode(int handle, DfxWidgetInterface::OnMoveInfo info);
public:

	DfxTranslateWidget();
	virtual ~DfxTranslateWidget();

	void onMove(int handle, DfxWidgetInterface::OnMoveInfo info) override;
	void onClick(int handle, DfxWidgetInterface::OnClickInfo info) override;
	void onRelease(int handle, DfxWidgetInterface::OnReleaseInfo info) override;

	glm::mat4 handleTransform(int handle, deepsg::SgXForm parentTransform) override;
};