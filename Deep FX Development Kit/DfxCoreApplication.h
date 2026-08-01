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

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#include <QtWidgets/QStatusBar>
#include <QtOpengl/QGLFormat> 
#include <QOpenGLcontext>
#include <QSurfaceFormat> 
#include <QtWidgets/QStyleFactory>
#include <QTextStream>
#include <QtWidgets/QMessageBox.h>
#include <QDebug>
#include <vector>
#include <mutex>
#include <QtWidgets\QLabel>
#include "DfxEntityDB.h"
#include "DfxOpenGLGlobal.h"
#include "DfxConsoleBuffer.h"
#include "DfxViewportVersion01.h"
#include "DfxConsole.h"
#include "DfxTinyPythonIDE.h"
#include "DfxServiceClassDefinition.h"
#include "DfxServiceRegistry.h"
#include "DfxObjLoaderPlugin.h"
#include "DfxSpotterItem.h"
#include "DfxObjectViewer.h"
#include "DfxMapUniformDisplacement.h"
#include "DfxMapUniformDisplacementProxy.h"
#include "DfxDefaultGlobalIDs.h"
#include "DfxRoot.h"
#include "DfxItem.h"
#include "DfxViewItem.h"
#include "DfxObjectItem.h"
#include "DfxObjectItemInfo.h"
#include "DfxObjectItemCopy.h"
#include "DfxObjectItemMerge.h"
#include "DfxCamera.h"
#include "DfxLight.h"
#include "DfxMeshXForm.h"
#include "DfxPointItem.h"
#include "DfxIOCommands.h"
#include "DfxItemCommands.h"
#include "DfxSceneCommands.h"
#include "DfxPlugin.h"
#include "DfxDarkOrangeStyle.h"

class DfxCoreApplication: public QMainWindow
{
	Q_OBJECT
private:
	static DfxCoreApplication *instance;
	static QApplication *qapp;
	static QSurfaceFormat glFormat;
	static QOpenGLContext *glContext;
	static QLabel *statMsg;

	DfxCoreApplication(QWidget *parent = Q_NULLPTR, bool addViewport = true);
private:
	std::vector<DfxAbstractViewport*> viewports;
	DfxConsole *console;
	DfxTinyPythonIDE *pythonIDE;
	static std::mutex updateViewportsLock;
private:
	static void initInstance();

	// void initViewports();
	void setViewport(int );
public:
	void _showConsole();
	DfxConsole *_getConsole();

	void _showPythonIDE();
	DfxTinyPythonIDE *_getPythonIDE();
protected:
	void closeEvent(QCloseEvent *) override;
public:
	
	~DfxCoreApplication();

	QImage *captureViewport(uint inx=0);

	static bool initialViewport;
public:
	//Static Functions
	static DfxCoreApplication *getInstance();

	static int startApp(int argc, char *argv[]);
	static int startAppMaximised(int argc = 0, char *argv[] = nullptr);
	static bool isOpen();

	static void showConsole();
	static DfxConsole *getConsole();

	static void showPythonIDE();
	static DfxTinyPythonIDE *getPythonIDE();

	static void setFusionDarkTheme();

	static void initQApplication(int argc = 0, char *argv[] = nullptr);

	static void initCoreFunctionality();

	static void initCoreServices();
	static void initCoreEntities();
	static void initCorePlugins();
	static void initCoreCommands();
	static void updateViewports();
	static void resetViewports();
	static void processQAppEvents();

	static void setOrganizationName(QString);
	static void setOrganizationDomain(QString);
	static void setApplicationName(QString);

	static void closeApplication();

	void initViewports();

public:
	size_t viewportCount();
	DfxAbstractViewport *getViewport(size_t);

public slots:
	void statusMessage(QString);
};
