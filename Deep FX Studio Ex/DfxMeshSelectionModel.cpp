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

#include "DfxMeshSelectionModel.h"

DfxMeshSelectionModel *DfxMeshSelectionModel::_instance = nullptr;

DfxMeshSelectionModel::DfxMeshSelectionModel() {
	reference = nullptr;
	updateSelectionReference();

	_vertexCenter = glm::vec3(0);
	_edgeCenter = glm::vec3(0);
	_faceCenter = glm::vec3(0);

	QObject::connect(DfxMode::getInstance(), &DfxMode::modeChanged, this, &DfxMeshSelectionModel::updateSelectionReference);
}

DfxMeshSelectionModel::~DfxMeshSelectionModel() {

}

DfxMeshSelectionModel *DfxMeshSelectionModel::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxMeshSelectionModel;
	}

	return _instance;
}

std::vector<size_t> &DfxMeshSelectionModel::selectedVertexIndices() {
	return _selectedVertexIndices;
}

std::vector<size_t> &DfxMeshSelectionModel::selectedEdgeIndices() {
	return _selectedEdgeIndices;
}

std::vector<size_t> &DfxMeshSelectionModel::selectedFaceIndices() {
	return _selectedFaceIndices;
}

std::vector<unsigned int> &DfxMeshSelectionModel::selectedFaceTriangleIndices() {
	return _selectedFaceTriangleIndices;
}

void DfxMeshSelectionModel::generateSelectedFaceTriangleIndices() {
	_selectedFaceTriangleIndices.clear();

	if (_selectedFaceIndices.size() > 0) {
		if (DfxMode::getInstance()->mode() == DfxMode::Mode::FACE_EDIT) {
			if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr &&
				!DfxMeshSelectionModel::getInstance()->selectedFaceIndices().empty()) {

				auto geo = ((deepsg::SgGeoNode*)DfxMeshSelectionModel::getInstance()->nodeReference())->getGeometryAdapter();

				if (geo->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {
					
					auto mesh = (ds::Mesh*)geo->meshPtr();

					std::vector<size_t> uids;
					for (auto j : DfxMeshSelectionModel::getInstance()->selectedFaceIndices()) {
						uids.push_back(mesh->faces()[j]->getUID());
					}

					((DfxDeepSimplexGeoAdapter*)geo)->getTriangleSelectedIndexList(uids, _selectedFaceTriangleIndices);
				}
				else if (geo->type() == DfxGeoAdapterType::DYNA_MESH) {
					
					((DfxDynaMeshGeoAdapter*)geo)->getTriangleSelectedIndexList(DfxMeshSelectionModel::getInstance()->selectedFaceIndices(), _selectedFaceTriangleIndices);
				}
			}
		}
	}
}

void DfxMeshSelectionModel::clearSelectedFaceTriangleIndices() {
	_selectedFaceTriangleIndices.clear();
}

deepsg::SgGeoNode *DfxMeshSelectionModel::nodeReference() {
	return reference;
}

void DfxMeshSelectionModel::setNodeReference(deepsg::SgGeoNode *nd) {
	reference = nd;
}

bool DfxMeshSelectionModel::hasVertexIndex(size_t inx) {
	auto itr = std::find(_selectedVertexIndices.begin(), _selectedVertexIndices.end(), inx);

	if (itr != _selectedVertexIndices.end()) return true;
	else return false;
}

bool DfxMeshSelectionModel::hasEdgeIndex(size_t inx) {
	auto itr = std::find(_selectedEdgeIndices.begin(), _selectedEdgeIndices.end(), inx);

	if (itr != _selectedEdgeIndices.end()) return true;
	else return false;
}

bool DfxMeshSelectionModel::hasFaceIndex(size_t inx) {
	auto itr = std::find(_selectedFaceIndices.begin(), _selectedFaceIndices.end(), inx);

	if (itr != _selectedFaceIndices.end()) return true;
	else return false;
}

bool DfxMeshSelectionModel::findReference() {

	bool referenceFound = false;

	if (!DfxSceneSelectionModel::getInstance()->selected().empty()) {
		if (DfxSceneSelectionModel::getInstance()->selected()[0]->getType() == deepsg::SgNodeType::GEOMETRY) {
			reference = (deepsg::SgGeoNode*)DfxSceneSelectionModel::getInstance()->selected()[0];
			referenceFound = true;
		}
	}

	if(!referenceFound){
		auto gnodes = deepsg::SgRootNode::getInstance()->findAllNodeTypes(deepsg::SgNodeType::GEOMETRY);

		if (!gnodes.empty()){
			reference = (deepsg::SgGeoNode*)gnodes[0];
			referenceFound = true;
		}
	}

	return referenceFound;
}

void DfxMeshSelectionModel::updateSelectionReference() {

	if (DfxMode::getInstance()->mode() == DfxMode::Mode::VERTEX_EDIT ||
		DfxMode::getInstance()->mode() == DfxMode::Mode::EDGE_EDIT   ||
		DfxMode::getInstance()->mode() == DfxMode::Mode::FACE_EDIT) {

		if (!findReference()) {
			reference = nullptr;
		}

	}
	else {
		reference = nullptr;
	}
}

glm::vec3 DfxMeshSelectionModel::vertexCenter() {
	return _vertexCenter;
}

glm::vec3 DfxMeshSelectionModel::edgeCenter() {
	return _edgeCenter;
}

glm::vec3 DfxMeshSelectionModel::faceCenter() {
	return _faceCenter;
}

void DfxMeshSelectionModel::calcVertexCenter(ds::Mesh* msh) {
	_vertexCenter = glm::vec3(0);

	for (auto v : _selectedVertexIndices) {
		_vertexCenter += msh->vertices()[v]->position();
	}

	_vertexCenter = _vertexCenter / (float)_selectedVertexIndices.size();
}

void DfxMeshSelectionModel::calcEdgeCenter(ds::Mesh* msh) {
	_edgeCenter = glm::vec3(0);

	for (auto v : _selectedEdgeIndices) {
		auto center = (msh->edges()[v]->v00()->position() + msh->edges()[v]->v01()->position()) / 2.f;
		_edgeCenter += center;
	}

	_edgeCenter = _edgeCenter / (float)_selectedEdgeIndices.size();
}

void DfxMeshSelectionModel::calcFaceCenter(ds::Mesh* msh) {
	_faceCenter = glm::vec3(0);

	for (auto v : _selectedFaceIndices) {
		glm::vec3 center(0);

		for (int i = 0; i < msh->faces()[v]->verticesCount(); i++) {
			center += msh->faces()[v]->vertices()[i]->position();
		}
		
		center = center / (float)msh->faces()[v]->verticesCount();
		_faceCenter += center;
	}

	_faceCenter = _faceCenter / (float)_selectedFaceIndices.size();
}