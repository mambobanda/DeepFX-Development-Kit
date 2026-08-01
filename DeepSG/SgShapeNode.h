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

#include <glm/vec3.hpp>
#include "SgNode.h"
#include "SgShape.h"
#include "SgTransform.h"

namespace deepsg {

	class SgShapeNode : public SgNode {
	private:
		SgShape *_shape;
		glm::ivec3 color;
		unsigned int _shapeType;

	public:

		SgShapeNode();
		virtual ~SgShapeNode();

		void setShape(SgShape *);
		SgShape *shape();

		glm::ivec3 getColor();
		void setColor(glm::ivec3 cl);

		unsigned int shapeType();
		void setShapeType(unsigned int type);

		virtual std::string name();
	};

}