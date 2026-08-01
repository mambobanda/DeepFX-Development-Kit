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

#include <algorithm>
#include <limits>
#include <random>
#include <DfxEntity.h>
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxBaseCompositeNode.h"
#include "DfxEntityDB.h"
#include "DfxGlobal.h"


namespace DfxNodes {

	static DfxGlobalEntityName DfxStandardCompositeNode_Identifier = "2F2X252X_DFX_	STANDARD_COMPOSITE_NODE";

	class DfxStandardCompositeNode : public DfxBaseCompositeNode {
	public:

		DfxStandardCompositeNode();

		virtual ~DfxStandardCompositeNode();

		void compute() override;

		bool isLiftable() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};
}