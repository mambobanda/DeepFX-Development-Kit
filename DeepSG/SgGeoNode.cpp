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

#include "SgGeoNode.h"
#include "SgVertexFilter.h"


namespace deepsg {

	SgGeoNode::SgGeoNode() :SgNode(SgNodeType::GEOMETRY) {
		_interface = nullptr;
		_vertex_buffer = nullptr;
		_vertex_buffer_size = 0;
		_vertexBufferUpdate = false;

		color = glm::vec3(0);
		independent = false;

		_vertexBufferDirty = true;

		_dynaMesh = nullptr;
		_dynaMeshRequestCounter = 0;
	}

	SgGeoNode::~SgGeoNode() {

	}

	void SgGeoNode::setGeometryAdapter(SgGeoAdapterInterface *_inter) {
		_interface = _inter;
	}

	SgGeoAdapterInterface *SgGeoNode::getGeometryAdapter() {
		if (_dynaMeshRequestCounter > 0 && _dynaMesh != nullptr) {
			return _dynaMesh->getGeometryAdapter();
		}
		else {
			return _interface;
		}
	}

	void SgGeoNode::clearAdapter() {
		_interface = nullptr;
	}

	bool SgGeoNode::isNull() {
		return _interface == nullptr;
	}

	glm::ivec3 SgGeoNode::getColor() {
		return color;
	}

	void SgGeoNode::setColor(glm::ivec3 cl) {
		color = cl;
	}

	void SgGeoNode::clearVertexBuffer() {
		delete[] _vertex_buffer;

		_vertex_buffer = nullptr;
		_vertex_buffer_size = 0;
	}

	void SgGeoNode::initVertexBuffer(size_t vertex_count, float initial_val) {
		clearVertexBuffer();

		if (vertex_count > 0) {
			_vertex_buffer = new float[vertex_count * 3];
			_vertex_buffer_size = vertex_count;

			for (size_t i = 0; i < (vertex_count * 3); i++) _vertex_buffer[i] = initial_val;
		}
	}

	glm::vec3 SgGeoNode::getVertex(size_t index) {
		size_t offset = index * 3;
		return glm::vec3(_vertex_buffer[offset], _vertex_buffer[offset + 1], _vertex_buffer[offset + 2]);
	}

	size_t SgGeoNode::vertexBufferSize() {
		return _vertex_buffer_size;
	}

	void SgGeoNode::setVertex(size_t index, glm::vec3 vert) {
		size_t offset = index * 3;

		_vertex_buffer[offset]	= vert.x;
		_vertex_buffer[offset + 1] = vert.y;
		_vertex_buffer[offset + 2] = vert.z;
	}

	float *SgGeoNode::vertexBuffer() {
		return _vertex_buffer;
	}

	void  SgGeoNode::copyBaseToVertexBuffer() {
		if (_interface != nullptr) {
			if (_interface->getVertexCount() > 0) {
				clearVertexBuffer();
				_vertex_buffer = _interface->getFlatVertexList();
				_vertex_buffer_size = _interface->getVertexCount();
			}
		}
	}

	bool SgGeoNode::isVertexBufferDirty() {
		return _vertexBufferDirty;
	}

	void SgGeoNode::disableDirtyVertexBufferFlag() {
		_vertexBufferDirty = false;
	}

	void SgGeoNode::enableDirtyVertexBufferFlag() {
		_vertexBufferDirty = true;
	}

	void SgGeoNode::addVertexFilter(SgVertexFilter *flt) {
		flt->setNode(this);
		_vertexfilters.push_back(flt);
	}

	void SgGeoNode::removeVertexFilter(SgVertexFilter *flt) {
		auto itr = _vertexfilters.begin();

		while (itr != _vertexfilters.end()) {

			if ((*itr)->id() == flt->id()) {
				(*itr)->setNode(nullptr);
				_vertexfilters.erase(itr);
				return;
			}

			itr++;
		}
	}

	void SgGeoNode::beginningEvaluatingVertexFilters() {}

	void SgGeoNode::evaluateVertexFilters() {
		auto adapter = SgGeoNode::getGeometryAdapter();

		if (adapter != nullptr) {
			if (adapter->getVertexCount() > 0) {

				void *meshPtr = adapter->meshPtr();
				size_t meshType = adapter->type();

				for (auto filter : _vertexfilters) {
				
					filter->begin(meshPtr, meshType);
					#pragma omp parallel default(shared)
					{
						#pragma omp for schedule(static)
						for (int inx = 0; inx < adapter->getVertexCount(); inx++) {
							int offset = inx * 3;

							glm::vec3 base_pos = adapter->getVertex(inx);
							glm::vec3 final_pos = glm::vec3(_vertex_buffer[offset], _vertex_buffer[offset + 1], _vertex_buffer[offset + 2]);	
							auto new_pos = filter->filter(base_pos, final_pos, inx, meshPtr, meshType);

							_vertex_buffer[offset] = new_pos.x;
							_vertex_buffer[offset + 1] = new_pos.y;
							_vertex_buffer[offset + 2] = new_pos.z;
						}
					}
					filter->end(meshPtr, meshType);

					_vertexBufferUpdate = true;
				}
			}
		} 
	}

	void SgGeoNode::finishedEvaluatingVertexFilters() {}

	bool SgGeoNode::hasVertexBufferUpdate() {
		return _vertexBufferUpdate;
	}

	void SgGeoNode::disableVertexBufferUpdateFlag() {
		_vertexBufferUpdate = false;
	}

	SgDynaMesh *SgGeoNode::getDynaMesh() {
		return _dynaMesh;
	}
}