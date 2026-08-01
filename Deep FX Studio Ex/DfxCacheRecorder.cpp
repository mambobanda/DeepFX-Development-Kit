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

#include "DfxCacheRecorder.h"

DfxCacheRecorder::DfxCacheRecorder(xvault::File *file) {
	ID_GEN = 0x3e8;

	_file = file;

	startingFrame = 0;
	simplexSizeTracker = 0;
	stepCounter = 0;
}

DfxCacheRecorder::~DfxCacheRecorder() {

}

std::vector<std::string>  DfxCacheRecorder::saveAttributes(DfxFluentChannelMap &map, std::string pathPrefix) {
	std::vector<std::string> output;

	if (map.size() > 0) {
		for (auto channel : map.allAttributes()) {
			if (channel.second->cType() == DfxFluenType::FLOAT) {
				auto buffer = new float[map.size()];

				for (size_t i = 0; i < map.size(); i++) buffer[i] = channel.second->at<float>()[i];

				xvault::h5::h5OBufferResource bufferResource("buffer", "float");
				auto xbuffer = new xvault::h5::h5Buffer();

				xbuffer->setName("buffer");
				xbuffer->setType(xvault::type::FLOAT);
				xbuffer->setBufferSize(map.size());
				xbuffer->setBufferFloat(buffer);

				auto _attrNameAttribute = new xvault::h5::h5Attribute;
				_attrNameAttribute->setName("__attribute_name__");
				_attrNameAttribute->setType(xvault::type::STRING);
				_attrNameAttribute->setValueString(channel.first);

				bufferResource.setBuffer(xbuffer);
				bufferResource.addAttribute(_attrNameAttribute);
				bufferResource.finalize();

				auto path = pathPrefix + std::to_string(++ID_GEN);
				_file->writeResource(path, bufferResource);

				output.push_back(path);
			}
			else if (channel.second->cType() == DfxFluenType::VEC3) {
				auto buffer = new float[map.size() * 3];

				for (size_t i = 0; i < map.size(); i++) {
					size_t offset = i * 3;
					auto vec = channel.second->at<glm::vec3>()[i];

					buffer[offset] = vec.x;
					buffer[offset + 1] = vec.y;
					buffer[offset + 2] = vec.z;
				}

				xvault::h5::h5OBufferResource bufferResource("buffer", "vec3");
				auto xbuffer = new xvault::h5::h5Buffer();

				xbuffer->setName("buffer");
				xbuffer->setType(xvault::type::FLOAT);
				xbuffer->setBufferSize(map.size() * 3);
				xbuffer->setBufferFloat(buffer);

				auto _attrNameAttribute = new xvault::h5::h5Attribute;
				_attrNameAttribute->setName("__attribute_name__");
				_attrNameAttribute->setType(xvault::type::STRING);
				_attrNameAttribute->setValueString(channel.first);

				bufferResource.setBuffer(xbuffer);
				bufferResource.addAttribute(_attrNameAttribute);
				bufferResource.finalize();

				auto path = pathPrefix + std::to_string(++ID_GEN);
				_file->writeResource(path, bufferResource);

				output.push_back(path);
			}
			else if (channel.second->cType() == DfxFluenType::SIZE_T) {
				auto buffer = new unsigned long long[map.size()];

				for (size_t i = 0; i < map.size(); i++) buffer[i] = channel.second->at<size_t>()[i];

				xvault::h5::h5OBufferResource bufferResource("buffer", "ULongLong");
				auto xbuffer = new xvault::h5::h5Buffer();

				xbuffer->setName("buffer");
				xbuffer->setType(xvault::type::UNSIGNED_LONG_LONG);
				xbuffer->setBufferSize(map.size());
				xbuffer->setBufferULongLong(buffer);

				auto _attrNameAttribute = new xvault::h5::h5Attribute;
				_attrNameAttribute->setName("__attribute_name__");
				_attrNameAttribute->setType(xvault::type::STRING);
				_attrNameAttribute->setValueString(channel.first);

				bufferResource.setBuffer(xbuffer);
				bufferResource.addAttribute(_attrNameAttribute);
				bufferResource.finalize();

				auto path = pathPrefix + std::to_string(++ID_GEN);
				_file->writeResource(path, bufferResource);

				output.push_back(path);
			}
			else if (channel.second->cType() == DfxFluenType::MAT4) {
				auto buffer = new float[map.size() * 16];

				for (size_t i = 0; i < map.size(); i++) {

					size_t offset = i * 16;
					auto mat = channel.second->at<glm::mat4>()[i];
					const float *pSource = (const float*)glm::value_ptr(mat);

					buffer[offset]		= pSource[0];
					buffer[offset + 1]	= pSource[1];
					buffer[offset + 2]	= pSource[2];
					buffer[offset + 3]	= pSource[3];
					buffer[offset + 4]	= pSource[4];
					buffer[offset + 5]	= pSource[5];
					buffer[offset + 6]	= pSource[6];
					buffer[offset + 7]	= pSource[7];
					buffer[offset + 8]	= pSource[8];
					buffer[offset + 9]	= pSource[9];
					buffer[offset + 10] = pSource[10];
					buffer[offset + 11] = pSource[11];
					buffer[offset + 12] = pSource[12];
					buffer[offset + 13] = pSource[13];
					buffer[offset + 14] = pSource[14];
					buffer[offset + 15] = pSource[15];
			
				}

				xvault::h5::h5OBufferResource bufferResource("buffer", "mat4");
				auto xbuffer = new xvault::h5::h5Buffer();

				xbuffer->setName("buffer");
				xbuffer->setType(xvault::type::FLOAT);
				xbuffer->setBufferSize(map.size() * 16);
				xbuffer->setBufferFloat(buffer);

				auto _attrNameAttribute = new xvault::h5::h5Attribute;
				_attrNameAttribute->setName("__attribute_name__");
				_attrNameAttribute->setType(xvault::type::STRING);
				_attrNameAttribute->setValueString(channel.first);

				bufferResource.setBuffer(xbuffer);
				bufferResource.addAttribute(_attrNameAttribute);
				bufferResource.finalize();

				auto path = pathPrefix + std::to_string(++ID_GEN);
				_file->writeResource(path, bufferResource);

				output.push_back(path);
			}
			else if (channel.second->cType() == DfxFluenType::STRING) {
				std::stringstream ss;
				for (size_t i = 0; i < map.size(); i++) {
					if (i == (map.size() - 1)) ss << channel.second->at<std::string>()[i];
					else ss << channel.second->at<std::string>()[i] << "~!";
				}

				std::string strBuffer = ss.str();

				char *cstr = new char[strBuffer.length() + 1];
				strcpy_s(cstr, strBuffer.length() + 1, strBuffer.c_str());

				xvault::h5::h5OBufferResource bufferResource("buffer", "string");
				auto xbuffer = new xvault::h5::h5Buffer();

				xbuffer->setName("buffer");
				xbuffer->setType(xvault::type::CHAR);
				xbuffer->setBufferSize(strBuffer.length() + 1);
				xbuffer->setBufferChar(cstr);

				auto _attrNameAttribute = new xvault::h5::h5Attribute;
				_attrNameAttribute->setName("__attribute_name__");
				_attrNameAttribute->setType(xvault::type::STRING);
				_attrNameAttribute->setValueString(channel.first);

				bufferResource.setBuffer(xbuffer);
				bufferResource.addAttribute(_attrNameAttribute);
				bufferResource.finalize();

				auto path = pathPrefix + std::to_string(++ID_GEN);
				_file->writeResource(path, bufferResource);

				output.push_back(path);
			}
		}
	}

	return output;
}

void DfxCacheRecorder::saveSimplexResource(DfxFluentSimplexObject *simObj, DfxCacheSimplexInfo &info) {

	// Save Vertex Buffer
	if (simObj->vertexCount() > 0 && info.updateVertexBuffer) {
		xvault::h5::h5OBufferResource bufferResource("buffer", "float");
		auto buffer = new xvault::h5::h5Buffer(false);

		buffer->setName("buffer");
		buffer->setType(xvault::type::FLOAT);
		buffer->setBufferSize(simObj->vertexCount() * 3);
		buffer->setBufferFloat(simObj->vertices());

		bufferResource.setBuffer(buffer);
		bufferResource.finalize();

		_file->writeResource(info.vertexBufferReference, bufferResource);
	}
	else if (simObj->vertexCount() <= 0) {
		info.vertexBufferReference = "empty";
	}

	// Save Vertex Normals
	if (simObj->vertexCount() > 0 && info.updateVertexNormalBuffer){
		xvault::h5::h5OBufferResource bufferResource("buffer", "float");
		auto buffer = new xvault::h5::h5Buffer(false);

		buffer->setName("buffer");
		buffer->setType(xvault::type::FLOAT);
		buffer->setBufferSize(simObj->vertexCount() * 3);
		buffer->setBufferFloat(simObj->smoothNormals());

		bufferResource.setBuffer(buffer);
		bufferResource.finalize();

		_file->writeResource(info.vertexNormalBufferReference, bufferResource);
	}
	else if(simObj->vertexCount() <= 0){
		info.vertexNormalBufferReference = "empty";
	}

	// Save uvs
	if (simObj->triangleCount() > 0 && info.updateUvBuffer) {
		xvault::h5::h5OBufferResource bufferResource("buffer", "float");
		auto buffer = new xvault::h5::h5Buffer(false);

		buffer->setName("buffer");
		buffer->setType(xvault::type::FLOAT);
		buffer->setBufferSize(simObj->triangleCount() * 3 * 2);
		buffer->setBufferFloat(simObj->uvs());

		bufferResource.setBuffer(buffer);
		bufferResource.finalize();

		_file->writeResource(info.uvBufferReference, bufferResource);
	}
	else if (simObj->triangleCount() <= 0) {
		info.uvBufferReference = "empty";
	}

	// Save MaterialID
	if (simObj->triangleCount() > 0 && info.updateMaterialIDBuffer) {
		xvault::h5::h5OBufferResource bufferResource("buffer", "uint");
		auto buffer = new xvault::h5::h5Buffer(false);

		buffer->setName("buffer");
		buffer->setType(xvault::type::UNSIGNED_INT);
		buffer->setBufferSize(simObj->triangleCount());
		buffer->setBufferUInt(simObj->faceIDs());

		bufferResource.setBuffer(buffer);
		bufferResource.finalize();

		_file->writeResource(info.materialIDBufferReference, bufferResource);
	}
	else if (simObj->triangleCount() <= 0) {
		info.materialIDBufferReference = "empty";
	}

	// Save Indices
	if (simObj->triangleCount() > 0 && info.updateIndexBuffer) {
		xvault::h5::h5OBufferResource bufferResource("buffer", "uint");
		auto buffer = new xvault::h5::h5Buffer(false);

		buffer->setName("buffer");
		buffer->setType(xvault::type::UNSIGNED_INT);
		buffer->setBufferSize(simObj->triangleCount() * 3);
		buffer->setBufferUInt(simObj->indices());

		bufferResource.setBuffer(buffer);
		bufferResource.finalize();

		_file->writeResource(info.indexBufferReference, bufferResource);
	}
	else if (simObj->triangleCount() <= 0) {
		info.indexBufferReference = "empty";
	}

	info.updateVertexBuffer = false;
	info.updateVertexNormalBuffer = false;
	info.updateUvBuffer = false;
	info.updateIndexBuffer = false;
}


void DfxCacheRecorder::resetVertexBufferInfo(DfxFluentSimplexObject *simObj, DfxCacheSimplexInfo &info, bool update) {
	info.vertexBufferVersion = simObj->vertexVersion();
	info.vertexBufferReference = "vertexBuffers/" + std::to_string(++ID_GEN);
	info.updateVertexBuffer = update;
}

void DfxCacheRecorder::resetVertexNormalBufferInfo(DfxFluentSimplexObject *simObj, DfxCacheSimplexInfo &info, bool update) {
	info.vertexNormalBufferVersion = simObj->vertexNormalVersion();
	info.vertexNormalBufferReference = "vertexNormalBuffers/" + std::to_string(++ID_GEN);
	info.updateVertexNormalBuffer = true;
}

void DfxCacheRecorder::resetUvBufferInfo(DfxFluentSimplexObject *simObj, DfxCacheSimplexInfo &info, bool update) {
	info.uvBufferVersion = simObj->uvVersion();
	info.uvBufferReference = "uvBuffers/" + std::to_string(++ID_GEN);
	info.updateUvBuffer = true;
}

void DfxCacheRecorder::resetMaterialIDBufferInfo(DfxFluentSimplexObject *simObj, DfxCacheSimplexInfo &info, bool update) {
	info.materialIDBufferVersion = simObj->materialIDVersion();
	info.materialIDBufferReference = "materialIDBuffers/" + std::to_string(++ID_GEN);
	info.updateMaterialIDBuffer = true;
}

void DfxCacheRecorder::resetIndexBufferInfo(DfxFluentSimplexObject *simObj, DfxCacheSimplexInfo &info, bool update) {
	info.indexBufferVersion = simObj->indexVersion();
	info.indexBufferReference = "indexBuffers/" + std::to_string(++ID_GEN);
	info.updateIndexBuffer = true;
}

void DfxCacheRecorder::stepSimplexObjects() {

	auto simplexCollectionSize = DfxFluentSimplexCollection::getInstance()->size();
	auto oldSize = simplexSizeTracker;
	bool cacheNewSimplexObjects = false;

	if (simplexCollectionSize > simplexSizeTracker) {
		DfxCacheSimplexInfo sinfo;
		sinfo.indexBufferReference = "";
		sinfo.materialIDBufferReference = "";
		sinfo.uvBufferReference = "";
		sinfo.vertexBufferReference = "";
		sinfo.vertexNormalBufferReference = "";

		simplexInfo.resize(simplexCollectionSize);
		for (int i = oldSize; i < simplexCollectionSize; i++) {
			simplexInfo[i] = sinfo;
		}

		cacheNewSimplexObjects = true;
		simplexSizeTracker = simplexCollectionSize;
	}

	if (cacheNewSimplexObjects) {

		for (int i = oldSize; i < simplexCollectionSize; i++) {
			auto simObj = DfxFluentSimplexCollection::getInstance()->simplexObjects()[i];

			resetVertexBufferInfo(simObj, simplexInfo[i], true);
			resetVertexNormalBufferInfo(simObj, simplexInfo[i], true);
			resetUvBufferInfo(simObj, simplexInfo[i], true);
			resetMaterialIDBufferInfo(simObj, simplexInfo[i], true);
			resetIndexBufferInfo(simObj, simplexInfo[i], true);

			simplexInfo[i].topologyVersion = simObj->topolgyVersion();
		}
	}

	// Mark Simplex buffers that need to be updated
	{
		for (int i = 0; i < simplexCollectionSize; i++) {
			auto simObj = DfxFluentSimplexCollection::getInstance()->simplexObjects()[i];

			if (simplexInfo[i].topologyVersion == simObj->topolgyVersion()) {
				if (simplexInfo[i].vertexBufferVersion < simObj->vertexVersion()) {
					resetVertexBufferInfo(simObj, simplexInfo[i], true);
				}

				if (simplexInfo[i].vertexNormalBufferVersion < simObj->vertexNormalVersion()) {
					resetVertexNormalBufferInfo(simObj, simplexInfo[i], true);
				}

				if (simplexInfo[i].uvBufferVersion < simObj->uvVersion()) {
					resetUvBufferInfo(simObj, simplexInfo[i], true);
				}

				if (simplexInfo[i].materialIDBufferVersion < simObj->materialIDVersion()) {
					resetMaterialIDBufferInfo(simObj, simplexInfo[i], true);
				}
				 
				if (simplexInfo[i].indexBufferVersion < simObj->indexVersion()) {
					resetIndexBufferInfo(simObj, simplexInfo[i], true);
				}
			}
			else {
				resetVertexBufferInfo(simObj, simplexInfo[i], true);
				resetVertexNormalBufferInfo(simObj, simplexInfo[i], true);
				resetUvBufferInfo(simObj, simplexInfo[i], true);
				resetMaterialIDBufferInfo(simObj, simplexInfo[i], true);
				resetIndexBufferInfo(simObj, simplexInfo[i], true);

				simplexInfo[i].topologyVersion = simObj->topolgyVersion();
			}
		}
	}

	// Update needed buffers
	{
		for (int i = oldSize; i < simplexCollectionSize; i++) {
			auto simObj = DfxFluentSimplexCollection::getInstance()->simplexObjects()[i];
			saveSimplexResource(simObj, simplexInfo[i]);
		}
	}

}

void DfxCacheRecorder::stepSimplexAttributes() {
	simplexAttributePaths.clear();
	simplexAttributePaths =  saveAttributes(DfxFluentSimplexCollection::getInstance()->atrributeMap(), std::string("simplex_attributes/") + std::to_string(stepCounter) + "/");
}

void DfxCacheRecorder::stepStreamAttributes() {
	streamAttributePaths.clear();
	streamAttributePaths = saveAttributes(DfxFluentStreamCollection::getInstance()->atrributeMap(), std::string("stream_attributes/") + std::to_string(stepCounter) + "/");
}

void DfxCacheRecorder::stepMetadata() {

	xvault::Document *_vdoc = new xvault::rapid::rjDocument;

	xvault::Node *sets_node = new xvault::rapid::rjNode("sets");
	xvault::Node *simplex_nodes = new xvault::rapid::rjNode("simplex");
	xvault::Node *simplex_attribute_node = new xvault::rapid::rjNode("simplex_attributes");
	xvault::Node *stream_attribute_node  = new xvault::rapid::rjNode("stream_attributes");

	size_t simplexCounter = 0;
	for (auto &p : DfxFluentSetManager::getInstance()->setMap()) {	

		xvault::Node *set_node = new xvault::rapid::rjNode(std::to_string(simplexCounter++));
		xvault::Node *set_indices_node = new xvault::rapid::rjNode("indices");

		auto name = xvault::h5::h5Attribute::newValueString("name", xvault::type::STRING, p.second->name());
		set_node->attributes().push_back(name);

		auto type = xvault::h5::h5Attribute::newValueUInt("type", xvault::type::UNSIGNED_INT, p.second->type());
		set_node->attributes().push_back(type);

		auto color = xvault::h5::h5Attribute::newValueFloat3("color", xvault::type::FLOAT_3, p.second->color());
		set_node->attributes().push_back(color);

		auto visible = xvault::h5::h5Attribute::newValueUInt("visible", xvault::type::UNSIGNED_INT, p.second->isVisible() ? 1 : 0);
		set_node->attributes().push_back(visible);

		for (auto i : p.second->indices()) {
			auto index = xvault::h5::h5Attribute::newValueUInt(std::to_string(i), xvault::type::UNSIGNED_INT, i);
			set_indices_node->attributes().push_back(index);
		}

		set_node->nodes().push_back(set_indices_node);
		sets_node->nodes().push_back(set_node);
		
	}

	size_t simplexInfoCounter = 0;
	for (auto info : simplexInfo) {
		xvault::Node *s_node = new xvault::rapid::rjNode(std::to_string(simplexInfoCounter++));

		auto vbuffer = xvault::h5::h5Attribute::newValueString("vertexBuffer", xvault::type::STRING, info.vertexBufferReference);
		s_node->attributes().push_back(vbuffer);

		auto vNbuffer = xvault::h5::h5Attribute::newValueString("vertexNormalBuffer", xvault::type::STRING, info.vertexNormalBufferReference);
		s_node->attributes().push_back(vNbuffer);

		auto uvbuffer = xvault::h5::h5Attribute::newValueString("uvBuffer", xvault::type::STRING, info.uvBufferReference);
		s_node->attributes().push_back(uvbuffer);

		auto matIDbuffer = xvault::h5::h5Attribute::newValueString("materialIDBuffer", xvault::type::STRING, info.materialIDBufferReference);
		s_node->attributes().push_back(matIDbuffer);

		auto ibuffer = xvault::h5::h5Attribute::newValueString("indexBuffer", xvault::type::STRING, info.indexBufferReference);
		s_node->attributes().push_back(ibuffer);

		simplex_nodes->nodes().push_back(s_node);
	}

	size_t simplexAttributerCounter = 0;
	for (auto path : simplexAttributePaths) {
		auto pth = xvault::h5::h5Attribute::newValueString(std::to_string(simplexAttributerCounter++), xvault::type::STRING, path);
		simplex_attribute_node->attributes().push_back(pth);
	}

	size_t streamAttributerCounter = 0;
	for (auto path : streamAttributePaths) {
		auto pth = xvault::h5::h5Attribute::newValueString(std::to_string(streamAttributerCounter++), xvault::type::STRING, path);
		stream_attribute_node->attributes().push_back(pth);
	}

	_vdoc->root()->nodes().push_back(sets_node);
	_vdoc->root()->nodes().push_back(simplex_nodes);
	_vdoc->root()->nodes().push_back(simplex_attribute_node);
	_vdoc->root()->nodes().push_back(stream_attribute_node);

	xvault::h5::h5ODocumentResource resource;
	resource.setText(_vdoc->toString());

	_file->writeResource("step/" + std::to_string(stepCounter), resource);

	delete _vdoc;
}

void DfxCacheRecorder::finalize() {

	xvault::h5::h5OResource resource;

	auto content = xvault::h5::h5Attribute::newValueString("__content__", xvault::type::STRING, "fluent_header");
	resource.addAttribute(content);

	auto format = xvault::h5::h5Attribute::newValueString("format", xvault::type::STRING, DfxCacheFormat::format);
	resource.addAttribute(format);

	auto version = xvault::h5::h5Attribute::newValueUInt("version", xvault::type::UNSIGNED_INT, DfxCacheFormat::version);
	resource.addAttribute(version);

	auto sequence = xvault::h5::h5Attribute::newValueUInt("sequence", xvault::type::UNSIGNED_INT, 0);
	resource.addAttribute(sequence);

	auto sFrame = xvault::h5::h5Attribute::newValueString("startingFrame", xvault::type::STRING, std::to_string(startingFrame));
	resource.addAttribute(sFrame);

	auto sCounter = xvault::h5::h5Attribute::newValueString("stepCount", xvault::type::STRING, std::to_string(stepCounter));
	resource.addAttribute(sCounter);

	_file->writeResource("header", resource);

}

void DfxCacheRecorder::step() {

	stepSimplexObjects();
	stepSimplexAttributes();
	stepStreamAttributes();
	stepMetadata();

	stepCounter++;
}