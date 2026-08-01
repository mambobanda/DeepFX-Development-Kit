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

#include <SgShapeBuffer.h>
#include <DfxMode.h>
#include <Partio.h>
#include <sstream>
#include <filesystem>
#include "DfxPointCache.h"
#include "DfxTimelineManager.h"
#include "DfxRiptideCommon.h"
#include "DfxSimulationStagePropBase.h"
#include "DfxPointCloudHandler.h"
#include "DfxQEmptyWidget.h"
#include "DfxQStackControl.h"
#include "DfxQControlPanel.h"
#include "DfxQControlArray.h"
#include "DfxQBoolControl.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQFluentPathControl.h"
#include "DfxQLineEditControl.h"
#include "DfxQFilePathControl.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxQColorControl.h"
#include "DfxQPushButtonStyles.h"

namespace filesystem = std::experimental::filesystem;

class DfxRiptideCachePropNode : public DfxSimulationStagePropBase {
private:

	class CurrentFrameObserver : public fl::Observer {
	public:
		DfxRiptideCachePropNode *_cacheNode;

		CurrentFrameObserver(DfxRiptideCachePropNode *);
		void update(fl::Any *any) override;
	};

	CurrentFrameObserver *currentFrameObserver;
public:

	struct PropState { enum { NONE = 0, SPAWN = 1 }; };
	struct Location { enum { MEMORY = 0, DISK = 1 }; };
	struct ParticleFormat { enum { BGEO = 0, }; };

private:
	size_t _propState;

	std::vector<std::pair<std::string, unsigned int>> locationTypeQControlenums;
	std::vector<std::pair<std::string, unsigned int>> paticleFormatTypeQControlenums;

private:
	static std::string _uniqueName;

private:
	DfxPointCache _cache;
	void updatePointCloud(DfxPointFrame *frame, DfxPointCloudNode *pcloud, float pVelocity, glm::vec3 secolor);
public:

	DfxRiptideCachePropNode(deepsg::SgStageNode *stage);
	virtual ~DfxRiptideCachePropNode();

	std::string uniqueName();
	static std::string getUniqueName();

	size_t propState();
	void setPropState(size_t);

	void getPreviewColor(float speed, float limit, glm::vec3 primary, glm::vec3 secondary, glm::vec3 &out);

	QWidget *properties();
	QWidget *visualisation();

	std::vector<std::pair<QWidget*, QString>> propertyForms();

	void clearCache();
	DfxPointFrame *getCachedFrame(int cframe);
	void addCachedFrame(int cframe, DfxPointFrame *frame);
	void updatePointCloud(DfxPointFrame *frame, int frameNumber);
	DfxPointCloudNode *pointCloud();
	void saveToFile(DfxPointFrame *frame, int frameNumber);
};