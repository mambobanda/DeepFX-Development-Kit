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

#include <glm/vec3.hpp>
#include "SgNode.h"
#include "SgWidget.h"
#include "SgTransform.h"

namespace deepsg {

	class SgRenderNode : public SgNode {
	private:
		size_t _engineId;

	public:

		SgRenderNode(size_t eid);
		virtual ~SgRenderNode();

		size_t getRenderEngineId();

	};

}