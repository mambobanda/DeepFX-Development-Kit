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

#include <experimental\filesystem>
#include "DfxIOCommands.h"

namespace filesystem = std::experimental::filesystem;

namespace DfxCommands {

	void LoadObjectCommand::execute() {		
		DfxCmdArguments args;
		execute(args);
	}

	void LoadObjectCommand::execute(DfxCmdArguments args) {
		
		if (args.size() > 1) {
			filesystem::path path(args[1].c_str());

			if (!path.has_extension()) {
				DfxGlobal::console::println("Failed to load object: Invalid path, unknown file, no extension found.");
			}
			else {

				if (filesystem::exists(path)) {

					DfxString ext(path.extension().string().substr(1, path.extension().string().size() -1).c_str());		

					if (DfxObjectLoaderService::hasLoader(ext.c_str())) {
						DfxObjectLoaderService *objLoader = nullptr;
						DfxObjectItem *oitem = new DfxObjectItem(DfxObjectItem_Identifier);

						objLoader = DfxObjectLoaderService::fetchLoader("obj");
						
						objLoader->setFilename(path.string());

						objLoader->loadObject(*oitem->getMeshData());

						if (objLoader->getStatus() == DfxObjectLoaderService::SUCCESS) {
							
							oitem->setLogicalName(path.stem().string());
							oitem->setFilePath(args[1]);
							oitem->calculateBounds();
							oitem->initOpenGlBuffer();
							oitem->setIsReady(true);
							DfxScene::getInstance()->addItem(oitem);
							
							DfxCoreApplication::updateViewports();
							
							std::stringstream ss;
							ss << "loadObject '" << path.stem().string() << path.extension().string() << "' done...";
							DfxGlobal::console::println(ss.str());
			
						}
						else {
							delete objLoader;
							delete oitem;
						}

						delete objLoader;
					}
					else {
						DfxGlobal::console::println("No Loader found.");
					}
				}
				else {
					DfxGlobal::console::println("Failed to load object: file doesn't exist.");
				}
			}

		}
		else {
			DfxGlobal::console::println("Failed to load object: No path given");
		}
	}

	void *LoadObjectCommand::create() {
		return new LoadObjectCommand;
	}
}