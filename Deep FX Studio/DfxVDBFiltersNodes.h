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
#include <DfxUtility.h>
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxEntityDB.h"
#include "DfxGlobal.h"


namespace DfxNodes {

	// Dilate
	static DfxGlobalEntityName DfxVDBDilateNode_Identifier = "2F2X252X_DFX_VDB_DILATE_NODE";

	class DfxVDBDilateNode : public DfxEntity {
	public:
		static const DfxPropertyID outRef			= 0;
		static const DfxPropertyID inRef			= 1;
		static const DfxPropertyID inOffset			= 2;
		static const DfxPropertyID inIterations		= 3;	

		DfxVDBDilateNode(DfxGlobalEntityName uniquename);

		virtual ~DfxVDBDilateNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();

	};

	// Erode
	static DfxGlobalEntityName DfxVDBErodeNode_Identifier = "2F2X252X_DFX_VDB_ERODE_NODE";

	class DfxVDBErodeNode : public DfxEntity {
	public:
		static const DfxPropertyID outRef			= 0;
		static const DfxPropertyID inRef			= 1;
		static const DfxPropertyID inOffset			= 2;
		static const DfxPropertyID inIterations		= 3;

		DfxVDBErodeNode(DfxGlobalEntityName uniquename);

		virtual ~DfxVDBErodeNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();

	};

	// Median
	static DfxGlobalEntityName DfxVDBMedianNode_Identifier = "2F2X252X_DFX_VDB_MEDIAN_NODE";

	class DfxVDBMedianNode : public DfxEntity {
	public:
		static const DfxPropertyID outRef			= 0;
		static const DfxPropertyID inRef			= 1;
		static const DfxPropertyID inWidth			= 2;
		static const DfxPropertyID inIterations		= 3;

		DfxVDBMedianNode(DfxGlobalEntityName uniquename);

		virtual ~DfxVDBMedianNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();

	};

	// Mean
	static DfxGlobalEntityName DfxVDBMeanNode_Identifier = "2F2X252X_DFX_VDB_MEAN_NODE";

	class DfxVDBMeanNode : public DfxEntity {
	public:
		static const DfxPropertyID outRef			= 0;
		static const DfxPropertyID inRef			= 1;
		static const DfxPropertyID inWidth			= 2;
		static const DfxPropertyID inIterations		= 3;

		DfxVDBMeanNode(DfxGlobalEntityName uniquename);

		virtual ~DfxVDBMeanNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();

	};

	// Gaussian
	static DfxGlobalEntityName DfxVDBGaussianNode_Identifier = "2F2X252X_DFX_VDB_GAUSSIAN_NODE";

	class DfxVDBGaussianNode : public DfxEntity {
	public:
		static const DfxPropertyID outRef			= 0;
		static const DfxPropertyID inRef			= 1;
		static const DfxPropertyID inWidth			= 2;
		static const DfxPropertyID inIterations		= 3;

		DfxVDBGaussianNode(DfxGlobalEntityName uniquename);

		virtual ~DfxVDBGaussianNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();

	};
}
