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
#include <atomic>
#include "SgGeoAdapterInterface.h"
#include "SgCommon.h"
#include "SgNode.h"
#include "SgDynaMesh.h"
#include "SgTransform.h"

namespace deepsg {

	class SgGeoNode : public SgNode {
	private:
	
		SgGeoAdapterInterface *_interface;
		std::vector<SgVertexFilter*> _vertexfilters;

		glm::ivec3 color;

		bool _vertexBufferUpdate;

	protected:
		SgDynaMesh *_dynaMesh;
		std::atomic<unsigned int> _dynaMeshRequestCounter;

	private:
		float *_vertex_buffer;
		size_t _vertex_buffer_size;
	private:
		bool _vertexBufferDirty;
	public:

		SgGeoNode();
		virtual ~SgGeoNode();

		void setGeometryAdapter(SgGeoAdapterInterface *);
		SgGeoAdapterInterface *getGeometryAdapter();

		void clearAdapter();
		bool isNull();

		glm::ivec3 getColor();
		void setColor(glm::ivec3 cl);

		void clearVertexBuffer();
		void initVertexBuffer(size_t vertex_count, float initial_val = 0.f);
		void copyBaseToVertexBuffer();
		size_t vertexBufferSize();
		glm::vec3 getVertex(size_t index);
		void setVertex(size_t index, glm::vec3 vert);
		float *vertexBuffer();

		bool isVertexBufferDirty();
		void disableDirtyVertexBufferFlag();
		void enableDirtyVertexBufferFlag();

		void addVertexFilter(SgVertexFilter *);
		void removeVertexFilter(SgVertexFilter *);

		virtual void beginningEvaluatingVertexFilters();
		void evaluateVertexFilters();
		virtual void finishedEvaluatingVertexFilters();

		bool hasVertexBufferUpdate();
		void disableVertexBufferUpdateFlag();

		SgDynaMesh *getDynaMesh();

		virtual bool requestDynaMesh() = 0;
		virtual void releaseDynaMesh() = 0;
	};
}
