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
#include "DfxUtility.h"
#include "DfxGlobal.h"
#include "DfxViewItem.h"

static DfxGlobalEntityName DfxMergeObjectItem_Identifier = "2F2X252X_DFX_MERGE_OBJECT_ITEM";

class DfxMergeObjectItem : public DfxEntity {
private:
	
	class TempMeshReference {
	public:
		size_t index;
		bool hasTransform;
		glm::mat4 transform;
		Dfx3dMeshData *meshPtr;
	};

	std::vector<DfxEntityPropertyTypes::ProxyObjectArray *> findAllProxyObjects();
	std::vector<Dfx3dMeshData *> findAllMeshData();
public:
	static const DfxPropertyID mesh		= 0;
	static const DfxPropertyID outRef	= 1;
	static const DfxPropertyID inRef	= 2;
	
public:

	DfxMergeObjectItem(DfxGlobalEntityName uniquename);
	virtual ~DfxMergeObjectItem();

	void compute() override;

	void onDataChanged(DfxEntityDataInstance *instance) override;

	void importData(std::vector<std::pair<std::string, std::string>> data) override;

	std::vector<std::pair<std::string, std::string>> exportData() override;

	bool isExportable() override;

	glm::mat4 createTransformationMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl);

	static void *creator(DfxGlobalEntityName uniquename);

	static void initialize();

};
