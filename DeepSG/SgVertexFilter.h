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
#include <DsMesh.h>
#include <DsMeshQuery.h>
#include "SgCommon.h"


namespace deepsg {

	class SgVertexFilter {
	private:
		static size_t ID_GEN;
		size_t _id;
		SgGeoNode *_node;

	public:

		SgVertexFilter();
		virtual ~SgVertexFilter();

		SgGeoNode *node();
		void setNode(SgGeoNode *nd);

		size_t id();

		virtual bool isVisible() = 0;
		virtual bool isEnabled() = 0;

		virtual void begin(void *meshPtr = nullptr, size_t meshType = 0) = 0;
		virtual void end(void *meshPtr = nullptr, size_t meshType = 0) = 0;

		virtual glm::vec3 filter(glm::vec3 base_pos, glm::vec3 final_pos, size_t index, void *meshPtr = nullptr, size_t meshType = 0) = 0;

	};

}