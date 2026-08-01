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
#include "DfxPointItem.h"
#include "DfxSpotterItem.h"
#include "DfxObjectViewer.h"
#include "DfxCoreApplication.h"

namespace DfxCommands {

	// Add Point View To Scene
	class AddPointViewCommand : public DfxAbstractCommand {
	public:
		virtual void execute();

		virtual void execute(DfxCmdArguments);

		static void *create();
	};

	// Add Spotter View To Scene
	class AddSpotterCommand : public DfxAbstractCommand {
	public:
		virtual void execute();

		virtual void execute(DfxCmdArguments);

		static void *create();
	};

	// Add Object Proxy Viewer To Scene
	class AddObjectProxyViewerCommand : public DfxAbstractCommand {
	public:
		virtual void execute();

		virtual void execute(DfxCmdArguments);

		static void *create();
	};

}