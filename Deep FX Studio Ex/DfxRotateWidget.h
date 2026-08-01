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

#include <SgGeoNode.h>
#include <SgShapeNode.h>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <limits>
#include <GTE/Mathematics/DistLineLine.h>
#include <GTE/Mathematics/DistRayRay.h>
#include "DfxMathUtil.h"
#include "DfxWidgetInterface.h"
#include "DfxDeepSimplexGeoAdapter.h"


class DfxRotateWidget : public DfxWidgetInterface {
private:
	void createXHandle();
	void createYHandle();
	void createZHandle();

	float uniformSize;

	glm::mat4 getRotationMat4(int handle, deepsg::SgXForm parentTransform);

	glm::vec3 pivot;

	void onMoveObjectMode(int handle, DfxWidgetInterface::OnMoveInfo info);
	void onMoveVertexMode(int handle, DfxWidgetInterface::OnMoveInfo info);
	void onMoveEdgeMode(int handle, DfxWidgetInterface::OnMoveInfo info);
	void onMoveFaceMode(int handle, DfxWidgetInterface::OnMoveInfo info);

	void onClickVertexMode(int handle, DfxWidgetInterface::OnClickInfo info);
	void onClickEdgeMode(int handle, DfxWidgetInterface::OnClickInfo info);
	void onClickFaceMode(int handle, DfxWidgetInterface::OnClickInfo info);

	glm::mat4 calacRotationMat(int handle, DfxWidgetInterface::OnMoveInfo info);
public:

	DfxRotateWidget();
	virtual ~DfxRotateWidget();

	void onMove(int handle, DfxWidgetInterface::OnMoveInfo info) override;
	void onClick(int handle, DfxWidgetInterface::OnClickInfo info) override;
	void onRelease(int handle, DfxWidgetInterface::OnReleaseInfo info) override;

	glm::mat4 handleTransform(int handle, deepsg::SgXForm parentTransform) override;
};