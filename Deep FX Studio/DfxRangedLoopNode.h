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
#include <DfxEntity.h>
#include <DfxTraitUID.h>
#include "DfxDelegateUtil.h"
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxEntityDB.h"
#include "DfxGlobal.h"


namespace DfxNodes {
	
	//Ranged Loop 
	static DfxGlobalEntityName DfxRangedLoopNode_Identifier = "2F2X252X_DFX_RANGED_LOOP_NODE";
	
	class DfxRangedLoopNode : public DfxEntity {
	public:
		static const DfxPropertyID inFrom = 0;
		static const DfxPropertyID inTo = 1;

	public:
		DfxRangedLoopNode(DfxGlobalEntityName uniquename);

		virtual ~DfxRangedLoopNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

}