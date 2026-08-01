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

#include <DsMesh.h>
#include <DsMeshOperator.h>
#include <DsTools.h>
#include <DfxAbstractCommand.h>
#include "DfxUpdater.h"
#include "DfxProject.h"
#include "DfxQSceneManager.h"
#include "DfxObjectLoader.h"
#include "DfxTranslateWidget.h"
#include "DfxScaleWidget.h"
#include "DfxRotateWidget.h"
#include "DfxViewLayoutManager.h"
#include "DfxSpaceUtil.h"
#include "DfxQHotkeyEditor.h"
#include "DfxRenderTarget.h"
#include "DfxRenderManager.h"
#include "DfxHistoryManager.h"

namespace command {

	class ExitApplication : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class EchoSpace : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class OpenShorcutsEditor : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SceneViewContextMenu : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class UpdateScene : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class UpdateSceneAndKeys : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class ClearScene : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class ClearScene02 : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class OpenObj : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SaveProject : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SaveProjectAs : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class LoadProject : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SaveLayout : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class LoadLayout : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class LuxTestRender : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class Reset : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class Undo : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class Redo : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};
}