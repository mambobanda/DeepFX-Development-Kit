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

#include <DsTools.h>
#include <DfxAbstractCommand.h>
#include <DfxGlobal.h>
#include <QInputDialog>
#include <QLineEdit>
#include <set>
#include "DfxUpdater.h"
#include "DfxWidgetManager.h"
#include "DfxTranslateWidget.h"
#include "DfxScaleWidget.h"
#include "DfxRotateWidget.h"
#include "DfxUpdater.h"
#include "DsMeshOperator.h"
#include "DfxMaterialManager.h"
#include "DfxMaterialLinker.h"
#include "DfxSurfaceDictionary.h"

namespace command {

	class DeleteVertices : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class DeleteEdges : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class DeleteFaces : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class ExtrudeFaces : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class TriangulateFaces : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class BevelEdge : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class CreateFaceSet : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class TagSurface : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class LinkMaterialToSurfaces : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class NewMaterial : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class PrintSurfaces : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};
}