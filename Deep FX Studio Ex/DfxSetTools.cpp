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

#include "DfxSetTools.h"

std::vector<std::string> DfxSetTools::allVertexSets(ds::Mesh *msh) {
	std::vector<std::string> output;
	return output;
}

std::vector<std::string> DfxSetTools::allFaceSets(ds::Mesh *msh) {
	std::vector<std::string> output;

	auto all = msh->propertyKeys(ds::PropertyType::FACE);

	for (auto key : all) {
		fl::util::StringSplitter splitter(key, ':');

		if (splitter.isSplittable()) {
			if (splitter.first() == "set") {
				auto lst = splitter.wordList();
				output.push_back(lst[1]);
			}
		}
	}

	return output;
}

std::vector<std::string> DfxSetTools::allFaceSets(DfxDynaMesh *msh) {
	std::vector<std::string> output;

	/*auto all = msh->propertyKeys(ds::PropertyType::FACE);

	for (auto key : all) {
		fl::util::StringSplitter splitter(key, ':');

		if (splitter.isSplittable()) {
			if (splitter.first() == "set") {
				auto lst = splitter.wordList();
				output.push_back(lst[1]);
			}
		}
	}*/

	return output;
}

std::vector<std::string> DfxSetTools::allEdgeSets(ds::Mesh *msh) {
	std::vector<std::string> output;
	return output;
}