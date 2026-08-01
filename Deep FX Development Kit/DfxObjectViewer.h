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
#include "DfxUtility.h"
#include "DfxGlobal.h"
#include "DfxViewItem.h"

static DfxGlobalEntityName DfxObjectViewer_Identifier = "2F2X252X_DFX_OBJECT_VIEWER";

class DfxObjectViewer : public DfxViewItem {
private:
	bool glInitialized;
public:
	static const DfxPropertyID inRef				= 0;
	static const DfxPropertyID inRenderMode			= 1;
	static const DfxPropertyID inDisplacementIndex	= 2;
	
public:

	DfxObjectViewer(DfxGlobalEntityName uniquename);
	virtual ~DfxObjectViewer();

	void drawProxies();

	void draw() override;

	DfxBounds bounds() override;

	bool selectable() override;

	void compute() override;

	void calculateBounds() override;

	void importData(std::vector<std::pair<std::string, std::string>> data) override;

	std::vector<std::pair<std::string, std::string>> exportData() override;

	bool isExportable() override;

	static void *creator(DfxGlobalEntityName uniquename);

	static void initialize();

	DfxEntityPropertyTypes::ProxyObject *getProxyPointer(size_t inx, DfxEntityPropertyTypes::ProxyObjectArray *pxs);

	glm::mat4 createTransformationMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl);

	glm::vec3 getDisplacementVertex(size_t proxid, size_t index);

	void drawProxyObjects(DfxEntityPropertyTypes::ProxyObjectArray *);
	
	void drawObject(Dfx3dMeshData *);

};