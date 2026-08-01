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

#include <DfxEntity.h>
#include "DfxCSimulationEntity.h"
#include "DfxSimulationCoreGlobal.h"
#include "util.h"

namespace dfxcore {

	static DfxGlobalEntityName DfxCPlane_Identifier = "2F2X252X_DFX_C_PLANE";

	class DfxCPlane : public DfxCSimulationEntity {
	public:
		static const DfxPropertyID outNormal = 0;
		static const DfxPropertyID outPosition = 1;

	public:

		DfxCPlane(DfxGlobalEntityName uniquename);
		virtual ~DfxCPlane();

		void compute() override;

		void init() override;

		void clear() override;

		void done() override;

		void load(std::ifstream &) override;

		void draw() override;

		void drawSelection() override;

		DfxBounds bounds() override;

		bool selectable() override;

		void calculateBounds() override;

		void drawPlane();

		DfxBounds planeBounds();

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();

	};
}