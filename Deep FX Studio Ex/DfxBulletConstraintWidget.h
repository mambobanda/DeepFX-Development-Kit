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
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once


#include <SgShapeNode.h>
#include "DfxItemWidgetType.h"
#include "DfxBulletCommon.h"

class DfxBulletConstraintWidget :public deepsg::SgShapeNode, public deepsg::SgShape {
private:
	DfxBulletAgent *body_a;
	DfxBulletAgent *body_b;

	static std::string _uniqueName;
public:

	DfxBulletConstraintWidget();
	virtual ~DfxBulletConstraintWidget();

	std::string uniqueName();

	void setBodies(DfxBulletAgent *ba, DfxBulletAgent *bb);

	int shapeCount() override;
	std::vector<deepsg::SgShapeBuffer> &shapes() override;
}; 



