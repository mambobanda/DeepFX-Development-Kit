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
#include "DfxUpdater.h"
#include "DfxWidgetManager.h"
#include "DfxTranslateWidget.h"
#include "DfxDynaMesh.h"
#include "DfxScaleWidget.h"
#include "DfxRotateWidget.h"
#include "DfxUpdater.h"
#include "DsMeshOperator.h"
#include "DsMeshQuery.h"
#include "DfxSurfaceDictionary.h"
#include "DfxTime.h"

namespace command {

	class SelectConnectedFaces : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SelectFaceLoop : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SelectAllFaces : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SelectFaceSet : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SelectSurface : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SelectAllEdges : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SelectAllVertices : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SelectVertexIncidentEdges : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SelectEdgeAdjacentVertices : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SelectVertexIncidentFaces : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class PrintFaceID : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class PrintEdgeID : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class PrintVertexID : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/*class SelectEdgeIncidentFaces : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SelectEdgeIncidentFaces : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SelectFaceAdjacentEdges : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	

	class SelectFaceBoundingVertices : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SelectVertexNeighboringVertices : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SelectEdgeNeighboringEdges : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class SelectFaceNeighboringFaces : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};*/
}