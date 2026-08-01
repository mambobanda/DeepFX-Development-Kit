/*
* The Mailo Today Solutions Software License
*
* Copyright 2016 Mailo Today Solutions.
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

static DfxGlobalEntityName DfxPointItem_Identifier = "2F2X252X_DFX_POINT_ITEM";

class DfxPointItem: public DfxViewItem {
private:
	bool glInitialized;
public:
	static const DfxPropertyID inSize		= 0;
	static const DfxPropertyID inPoints		= 1;
public:

	DfxPointItem(DfxGlobalEntityName uniquename);
	virtual ~DfxPointItem();

	void draw() override;

	DfxBounds bounds() override;

	bool preprocessViewItem() override;

	void preprocess() override;

	bool selectable() override;

	void calculateBounds() override;

	void importData(std::vector<std::pair<std::string, std::string>> data) override;

	std::vector<std::pair<std::string, std::string>> exportData() override;

	bool isExportable() override;

	static void *creator(DfxGlobalEntityName uniquename);

	static void initialize();
};
