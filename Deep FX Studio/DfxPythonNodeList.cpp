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

#include <DfxPythonGlobal.h>
#include "DfxPythonNodeList.h"

std::vector<DfxPythonNodeLoader::PyNodeDesc*> DfxPythonNodeList::list;

void DfxPythonNodeList::add(DfxPythonNodeLoader::PyNodeDesc *desc) {
	list.push_back(desc);
}

void DfxPythonNodeList::disable(size_t index) {
	list[index]->autoload = false;
}

void DfxPythonNodeList::enable(size_t index) {
	list[index]->autoload = true;
}

size_t DfxPythonNodeList::count() {
	return list.size();
}

DfxPythonNodeLoader::PyNodeDesc *DfxPythonNodeList::getNodeInfo(size_t index) {
	return list[index];
}

void DfxPythonNodeList::loadAll() {

	DeepStudioRegistry::Entry entry;

	if (DeepStudioRegistry::findEntry(registry::_$PYTHON_NODE_DIR, entry)) {
		filesystem::path dir_path(entry.String);

		if (filesystem::exists(dir_path)) {

			for (auto &p : filesystem::directory_iterator(dir_path)) {
				
				if (p.path().extension().string().compare(".pynode") == 0) {
					auto info = DfxPythonNodeLoader::getPythonNodeInfo(p.path().string());

					if (info != nullptr) {
						DfxPythonNodeList::add(info);
					}
				}
			}
		}
		else
		{
			DfxGlobal::console::println("Failed to load Python Node Info: directory doesn't exist.\n");
		}
	}
	else {
		DfxGlobal::console::println("Failed to load Python Node Info: Unknown registry entry.\n");
	}
}

void DfxPythonNodeList::installMenus(QMenu *menu) {

	for (auto info : list) {

		if (info->autoload) {

			DfxSceneGraphMenuBar menubarSetup;
			menubarSetup.setRootMenu(menu);

			menubarSetup.addMenuEntry(info->menu, [info]() {
				DfxPythonGlobal::getInstance()->exec(info->script);
			});
		}
	}
}