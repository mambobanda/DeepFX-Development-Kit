/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include <SgRenderDelegateNode.h>
#include <QOpenGlFunctions_3_1>

class DfxRenderDelegateNode: public deepsg::SgRenderDelegateNode, public QOpenGLFunctions_3_1 {
public:
	virtual std::vector<std::pair<QWidget*, QString>> propertyForms() = 0;
};