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

#include "SgCommon.h"

namespace deepsg {

	class SgNodeFilter {
	private:
		static size_t ID_GEN;
		size_t _id;
		SgNode *_node;
	public:

		SgNodeFilter();
		virtual ~SgNodeFilter();

		SgNode *node();
		void setNode(SgNode *nd);

		size_t id();

		virtual void begin(SgNode *nd, SgStateInfo *info) = 0;
		virtual void end(SgNode *nd, deepsg::SgStateInfo *info) = 0;
		
		virtual void filter(SgNode *node, deepsg::SgStateInfo *info) = 0;
	};
}