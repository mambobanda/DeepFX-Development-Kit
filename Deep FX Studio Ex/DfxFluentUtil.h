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

#include <FlSpace.h>
#include <SgGeoAdapterInterface.h>
#include <SgPointCloudNode.h>
#include <FlInterpreterScope.h>
#include <FlModel.h>
#include <DfxGlobal.h>
#include "DfxFluentSimplexCollection.h"
#include "DfxFluentSimplexObject.h"
#include "DfxFluentSetManager.h"
#include "DfxComputeUtility.h"

namespace util {

	size_t objectToFluentSet(deepsg::SgGeoAdapterInterface *adapter, DfxFluentSet *set, std::string setName, std::string scene_path, glm::mat4 xform, bool &success, bool cache = true, glm::vec3 scale = glm::vec3(1.f));
	size_t geoNodeToFluentSet(deepsg::SgGeoNode *gnode, DfxFluentSet *set, std::string setName, std::string scene_path, glm::mat4 xform, bool &success, bool cache = true);
	std::vector<size_t> pointCloudToFluentSet(deepsg::SgPointCloudNode *pcloud, DfxFluentSet *set, std::string setName, std::string scene_path, glm::mat4 xform, bool &success);
	void copyAttributes(DfxFluentChannelMap &map, size_t from, std::vector<size_t> to);
};