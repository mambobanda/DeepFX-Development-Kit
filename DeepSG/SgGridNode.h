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

#include <glm/vec3.hpp>
#include <DsPointCloud.h>
#include "SgGridAdapterInterface.h"
#include "SgNode.h"
#include "SgTransform.h"

namespace deepsg {

	class SgGridNode : public SgNode {
	private:
		SgGridAdapterInterface *_interface;

	public:

		SgGridNode();
		virtual ~SgGridNode();

		void setGridAdapter(SgGridAdapterInterface *adp);
		SgGridAdapterInterface *getGridAdapter();
		virtual void clear() = 0;

	};
}