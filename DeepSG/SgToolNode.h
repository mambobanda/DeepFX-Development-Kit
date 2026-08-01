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

#include "SgNode.h"

namespace deepsg {

	class SgToolNode : public SgNode {
	public:

		SgToolNode();
		virtual ~SgToolNode();
		
		virtual bool isReady() = 0;
		virtual bool evaluateOnUpdate()   = 0;
		virtual bool evaluateOnNewFrame() = 0;
		virtual bool evaluateOnValueChanged() = 0;
		virtual void evaluate(SgNode *parent) = 0;
		virtual void create(SgNode *parent) = 0;
		virtual void destroy() = 0;
	};
}