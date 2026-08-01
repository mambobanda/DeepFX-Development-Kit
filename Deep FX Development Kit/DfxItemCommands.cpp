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

#include "DfxItemCommands.h"

namespace DfxCommands {

	void AddPointViewCommand::execute() {
		DfxPointItem *newPointItem = new DfxPointItem(DfxPointItem_Identifier);
		DfxScene::getInstance()->addItem(newPointItem);
	}

	void AddPointViewCommand::execute(DfxCmdArguments args) {
		execute();
	}

	void *AddPointViewCommand::create() {
		return new AddPointViewCommand();
	}

	// Spotter 
	void AddSpotterCommand::execute() {
		DfxSpotterItem *newSpotter = new DfxSpotterItem(DfxSpotterItem_Identifier);
		DfxScene::getInstance()->addItem(newSpotter);
	}

	void AddSpotterCommand::execute(DfxCmdArguments args) {
		execute();
	}

	void *AddSpotterCommand::create() {
		return new AddSpotterCommand();
	}

	// Object Proxy Viewer
	void AddObjectProxyViewerCommand::execute() {
		DfxObjectViewer *newViewer = new DfxObjectViewer(DfxObjectViewer_Identifier);
		DfxScene::getInstance()->addItem(newViewer);
	}

	void AddObjectProxyViewerCommand::execute(DfxCmdArguments args) {
		execute();
	}

	void *AddObjectProxyViewerCommand::create() {
		return new AddObjectProxyViewerCommand();
	}
}