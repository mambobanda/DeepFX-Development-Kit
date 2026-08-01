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

#include "DfxSimulationStagePropBase.h"

DfxSimulationStagePropBase::DfxSimulationStagePropBase(deepsg::SgStageNode *st, size_t type): deepsg::SgStagePropNode(st , type) {
	_sourcePath = "";
}

DfxSimulationStagePropBase::~DfxSimulationStagePropBase() {

}

std::string DfxSimulationStagePropBase::sourcePath() {
	return _sourcePath;
}

void DfxSimulationStagePropBase::setSourcePath(std::string ref) {
	_sourcePath = ref;
}