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

#include <DfxPluginManager.h>
#include "DfxSDKGlobal.h"


#ifdef __cplusplus
extern "C" {
#endif

Q_DECL_EXPORT void dfx_console_print(const char *str) {
	DfxGlobal::console::print(str);
}

Q_DECL_EXPORT void dfx_console_println(const char *str) {
	DfxGlobal::console::println(str);
}


Q_DECL_EXPORT void dfx_command_exec(const char *str) {
	DfxGlobal::command::exec(str);
}

Q_DECL_EXPORT void dfx_register_entity(const char *uniquename, void(*initFunc)(), void*(*creatorFunc)()) {

	DfxGlobalEntityName name = std::string(uniquename);
	DfxPluginManager::getInstance()->addCreator(name, creatorFunc);

	DfxEntityDB::registerDfxEntity(name, initFunc, DfxPluginManager::creator);
}

Q_DECL_EXPORT void dfx_add_normal_node_menu_entry(const char *path, const char *uniquename) {

	std::string uid = std::string(uniquename);
	std::string _path = std::string(path);

	QMenu *addMenu = DfxSceneGraph::getInstance()->getGraphUI()->getAddMenu();

	DfxSceneGraphMenuBar menubarSetup;
	menubarSetup.setRootMenu(addMenu);

	menubarSetup.addMenuEntry(_path, [uid]() {
		DfxLayerManager::getInstance()->addNode((DfxEntity*)DfxPluginManager::creator(uid));
	});	
}

DfxSDKGlobal *sdk_global_instance() {

	DfxSDKGlobal *instance = new DfxSDKGlobal;
	instance->console_print   = dfx_console_print;
	instance->console_println = dfx_console_println;
	instance->register_entity = dfx_register_entity;
	instance->add_normal_node_menu_entry = dfx_add_normal_node_menu_entry;
	instance->command_exec = dfx_command_exec;

	return instance;
}

#ifdef  __cplusplus
}
#endif