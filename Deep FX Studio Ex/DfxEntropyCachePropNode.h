/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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
#include <glm/gtx/matrix_decompose.hpp>
#include <Entropy.h>
#include <QTemporaryDir>
#include <filesystem>
#include "DfxTimelineManager.h"
#include "DfxEntropyCommon.h"
#include "DfxSimulationStagePropBase.h"
#include "DfxQEmptyWidget.h"
#include "DfxQStackControl.h"
#include "DfxQControlPanel.h"
#include "DfxQControlArray.h"
#include "DfxQBoolControl.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQFluentPathControl.h"
#include "DfxQLineEditControl.h"
#include "DfxQFilePathControl.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxQColorControl.h"
#include "DfxQPushButtonStyles.h"
#include "DfxQColorControl.h"
#include "DfxQCheckBoxControl.h"

namespace filesystem = std::experimental::filesystem;

class DfxEntropyCachePropNode : public DfxSimulationStagePropBase {
private:

	class CurrentFrameObserver : public fl::Observer {
	public:
		DfxEntropyCachePropNode *_cacheNode;

		CurrentFrameObserver(DfxEntropyCachePropNode *);
		void update(fl::Any *any) override;
	};

	CurrentFrameObserver *currentFrameObserver;

private:
	static std::string _uniqueName;

private:

	size_t random_num;
	void clearTempDir();

public:

	DfxEntropyCachePropNode(deepsg::SgStageNode *stage);
	virtual ~DfxEntropyCachePropNode();

	std::string uniqueName();
	static std::string getUniqueName();

	void clear();

	void cache(boom::Entropy *entropy, float voxel_size, glm::ivec3 dims);

	void updateVisualization();

	QWidget *properties();
	std::vector<std::pair<QWidget*, QString>> propertyForms() override;

};