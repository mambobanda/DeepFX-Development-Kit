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

//#include "DfxVDBPointMesher.h"
#include "DeepFxStudioOpenVDBPlugin.h"

extern "C" DEEPFXSTUDIOOPENVDBPLUGIN_EXPORT int init_dfx_plugin(DfxSDKGlobal *global) {

	global->console_println("Loading OpenVDB Point Mesher plugin...");

	//global->register_entity(DfxVDBPointMesher_Identifier.c_str(),
	//						DfxVDBPointMesher::initialize,
	//						DfxVDBPointMesher::creator);

	//global->add_normal_node_menu_entry("OpenVDB/Point Mesher", DfxVDBPointMesher_Identifier.c_str());

	return 0;
}