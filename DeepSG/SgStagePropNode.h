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
#include "SgShape.h"
#include "SgStageCommon.h"

namespace deepsg {

	class SgStagePropNode : public SgNode {
	private:
		SgStageNode *_stage;
		size_t _type;

	protected:
		glm::ivec3 _color;
		SgShape *_shape;

	public:

		SgStagePropNode(SgStageNode *stg, size_t type);
		virtual ~SgStagePropNode();

		size_t propType();

		void setColor(glm::ivec3 ivc);
		glm::ivec3 getColor();

		void addShape(SgShape* );
		virtual SgShape *shape();

		SgStageNode *stage();
		void setStage(SgStageNode *);
	};

};