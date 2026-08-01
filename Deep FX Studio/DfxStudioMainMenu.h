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

#include <DfxCoreApplication.h>
#include <DfxGlobal.h>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>
#include <QStringList>
#include <QList>
#include <sstream>
#include <experimental\filesystem>
#include <DfxColladaSceneImporter.h>
#include <DfxOpenSceneAction.h>
#include <QDesktopServices>
#include "DfxQPreferencesDialog.h"
#include "CompilationGlobals.h"
#include "DfxPlayBlastBuilder.h"
#include "DfxPlayBlastPlayer.h"
#include "DfxQRenderProperties.h"
#include "DeepStudioGlobal.h"
#include "DfxSceneSaver.h"
#include "DfxXMLSceneSaver.h"
#include "DfxSceneLoader.h"
#include "DfxXMLSceneLoader.h"
#include "DeepStudioRegistry.h"
#include "DeepStudioRegistryPaths.h"

namespace filesystem = std::experimental::filesystem;

class DfxStudioMainMenu {
private:
	static QMenuBar *_main;

	static QMenu *fileMenu;
	static QMenu *importMenu;
	static QMenu *recentScenes;
	static QAction *newSceneAction;
	static QAction *openSceneAction;
	static QAction *importObjectAction;
	static QAction *importColladaAction;
	static QAction *importParticleCacheAction;

	static QAction *saveSceneAction;
	static QAction *saveSceneAsAction;

	static QAction *exitAction;

	static QAction *viewportScreenshot;
	static QAction *createPlayBlast;
	static QAction *showConsoleAction;
	static QAction *showLayersAction;
	static QAction *showPythonIDEAction;
	static QAction *showSceneGraphAction;
	static QAction *showRenderPropertiesAction;
	static QAction *showPreferencesAction;

	static QAction *viewHelpAction;
	static QAction *submitBugReportAction;
	static QAction *aboutAction;

	static QMenu *editMenu;
	static QMenu *viewMenu;
	static QMenu *createMenu;
	static QMenu *selectMenu;
	static QMenu *helpMenu;
private:
	static QList<DfxOpenSceneAction*> recentSceneActions;
	static void initRecentSceneList();
public:

	static void addMenu();
public:
	static void newScene();
	static void openScene();
	static void importObject();
	static void importCollada();
	static void importParticleCache();

	static void updateRecentOpenedSceneList(std::string);

	static void showSceneGraph();
	static void showConsole();
	static void showLayers();
	static void showPythonIDE();
	static void showRenderProperties();
	static void showPreferences();

	static void saveScene();
	static void saveSceneAs();

	static void closeApplication();

	static void captureViewport();
	static void _createPlayBlast();

	static void viewHelp();
	static void submitBugReport();
	static void about();
};