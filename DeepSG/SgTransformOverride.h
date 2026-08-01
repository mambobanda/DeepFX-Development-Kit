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

#include <glm/mat4x4.hpp>
#include "SgCommon.h"
#include "SgTransformOverrideInfo.h"

namespace deepsg {

	class SgTransformOverride {
	private:
		static size_t ID_GEN;
		size_t _id;

		SgNode *_parent;

		unsigned int _visibility;
	public:
		enum { PUBLIC = 0, PRIVATE = 1 };

		SgTransformOverride(SgNode *prt = nullptr);
		virtual ~SgTransformOverride();

		size_t id();

		SgNode *parent();

		void setParent(SgNode *node);

		virtual glm::mat4 evaluate(SgTransformOverrideInfo info) = 0;

		unsigned int visibility();

		void setVisibility(unsigned int vis);
	};

}