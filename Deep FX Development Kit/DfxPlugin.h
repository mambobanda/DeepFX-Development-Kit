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

#include <Windows.h>
#include "DfxGlobal.h"
#include "DfxEntityDB.h"
#include "DfxServiceRegistry.h"
#include "DfxPluginCommon.h"
#include "DfxCommandDatabase.h"

#ifdef __cplusplus
extern "C" {
#endif
	enum {
		DFX_CODE_SUCCESS = 102,
		DFX_CODE_DLL_LOAD_FAIL = 104,
		DFX_CODE_INITIALIZE_NOT_FOUND = 106,
		DFX_CODE_REGISTER_FAILED = 108
	};

	typedef struct st_service {
		char *a;
		char *b;
	}sPair;

#ifdef  __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
	typedef int ERROR_CODE;

	typedef struct st_plugin_version
	{
		unsigned int major;
		unsigned int minor;
	} dfxPluginVersion;

	typedef void *(*dfxCreateFunc)();
	typedef void(*dfxDestroyFunc)(void *);

	typedef void(*InitializePlugin)();

	typedef struct st_plugin_object
	{
		char *name;
		char *path;
		unsigned int type;
		dfxPluginVersion version;
		dfxCreateFunc createFunc;
		dfxDestroyFunc destroyFunc;
	} dfxPluginReference;

	ERROR_CODE loadDfxPluginFromFile(const char *path, unsigned int str_len);

	// ERROR_CODE registerDfxPlugin(const char *name, unsigned int type, dfxPluginVersion version, dfxCreateFunc createFunc, dfxDestroyFunc destroyFunc);

	ERROR_CODE registerDfxCommand(const char *name, void*(create)());

	ERROR_CODE registerDfxEntity(DfxGlobalEntityName uniquename, void(*initFunc)());

	ERROR_CODE registerDfxSerivce(size_t class_id, DfxGlobalEntityName uniquename, sPair *tags, unsigned int tagsCount, void*(createService)(DfxGlobalEntityName uniquename));

	sPair* makeSPairList(size_t);

	void clearSPairList(sPair*, unsigned int scount, int freeflag = 1);

#ifdef  __cplusplus
}
#endif

