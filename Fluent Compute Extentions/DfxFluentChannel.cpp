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

#include "DfxFluentChannel.h"

DfxFluentChannel::~DfxFluentChannel() {
}

size_t DfxFluentChannel::cType() {
	return type;
}

void DfxFluentChannel::free_channel() {
	if (channel != nullptr) {

		if (type == DfxFluenType::INT) {
			auto v = (std::vector<int>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::SIZE_T) {
			auto v = (std::vector<size_t>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::UINT) {
			auto v = (std::vector<unsigned int>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::FLOAT) {
			auto v = (std::vector<float>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::DOUBLE) {
			auto v = (std::vector<double>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::STRING) {
			auto v = (std::vector<std::string>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::BOOL) {
			auto v = (std::vector<bool>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::CHAR) {
			auto v = (std::vector<char>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::VEC2) {
			auto v = (std::vector<glm::vec2>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::VEC3) {
			auto v = (std::vector<glm::vec3>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::VEC4) {
			auto v = (std::vector<glm::vec4>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::DVEC2) {
			auto v = (std::vector<glm::dvec2>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::DVEC3) {
			auto v = (std::vector<glm::dvec3>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::DVEC4) {
			auto v = (std::vector<glm::dvec4>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::IVEC2) {
			auto v = (std::vector< glm::ivec2>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::IVEC3) {
			auto v = (std::vector<glm::ivec3>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::IVEC4) {
			auto v = (std::vector<glm::ivec4>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::MAT3) {
			auto v = (std::vector<glm::mat3>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::MAT4) {
			auto v = (std::vector<glm::mat4>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::DMAT3) {
			auto v = (std::vector<glm::dmat3>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::DMAT4) {
			auto v = (std::vector<glm::dmat4>*)channel;
			delete v;
		}
		else if (type == DfxFluenType::QUAT) {
			auto v = (std::vector<glm::quat>*)channel;
			delete v;
		}

		channel = nullptr;
	}
}

void DfxFluentChannel::clear() {

	if (channel != nullptr) {

		if (type == DfxFluenType::INT) {
			auto v = (std::vector<int>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::SIZE_T) {
			auto v = (std::vector<size_t>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::UINT) {
			auto v = (std::vector<unsigned int>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::FLOAT) {
			auto v = (std::vector<float>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::DOUBLE) {
			auto v = (std::vector<double>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::STRING) {
			auto v = (std::vector<std::string>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::BOOL) {
			auto v = (std::vector<bool>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::CHAR) {
			auto v = (std::vector<char>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::VEC2) {
			auto v = (std::vector<glm::vec2>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::VEC3) {
			auto v = (std::vector<glm::vec3>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::VEC4) {
			auto v = (std::vector<glm::vec4>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::DVEC2) {
			auto v = (std::vector<glm::dvec2>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::DVEC3) {
			auto v = (std::vector<glm::dvec3>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::DVEC4) {
			auto v = (std::vector<glm::dvec4>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::IVEC2) {
			auto v = (std::vector< glm::ivec2>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::IVEC3) {
			auto v = (std::vector<glm::ivec3>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::IVEC4) {
			auto v = (std::vector<glm::ivec4>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::MAT3) {
			auto v = (std::vector<glm::mat3>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::MAT4) {
			auto v = (std::vector<glm::mat4>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::DMAT3) {
			auto v = (std::vector<glm::dmat3>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::DMAT4) {
			auto v = (std::vector<glm::dmat4>*)channel;
			v->clear();
		}
		else if (type == DfxFluenType::QUAT) {
			auto v = (std::vector<glm::quat>*)channel;
			v->clear();
		}
	}
}

void DfxFluentChannel::resize(size_t sz) {
	if (channel != nullptr) {

		if (type == DfxFluenType::INT) {
			auto v = (std::vector<int>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::SIZE_T) {
			auto v = (std::vector<size_t>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::UINT) {
			auto v = (std::vector<unsigned int>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::FLOAT) {
			auto v = (std::vector<float>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::DOUBLE) {
			auto v = (std::vector<double>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::STRING) {
			auto v = (std::vector<std::string>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::BOOL) {
			auto v = (std::vector<bool>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::CHAR) {
			auto v = (std::vector<char>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::VEC2) {
			auto v = (std::vector<glm::vec2>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::VEC3) {
			auto v = (std::vector<glm::vec3>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::VEC4) {
			auto v = (std::vector<glm::vec4>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::DVEC2) {
			auto v = (std::vector<glm::dvec2>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::DVEC3) {
			auto v = (std::vector<glm::dvec3>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::DVEC4) {
			auto v = (std::vector<glm::dvec4>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::IVEC2) {
			auto v = (std::vector< glm::ivec2>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::IVEC3) {
			auto v = (std::vector<glm::ivec3>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::IVEC4) {
			auto v = (std::vector<glm::ivec4>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::MAT3) {
			auto v = (std::vector<glm::mat3>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::MAT4) {
			auto v = (std::vector<glm::mat4>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::DMAT3) {
			auto v = (std::vector<glm::dmat3>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::DMAT4) {
			auto v = (std::vector<glm::dmat4>*)channel;
			v->resize(sz);;
		}
		else if (type == DfxFluenType::QUAT) {
			auto v = (std::vector<glm::quat>*)channel;
			v->resize(sz);;
		}
	}
}
