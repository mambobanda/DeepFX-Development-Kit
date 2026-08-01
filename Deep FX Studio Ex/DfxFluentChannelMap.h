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

#include <vector>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <DsType.h>
#include "DfxFluentChannel.h"

class DfxFluentChannelMap {
private:
	size_t _size;
	std::unordered_map<std::string, DfxFluentChannel*> _attributeMap;
public:

	DfxFluentChannelMap();
	virtual ~DfxFluentChannelMap();

	size_t size();

	void resize(size_t);

	DfxFluentChannel *channel(std::string attrName);

	std::vector<DfxFluentChannel*> allChannels();

	std::vector<std::pair<std::string, DfxFluentChannel*>> allAttributes();

	size_t attributeType(std::string attrName);

	size_t attributeCount();

	bool removeAttribute(std::string attrName);

	bool hasAttribute(std::string attrName);

	void clear();

	void removeAllAttributes();

	template<typename T>
	void addAttribute(std::string attrName) {

		unsigned int type = 0;
		bool validType = false;
		bool isString = false;

		if (std::is_same_v<T, int>) {
			type = DfxFluenType::INT;
			validType = true;
		}
		else if (std::is_same_v<T, size_t>) {
			type = DfxFluenType::SIZE_T;
			validType = true;
		}
		else if (std::is_same_v<T, unsigned int>) {
			type = DfxFluenType::UINT;
			validType = true;
		}
		else if (std::is_same_v<T, float>) {
			type = DfxFluenType::FLOAT;
			validType = true;
		}
		else if (std::is_same_v<T, double>) {
			type = DfxFluenType::DOUBLE;
			validType = true;
		}
		else if (std::is_same_v<T, std::string>) {
			type = DfxFluenType::STRING;
			validType = true;
		}
		else if (std::is_same_v<T, bool>) {
			type = DfxFluenType::BOOL;
			validType = true;
		}
		else if (std::is_same_v<T, char>) {
			type = DfxFluenType::CHAR;
			validType = true;
		}
		else if (std::is_same_v<T, glm::vec2>) {
			type = DfxFluenType::VEC2;
			validType = true;
		}
		else if (std::is_same_v<T, glm::vec3>) {
			type = DfxFluenType::VEC3;
			validType = true;
		}
		else if (std::is_same_v<T, glm::vec4>) {
			type = DfxFluenType::VEC4;
			validType = true;
		}
		else if (std::is_same_v<T, glm::dvec2>) {
			type = DfxFluenType::DVEC2;
			validType = true;
		}
		else if (std::is_same_v<T, glm::dvec3>) {
			type = DfxFluenType::DVEC3;
			validType = true;
		}
		else if (std::is_same_v<T, glm::dvec4>) {
			type = DfxFluenType::DVEC4;
			validType = true;
		}
		else if (std::is_same_v<T, glm::ivec2>) {
			type = DfxFluenType::IVEC2;
			validType = true;
		}
		else if (std::is_same_v<T, glm::ivec3>) {
			type = DfxFluenType::IVEC3;
			validType = true;
		}
		else if (std::is_same_v<T, glm::ivec4>) {
			type = DfxFluenType::IVEC4;
			validType = true;
		}
		else if (std::is_same_v<T, glm::mat3>) {
			type = DfxFluenType::MAT3;
			validType = true;
		}
		else if (std::is_same_v<T, glm::mat4>) {
			type = DfxFluenType::MAT4;
			validType = true;
		}
		else if (std::is_same_v<T, glm::dmat3>) {
			type = DfxFluenType::DMAT3;
			validType = true;
		}
		else if (std::is_same_v<T, glm::dmat4>) {
			type = DfxFluenType::DMAT4;
			validType = true;
		}
		else if (std::is_same_v<T, glm::quat>) {
			type = DfxFluenType::QUAT;
			validType = true;
		}
		else {
			throw std::runtime_error("Fluent Channel Map Error: addAttribute() unknown type.");
		}

		if (validType) {

			auto cnn = new DfxFluentChannel;
			cnn->type = type;
			cnn->name = attrName;

			auto v = new std::vector<T>();
			cnn->channel = v;

			if (size() > 0) v->resize(size());

			_attributeMap[attrName] = cnn;
		}

	}

	template<typename T>
	void fill(std::string attrName, T val) {
		auto itr = _attributeMap.find(attrName);

		if (itr != _attributeMap.end()) {
			auto cnn = (std::vector<T>*)itr->second->channel;
			for (size_t inx = 0; inx < cnn->size(); inx++) (*cnn)[inx] = val;

		}
	}

	template<typename T>
	T *at(std::string attrName) {

		auto itr = _attributeMap.find(attrName);

		if (itr != _attributeMap.end()) {
			auto cnn = (std::vector<T>*)itr->second->channel;
			return &(*cnn)[0];
		}

		return nullptr;
	}

};