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
#include "DfxEntityDB.h"
#include "DfxGlobal.h"
#include "DfxGlobalProperties.h"

namespace DfxNodes {

	//Util Functions
	void cleanLooseGlobal(std::string path, size_t instanceID);

	// Global Properties
	static DfxGlobalEntityName ScalarValueGlobal_Identifier = "2F2X252X_DFX_SCALAR_VALUE_GLOBAL_NODE";

	class ScalarValueGlobal : public DfxEntity {
	private:
		float *_value;
	public:
		static const DfxPropertyID inPath = 0;
		static const DfxPropertyID inValue = 1;
		static const DfxPropertyID outValue = 2;

		ScalarValueGlobal(DfxGlobalEntityName uniquename);

		virtual ~ScalarValueGlobal();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};
}