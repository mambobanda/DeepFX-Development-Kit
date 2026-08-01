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


#include <glm\geometric.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtx\transform.hpp>
#include "DfxGlobal.h"
#include "DfxViewItem.h"

static DfxGlobalEntityName DfxSpotterItem_Identifier = "2F2X252X_DFX_SPOTTER_ITEM";

class DfxSpotterItem : public DfxViewItem {
private:
	bool glInitialized;
public:
	float size;
	static const DfxPropertyID outPosition = 0;
public:

	DfxSpotterItem(DfxGlobalEntityName uniquename);
	virtual ~DfxSpotterItem();

	void draw() override;

	void drawSelection() override;

	DfxBounds bounds() override;

	bool selectable() override;

	void compute() override;

	void calculateBounds() override;

	void importData(std::vector<std::pair<std::string, std::string>> data) override;

	std::vector<std::pair<std::string, std::string>> exportData() override;

	bool isExportable() override;

	void load(std::ifstream &) override;

	static void *creator(DfxGlobalEntityName uniquename);

	static void initialize();
};