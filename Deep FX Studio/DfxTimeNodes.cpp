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

#include "DfxTimeNodes.h"

namespace DfxNodes {


	TimeInfo::TimeInfo(DfxGlobalEntityName uniquename): DfxEntity(uniquename) {
		setLogicalName("Time Info");

		setPropertyState(outCurrentFrame, DfxEntityPropertyState::OUTPUT_STATE);
		setPropertyState(outFrameCount, DfxEntityPropertyState::OUTPUT_STATE);

		DfxTime::getInstance()->addOnActionEntity(this);
	}

	TimeInfo::~TimeInfo() {
		DfxTime::getInstance()->removeOnActionEntity(getInstanceID());
	}

	void TimeInfo::compute() {
		
		*(float*)getProperty(outCurrentFrame) = DfxTime::getInstance()->getCurrentFrame();
		*(float*)getProperty(outFrameCount)   = DfxTime::getInstance()->getTotalFrames();

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void TimeInfo::onDataChanged(DfxEntityDataInstance *instance) {
	
	}

	void TimeInfo::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> TimeInfo::exportData() {
		return mapEntityProperties(this);
	}

	bool TimeInfo::isExportable() {
		return true;
	}

	void *TimeInfo::creator(DfxGlobalEntityName uniquename) {
		return new TimeInfo(uniquename);
	}

	void TimeInfo::initialize() {

		DfxEntityDB::createProperty(outCurrentFrame,"current_frame", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(outFrameCount, "frame_count", new DfxEntityPropertyTypes::FloatPropertyType());
	}
}

