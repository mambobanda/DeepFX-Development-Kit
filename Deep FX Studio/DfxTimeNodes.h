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

#include <DfxGlobal.h>
#include <DfxEntityBasicPropertyTypes.h>
#include <DfxEntity.h>
#include <DfxEntityDB.h>
#include "DfxTime.h"

namespace DfxNodes {

	// Time Info
	static DfxGlobalEntityName TimeInfo_Identifier = "2F2X252X_DFX_TIME_INFO";

	class TimeInfo : public DfxEntity {
	public:
		static const DfxPropertyID outCurrentFrame		= 0;
		static const DfxPropertyID outFrameCount		= 1;

		TimeInfo(DfxGlobalEntityName uniquename);

		virtual ~TimeInfo();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};
}
