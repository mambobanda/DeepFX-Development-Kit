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

#include <vector>
#include <QObject>
#include <QFile>
#include <QLabel>
#include <QTextBrowser>
#include <QDockWidget>
#include <QBoxLayout>
#include <DfxCoreApplication.h>
#include <DfxSimulationCoreGlobal.h>
#include <sstream>
#include <DfxShortcutManager.h>
#include <DfxIntegerTuple.h>
#include "DeepStudioShortcutFunctions.h"
#include "DfxSystemIDBase.h"
//#include "DfxSystemIDMotherBoard.h"
#include "DfxSystemIDMacAddress.h"
#include "DfxTime.h"
#include "DfxFrameState.h"
#include "DfxMathNodes.h"
#include "DfxTimeNodes.h"
#include "DfxVDBNodes.h"
#include "DfxRangedLoopNode.h"
#include "DfxVDBFiltersNodes.h"
#include "DfxGroupNode.h"
#include "DfxEnvelope.h"
#include "DfxTimeActionListener.h"
#include "DfxGlobalPropertyNodes.h"

class DeepStudioGlobal {
public:
	static const int V_MAJOR = 0;
	static const int V_MINOR = 5;
	static const int V_FIX   = 0;
protected:
	void updateWindowTitle();

	void *frameManipulatorPtr;
	void *timelineWidgetPtr;
	QDockWidget *graphDock;
	QDockWidget *layerDock;
	QDockWidget *consoleDock;
	QDockWidget *pythonIDEEditor;
	QDockWidget *renderPropertiesDock;
	DfxSystemIDBase *systemID;
public:
	static void installNodes();
	static DeepStudioGlobal *getInstance();

	static std::string verStr();

	~DeepStudioGlobal();

	int _getSimulationState();
	void _setSimulationState(int);

	static int getSimulationState();
	static void setSimulationState(int);

	bool isFrameCached();
	bool isFrameCached(int);

	bool isFrameValid();

	void cacheFrame();

	void makeFrameValid();

	int getLastCachedFrame();

	bool allFramesCached();

	void selectLayerRoot();

	void resetFrameState();

	void init();

	std::string getOpenScenePath();

	void setOpenScenePath(std::string);

	void setTimelineWidget(void *);
	void resetSimulation();

	void setFrameManipulator(void *);
	void *getFrameManipulator();

	void setGraphDock(QDockWidget *);
	void showGraphDock();

	void setLayerDock(QDockWidget *);
	void showLayerDock();

	void setConsoleDock(QDockWidget *);
	void showConsoleDock();

	void setPythonIDEDock(QDockWidget *);
	void showPythonIDEDock();

	void setRenderPropertiesDock(QDockWidget *);
	void showRenderPropertiesDock();

	void showHelpBrowser();

	void showAboutDialog();

	void updateLayerManager();

	void initShortcutActions();
	void bindShortcuts();

	DfxSystemIDBase *getSystemID();
private:
	DeepStudioGlobal();
	static DeepStudioGlobal *instance;

	std::vector<DfxFrameState> frameState;

	int simulationState;

	std::string openScenePath;
};