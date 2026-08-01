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

#include <sstream>
#include <regex>
#include <experimental\filesystem>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <DfxEntityBasicPropertyTypes.h>
#include "DfxCSimulationEntity.h"
#include "DfxSimulationCoreGlobal.h"
#include "DfxUtility.h"

namespace filesystem = std::experimental::filesystem;

#define xPI 3.14159265358979323846
static float min_e = 1.0e-3;

inline float pow2(float val) {
	return val * val;
}

inline float pow3(float val) {
	return val * val * val;
}

inline float pow4(float val) {
	return val * val * val * val;
}

inline float pow5(float val) {
	return val * val * val * val * val;
}

inline float pow6(float val) {
	return val * val * val * val * val * val;
}

inline float pow7(float val) {
	return val * val * val * val * val * val * val;
}

inline float pow8(float val) {
	return val * val * val * val * val * val * val * val;
}

inline float pow9(float val) {
	return val * val * val * val * val * val * val * val * val;
}

inline float pow10(float val) {
	return val * val * val * val * val * val * val * val * val * val;
}

inline std::string indexToKey(int index) {

	std::stringstream ss;
	ss << index << "_frame";

	return ss.str();
}

inline void getPreviewColor(float speed, float limit, glm::vec3 primary, glm::vec3 secondary, glm::vec3 &out) {

	if (speed > limit)
		speed = limit;

	if (speed < 0)
		speed = 0;

	float alpha = speed / limit;

	out.x = primary.x + (alpha * (secondary.x - primary.x));
	out.y = primary.y + (alpha * (secondary.y - primary.y));
	out.z = primary.z + (alpha * (secondary.z - primary.z));
}

std::string getFileDirName(std::string path);

std::vector<std::string> getDirectoryFilenames(std::string dirpath);

int getFileIncrement(const std::string s);

std::vector<dfxcore::DfxCSimulationEntity*> findSimulationEntities(DfxEntityPropertyTypes::UID *);

glm::vec3 transformPoint(glm::mat4 m, glm::vec3 point);