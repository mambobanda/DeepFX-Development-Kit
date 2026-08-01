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

#include <iostream>
#include <QApplication>
#include <QMessageBox>
#include <QSplashScreen>

#include <ospray/ospray.h>

#include "DfxSystemIDGlobal.h"
#include "DeepFxStudioLicenseManager.h"
#include "DfxLicenseInfoDialog.h"
#include "DfxTrialHandshake.h"

#include "DfxVersionNameGlobal.h"
#include "DfxUIStyleGlobal.h"
#include "DfxOpenglInfo.h"
#include "CommandInitializer.h"
#include "ShortcutConfig.h"
#include "ToolMapInitializer.h"
#include "WidgetInitializer.h"
#include "ViewMapInitializer.h"
#include "MainMenuInitializer.h"
#include "SpaceInitializer.h"
#include "RenderInitializer.h"
#include "MaterialsInitializer.h"
#include "MainEventLoopInitializer.h"
#include "ComputeNodeInitializer.h"
#include "DfxTimelineManager.h"
#include "FluentModelsInitializer.h"
#include "SceneGraphInitializer.h"
#include "SceneCleanerCallbackInitializer.h"
#include "FluentHelperFunctionInitializer.h"
#include "FluentInitializers.h"
#include "FluentComputeAttributesInitializer.h"
#include "DfxQSceneViewport.h"
#include "DfxViewportSynchronizer.h"
#include "DfxQViewportRenderGL_2_1.h"

#include "DfxExternalTools.h"
#include "LoadExternalTools.h"

#include "DfxQMainWindow.h"

int main(int argc, char *argv[]) {

	QApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
	QApplication app(argc, argv);
	
	QApplication::setOrganizationName("DeepFX");
	QApplication::setOrganizationDomain("deepfxworld.com");
	QApplication::setApplicationName("Deep FX Studio");

#if DFX_SPLASH
	QPixmap pixmap(":/splash/splash");
	QSplashScreen splash(pixmap);
	splash.show();
	splash.showMessage("Checking Licensing...", Qt::AlignBottom, Qt::white);
#endif

#if DFX_COMMERCIAL
	DfxSystemIDGlobal::getSystemID()->generateSystemID();

	DeepFxStudioLicenseManager::init();
	auto isNodeLocked = DeepFxStudioLicenseManager::tryNodeLockedLicenseMode();

	if (DeepFxStudioLicenseManager::getState() != DeepFxStudioLicenseManager::NODE_LOCKED_STATE) {
#if DFX_SPLASH
		DfxGlobal::console::println("\nRunning in Trial Mode.\n\n");
		splash.showMessage("Starting Trial Mode...", Qt::AlignBottom, Qt::white);
		// QString daysRemaining(std::to_string(DeepFxStudioLicenseManager::trialDays - DeepFxStudioLicenseManager::getTrailDaysPast()).c_str());
		// DfxCoreApplication::getInstance()->setWindowTitle(("Deep FX Studio v" + ver + "- TRIAL VERSION (" + daysRemaining.toStdString() + " Days Left)").c_str());

		if (DfxLicenseInfoDialog::trialModeInfo()) {
			auto answer = DfxTrialHandshake::askForPermission();
			if (!answer.first) {

				if (answer.second == 0) {
					DfxLicenseInfoDialog::activationErrorInfo("Unable to connect to remote host. Please check internet connection.", "");
				}
				else {
					DfxLicenseInfoDialog::activationErrorInfo("Unknown activation response. Please contact support with error code.", "Error Code: " + std::to_string(answer.second));
				}

				return 0;
			}

			DfxSceneCleaner::getInstance()->addCallback([]() { DeepFxStudioLicenseManager::clearTrialDate(); });
#if DFX_SPLASH
			splash.hide();
			//DfxCoreApplication::getInstance()->setWindowTitle((std::string("Deep FX Studio v" + ver).c_str()));
#endif
		}
		else {
			return 0;
		}
#endif
	}
	else {
#if DFX_SPLASH
		splash.showMessage("Commercial License Found, Starting...", Qt::AlignBottom, Qt::white);
		splash.hide();
		// DfxCoreApplication::getInstance()->setWindowTitle((std::string("Deep FX Studio v" + ver).c_str()));
#endif
		
	}
#endif

	// Set Theme
	DfxUIStyleGlobal::setFusionDarkTheme();

	// Init OpenGL
	DfxOpenglInfo::init();
	if (DfxOpenglInfo::glVersionMajor() < 3 || DfxOpenglInfo::glVersionMinor() < 1) {
		qDebug("OpenGl 3.1 not supported.");

		QMessageBox msgBox;
		msgBox.setWindowTitle(DfxVersionNameGlobal::versionNameString().c_str());
		msgBox.setText("OpenGl 3.1 not supported by system. Stopping Deep FX Studio.");
		msgBox.exec();

		return 0;
	}

	// Init Instance first
	DfxSpace::getInstance();

	// Global Fluent Initializers
	fluent_init::initSceneCleaners();

	// Initialize Callbacks
	scenecleaner::init();

	// Initialize Space
	space::initialize();
	space::registerCleanCallbacks();

	// Initialize Any Helpers
	flhelpers::init();

	// Initialize commands
	command::initCommands();

	// Configure/Create Shortcuts
	shortcuts::configure();

	// Initialize Toolmap
	toolmap::initialize();

	// Initialize tools
	widget::initialize();

	// Initialize View Map
	viewmap::initialize();
	viewmap::initResources();

	// Initialize Main Menu
	mainmenu::initialize();

	// Initialize Render
	render::intitialize();

	// Initialize Materials
	materials::initializeMaterialMap();

	// Initialize Compute Nodes
	compute::initNodes();

	// FluentModelsInitializer
	flmodel::initModels();

	// Scene Graph Callback Initializer
	scenegraph::initializeNodeCallbacks();

	// Initialize Main Event Loop
	eventloop::initialize();

	// Initialize fluent compute attribute names
	compute::initAttributeNames();

	// Load External Tools
	load_ext::load_dlls();

	/*auto initViewport = new DfxQSceneViewport(new DfxQViewportRenderGL_2_1);
	initViewport->show();
	initViewport->hide();
	DfxViewportSynchronizer::removeViewport(initViewport);*/

	// Main Window instance + Install Event filter
	auto mainWin = DfxQMainWindow::getInstance();
	mainWin->setWindowIcon(QIcon(":/icon/icon"));
	mainWin->showMaximized();
	QCoreApplication::instance()->installEventFilter(mainWin);

	// Init OSPRay
	OSPError osperr = ospInit();
	if (osperr != OSP_NO_ERROR) {
		DfxGlobal::console::println("Failed to initialize OSPRay API: error(" + std::to_string(osperr) +")");
	}
	else {
		osperr = ospLoadModule("denoiser");
		if (osperr != OSP_NO_ERROR) {
			DfxGlobal::console::println("Failed to initialize OSPRay API: error(" + std::to_string(osperr) + ")");
		}
	}

	app.exec();

	DfxTimelineManager::getInstance()->stop();
	
	ospShutdown();
	
	return 0;
}