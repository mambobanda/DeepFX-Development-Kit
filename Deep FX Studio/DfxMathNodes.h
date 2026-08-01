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

namespace DfxNodes {

	// sRandomArray
	static DfxGlobalEntityName sRandomArrayNode_Identifier = "2F2X252X_DFX_S_RANDOM_ARRAY_NODE";

	class sRandomArrayNode : public DfxEntity {
	public:
		static const DfxPropertyID inSeed		= 0;
		static const DfxPropertyID inMin		= 1;
		static const DfxPropertyID inMax		= 2;
		static const DfxPropertyID inSize		= 3;
		static const DfxPropertyID outArray		= 4;

		sRandomArrayNode(DfxGlobalEntityName uniquename);

		virtual ~sRandomArrayNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	// sIncrement
	static DfxGlobalEntityName sIncrementNode_Identifier = "2F2X252X_DFX_S_INCREMENT_NODE";

	class sIncrementNode : public DfxEntity {
	public:
		static const DfxPropertyID inValue = 0;
		static const DfxPropertyID outValue = 1;

		sIncrementNode(DfxGlobalEntityName uniquename);

		virtual ~sIncrementNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	// sPrintValue
	static DfxGlobalEntityName sPrintValueNode_Identifier = "2F2X252X_DFX_S_PRINT_VALUE_NODE";

	class sPrintValueNode : public DfxEntity {
	public:
		static const DfxPropertyID inValue = 0;

		sPrintValueNode(DfxGlobalEntityName uniquename);

		virtual ~sPrintValueNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	// sCount
	static DfxGlobalEntityName sCountNode_Identifier = "2F2X252X_DFX_S_COUNT_NODE";

	class sCountNode : public DfxEntity {
	public:
		static const DfxPropertyID inArray		= 0;
		static const DfxPropertyID outCount		= 1;

		sCountNode(DfxGlobalEntityName uniquename);

		virtual ~sCountNode();
			
		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	// sUniformArray
	static DfxGlobalEntityName sUniformArrayNode_Identifier = "2F2X252X_DFX_S_UNIFORM_ARRAY_NODE";

	class sUniformArrayNode : public DfxEntity {
	public:
		static const DfxPropertyID inValue		= 0;
		static const DfxPropertyID inCount		= 1;
		static const DfxPropertyID outArray		= 2;

		sUniformArrayNode(DfxGlobalEntityName uniquename);

		virtual ~sUniformArrayNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	// sValue
	static DfxGlobalEntityName sValueNode_Identifier = "2F2X252X_DFX_S_VALUE_NODE";

	class sValueNode: public DfxEntity{
	public:
		static const  DfxPropertyID inA	 = 0;
		static const  DfxPropertyID outC = 1;

		sValueNode(DfxGlobalEntityName uniquename);

		virtual ~sValueNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	// sAdd
	static DfxGlobalEntityName sAddNode_Identifier = "2F2X252X_DFX_S_ADD_NODE";

	class sAddNode : public DfxEntity {
	public:
		static const  DfxPropertyID inA		= 0;
		static const  DfxPropertyID inB		= 1;
		static const  DfxPropertyID outC	= 2;

		sAddNode(DfxGlobalEntityName uniquename);

		virtual ~sAddNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	// sSubtract
	static DfxGlobalEntityName sSubtractNode_Identifier = "2F2X252X_DFX_S_SUBTRACT_NODE";

	class sSubtractNode : public DfxEntity {
	public:
		static const  DfxPropertyID inA		= 0;
		static const  DfxPropertyID inB		= 1;
		static const  DfxPropertyID outC	= 2;

		sSubtractNode(DfxGlobalEntityName uniquename);

		virtual ~sSubtractNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	// sMultiply
	static DfxGlobalEntityName sMultiplyNode_Identifier = "2F2X252X_DFX_S_MULTIPLY_NODE";

	class sMultiplyNode : public DfxEntity {
	public:
		static const  DfxPropertyID inA		= 0;
		static const  DfxPropertyID inB		= 1;
		static const  DfxPropertyID outC	= 2;

		sMultiplyNode(DfxGlobalEntityName uniquename);

		virtual ~sMultiplyNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	// vCount
	static DfxGlobalEntityName vCountNode_Identifier = "2F2X252X_DFX_V_COUNT_NODE";

	class vCountNode : public DfxEntity {
	public:
		static const  DfxPropertyID inArray		= 0;
		static const  DfxPropertyID outCount	= 1;

		vCountNode(DfxGlobalEntityName uniquename);

		virtual ~vCountNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	// vUniformArray
	static DfxGlobalEntityName vUniformArrayNode_Identifier = "2F2X252X_DFX_V_UNIFORM_ARRAY_NODE";

	class vUniformArrayNode : public DfxEntity {
	public:
		static const DfxPropertyID inValue		= 0;
		static const DfxPropertyID inCount		= 1;
		static const DfxPropertyID outArray		= 2;

		vUniformArrayNode(DfxGlobalEntityName uniquename);

		virtual ~vUniformArrayNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	// vValue
	static DfxGlobalEntityName vValueNode_Identifier = "2F2X252X_DFX_V_VALUE_NODE";

	class vValueNode : public DfxEntity {
	public:
		static const DfxPropertyID inA		= 0;
		static const DfxPropertyID outC		= 1;

		vValueNode(DfxGlobalEntityName uniquename);

		virtual ~vValueNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	// vDivide
	static DfxGlobalEntityName sDivideNode_Identifier = "2F2X252X_DFX_V_DIVIDE_NODE";

	class sDivideNode : public DfxEntity {
	public:
		static const DfxPropertyID inA	=	0;
		static const DfxPropertyID inB	=	1;
		static const DfxPropertyID outC =	2;

		sDivideNode(DfxGlobalEntityName uniquename);

		virtual ~sDivideNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	// vAdd
	static DfxGlobalEntityName vAddNode_Identifier = "2F2X252X_DFX_V_ADD_NODE";

	class vAddNode : public DfxEntity {
	public:
		static const DfxPropertyID inA	= 0;
		static const DfxPropertyID inB	= 1;
		static const DfxPropertyID outC	= 2;

		vAddNode(DfxGlobalEntityName uniquename);

		virtual ~vAddNode();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};
}