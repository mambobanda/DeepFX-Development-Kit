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

#include "DfxSimulationStageBase.h"

size_t DfxSimulationStageBase::ID_GEN = 0xff;

DfxSimulationStageBase::DfxSimulationStageBase(size_t stageT): deepsg::SgStageNode(stageT){
	_simID = ID_GEN;
}

DfxSimulationStageBase::~DfxSimulationStageBase() { 
}

size_t DfxSimulationStageBase::simulationID() {
	return _simID;
}