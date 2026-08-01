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
#include "DfxQNodeLayoutInterface.h"


class DfxQHierarchicalLayoutAlpha: public DfxQNodeLayoutInterface {
public:
	enum class YLayout{ UNIFORM_LAYOUT, DAG_LAYOUT, RUBBERBAND_LAYOUT };
private:

	struct GNodeExtra {
		bool dummy;
		size_t dummyID;
		GNode gnode;
		size_t layer;
		float order;
		std::vector<GNodeExtra*> children;
	};

	struct _Layer {
		size_t index;
		float max_width;
		std::vector<GNodeExtra*> nodes;
	};

	std::vector<_Layer*> layers;
	std::unordered_map<size_t, GNodeExtra*> extras;
	float layerOffset;
	float ySpacing;
	size_t iterations;
	YLayout ylayoutType;

private:

	void initExtras();
	void assignLayers();
	void insertDummyNodes();
	void setXCoordinate();
	void crossingReduction();
	void assignYCoordinates();
	void uniformYLayout();
	void dagYLayout();
	void rubberbandYLayout();
	void resync();

public:

	DfxQHierarchicalLayoutAlpha();
	virtual ~DfxQHierarchicalLayoutAlpha();

	virtual void solveLayout() override;

	void  setLayerOffset(float );
	float getLayerOffset();

	void setYLayoutType(YLayout );
	YLayout getYLayoutType();

	void setYLayout(float val);
	float getYLayoutSpacing();

	void setReductionCrossingIterations(size_t);

	size_t getReductionCrossingIterations();
};