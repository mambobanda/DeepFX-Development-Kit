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

#include "DfxGlobal.h"
#include "DfxScene.h"
#include "DfxAbstractCommand.h"
#include "DfxDefaultGlobalIDs.h"
#include "DfxCoreApplication.h"


namespace DfxCommands {

	// Update Viewport
	class UpdateViewportCommand : public DfxAbstractCommand {
	public:
		virtual void execute();

		virtual void execute(DfxCmdArguments);

		static void *create();
	};

	class ResetViewportCommand : public DfxAbstractCommand {
	public:
		virtual void execute();

		virtual void execute(DfxCmdArguments);

		static void *create();
	};

	class SyncInterfaceCommand : public DfxAbstractCommand {
	public:
		virtual void execute();

		virtual void execute(DfxCmdArguments);

		static void *create();
	};

	class ClearSelectionCommand : public DfxAbstractCommand {
	public:
		virtual void execute();

		virtual void execute(DfxCmdArguments);

		static void *create();
	};

}