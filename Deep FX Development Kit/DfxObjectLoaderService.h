/*
* The Mailo Today Solutions Software License
*
* Copyright 2018 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/
#pragma once


#include "DfxEntity.h"
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxServiceRegistry.h"
#include "DfxServiceClassDefinition.h"
#include "DfxEntityDB.h"
#include "Dfx3dMeshData.h"


class DfxObjectLoaderService : public DfxEntity {
public:
	enum 
	{
		FILE_NOT_FOUND = 100,
		ACCESS_VIOLATION = 101,
		WRONG_FILE_FORMAT = 102,
		CORRUPT_DATA = 103,
		UNABLE_TO_READ_FILE = 104,
		SUCCESS = 105
	};
public:
	static const DfxPropertyID filename	= 0;
	static const DfxPropertyID status	= 1;

public:

	DfxObjectLoaderService(DfxGlobalEntityName uniquename);
	virtual ~DfxObjectLoaderService();

	void compute() override;

	void onDataChanged(DfxEntityDataInstance *instance) override;

	void setFilename(std::string );

	std::string getFilename();

	int getStatus();

	void setStatus(size_t);
	
	virtual void loadObject(Dfx3dMeshData &mesh) = 0;

	static void initialize();

	static bool hasLoader (std::string);

	static DfxObjectLoaderService* fetchLoader(std::string);
};