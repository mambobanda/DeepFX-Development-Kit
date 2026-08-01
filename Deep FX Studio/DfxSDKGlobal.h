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

#include <string>
#include <DfxGlobal.h>
#include <DfxEntityDB.h>
#include <QtCore/qglobal.h>
#include "DfxSceneGraph.h"
#include "DfxSceneGraphMenuBar.h"


#ifdef __cplusplus
extern "C" {
#endif

	typedef struct _dfxsdkglobal {
		void (*console_print)(const char *);
		void (*console_println)(const char *);
		void (*command_exec)(const char *);
		void (*register_entity)(const char *, void(*initFunc)(), void*(*creatorFunc)());
		void (*add_normal_node_menu_entry)(const char *path, const char *uniquename);
	}DfxSDKGlobal;

	Q_DECL_EXPORT void dfx_register_entity(const char *uniquename, void(*initFunc)(), void*(*creatorFunc)());

	Q_DECL_EXPORT void dfx_add_normal_node_menu_entry(const char *path, const char *uniquename);

	Q_DECL_EXPORT void dfx_console_print(const char *);
	Q_DECL_EXPORT void dfx_console_println(const char*);
	Q_DECL_EXPORT void dfx_command_exec(const char *);

	DfxSDKGlobal *sdk_global_instance();

#ifdef  __cplusplus
}
#endif

