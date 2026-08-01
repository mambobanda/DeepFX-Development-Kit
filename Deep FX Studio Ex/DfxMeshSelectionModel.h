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

#include <QObject>
#include <vector>
#include <algorithm>
#include <SgGeoNode.h>
#include <SgRootNode.h>
#include <DsMesh.h>
#include <DfxDynaMeshGeoAdapter.h>
#include "DfxSceneSelectionModel.h"
#include "DfxMode.h"


class DfxMeshSelectionModel: public QObject {
	Q_OBJECT
private:
	static DfxMeshSelectionModel *_instance;

	deepsg::SgGeoNode *reference;

	std::vector<size_t> _selectedVertexIndices;
	std::vector<size_t> _selectedEdgeIndices;
	std::vector<size_t> _selectedFaceIndices;

	std::vector<unsigned int> _selectedFaceTriangleIndices;

	glm::vec3 _vertexCenter;
	glm::vec3 _edgeCenter;
	glm::vec3 _faceCenter;

	DfxMeshSelectionModel();
public:
	virtual ~DfxMeshSelectionModel();

	static DfxMeshSelectionModel *getInstance();

	std::vector<size_t> &selectedVertexIndices();
	std::vector<size_t> &selectedEdgeIndices();
	std::vector<size_t> &selectedFaceIndices();

	std::vector<unsigned int> &selectedFaceTriangleIndices();

	void generateSelectedFaceTriangleIndices();
	void clearSelectedFaceTriangleIndices();

	deepsg::SgGeoNode *nodeReference();
	void setNodeReference(deepsg::SgGeoNode *nd);

	bool hasVertexIndex(size_t inx);
	bool hasEdgeIndex(size_t inx);
	bool hasFaceIndex(size_t inx);

	glm::vec3 vertexCenter();
	glm::vec3 edgeCenter();
	glm::vec3 faceCenter();

	void calcVertexCenter(ds::Mesh*);
	void calcEdgeCenter(ds::Mesh*);
	void calcFaceCenter(ds::Mesh*);

	bool findReference();

public slots:
	void updateSelectionReference();
};