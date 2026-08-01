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
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxEntityDB.h"
#include "DfxGlobal.h"


namespace DfxNodes {
	// Goup Node
	static DfxGlobalEntityName DfxGroupNode_Identifier = "2F2X252X_DFX_GROUP_NODE";

	class DfxGroupNode : public DfxEntity {
	private:
		std::vector<DfxEntityPropertyTypes::UID*> uids;
	public:
		static const DfxPropertyID out		= 0;
		static const DfxPropertyID inName	= 1;

		DfxGroupNode(DfxGlobalEntityName uniquename);

		virtual ~DfxGroupNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();

		void addUID(DfxEntityPropertyTypes::UID*);

		void removeUID(DfxEntityPropertyTypes::UID*);
	};
}