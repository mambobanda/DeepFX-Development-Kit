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


#include <QWidget>
#include <SgRenderNode.h>
#include "DfxRenderCommon.h"

class DfxRenderNodeBase : public deepsg::SgRenderNode {
public:

	DfxRenderNodeBase(size_t renderTarget);
	virtual ~DfxRenderNodeBase();

	virtual DfxRenderSessionInterface *renderSession() = 0;
	virtual void stopRendering() = 0;
	virtual DfxRenderOutput *renderOutput() = 0;

	virtual std::vector<std::pair<QWidget*, QString>> propertyForms() = 0;
};