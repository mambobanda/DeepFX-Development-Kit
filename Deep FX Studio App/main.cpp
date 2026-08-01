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

#include <QTimer>
#include <QSplashScreen>
#include <QtWidgets/QApplication>
#include <DeepFxStudioLicenseManager.h>
#include <DfxCoreApplication.h>
#include <DfxSimulationCoreGlobal.h>
#include <DfxNvBlastGlobal.h>
#include <DfxMainLoop.h>
#include <DfxPluginManager.h>
#include <DfxTimelineDock.h>
#include <DfxQNodeEditorDock.h>
#include <DfxLayersEditorDock.h>
#include <DfxRenderPropertiesDock.h>
#include <DfxStudioCommon.h>
#include <DeepStudioGlobal.h>
#include <DfxStudioMainMenu.h>
#include <DfxViewportToolbar.h>
#include <DeepStudioRegistry.h>
#include <DeepStudioRegistryPaths.h>
#include <DfxPythonNodeList.h>
#include <DfxPythonIDECallbackFunctions.h>
#include <DfxQToolShelfWidget.h>
#include <DfxQToolShelfConstructor.h>



#ifndef _DEBUG
	#undef slots
	#include <DfxGlobalPythonBindings.h>
	#include <DfxGenericPythonEntityBindings.h>	
	#include <DfxPythonFunctionBindings.h>
	#include <DfxPythonGraphInterfaceBindings.h>
	#include <DfxPythonEntityTypes.h>
	#include <DfxPythonGlobal.h>
	#define slots
#endif



int main(int argc, char *argv[]) {

	dfxblast::DfxNvBlastGlobal::init();

	registry::initRegistry();

	DfxCoreApplication::initQApplication(argc, argv);
	DfxCoreApplication::setFusionDarkTheme();

	DfxCoreApplication::setOrganizationName("DeepFX");
	DfxCoreApplication::setOrganizationDomain("deepfxworld.com");
	DfxCoreApplication::setApplicationName("Deep FX Studio");

	DeepStudioGlobal::getInstance()->initShortcutActions();
	DeepStudioGlobal::getInstance()->bindShortcuts();

	QPixmap pixmap(":/splash/splash");
	QSplashScreen splash(pixmap);
	splash.show();
	splash.showMessage("Checking Licensing...", Qt::AlignBottom, Qt::white);

	DeepStudioGlobal::getInstance()->getSystemID()->generateSystemID();

	DeepFxStudioLicenseManager::init();
	bool isNodeLocked = DeepFxStudioLicenseManager::tryNodeLockedLicenseMode();
	if (!isNodeLocked) DeepFxStudioLicenseManager::tryTrialMode();

	if (DeepFxStudioLicenseManager::getState() != DeepFxStudioLicenseManager::LOCKED_STATE) {

		if (DeepFxStudioLicenseManager::getState() != DeepFxStudioLicenseManager::TRIAL_MODE_EXPIRED_STATE)
		{
			std::string ver = DeepStudioGlobal::verStr();

			if (DeepFxStudioLicenseManager::getState() == DeepFxStudioLicenseManager::TRIAL_MODE_ACTIVE_STATE) {
				DfxGlobal::console::println("\nRunning in Trial Mode.\n\n");
				splash.showMessage("Starting Trial Mode...", Qt::AlignBottom, Qt::white);
				QString daysRemaining(std::to_string(DeepFxStudioLicenseManager::trialDays - DeepFxStudioLicenseManager::getTrailDaysPast()).c_str());
				DfxCoreApplication::getInstance()->setWindowTitle(("Deep FX Studio v" + ver + "- TRIAL VERSION (" + daysRemaining.toStdString() + " Days Left)").c_str());
			}
			else {
				splash.showMessage("Commercial License Found, Starting...", Qt::AlignBottom, Qt::white);
				DfxCoreApplication::getInstance()->setWindowTitle((std::string("Deep FX Studio v" + ver).c_str()));
			}

			DfxCoreApplication::initCoreFunctionality();
			DfxSimulationCoreGlobal::installNodes();
			DeepStudioGlobal::installNodes();

			
			DfxCoreApplication::getInstance()->setWindowIcon(QIcon(":/icon/icon"));

			DfxTimelineDock *timelineDock = new DfxTimelineDock(DfxCoreApplication::getInstance());
			DfxLayersEditorDock *layerEditorDock = new DfxLayersEditorDock;

			DfxSceneGraph::getInstance()->initGraphUI();

			DfxQToolShelfConstructor::construct(DfxQToolShelf::getInstance());

			QDockWidget *toolShelfDock = new QDockWidget();
			toolShelfDock->setAllowedAreas(Qt::TopDockWidgetArea);
			toolShelfDock->setTitleBarWidget(new QWidget());
			toolShelfDock->setWidget(DfxQToolShelf::getInstance());

			DfxRenderPropertiesDock *renderProperties = new DfxRenderPropertiesDock();

			//DfxCoreApplication::getInstance()->addDockWidget(Qt::LeftDockWidgetArea, DfxCoreApplication::getConsole(), Qt::Vertical);
			//DfxCoreApplication::getInstance()->addDockWidget(Qt::LeftDockWidgetArea, layerEditorDock, Qt::Vertical);
			//DfxCoreApplication::getInstance()->addDockWidget(Qt::LeftDockWidgetArea, DfxCoreApplication::getPythonIDE(), Qt::Vertical);
			//DfxCoreApplication::getInstance()->addDockWidget(Qt::BottomDockWidgetArea, timelineDock, Qt::Vertical);
			//DfxCoreApplication::getInstance()->addDockWidget(Qt::TopDockWidgetArea, toolShelfDock, Qt::Vertical);
			//DfxCoreApplication::getInstance()->addDockWidget(Qt::RightDockWidgetArea, DfxSceneGraph::getInstance()->getGraphUI(), Qt::Vertical);

			DfxCoreApplication::getInstance()->tabifyDockWidget(DfxCoreApplication::getPythonIDE(), layerEditorDock);

			DfxStudioMainMenu::addMenu();
			DfxViewportToolbar::addToolbar();

			DfxCoreApplication::getPythonIDE()->setSaveScriptCallback(save_script);
			DfxCoreApplication::getPythonIDE()->setSaveAsScriptCallback(save_script_as);
			DfxCoreApplication::getPythonIDE()->setLoadScriptCallback(load_script);
			DfxCoreApplication::getPythonIDE()->setInstallScriptCallback(install_script);

			QTimer *customMainLoopEvents = new QTimer(DfxCoreApplication::getInstance());
			QObject::connect(customMainLoopEvents, &QTimer::timeout, dfxMainLoopEvent);
			customMainLoopEvents->start();

			DfxMainLoop::getInstance()->setTimeline(timelineDock->getTimelineWidget());

			#ifndef _DEBUG
			DfxPythonGlobal::getInstance()->enablePython(true);
			DfxPythonGlobal::getInstance()->setPythonDir(L".");
			DfxPythonGlobal::getInstance()->initPythonInterpreter();
			#endif

			DfxPythonNodeList::loadAll();
			DfxPythonNodeList::installMenus(DfxSceneGraph::getInstance()->getGraphUI()->getAddMenu());

			QThread::sleep(3);
			splash.finish(DfxCoreApplication::getInstance());

			DeepStudioGlobal::getInstance()->setTimelineWidget(timelineDock);
			DeepStudioGlobal::getInstance()->setFrameManipulator(timelineDock->getTimelineWidget()->getFrameManipulator());
			DeepStudioGlobal::getInstance()->setGraphDock(DfxSceneGraph::getInstance()->getGraphUI());
			//DeepStudioGlobal::getInstance()->setConsoleDock(DfxCoreApplication::getConsole());
			DeepStudioGlobal::getInstance()->setLayerDock(layerEditorDock);
			DeepStudioGlobal::getInstance()->setPythonIDEDock(DfxCoreApplication::getPythonIDE());
			DeepStudioGlobal::getInstance()->setRenderPropertiesDock(renderProperties);

			DeepStudioGlobal::getInstance()->selectLayerRoot();

			return DfxCoreApplication::startAppMaximised();
		}
		else {
			QDialog dialog(nullptr, Qt::WindowCloseButtonHint);

			QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
			QTextBrowser *browser = new QTextBrowser();

			QLabel *banner = new QLabel("");
			QPixmap img(":/misc/about_banner");
			banner->setPixmap(img);

			std::stringstream ss;
			ss << "<h4> Deep Fx Copyright 2015-2019 All Rights Reserved" << "</h4>" << std::endl;
			ss << "<h3> System ID / MAC Address: <span style='color:green;'>" << DeepStudioGlobal::getInstance()->getSystemID()->getSystemID() << "</span></h3>" << std::endl;
			//ss << "<br/>";
			ss << "<h3> Deep FX Studio Trial Expired </h3>"<< std::endl;
			ss << "<h3> Please purchase the full version of Deep FX Studio.</h3>" << std::endl;

			browser->setHtml(ss.str().c_str());

			mainLayout->setMargin(0);
			mainLayout->addWidget(banner);
			mainLayout->addWidget(browser);

			dialog.setWindowTitle("Deep FX Studio Trial Expired");
			dialog.setMinimumWidth(400);

			dialog.setLayout(mainLayout);

			dialog.exec();
			return 0;
		}
	}
	else {
		QMessageBox msgBox;
		msgBox.setStyleSheet("QLabel{min-width: 500px; }");
		msgBox.setText("Deep FX Studio Trial Installation Corrupt");
		msgBox.setInformativeText("License not found, trial mode either expired or trial info corrupt.");
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();

		return 0;
	}
}