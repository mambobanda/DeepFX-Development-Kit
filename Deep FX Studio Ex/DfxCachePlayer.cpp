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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxCachePlayer.h"
#include "DfxOpenGLCacheQueue.h"


DfxCachePlayer::DfxCachePlayer() {
	_snapshot = new DfxCacheSnapshot;;
	_currentPath = "";

	_simplexCollectionSize = 0;
}

DfxCachePlayer::~DfxCachePlayer() {
	_snapshot->clearAll();
	delete _snapshot;
}

DfxCacheSnapshot *DfxCachePlayer::snapshot() {
	return _snapshot;
}

void DfxCachePlayer::updateSimplexObject(xvault::File *ifile, size_t index, xvault::Node *node) {

	DfxFluentSimplexObject *simObj = nullptr;

	if (_simplexObjects[index] == nullptr) {
		simObj = new DfxFluentSimplexObject;
		_simplexObjects[index] = simObj;
	}
	else {
		simObj = _simplexObjects[index];
	}

	auto vertexBufferRef = node->attributes()[0]->valueString();
	auto vertexNormalBufferRef = node->attributes()[1]->valueString();
	auto uvBufferRef = node->attributes()[2]->valueString();
	auto materialIDBufferRef = node->attributes()[3]->valueString();
	auto indexBufferRef = node->attributes()[4]->valueString();

	bool bufferUpdated = false;

	// Update Vertex Buffer
	if (vertexBufferRef == "empty") {
		_simplexObjectInfo[index].vertexBufferReference = vertexBufferRef;
		simObj->setVertexCount(0);
		simObj->setTriangleCount(0);

		bufferUpdated = true;
	}
	else if (vertexBufferRef != _simplexObjectInfo[index].vertexBufferReference) {
		auto iResource = new xvault::h5::h5IResource();
		_simplexObjectInfo[index].vertexBufferReference = vertexBufferRef;

		if (ifile->readResource(vertexBufferRef, *iResource)) {
			if (simObj->vertices() != nullptr) simObj->clearVertices();
			
			simObj->setVertices(iResource->buffers()[0]->bufferFloat());
			simObj->setVertexCount(iResource->buffers()[0]->bufferSize() / 3);
			
			((xvault::h5::h5Buffer*)iResource->buffers()[0])->setAutoClear(false);

		}

		bufferUpdated = true;
		delete iResource;
	}

	// Update Vertex Normal Buffer
	if (vertexNormalBufferRef == "empty") {
		_simplexObjectInfo[index].vertexNormalBufferReference = vertexNormalBufferRef;

		bufferUpdated = true;
	}
	else if (vertexNormalBufferRef != _simplexObjectInfo[index].vertexNormalBufferReference) {
		auto iResource = new xvault::h5::h5IResource();
		_simplexObjectInfo[index].vertexNormalBufferReference = vertexNormalBufferRef;

		if (ifile->readResource(vertexNormalBufferRef, *iResource)) {
			if (simObj->smoothNormals() != nullptr) simObj->clearSmoothNormals();

			simObj->setSmoothNormals(iResource->buffers()[0]->bufferFloat());

			((xvault::h5::h5Buffer*)iResource->buffers()[0])->setAutoClear(false);

		}

		bufferUpdated = true;
		delete iResource;
	}

	// Update Uv Buffer
	if (uvBufferRef == "empty") {
		_simplexObjectInfo[index].uvBufferReference = uvBufferRef;

		bufferUpdated = true;
	}
	else if (uvBufferRef != _simplexObjectInfo[index].uvBufferReference) {
		auto iResource = new xvault::h5::h5IResource();
		_simplexObjectInfo[index].uvBufferReference = uvBufferRef;

		if (ifile->readResource(uvBufferRef, *iResource)) {
			if (simObj->uvs() != nullptr) simObj->clearUvs();

			simObj->setUvs(iResource->buffers()[0]->bufferFloat());

			((xvault::h5::h5Buffer*)iResource->buffers()[0])->setAutoClear(false);

		}

		bufferUpdated = true;
		delete iResource;
	}

	// Update MaterialID Buffer
	if (materialIDBufferRef == "empty") {
		_simplexObjectInfo[index].materialIDBufferReference = materialIDBufferRef;

		bufferUpdated = true;
	}
	else if (materialIDBufferRef != _simplexObjectInfo[index].materialIDBufferReference) {
		auto iResource = new xvault::h5::h5IResource();
		_simplexObjectInfo[index].materialIDBufferReference = materialIDBufferRef;

		if (ifile->readResource(materialIDBufferRef, *iResource)) {
			if (simObj->indices() != nullptr) simObj->clearIndices();

			simObj->setFaceIDs(iResource->buffers()[0]->bufferUInt());

			((xvault::h5::h5Buffer*)iResource->buffers()[0])->setAutoClear(false);

		}

		bufferUpdated = true;
		delete iResource;
	}

	// Update Index Buffer
	if (indexBufferRef == "empty") {
		_simplexObjectInfo[index].indexBufferReference = indexBufferRef;

		bufferUpdated = true;
	}
	else if (indexBufferRef != _simplexObjectInfo[index].indexBufferReference) {
		auto iResource = new xvault::h5::h5IResource();
		_simplexObjectInfo[index].indexBufferReference = indexBufferRef;

		if (ifile->readResource(indexBufferRef, *iResource)) {
			if (simObj->indices() != nullptr) simObj->clearIndices();

			simObj->setIndices(iResource->buffers()[0]->bufferUInt());
			simObj->setTriangleCount(iResource->buffers()[0]->bufferSize() / 3);

			((xvault::h5::h5Buffer*)iResource->buffers()[0])->setAutoClear(false);

		}

		bufferUpdated = true;
		delete iResource;
	}

	// Cache Mesh
	if (simObj->vertexCount() > 0 && simObj->triangleCount() > 0 && bufferUpdated) {
		simObj->buildEdgeList();
		simObj->cache()->clear();
		DfxOpenGLCacheQueue::getInstance()->queueCache(simObj->cache());
	}
}

DfxFluentSet *DfxCachePlayer::retrieveSet(xvault::Node *node) {
	auto set = new DfxFluentSet(0);

	auto name = node->attributes()[0];
	auto type = node->attributes()[1];
	auto color = node->attributes()[2];
	auto visible = node->attributes()[3];

	set->setName(name->valueString());
	set->setType((uint8_t)type->valueUInt());
	set->setColor(color->valueFloat3());
	set->setIsVisible(visible->valueUInt() == 0 ? false : true);

	for (auto index: node->nodes()[0]->attributes()) {
		set->indices().push_back(index->valueUInt());
	}

	return set;
}

void DfxCachePlayer::loadAttribute(xvault::File *ifile, DfxFluentChannelMap &map, std::string path) {

	auto ibuffer = new xvault::h5::h5IResource();

	if (ifile->readResource(path, *ibuffer)) {
		auto name = ibuffer->attributes()[0];
		auto type = ibuffer->attributes()[2];

		if (type->valueString() == "float") {
			map.addAttribute<float>(name->valueString());

			if (ibuffer->buffers()[0]->bufferSize() > 0) {
				if (map.size() != ibuffer->buffers()[0]->bufferSize()) map.resize(ibuffer->buffers()[0]->bufferSize());

				auto fattribute = map.at<float>(name->valueString());
				for (size_t i = 0; i < ibuffer->buffers()[0]->bufferSize(); i++) {
					fattribute[i] = ibuffer->buffers()[0]->bufferFloat()[i];
				}
			}
		}
		else if (type->valueString() == "vec3") {
			map.addAttribute<glm::vec3>(name->valueString());

			if (ibuffer->buffers()[0]->bufferSize() > 0) {
				if (map.size() != ibuffer->buffers()[0]->bufferSize()) map.resize(ibuffer->buffers()[0]->bufferSize());

				auto fattribute = map.at<glm::vec3>(name->valueString());
				for (size_t i = 0; i < ibuffer->buffers()[0]->bufferSize() / 3; i++) {
					auto offset = i * 3;
					fattribute[i] = glm::vec3(
						ibuffer->buffers()[0]->bufferFloat()[offset],
						ibuffer->buffers()[0]->bufferFloat()[offset + 1],
						ibuffer->buffers()[0]->bufferFloat()[offset + 2]);
				}
			}
		}
		else if (type->valueString() == "ULongLong") {
			map.addAttribute<size_t>(name->valueString());

			if (ibuffer->buffers()[0]->bufferSize() > 0) {
				if (map.size() != ibuffer->buffers()[0]->bufferSize()) map.resize(ibuffer->buffers()[0]->bufferSize());

				auto fattribute = map.at<size_t>(name->valueString());
				for (size_t i = 0; i < ibuffer->buffers()[0]->bufferSize(); i++) {
					fattribute[i] = ibuffer->buffers()[0]->bufferULongLong()[i];
				}
			}
		}
		else if (type->valueString() == "mat4") {
			map.addAttribute<glm::mat4>(name->valueString());

			if (ibuffer->buffers()[0]->bufferSize() > 0) {
				if (map.size() != ibuffer->buffers()[0]->bufferSize()) map.resize(ibuffer->buffers()[0]->bufferSize());

				auto fattribute = map.at<glm::mat4>(name->valueString());
				for (size_t i = 0; i < ibuffer->buffers()[0]->bufferSize() / 16; i++) {
					auto offset = i * 16;
					glm::mat4 mat;
					auto matPtr = &mat[0][0];

					matPtr[0] = ibuffer->buffers()[0]->bufferFloat()[offset];
					matPtr[1] = ibuffer->buffers()[0]->bufferFloat()[offset + 1];
					matPtr[2] = ibuffer->buffers()[0]->bufferFloat()[offset + 2];
					matPtr[3] = ibuffer->buffers()[0]->bufferFloat()[offset + 3];
					matPtr[4] = ibuffer->buffers()[0]->bufferFloat()[offset + 4];
					matPtr[5] = ibuffer->buffers()[0]->bufferFloat()[offset + 5];
					matPtr[6] = ibuffer->buffers()[0]->bufferFloat()[offset + 6];
					matPtr[7] = ibuffer->buffers()[0]->bufferFloat()[offset + 7];
					matPtr[8] = ibuffer->buffers()[0]->bufferFloat()[offset + 8];
					matPtr[9] = ibuffer->buffers()[0]->bufferFloat()[offset + 9];
					matPtr[10] = ibuffer->buffers()[0]->bufferFloat()[offset + 10];
					matPtr[11] = ibuffer->buffers()[0]->bufferFloat()[offset + 11];
					matPtr[12] = ibuffer->buffers()[0]->bufferFloat()[offset + 12];
					matPtr[13] = ibuffer->buffers()[0]->bufferFloat()[offset + 13];
					matPtr[14] = ibuffer->buffers()[0]->bufferFloat()[offset + 14];
					matPtr[15] = ibuffer->buffers()[0]->bufferFloat()[offset + 15];

					fattribute[i] = mat;
				}
			}
		}
		else if (type->valueString() == "string") {
			map.addAttribute<std::string>(name->valueString());

			if (ibuffer->buffers()[0]->bufferSize() > 0) {

				QString arr(ibuffer->buffers()[0]->bufferChar());
				auto list = arr.split("~!");

				if (map.size() != list.size()) map.resize(list.size());

				auto fattribute = map.at<std::string>(name->valueString());
				for (size_t i = 0; i < list.size(); i++) {
					fattribute[i] = list[i].toStdString();
				}
			}
		}
	}

	delete ibuffer;
	
}

void DfxCachePlayer::clear() {
	for (auto simObj : _simplexObjects) delete simObj;
	_simplexObjects.clear();
}

void DfxCachePlayer::takeSnapshot(std::string cachePath, int currentFrame, int offset) {
	
	_snapshot->clearAll();

	if (_currentPath != cachePath) {
		_currentPath = cachePath;
		clear();
	}

	filesystem::path path(cachePath);

	if (cachePath.empty()) {
		return;
	}

	if (!filesystem::exists(cachePath)) {
		DfxGlobal::console::println("Failed to take snap shot of cache: File path doesn't exist.");
		return;
	}

	std::unique_ptr<xvault::File> ifile(new xvault::h5::h5File);

	if (ifile->open(cachePath, xvault::io::OPEN_READ_ONLY)) {
		auto iHeader = new xvault::h5::h5IResource();
		auto iDocument = new xvault::h5::h5IResource();
	
		if (ifile->readResource("/header", *iHeader)) {

			auto format = iHeader->findAttribute("format");
			auto version = iHeader->findAttribute("version");
			auto startingFrame = iHeader->findAttribute("startingFrame");
			auto stepCount = iHeader->findAttribute("stepCount");

			if (format == nullptr || version == nullptr || startingFrame == nullptr || stepCount == nullptr) {
				DfxGlobal::console::println("Failed to take snap shot of cache: Broken Header.");
				delete iHeader;
				return;
			}

			if (DfxCacheFormat::format != format->valueString()) {
				DfxGlobal::console::println("Failed to take snap shot of cache: wrong format.");
				delete iHeader;
				return;
			}

			int stFrame = std::atoi(startingFrame->valueString().c_str());
			int stCount = std::atoi(stepCount->valueString().c_str());
			int currentStep = currentFrame - stFrame;

			if (currentFrame < stFrame) {
				delete iHeader;
				delete iDocument;
				return;
			}

			if (currentStep > stCount) {
				delete iHeader;
				delete iDocument;
				return;
			}

			if(ifile->readResource("/step/" + std::to_string(currentStep), *iDocument)) {
				auto document = iDocument->findBuffer("document");

				if (document != nullptr) {
					xvault::rapid::rjDocument rStepDocument;

					std::string errorMsg("");
					if (rStepDocument.fromString(document->bufferChar(), errorMsg)) {

						auto sets = rStepDocument.root()->findNodeByName("sets");
						auto simplex = rStepDocument.root()->findNodeByName("simplex");
						auto simplex_attr = rStepDocument.root()->findNodeByName("simplex_attributes");
						auto stream_attr  = rStepDocument.root()->findNodeByName("stream_attributes");

						if (sets != nullptr && simplex != nullptr && simplex_attr != nullptr && stream_attr != nullptr) {

							if (_simplexCollectionSize < simplex->nodes().size()) {
								_simplexCollectionSize = simplex->nodes().size();

								DfxCacheSimplexInfo tempInfo;
								tempInfo.vertexBufferReference = "empty00";
								tempInfo.vertexNormalBufferReference = "empty00";
								tempInfo.uvBufferReference = "empty00";
								tempInfo.indexBufferReference = "empty00";

								_simplexObjects.resize(simplex->nodes().size(), nullptr);
								_simplexObjectInfo.resize(simplex->nodes().size(), tempInfo);

							}

							for (auto node : simplex->nodes()) {
								auto index = std::atoi(node->name().c_str());
								updateSimplexObject(ifile.get(), index, node);
							}

							_snapshot->simplexObjects() = _simplexObjects;

							for (auto node : sets->nodes()) {
								auto set = retrieveSet(node);
								_snapshot->sets().push_back(set);
							}

							for (auto attribute : simplex_attr->attributes()) {
								loadAttribute(ifile.get(), _snapshot->simplexAttributes(), attribute->valueString());
							}

							for (auto attribute : stream_attr->attributes()) {
								loadAttribute(ifile.get(), _snapshot->streamAttributes(), attribute->valueString());
							}
						}
						else {
							DfxGlobal::console::println("Failed load step document.");
						}
					}
					else {
						DfxGlobal::console::println("Failed to parse step document: \n" + errorMsg);
					}
				}
						
			}

			delete iHeader;
			delete iDocument;
		}
		else {
			DfxGlobal::console::println(std::string("Failed to take snap shot of cache: ") + std::string("Couldn't find header."));
			return;
		}
	}
	else {
		DfxGlobal::console::println("Failed to take snap shot of cache: Failed to open file.");
		return;
	}
}