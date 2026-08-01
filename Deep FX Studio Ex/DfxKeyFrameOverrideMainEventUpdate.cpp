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

#include "DfxKeyFrameOverrideMainEventUpdate.h"

DfxKeyFrameOverrideMainEventUpdate::DfxKeyFrameOverrideMainEventUpdate() :DfxMainEventProcess(DfxMainEventUpdateID::KEYFRAME_WRANGLER_UPDATE) {

}

DfxKeyFrameOverrideMainEventUpdate::~DfxKeyFrameOverrideMainEventUpdate() {

}

void DfxKeyFrameOverrideMainEventUpdate::process(int frame, unsigned int updateType) {

	auto overrides = DfxPropertyConduit::getInstance()->allConduitPaths();

	for (auto ov : overrides) {
		DfxPropertyConduit::getInstance()->applyKeyframeFloatOverride(ov, frame);
	}
	
}