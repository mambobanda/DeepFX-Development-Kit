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

#include "DfxTransformIdentityOverride.h"
#include "DfxTimelineManager.h"

DfxTransformIdentityOverride::DfxTransformIdentityOverride() {
	_ignoreInitialFrame = true;
}

DfxTransformIdentityOverride::~DfxTransformIdentityOverride() {

}

bool DfxTransformIdentityOverride::ignoreInitialFrame() {
	return _ignoreInitialFrame;
}

void DfxTransformIdentityOverride::setIgnoreInitialFrame(bool val) {
	_ignoreInitialFrame = val;
}

glm::mat4 DfxTransformIdentityOverride::evaluate(deepsg::SgTransformOverrideInfo info) {
	if (info.frame == DfxTimelineManager::getInstance()->firstFrame()) {
		return info.globalTransform;
	}
	else {
		return glm::mat4(1.f);
	}
}