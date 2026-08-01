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


#include <glm\vec3.hpp>
#include "DfxAbstractGizmo.h"
#include "DfxGizmoUtil.h"

class DfxTranslateGizmo : public DfxAbstractGizmo {
private:

	float sc;
	float hdHt;
	float tlHt;
	float tlWd;

	glm::vec3 lastOffset;
	uint32_t x_lastOffset;
	uint32_t y_lastOffset; 
public:
	DfxTranslateGizmo(DfxViewItem *itptr);
	~DfxTranslateGizmo();

	uint numberOfHandles() override;
	void pressed(DfxGizmoEvent) override;
	void released(DfxGizmoEvent) override;
	void moving(DfxGizmoEvent) override;
	void done() override;
	void reposition() override;
};