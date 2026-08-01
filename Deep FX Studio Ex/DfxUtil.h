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

#include <fstream>
#include <string>
#include <chrono>
#include <random>
#include <iostream>
#include <sstream>
#include <DfxSpace.h>
#include <VDocument.h>
#include <VH5Attribute.h>
#include <VH5Buffer.h>
#include <SgRootNode.h>
#include <FlFlow.h>
#include <SgPointCloudNode.h>
#include <DfxGroupNode.h>
#include <DfxNullNode.h>
#include <DfxBoundsMinMax.h>
#include "DfxSgNodeDB.h"
#include "DfxFluentTypes.h"
#include "DfxKeyFrameManager.h"
#include "DfxSurfaceDictionary.h"
#include "DfxFlowDB.h"


inline void appendToFile(std::string path, std::string content) {
	std::ofstream file;
	file.open(path, std::ofstream::out | std::ofstream::app);
	file << content;
	file.close();
}

inline void writeToFile(std::string path, std::string content) {
	std::ofstream file;
	file.open(path, std::ofstream::out);
	file << content;
	file.close();
}

inline float fovToFocalLength(float fov, float lens_size) {
	return (lens_size / 2.0f) / std::tanf((fov * 0.5f) * 3.14159265359f / 180.0f);
}

class xTimer {
private:
	std::chrono::system_clock::time_point start;
	double finalTime;

public:

	inline void startTimer() {
		start = std::chrono::system_clock::now();
	}

	inline void stopTimer() {
		auto diff = std::chrono::system_clock::now() - start;
		finalTime = std::chrono::duration <double, std::milli>(diff).count();
	}

	inline double getTime() {
		return finalTime;
	}
};

namespace util {

	deepsg::SgGeoNode *createEmptyPrimitive();
	DfxBoundsMinMax boundingBox(deepsg::SgGeoNode *);

	std::string replaceAll(std::string str, std::string substring, std::string replace);

	std::string spaceTypeToString(unsigned int typ);
	unsigned int spaceTypeToEnum(std::string typ);
	std::string anyValueToString(fl::Any *any);
	std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*> mapAnyValue(fl::Any *any, bool &isAttr, bool &isBuff);
	std::string echoSpace(fl::Any *any = nullptr, int level = 0);
	xvault::Document *spaceToVDocument(xvault::Node *node = nullptr, fl::Any *any = nullptr, unsigned int level = 0);
	xvault::Document *spaceBindingsToVDocument();
	xvault::Document *graphToVDocument(xvault::Node *node = nullptr, deepsg::SgNode *sgnode = nullptr, unsigned int level = 0);
	xvault::Document *surfaceDictionaryToVDocument();
	xvault::Document *materialLinksToVDocument();
	xvault::Document *fluentFlowsToVDocument(bool rootFlow = false, size_t uid = 0, fl::Flow *flow = nullptr, xvault::Node *parent = nullptr, size_t doc_id = 0);
	xvault::Document *tracksToVDocument();

	void deleteAllSgNodes();
	void deleteSgNodes(std::vector<deepsg::SgNode*> node);
	bool deleteSgNode(deepsg::SgNode *node, std::vector<size_t> &deletedNodes);

	std::pair<deepsg::SgNode*,size_t> fetchNode(std::string referencePath);

	std::string Mat4ToString(glm::mat4 mat);

	template<typename T>
	T rand(T first, T last) {
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<T> dist(first, last);

		return dist(mt);
	}
}