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
#include "DfxCoreApplication.h"
#include "DfxGlobal.h"
#include "DfxObjLoaderPlugin.h"

// Init Static globals
DfxCoreApplication *DfxCoreApplication::instance = nullptr;
QApplication *DfxCoreApplication::qapp = nullptr;
QSurfaceFormat DfxCoreApplication::glFormat;
QOpenGLContext *DfxCoreApplication::glContext = nullptr;
QLabel *DfxCoreApplication::statMsg = nullptr;
bool DfxCoreApplication::initialViewport = true;
std::mutex DfxCoreApplication::updateViewportsLock;

DfxCoreApplication::DfxCoreApplication(QWidget *parent, bool addViewport): QMainWindow(parent) {

	glContext = new QOpenGLContext;
	glContext->create();

	glFormat.setDepthBufferSize(24);
	glFormat.setStencilBufferSize(8);
	glFormat.setVersion(2, 1);
	glFormat.setProfile(QSurfaceFormat::CompatibilityProfile);

	if (initialViewport) {
		initViewports();
		setCentralWidget(viewports[0]);
	}

	console = new DfxConsole();
	pythonIDE = new DfxTinyPythonIDE();

	/*QObject::connect(DfxConsoleBuffer::getInstance(), &DfxConsoleBuffer::bufferUpdateEvent,
				     console, &DfxConsole::updateTextOuput);*/


	DfxGlobal::console::print("");

	statMsg = new QLabel("", this);
	statusBar()->addWidget(statMsg);
}

DfxCoreApplication *DfxCoreApplication::getInstance() {
	initInstance();

	return instance;
}

DfxCoreApplication::~DfxCoreApplication() {
	delete console;
}

void DfxCoreApplication::initInstance() {

	if (instance == nullptr) {
		instance = new DfxCoreApplication();
	}
}

void DfxCoreApplication::initViewports() {

	DfxViewportVersion01 *vp01 = new DfxViewportVersion01(glFormat);
	viewports.push_back(new DfxViewportVersion01());
}

void DfxCoreApplication::setViewport(int index) {

}

void DfxCoreApplication::closeEvent(QCloseEvent *evt) {

	QMessageBox::StandardButton btn = QMessageBox::question(this, "Deep FX Studio",
															tr("Do you really want to quit?"),
															QMessageBox::No | QMessageBox::Yes,
															QMessageBox::Yes);

	if (btn != QMessageBox::Yes) {
		evt->ignore();
	}
}

QImage *DfxCoreApplication::captureViewport(uint inx) {
	return viewports[inx]->capture();
}

int DfxCoreApplication::startApp(int argc, char *argv[]) {
	
	if (qapp != nullptr) delete qapp;
	qapp = new QApplication(argc, argv);

	initInstance();

	if (instance->isVisible()) return -1;
	

	instance->show();

	return qapp->exec();
}

int DfxCoreApplication::startAppMaximised(int argc, char *argv[]) {
	
	initQApplication(argc, argv);

	initInstance();

	if (instance->isVisible()) return -1;

	instance->showMaximized();

	if (initialViewport) {
		DfxOpenGLGlobal::getInstance()->initDefaultOpenGL();
	}

	return qapp->exec();
}

bool DfxCoreApplication::isOpen() {
	initInstance();

	return instance->isVisible();
}

void DfxCoreApplication::_showConsole() {
	console->show();
}

DfxConsole *DfxCoreApplication::_getConsole() {
	return console;
}

void DfxCoreApplication::showConsole() {
	initInstance();

	instance->_showConsole();
}

DfxConsole *DfxCoreApplication::getConsole() {
	initInstance();

	return instance->_getConsole();
}

void DfxCoreApplication::_showPythonIDE() {
	pythonIDE->show();
}

DfxTinyPythonIDE *DfxCoreApplication::_getPythonIDE() {
	return pythonIDE;
}

void DfxCoreApplication::showPythonIDE() {
	initInstance();

	instance->_showPythonIDE();
}

DfxTinyPythonIDE *DfxCoreApplication::getPythonIDE() {
	initInstance();

	return instance->_getPythonIDE();
}

void DfxCoreApplication::initCoreFunctionality() {
	initCoreEntities();
	initCoreServices();
	initCorePlugins();
	initCoreCommands();
}

void DfxCoreApplication::initCoreServices() {
	
	DfxServiceClass *objectLoaderClass = new DfxServiceClass;
	objectLoaderClass->unique_id = DFX_OBJECTLOADER_SERVICE;
	objectLoaderClass->name = "ObjectLoader_Class";
	std::pair<size_t, DfxServiceClass*> objLoaderClassPair(objectLoaderClass->unique_id, objectLoaderClass);
	DfxServiceRegistry::services.insert(objLoaderClassPair);

}

void DfxCoreApplication::initCoreEntities() {

	// Root
	DfxEntityDB::registerDfxEntity("0", DfxRoot::initialize);

	// Item Entities
	DfxEntityDB::registerDfxEntity(DfxCamera_Identifier,
								   DfxCamera::initialize,
								   DfxCamera::creator);

	DfxEntityDB::registerDfxEntity(DfxLight_Identifier,
								   DfxLight::initialize,
								   DfxLight::creator);

	DfxEntityDB::registerDfxEntity( DfxObjectItem_Identifier, 
									DfxObjectItem::initialize,
									DfxObjectItem::creator);

	DfxEntityDB::registerDfxEntity( DfxObjectItemInfo_Identifier, 
									DfxObjectItemInfo::initialize,
									DfxObjectItemInfo::creator);
	
	DfxEntityDB::registerDfxEntity( DfxPointItem_Identifier, 
		                            DfxPointItem::initialize,
									DfxPointItem::creator);

	DfxEntityDB::registerDfxEntity( DfxSpotterItem_Identifier,
								    DfxSpotterItem::initialize,
								    DfxSpotterItem::creator);
	
	// Item Entities Support
	DfxEntityDB::registerDfxEntity( DfxMapUniformDisplacement_Identifier, 
								    DfxMapUniformDisplacement::initialize,
								    DfxMapUniformDisplacement::creator);

	DfxEntityDB::registerDfxEntity(DfxMapUniformDisplacementProxy_Identifier,
								   DfxMapUniformDisplacementProxy::initialize,
								   DfxMapUniformDisplacementProxy::creator);

	DfxEntityDB::registerDfxEntity(DfxObjectItemCopy_Identifier,
								   DfxObjectItemCopy::initialize,
								   DfxObjectItemCopy::creator);

	DfxEntityDB::registerDfxEntity(DfxObjectViewer_Identifier,
								   DfxObjectViewer::initialize,
								   DfxObjectViewer::creator);

	DfxEntityDB::registerDfxEntity(DfxMergeObjectItem_Identifier,
								   DfxMergeObjectItem::initialize,
								   DfxMergeObjectItem::creator);

	DfxEntityDB::registerDfxEntity(DfxMeshXForm_Identifier,
								   DfxMeshXForm::initialize,
								   DfxMeshXForm::creator);
	// Object Loaders
	DfxEntityDB::registerDfxEntity( DfxObjLoaderPlugin_Identifier, 
									DfxObjLoaderPlugin::initialize );
}

void DfxCoreApplication::initCorePlugins() {

	// Object Loader Services
	DfxServiceEntry *objLoaderServiceEntry = new DfxServiceEntry;
	objLoaderServiceEntry->setUniqueName(DfxObjLoaderPlugin_Identifier);
	objLoaderServiceEntry->addTag( "format", "obj" );
	objLoaderServiceEntry->setCreate( DfxObjLoaderPlugin::createService );

	DfxServiceRegistry::registerSerivce( DFX_OBJECTLOADER_SERVICE, objLoaderServiceEntry );
}

void DfxCoreApplication::initCoreCommands() {

	registerDfxCommand("updateViewport", DfxCommands::UpdateViewportCommand::create);
	registerDfxCommand("syncInterface", DfxCommands::SyncInterfaceCommand::create);
	registerDfxCommand("resetViewport", DfxCommands::ResetViewportCommand::create);
	registerDfxCommand("clearSelection", DfxCommands::ClearSelectionCommand::create);
	registerDfxCommand("addPointView", DfxCommands::AddPointViewCommand::create);
	registerDfxCommand("addSpotter",   DfxCommands::AddSpotterCommand::create);
	registerDfxCommand("addObjectProxyViewer", DfxCommands::AddObjectProxyViewerCommand::create);
	registerDfxCommand("loadObject",   DfxCommands::LoadObjectCommand::create);
}

void DfxCoreApplication::updateViewports() {
	initInstance();

	while (!updateViewportsLock.try_lock());

	for (auto viewport : instance->viewports) {
		viewport->update();
	}

	updateViewportsLock.unlock();
}

void DfxCoreApplication::initQApplication(int argc, char *argv[]) {
	if (qapp == nullptr) {
		qapp = new QApplication(argc, argv);
	}
}

void DfxCoreApplication::setFusionDarkTheme() {
	qapp->setStyle(QStyleFactory::create("Fusion"));

	QPalette darkPalette;
	darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
	darkPalette.setColor(QPalette::WindowText, Qt::white);
	darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
	darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
	darkPalette.setColor(QPalette::ToolTipText, Qt::white);
	darkPalette.setColor(QPalette::Text, Qt::white);
	darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
	darkPalette.setColor(QPalette::ButtonText, Qt::white);
	darkPalette.setColor(QPalette::BrightText, Qt::red);
	darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
	
	//darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	darkPalette.setColor(QPalette::Highlight, QColor(255, 127, 42));
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);

	qapp->setPalette(darkPalette);

	qapp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

void DfxCoreApplication::resetViewports() {
	initInstance();

	while (!updateViewportsLock.try_lock());

	for (auto viewport : instance->viewports) {
		viewport->reset();
	}

	updateViewportsLock.unlock();
}

void DfxCoreApplication::processQAppEvents() {
	qapp->processEvents();
}

void DfxCoreApplication::statusMessage(QString str) {
	//statusBar()->showMessage(str);
	statMsg->setText(str);
	statMsg->update();
	processQAppEvents();
}

void DfxCoreApplication::closeApplication() {
	qapp->quit();
}

void DfxCoreApplication::setOrganizationName(QString str) {
	qapp->setOrganizationName(str);
}

void DfxCoreApplication::setOrganizationDomain(QString str) {
	qapp->setOrganizationDomain(str);
}

void DfxCoreApplication::setApplicationName(QString str) {
	qapp->setApplicationName(str);
}

size_t DfxCoreApplication::viewportCount() {
	return viewports.size();
}

DfxAbstractViewport *DfxCoreApplication::getViewport(size_t inx) {
	return viewports[inx];
}