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

#include "DfxPlugin.h"
#include "DfxPluginDatabase.h"

#ifdef __cplusplus
extern "C" {
#endif

	ERROR_CODE loadDfxPluginFromFile(const char *path, unsigned int str_len) {

		wchar_t *w_str = new wchar_t[str_len + 1];
		mbstowcs(w_str, path, str_len + 1);
		HINSTANCE hLib = LoadLibrary(w_str);

		if (hLib != NULL) {

			InitializePlugin init = (InitializePlugin)GetProcAddress(hLib, "initializePlugin");

			if (init != NULL) {
				//DfxPluginDatabase::setPath(path);
				init();
			}
			else {
				return DFX_CODE_INITIALIZE_NOT_FOUND;
			}
		}
		else {
			return DFX_CODE_DLL_LOAD_FAIL;
		}

		return DFX_CODE_SUCCESS;
	}

	ERROR_CODE registerDfxEntity(DfxGlobalEntityName uniquename, void(*initFunc)()) {

		DfxEntityDB::registerDfxEntity(uniquename, initFunc);

		return DFX_CODE_SUCCESS;
	}

	ERROR_CODE registerDfxSerivce(size_t class_id, DfxGlobalEntityName uniquename, sPair *tags, unsigned int tagsCount, void*(createService)(DfxGlobalEntityName uniquename)) {
		
		DfxServiceEntry *objLoaderServiceEntry = new DfxServiceEntry;
		
		objLoaderServiceEntry->setUniqueName(uniquename);

		for(int i=0; i < tagsCount; i++) objLoaderServiceEntry->addTag(tags[i].a, tags[i].b);
		
		objLoaderServiceEntry->setCreate(createService);

		DfxServiceRegistry::registerSerivce(class_id, objLoaderServiceEntry);

		return DFX_CODE_SUCCESS;
	}

	/*ERROR_CODE registerDfxPlugin(const char *name,
		unsigned int type,
		dfxPluginVersion version,
		dfxCreateFunc createFunc,
		dfxDestroyFunc destroyFunc) {

		dfxPluginReference *ref = new dfxPluginReference;

		ref->name = _strdup(name);
		ref->type = type;
		ref->createFunc = createFunc;
		ref->destroyFunc = destroyFunc;

		DfxPluginDatabase::addPlugin(type, ref);

		return DFX_CODE_SUCCESS;
	}*/

	ERROR_CODE registerDfxCommand(const char *name, void*(create)()) {

		bool rc = DfxCommandDatabase::getInstance()->mapCommand(DfxString(name), create);

		if (!rc) {
			DfxGlobal::console::println(std::string("Failed to register command: ") + std::string(name));
			return DFX_CODE_REGISTER_FAILED;
		}

		return DFX_CODE_SUCCESS;
	}

	sPair* makeSPairList(size_t sz) {

		return (sPair*)malloc(sizeof(sPair) * (sz + 1));
	}

	void clearSPairList(sPair* p, unsigned int count, int freeflag) {

		for (int i = 0; i < count; i++) {
			if (freeflag == 1) {
				free(p[i].a);
				free(p[i].b);
			}
		}

		free(p);
	}
#ifdef  __cplusplus
}
#endif