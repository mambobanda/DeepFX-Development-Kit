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

#include "Util.h"


std::string getFileDirName(std::string path) {

	bool useBackSlash = true;

	std::size_t found = path.rfind("\\");

	if (found == std::string::npos)
		useBackSlash = false;

	std::size_t index = -1;
	if (useBackSlash) {
		index = path.rfind("\\");
	}
	else {
		index = path.rfind("/");
	}

	if (std::string::npos != index) {
		return path.substr(0, index);
	}
	else {
		return path;
	}
}

std::vector<std::string> getDirectoryFilenames(std::string dirpath) {

	std::vector <std::string> names;

	for (auto & p: filesystem::directory_iterator(dirpath)) {
		names.push_back(p.path().filename().string());
	}

	return names;
}

int getFileIncrement(const std::string s) {

	std::regex reg("@\\d+.");
	std::smatch match;

	if (regex_search(s.begin(), s.end(), match, reg)) {
		std::string str = match[0];
		unsigned int val = str.find('@');
		unsigned int val2 = str.find('.');
		std::string sub = str.substr(val + 1, val2 - 1);

		return std::stoi(sub);
	}
	else
		return -1;
}

std::vector<dfxcore::DfxCSimulationEntity*> findSimulationEntities(DfxEntityPropertyTypes::UID *uid) {
	using namespace DfxEntityPropertyTypes;

	std::vector<dfxcore::DfxCSimulationEntity*> arr;

	std::vector<DfxEntity*> candidates = dfxutil::traitUIDFindAll(*uid);

	for (auto _id : candidates) {
		dfxcore::DfxCSimulationEntity *rc = DfxSimulationCoreGlobal::getInstance()->findSimulationEntity(_id->getInstanceID());

		if (rc != nullptr) {
			arr.push_back(rc);
		}
	}

	return arr;
}


glm::vec3 transformPoint(glm::mat4 m, glm::vec3 point) {
	return glm::vec3(m * glm::vec4(point, 1.f));
}
