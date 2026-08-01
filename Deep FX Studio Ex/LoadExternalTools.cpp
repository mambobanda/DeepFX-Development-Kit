/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include <QDir>
#include <QLibrary>
#include <DfxGlobal.h>
#include <DfxPlugin.h>
#include "DfxQMainMenu.h"
#include "DfxExternalToolsCommon.h"
#include "LoadExternalTools.h"

namespace load_ext {

	void load_dlls() {
		
		const char path[] = "external tools";

		QDir dll_dir(path);
		
		if (dll_dir.exists()) {
			for (const auto & entry : filesystem::directory_iterator(path)) {
				QLibrary lib(entry.path().string().c_str());

				if (lib.isLibrary(entry.path().string().c_str())) {
					if (lib.load()) {
						DfxGlobal::console::println("Loading external tool...\n");

						auto name = (toolNameCB)lib.resolve("toolName");
						auto command = (toolCommandCB)lib.resolve("toolCommand");
						auto sGState = (defineGlobalStateCB)lib.resolve("defineGlobalState");
						auto toolDef = (toolDefinitionCB)lib.resolve("toolDefinition");

						std::string tname = "";
						std::string cname = "";

						if (name) {
							tname = name();
							DfxGlobal::console::println("Tool Name: " + tname);
						}
						else DfxGlobal::console::println("Failed to resolve 'toolName'.");

						if (command) {
							cname = command();
							DfxGlobal::console::println("Tool Command: " + cname + "\n");
						}
						else DfxGlobal::console::println("Failed to resolve 'toolCommand'.");

						if (sGState) {
							sGState(GlobalState::createGlobalState);
						}
						else DfxGlobal::console::println("Failed to resolve 'setGlobalState'.");

						if (toolDef) {
							auto def = toolDef();
							registerDfxCommand(def->commandName, def->commandInstance);
						}
						else DfxGlobal::console::println("Failed to resolve 'toolDefinition'.");

						if (!tname.empty() && !cname.empty()) {

							DfxQMainMenu::getInstance()->addAction(
								(std::string("External Tools/") + tname).c_str(), 
								[cname]() { DfxGlobal::command::exec(cname); }
							);
						}
					}
					else {
						DfxGlobal::console::println("Failed to load external tool...\n");
					}
				}
			}
		}
		
	}

}