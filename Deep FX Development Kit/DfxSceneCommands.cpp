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

#include "DfxSceneCommands.h"


namespace DfxCommands {

	void UpdateViewportCommand::execute() {
		DfxCoreApplication::updateViewports();
	}

	void UpdateViewportCommand::execute(DfxCmdArguments) {
		execute();
	}

	void *UpdateViewportCommand::create() {
		return new UpdateViewportCommand();
	}

	//
	void ResetViewportCommand::execute() {
		DfxCoreApplication::resetViewports();
	}

	void ResetViewportCommand::execute(DfxCmdArguments) {
		execute();
	}

	void *ResetViewportCommand::create() {
		return new ResetViewportCommand();
	}

	//
	void SyncInterfaceCommand::execute() {
		DfxCoreApplication::processQAppEvents();
	}

	void SyncInterfaceCommand::execute(DfxCmdArguments) {
		execute();
	}

	void *SyncInterfaceCommand::create() {
		return new SyncInterfaceCommand();
	}

	// Clear selection
	void ClearSelectionCommand::execute() {
		DfxScene::getInstance()->clearSelection();
		DfxGlobal::command::exec("updateViewport");
	}

	void ClearSelectionCommand::execute(DfxCmdArguments) {
		execute();
	}

	void *ClearSelectionCommand::create() {
		return new ClearSelectionCommand();
	}
}