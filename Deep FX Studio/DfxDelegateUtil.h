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


#include <vector>
#include <DfxEntity.h>
#include <DfxTraitUID.h>
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxEntityDB.h"
#include "DfxGlobal.h"

namespace DfxDelegate {
	std::vector<DfxEntity*> findRoots(DfxEntity *delegator);
	std::vector<DfxEntity*> findTips(DfxEntity *delegator);
	bool isTip(DfxEntity *ent, std::vector<DfxEntity*> tips);
	void executeDelegator(DfxEntity *, bool executeRootChildren = false);
}