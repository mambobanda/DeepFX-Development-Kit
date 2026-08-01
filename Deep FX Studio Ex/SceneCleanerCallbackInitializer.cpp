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

#include "SceneCleanerCallbackInitializer.h"
#include "DfxSceneCleaner.h"
#include "DfxSurfaceDictionary.h"
#include "DfxMaterialLinker.h"
#include "DfxFluentCleaner.h"
#include "DfxCacheTrackManager.h"

namespace scenecleaner {

	void init() {

		DfxSceneCleaner::getInstance()->addCallback(DfxMaterialLinker::clearCallback);
		DfxSceneCleaner::getInstance()->addCallback(DfxSurfaceDictionary::clearCallback);
		DfxSceneCleaner::getInstance()->addCallback(DfxCacheTrackManager::clearCallback);
		DfxSceneCleaner::getInstance()->addCallback(DfxFluentCleaner::cleanAll);
	}

};
