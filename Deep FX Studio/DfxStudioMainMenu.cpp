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

#include "DfxStudioMainMenu.h"
#include <DfxCParticleCacheLoader.h>

QMenuBar *DfxStudioMainMenu::_main;

QMenu *DfxStudioMainMenu::fileMenu;
QMenu *DfxStudioMainMenu::recentScenes;
QMenu *DfxStudioMainMenu::importMenu;
QAction *DfxStudioMainMenu::importObjectAction;
QAction *DfxStudioMainMenu::importColladaAction;
QAction *DfxStudioMainMenu::importParticleCacheAction;
QAction *DfxStudioMainMenu::newSceneAction;
QAction *DfxStudioMainMenu::openSceneAction;

QAction *DfxStudioMainMenu::saveSceneAction;
QAction *DfxStudioMainMenu::saveSceneAsAction;

QAction *DfxStudioMainMenu::showSceneGraphAction;
QAction *DfxStudioMainMenu::viewportScreenshot;
QAction *DfxStudioMainMenu::showConsoleAction;
QAction *DfxStudioMainMenu::showLayersAction;
QAction *DfxStudioMainMenu::showPythonIDEAction;
QAction *DfxStudioMainMenu::showPreferencesAction;
QAction *DfxStudioMainMenu::showRenderPropertiesAction;
QAction *DfxStudioMainMenu::createPlayBlast;

QAction *DfxStudioMainMenu::exitAction;

QAction *DfxStudioMainMenu::viewHelpAction;
QAction *DfxStudioMainMenu::submitBugReportAction;
QAction *DfxStudioMainMenu::aboutAction;

QMenu *DfxStudioMainMenu::editMenu;
QMenu *DfxStudioMainMenu::viewMenu;
QMenu *DfxStudioMainMenu::createMenu;
QMenu *DfxStudioMainMenu::selectMenu;
QMenu *DfxStudioMainMenu::helpMenu;

QList<DfxOpenSceneAction*> DfxStudioMainMenu::recentSceneActions;

void DfxStudioMainMenu::addMenu() {

	_main = new QMenuBar(DfxCoreApplication::getInstance());

	fileMenu = new QMenu("File", _main);
	recentScenes = new QMenu("Recent Scenes", _main);
	editMenu = new QMenu("Edit", _main);
	viewMenu = new QMenu("View", _main);
	createMenu = new QMenu("Create", _main);
	selectMenu = new QMenu("Select", _main);
	helpMenu = new QMenu("Help", _main);

	newSceneAction = new QAction("New Scene");
	openSceneAction = new QAction("Open Scene");

	showSceneGraphAction = new QAction("Scene Graph");
	showConsoleAction = new QAction("Console");
	showLayersAction = new QAction("Layers");
	showPythonIDEAction = new QAction("Python IDE");

#if ENABLE_PROTOTYPE_FEATURES
	showRenderPropertiesAction = new QAction("Render Properties");
#endif

	viewportScreenshot = new QAction("Capture Viewport");
	createPlayBlast = new QAction("Create Play Blast");
	showPreferencesAction = new QAction("Preferences...");

	importMenu = new QMenu("Import", _main);
	importObjectAction = new QAction("Import .Obj");
	importColladaAction = new QAction("Import Collada");
	importParticleCacheAction = new QAction("Import Particle Cache");

	importMenu->addAction(importObjectAction);
	importMenu->addAction(importColladaAction);
	importMenu->addSeparator();
	importMenu->addAction(importParticleCacheAction);

	saveSceneAction = new QAction("Save Scene");
	saveSceneAsAction = new QAction("Save Scene As");

	exitAction = new QAction("Exit");

	viewHelpAction			= new QAction("View Online Help");
	submitBugReportAction	= new QAction("Submit Bug Report");
	aboutAction				= new QAction("About");

	fileMenu->addAction(newSceneAction);
	fileMenu->addAction(openSceneAction);
	fileMenu->addMenu(recentScenes);
	fileMenu->addMenu(importMenu);
	fileMenu->addSeparator();
	fileMenu->addAction(saveSceneAction);
	fileMenu->addAction(saveSceneAsAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	viewMenu->addAction(showSceneGraphAction);
	viewMenu->addAction(showLayersAction);
	viewMenu->addAction(showPythonIDEAction);
	viewMenu->addAction(showConsoleAction);
	viewMenu->addSeparator();
	viewMenu->addAction(showRenderPropertiesAction);
	viewMenu->addSeparator();
	viewMenu->addAction(viewportScreenshot);
	viewMenu->addAction(createPlayBlast);
	viewMenu->addAction(showPreferencesAction);

	helpMenu->addAction(viewHelpAction);
	helpMenu->addAction(submitBugReportAction);
	helpMenu->addAction(aboutAction);

	_main->addMenu(fileMenu);
	//_main->addMenu(editMenu);
	_main->addMenu(viewMenu);
	//_main->addMenu(createMenu);
	//_main->addMenu(selectMenu);
	_main->addMenu(helpMenu);

	initRecentSceneList();

	DfxCoreApplication::getInstance()->setMenuBar(_main);

	QObject::connect(newSceneAction, &QAction::triggered, DfxStudioMainMenu::newScene);
	QObject::connect(saveSceneAction, &QAction::triggered, DfxStudioMainMenu::saveScene);
	QObject::connect(saveSceneAsAction, &QAction::triggered, DfxStudioMainMenu::saveSceneAs);
	QObject::connect(openSceneAction, &QAction::triggered, DfxStudioMainMenu::openScene);
	QObject::connect(importObjectAction, &QAction::triggered, DfxStudioMainMenu::importObject);
	QObject::connect(importColladaAction, &QAction::triggered, DfxStudioMainMenu::importCollada);
	QObject::connect(importParticleCacheAction, &QAction::triggered, DfxStudioMainMenu::importParticleCache);

	QObject::connect(showSceneGraphAction, &QAction::triggered, DfxStudioMainMenu::showSceneGraph);
	QObject::connect(showConsoleAction, &QAction::triggered, DfxStudioMainMenu::showConsole);
	QObject::connect(showPythonIDEAction, &QAction::triggered, DfxStudioMainMenu::showPythonIDE);
	QObject::connect(showLayersAction, &QAction::triggered, DfxStudioMainMenu::showLayers);
	QObject::connect(showPreferencesAction, &QAction::triggered, DfxStudioMainMenu::showPreferences);
	QObject::connect(showRenderPropertiesAction, &QAction::triggered, DfxStudioMainMenu::showRenderProperties);
	QObject::connect(viewportScreenshot, &QAction::triggered, DfxStudioMainMenu::captureViewport);
	QObject::connect(createPlayBlast, &QAction::triggered, DfxStudioMainMenu::_createPlayBlast);
	
	QObject::connect(exitAction, &QAction::triggered, DfxStudioMainMenu::closeApplication);

	QObject::connect(viewHelpAction, &QAction::triggered, DfxStudioMainMenu::viewHelp);
	QObject::connect(submitBugReportAction, &QAction::triggered, DfxStudioMainMenu::submitBugReport);
	QObject::connect(aboutAction, &QAction::triggered, DfxStudioMainMenu::about);
}


void DfxStudioMainMenu::importObject() {
	
	QSettings settings;
	QString initialDir = settings.value("io/importObject", QDir::homePath()).toString();

	QString fileName
		= QFileDialog::getOpenFileName(nullptr, QString("Load Object"), initialDir, QString("Files (*.obj)"));

	std::stringstream ss;

	ss << "loadObject " << fileName.toUtf8().toStdString();

	DfxGlobal::command::exec(ss.str());

	if (fileName.length() > 0) {
		settings.setValue("io/importObject", fileName);
	}
}

void DfxStudioMainMenu::importParticleCache() {

	QSettings settings;
	QString initialDir = settings.value("io/importParticleCache", QDir::homePath()).toString();

	QString fileName
		= QFileDialog::getOpenFileName(nullptr, QString("Load Particle Cache"), initialDir, QString("Files (*.bgeo)"));

	if (fileName.size()) {
		filesystem::path path(fileName.toStdString());

		if (filesystem::exists(path)) {

			dfxcore::ParticleCacheLoader *loader = new dfxcore::ParticleCacheLoader(dfxcore::ParticleCacheLoader_Identifier);
			loader->setFilepath(path.string());

			DfxLayerManager::getInstance()->addViewNode(loader);

			settings.setValue("io/importParticleCache", fileName);
		}
		else {

			QMessageBox msgBox;
			msgBox.setText("Unable to load file.");
			msgBox.exec();
		}
	}
}

void DfxStudioMainMenu::newScene() {
	DeepStudioGlobal::getInstance()->resetFrameState();
	DfxSimulationCoreGlobal::getInstance()->deinitializeSimulationDomain();

	DeepStudioGlobal::getInstance()->setOpenScenePath("");

	DfxGlobal::command::exec("clearSelection");
	DfxLayerManager::getInstance()->reset();
	DeepStudioGlobal::getInstance()->selectLayerRoot();
	DfxGlobal::command::exec("resetViewport");
}

void DfxStudioMainMenu::openScene() {

	QSettings settings;
	QString initialDir = settings.value("io/openScene", QDir::homePath()).toString();

	DfxLayerManager::getInstance()->setUseMaximiseScene(false);
	
	QString fileName
		= QFileDialog::getOpenFileName(nullptr, QString("Load Scene"), initialDir, QString("Files (*.sxx)"));

	if (fileName.size() > 0) {

		DeepStudioGlobal::getInstance()->resetFrameState();
		DfxSimulationCoreGlobal::getInstance()->deinitializeSimulationDomain();

		DfxLayerManager::getInstance()->reset();
		DfxGlobal::command::exec("resetViewport");
		DfxLayerManager::getInstance()->removeLayer(0);

		DfxSceneLoader *loaderListener = new DfxSceneLoader;

		DfxXMLSceneLoader loader;
		loader.load(fileName.toStdString());

		settings.setValue("io/openScene", fileName);

		updateRecentOpenedSceneList(fileName.toStdString());

		DeepStudioGlobal::getInstance()->setOpenScenePath(fileName.toStdString());
	}

	DfxLayerManager::getInstance()->resetSelectedLayer();
	DfxLayerManager::getInstance()->forceMaximise();
	DfxLayerManager::getInstance()->setUseMaximiseScene(true);

	DeepStudioGlobal::getInstance()->selectLayerRoot();
}

void DfxStudioMainMenu::importCollada() {

	QSettings _settings;
	QString initialDir = _settings.value("io/importCollada", QDir::homePath()).toString();

	QString fileName
		= QFileDialog::getOpenFileName(nullptr, QString("Import Collada"), initialDir, QString("Files (*.dae;)"));

	if (fileName.size() > 0) {


		DfxColladaSceneImporter importer;

		DfxColladaSceneImporterSettings settings;
		settings.flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals ;


		importer.importFile(fileName.toStdString(), settings);

		_settings.setValue("io/importCollada", fileName);
	}
}

void DfxStudioMainMenu::saveScene() {

	std::string str = DeepStudioGlobal::getInstance()->getOpenScenePath();

	if (str.length() > 0) {
		DfxXMLSceneSaver saver;
		saver.makeUnreferencedObjectsPersistent(str);
		saver.save(str);
	}
	else 
	{
		QSettings settings;
		QString initialDir = settings.value("io/saveScene", QDir::homePath()).toString();

		QString fileName
			= QFileDialog::getSaveFileName(nullptr, QString("Save Scene"), initialDir, QString("Files (*.sxx)"));

		if (fileName.size() > 0) {
			DfxXMLSceneSaver saver;
			saver.makeUnreferencedObjectsPersistent(fileName.toStdString());
			saver.save(fileName.toStdString());

			settings.setValue("io/saveScene", fileName);

			updateRecentOpenedSceneList(fileName.toStdString());

			DeepStudioGlobal::getInstance()->setOpenScenePath(fileName.toStdString());
		}
	}
}

void DfxStudioMainMenu::saveSceneAs() {
	QSettings settings;
	QString initialDir = settings.value("io/saveScene", QDir::homePath()).toString();

	QString fileName
		= QFileDialog::getSaveFileName(nullptr, QString("Save Scene"), initialDir, QString("Files (*.sxx)"));

	if (fileName.size() > 0) {
		DfxXMLSceneSaver saver;
		saver.makeUnreferencedObjectsPersistent(fileName.toStdString());
		saver.save(fileName.toStdString());

		settings.setValue("io/saveScene", fileName);

		updateRecentOpenedSceneList(fileName.toStdString());

		DeepStudioGlobal::getInstance()->setOpenScenePath(fileName.toStdString());
	}
}

void DfxStudioMainMenu::captureViewport() {

	QSettings settings;
	QString initialDir = settings.value("io/captureViewport", QDir::homePath()).toString();

	QString fileName
		= QFileDialog::getSaveFileName(nullptr, QString("Save Image"), initialDir, QString("Files (*.png)"));

	if (fileName.size() > 0) {

		auto img = DfxCoreApplication::getInstance()->captureViewport();

		img->save(fileName, "PNG");

		delete img;

		settings.setValue("io/captureViewport", fileName);
	}
}

void DfxStudioMainMenu::_createPlayBlast() {

	auto images = DfxPlayBlastBuilder::buildPlayBlast();

	DfxPlayBlastPlayer player(0, Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
	player.setImageList(images);
	player.exec();

	for (auto img : images) delete img;
}

void  DfxStudioMainMenu::closeApplication(){

	QMessageBox::StandardButton btn = QMessageBox::question(Q_NULLPTR, "Deep FX Studio",
															QString("Do you really want to quit?"),
															QMessageBox::No | QMessageBox::Yes,
															QMessageBox::Yes);

	if (btn == QMessageBox::Yes) {
		DfxCoreApplication::closeApplication();
	}
}


void DfxStudioMainMenu::initRecentSceneList() {
	QSettings settings;
	auto list = settings.value("io/recentFileList", QStringList()).value<QStringList>();

	for (int inx = 0; inx < 10; inx++) {
		DfxOpenSceneAction *act = new DfxOpenSceneAction();
		act->setPath("");
		act->setText("");
		act->setVisible(false);

		recentScenes->addAction(act);
		recentSceneActions.push_back(act);
	}

	for (int inx = 0; inx < list.size(); inx++) {
		recentSceneActions[inx]->setPath(list[inx]);
		recentSceneActions[inx]->setText(list[inx]);
		recentSceneActions[inx]->setVisible(true);
	}
}

void DfxStudioMainMenu::updateRecentOpenedSceneList(std::string path) {
	QSettings settings;
	auto list = settings.value("io/recentFileList", QStringList()).value<QStringList>();

	if (list.size() > 0) {
		if (list[0].compare(path.c_str(), Qt::CaseInsensitive) == 0) return;
	}

	if (list.size() >= 10) {
		list.removeAt(list.size() - 1);
		list.push_front(path.c_str());
	}
	else {
		list.push_front(path.c_str());
	}

	for (int inx = 0; inx < recentSceneActions.size(); inx++) recentSceneActions[inx]->setVisible(false);

	for (int inx = 0; inx < list.size(); inx++) {
		recentSceneActions[inx]->setPath(list[inx]);
		recentSceneActions[inx]->setText(list[inx]);
		recentSceneActions[inx]->setVisible(true);
	}

	settings.setValue("io/recentFileList", list);

}

void DfxStudioMainMenu::showSceneGraph() {
	DeepStudioGlobal::getInstance()->showGraphDock();
}

void DfxStudioMainMenu::showConsole () {
	DeepStudioGlobal::getInstance()->showConsoleDock();
}

void DfxStudioMainMenu::showLayers() {
	DeepStudioGlobal::getInstance()->showLayerDock();
}

void DfxStudioMainMenu::showPythonIDE() {
	DeepStudioGlobal::getInstance()->showPythonIDEDock();
}

void DfxStudioMainMenu::showPreferences() {
	DfxQPreferencesDialog pref;

	pref.exec();
}

void DfxStudioMainMenu::viewHelp() {
	// DeepStudioGlobal::getInstance()->showHelpBrowser();
	QDesktopServices::openUrl(QUrl("https://www.deepfxworld.com/documentation/DeepFxv1/Welcome.html"));
}

void DfxStudioMainMenu::submitBugReport() {
	QDesktopServices::openUrl(QUrl("https://www.deepfxworld.com/support"));
}

void DfxStudioMainMenu::about() {
	DeepStudioGlobal::getInstance()->showAboutDialog();
}

void DfxStudioMainMenu::showRenderProperties() {
	DeepStudioGlobal::getInstance()->showRenderPropertiesDock();
}