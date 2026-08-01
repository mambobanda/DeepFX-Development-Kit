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

static DfxGlobalEntityName DfxCamera_Identifier = "2F2X252X_DFX_CAMERA";

class DfxCamera : public DfxViewItem {
private:
	bool glInitialized;
public:
	float size;
	static const DfxPropertyID outRef = 0;
	static const DfxPropertyID inRef  = 1;
	static const DfxPropertyID inTarget = 2;
	static const DfxPropertyID inWidth  = 3;
	static const DfxPropertyID inHeight = 4;
	static const DfxPropertyID inHoriziontalFOV = 5;
	static const DfxPropertyID inVerticalFOV  = 6;
	static const DfxPropertyID inFocalLength  = 7;

public:

	DfxCamera(DfxGlobalEntityName uniquename);
	virtual ~DfxCamera();

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

	bool hasTarget();

	glm::vec3 getTarget();

	glm::mat4 lookAtTranform();

	glm::mat4 prefferedTransform();

	void defaultSettings();
};