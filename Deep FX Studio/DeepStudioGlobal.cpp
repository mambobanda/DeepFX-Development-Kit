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

#include <DfxPlugin.h>
#include <QDesktopServices> 
#include <QVariant>
#include <QPair>
#include <DfxLayersEditorDock.h>
#include "DfxTimelineDock.h"
#include "DeepFxStudioLicenseManager.h"
#include "DeepStudioGlobal.h"
#include "DfxLayersEditorDock.h"
#include "DfxVDBParticleMesher.h"
#include "DfxRadeonProRenderNode .h"
#include "DfxSpherePrimitiveCommand.h"
#include "DfxCubePrimitiveCommand.h"
#include "DfxPlanePrimitiveCommand.h"
#include "DfxTorusPrimitiveCommand.h"

typedef QPair<int, int> cQPair;
Q_DECLARE_METATYPE(cQPair)

DeepStudioGlobal *DeepStudioGlobal::instance = nullptr;

void DeepStudioGlobal::installNodes() {
	DeepStudioGlobal::getInstance()->init();

	// Primitive Commands
	registerDfxCommand("createSphere", DfxCommands::SpherePrimitiveCommand::create);
	registerDfxCommand("createBox", DfxCommands::CubePrimitiveCommand::create);
	registerDfxCommand("createPlane", DfxCommands::PlanePrimitiveCommand::create);
	registerDfxCommand("createTorus", DfxCommands::TorusPrimitiveCommand::create);


	// Render Node
#if ENABLE_PROTOTYPE_FEATURES
	DfxEntityDB::registerDfxEntity(radeon::DfxRadeonProRenderNode_Identifier,
								   radeon::DfxRadeonProRenderNode::initialize,
								   radeon::DfxRadeonProRenderNode::creator);
#endif

	// Group Nodes
	DfxEntityDB::registerDfxEntity(DfxNodes::DfxGroupNode_Identifier,
								   DfxNodes::DfxGroupNode::initialize,
								   DfxNodes::DfxGroupNode::creator);

	// Math Nodes
	DfxEntityDB::registerDfxEntity(DfxNodes::sPrintValueNode_Identifier,
								   DfxNodes::sPrintValueNode::initialize,
								   DfxNodes::sPrintValueNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::sRandomArrayNode_Identifier,
								   DfxNodes::sRandomArrayNode::initialize,
								   DfxNodes::sRandomArrayNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::sIncrementNode_Identifier,
								   DfxNodes::sIncrementNode::initialize,
								   DfxNodes::sIncrementNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::sCountNode_Identifier,
								   DfxNodes::sCountNode::initialize,
								   DfxNodes::sCountNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::sUniformArrayNode_Identifier,
								   DfxNodes::sUniformArrayNode::initialize,
								   DfxNodes::sUniformArrayNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::vCountNode_Identifier,
								   DfxNodes::vCountNode::initialize,
								   DfxNodes::vCountNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::vUniformArrayNode_Identifier,
								   DfxNodes::vUniformArrayNode::initialize,
								   DfxNodes::vUniformArrayNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::sValueNode_Identifier,
								   DfxNodes::sValueNode::initialize,
								   DfxNodes::sValueNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::vValueNode_Identifier,
								   DfxNodes::vValueNode::initialize,
								   DfxNodes::vValueNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::sAddNode_Identifier,
								   DfxNodes::sAddNode::initialize,
								   DfxNodes::sAddNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::sSubtractNode_Identifier, 
								   DfxNodes::sSubtractNode::initialize,
								   DfxNodes::sSubtractNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::sMultiplyNode_Identifier, 
								   DfxNodes::sMultiplyNode::initialize,
								   DfxNodes::sMultiplyNode::creator);


	DfxEntityDB::registerDfxEntity(DfxNodes::sDivideNode_Identifier,
								   DfxNodes::sDivideNode::initialize,
								   DfxNodes::sDivideNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::vAddNode_Identifier, 
								   DfxNodes::vAddNode::initialize,
								   DfxNodes::vAddNode::creator);
	
	//Time Nodes
	DfxEntityDB::registerDfxEntity(DfxNodes::TimeInfo_Identifier, 
								   DfxNodes::TimeInfo::initialize,
								   DfxNodes::TimeInfo::creator);

	//Envelope Entity
	DfxEntityDB::registerDfxEntity(DfxEnvelope_Identifier,
								   DfxEnvelope::initialize,
								   nullptr);

	//Loops and Conditionals
	DfxEntityDB::registerDfxEntity(DfxNodes::DfxRangedLoopNode_Identifier,
								   DfxNodes::DfxRangedLoopNode::initialize,
								   DfxNodes::DfxRangedLoopNode::creator);

	//Global Properties
	DfxEntityDB::registerDfxEntity(DfxNodes::ScalarValueGlobal_Identifier,
								   DfxNodes::ScalarValueGlobal::initialize,
								   DfxNodes::ScalarValueGlobal::creator);

	// VDB Nodes
#ifdef USE_OPENVDB_LIB
	DfxEntityDB::registerDfxEntity(DfxNodes::DfxVDBParticleMesher_Identifier,
								   DfxNodes::DfxVDBParticleMesher::initialize,
								   DfxNodes::DfxVDBParticleMesher::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::DfxVDBDilateNode_Identifier,
								   DfxNodes::DfxVDBDilateNode::initialize,
								   DfxNodes::DfxVDBDilateNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::DfxVDBErodeNode_Identifier,
								   DfxNodes::DfxVDBErodeNode::initialize,
								   DfxNodes::DfxVDBErodeNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::DfxVDBMedianNode_Identifier,
								   DfxNodes::DfxVDBMedianNode::initialize,
								   DfxNodes::DfxVDBMedianNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::DfxVDBMeanNode_Identifier,
								   DfxNodes::DfxVDBMeanNode::initialize,
								   DfxNodes::DfxVDBMeanNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::DfxVDBGaussianNode_Identifier,
								   DfxNodes::DfxVDBGaussianNode::initialize,
								   DfxNodes::DfxVDBGaussianNode::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::VDBMeshToVolume_Identifier,
								   DfxNodes::VDBMeshToVolume::initialize,
								   DfxNodes::VDBMeshToVolume::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::VDBVolumeToMesh_Identifier,
								   DfxNodes::VDBVolumeToMesh::initialize,
								   DfxNodes::VDBVolumeToMesh::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::VDBVolumeToPoints_Identifier,
								   DfxNodes::VDBVolumeToPoints::initialize,
								   DfxNodes::VDBVolumeToPoints::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::VDBBoolean_Identifier,
								   DfxNodes::VDBBoolean::initialize,
								   DfxNodes::VDBBoolean::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::VDBDilate_Identifier,
								   DfxNodes::VDBDilate::initialize,
								   DfxNodes::VDBDilate::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::VDBFilter_Identifier,
								   DfxNodes::VDBFilter::initialize,
								   DfxNodes::VDBFilter::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::VDBResample_Identifier,
								   DfxNodes::VDBResample::initialize,
								   DfxNodes::VDBResample::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::VDBPointsToVolume_Identifier,
								   DfxNodes::VDBPointsToVolume::initialize,
								   DfxNodes::VDBPointsToVolume::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::VDBFromFile_Identifier,
								   DfxNodes::VDBFromFile::initialize,
								   DfxNodes::VDBFromFile::creator);

	DfxEntityDB::registerDfxEntity(DfxNodes::VDBToFile_Identifier,
								   DfxNodes::VDBToFile::initialize,
								   DfxNodes::VDBToFile::creator);

#endif
}

DeepStudioGlobal::DeepStudioGlobal() {
	systemID = new DfxSystemIDMacAddress();
	//systemID = new DfxSystemIDMotherBoard();
	frameState.resize(DfxTime::getInstance()->getTotalFrames());
	openScenePath = "";
}

DeepStudioGlobal *DeepStudioGlobal::getInstance() {

	if (instance == nullptr) {
		instance = new DeepStudioGlobal();
	}

	return instance;
}

DeepStudioGlobal::~DeepStudioGlobal() {

}

int DeepStudioGlobal::_getSimulationState() {
	return simulationState;
}

void DeepStudioGlobal::_setSimulationState(int st) {

	simulationState = st;
}

int DeepStudioGlobal::getSimulationState() {
	return DeepStudioGlobal::getInstance()->_getSimulationState();
}

void DeepStudioGlobal::setSimulationState(int st) {
	DeepStudioGlobal::getInstance()->_setSimulationState(st);
}

bool DeepStudioGlobal::isFrameCached(int f) {
	return frameState[f - 1].cached;
}

bool DeepStudioGlobal::isFrameCached() {
	int frame = DfxTime::getInstance()->getCurrentFrame();

	return frameState[frame - 1].cached;
}

bool DeepStudioGlobal::isFrameValid() {
	int frame = DfxTime::getInstance()->getCurrentFrame();

	return frameState[frame - 1].valid;
}

void DeepStudioGlobal::cacheFrame() {
	int frame = DfxTime::getInstance()->getCurrentFrame();

	frameState[frame - 1].cached = true;
}

void DeepStudioGlobal::makeFrameValid() {
	int frame = DfxTime::getInstance()->getCurrentFrame();

	frameState[frame - 1].valid = true;
}

int DeepStudioGlobal::getLastCachedFrame() {

	for (int i = frameState.size() - 1; i >= 0; i--) {
		if (frameState[i].cached) {
			return i + 1;
		}
	}

	return -1;
}

bool DeepStudioGlobal::allFramesCached() {

	for (int i = frameState.size() - 1; i >= 0; i--) {
		if (!frameState[i].cached) {
			return false;
		}
	}

	return true;
}

void DeepStudioGlobal::resetFrameState() {

	for (int i = 0; i < frameState.size(); i++) {
		frameState[i].reset();
	}
}

void DeepStudioGlobal::init() {

	DfxTimeActionListener *action = new DfxTimeActionListener([this](DfxTimeActionInfo info) {

		if (info.actionCode == DfxTimeActionListener::NEW_FRAME_COUNT_ACTION) {
			frameState.resize(DfxTime::getInstance()->getTotalFrames());
		}
	});

	DfxTime::getInstance()->addActionListener(action);
}

std::string DeepStudioGlobal::getOpenScenePath() {
	return openScenePath;
}

void DeepStudioGlobal::setOpenScenePath(std::string pt) {
	openScenePath = pt;
	updateWindowTitle();
}

void DeepStudioGlobal::updateWindowTitle() {
	std::string ver = DeepStudioGlobal::verStr();

	if (DeepFxStudioLicenseManager::getState() == DeepFxStudioLicenseManager::TRIAL_MODE_ACTIVE_STATE) {
		QString daysRemaining(std::to_string(DeepFxStudioLicenseManager::trialDays - DeepFxStudioLicenseManager::getTrailDaysPast()).c_str());
		DfxCoreApplication::getInstance()->setWindowTitle(("Deep FX Studio v" + ver + " - TRIAL VERSION (" + daysRemaining.toStdString() + " Days Left): ").c_str() + QString(openScenePath.c_str()));
	}
	else {
		DfxCoreApplication::getInstance()->setWindowTitle((std::string("Deep FX Studio v" + ver).c_str()) +QString(" - ") + QString(openScenePath.c_str()));
	}
	
}

void DeepStudioGlobal::setGraphDock(QDockWidget *gd) {
	graphDock = gd;
}

void DeepStudioGlobal::showGraphDock() {
	if (graphDock->isHidden()) graphDock->show();
}

void DeepStudioGlobal::setLayerDock(QDockWidget *ld) {
	layerDock = ld;
}

void DeepStudioGlobal::showLayerDock() {
	if (layerDock->isHidden()) layerDock->show();
}

void DeepStudioGlobal::setConsoleDock(QDockWidget *cd) {
	consoleDock = cd;
}

void DeepStudioGlobal::showConsoleDock() {
	if (consoleDock->isHidden()) consoleDock->show();
}

void DeepStudioGlobal::setPythonIDEDock(QDockWidget *pde) {
	pythonIDEEditor = pde;
}

void DeepStudioGlobal::showPythonIDEDock() {
	if (pythonIDEEditor->isHidden()) pythonIDEEditor->show();
}

void DeepStudioGlobal::setFrameManipulator(void *ptr) {
	frameManipulatorPtr = ptr;
}

void *DeepStudioGlobal::getFrameManipulator() {
	return frameManipulatorPtr;
}

void DeepStudioGlobal::setTimelineWidget(void *ptr) {
	timelineWidgetPtr = ptr;
}

void DeepStudioGlobal::resetSimulation() {
	((DfxTimelineDock*)timelineWidgetPtr)->getTimelineWidget()->resetSimulationDomain();
}

void DeepStudioGlobal::showHelpBrowser() {

	QFile file("documentation\\doc_fx.chm");

	if (file.exists()) {

		bool success = QDesktopServices::openUrl(QUrl::fromLocalFile("documentation\\doc_fx.chm"));

		if (!success)
		{
			QMessageBox msgBox;
			msgBox.setStyleSheet("QLabel{min-width: 500px; }");
			msgBox.setText("CHM File Support");
			msgBox.setInformativeText("Failed to open 'documentation\\doc_fx.chm', file not supported.");
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.setDefaultButton(QMessageBox::Ok);
			msgBox.exec();
		}
	}
	else {
		QMessageBox msgBox;
		msgBox.setStyleSheet("QLabel{min-width: 500px; }");
		msgBox.setText("Help files not found");
		msgBox.setInformativeText("Deep FX Studio Failed to find 'documentation\\doc_fx.chm'. Use Online documentation.");
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();
	}
}

DfxSystemIDBase *DeepStudioGlobal::getSystemID() {
	return systemID;
}

void DeepStudioGlobal::showAboutDialog() {
	QDialog dialog(nullptr, Qt::WindowCloseButtonHint);

	QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
	QTextBrowser *browser = new QTextBrowser();

	QLabel *banner = new QLabel("");
	QPixmap img (":/misc/about_banner");
	banner->setPixmap(img);

	std::stringstream ss;
	ss << "<h4> Version: " << V_MAJOR << "." << V_MINOR << "." << V_FIX << "</h4>" << std::endl;
	ss << "<h4> Deep Fx Copyright 2015-2019 All Rights Reserved" << "</h4>" << std::endl;
	ss << "<h3> System ID: <span style='color:green;'>" << systemID->getSystemID() << "</span></h3>" << std::endl;

	browser->setHtml(ss.str().c_str());

	mainLayout->setMargin(0);
	mainLayout->addWidget(banner);
	mainLayout->addWidget(browser);

	dialog.setWindowTitle("About");
	dialog.setMinimumWidth(400);

	dialog.setLayout(mainLayout);

	dialog.exec();
}

void DeepStudioGlobal::selectLayerRoot() {

	auto layers = (DfxLayersEditorDock*)layerDock;

	layers->getView()->resetLayerSelection();
}

std::string DeepStudioGlobal::verStr() {
	return std::to_string(V_MAJOR) + "." + std::to_string(V_MINOR) + "." + std::to_string(V_FIX);
}

void DeepStudioGlobal::initShortcutActions() {
	
	DfxShortcutManager::getInstance()->addShortcutAction("viewport/shading/solid", "Solid Shading", "Viewport", 
		DfxCoreApplication::getInstance(), QKeySequence(Qt::Key_1), DeepStudioShortcutFunctions::activeViewportSolidMode);

	DfxShortcutManager::getInstance()->addShortcutAction("viewport/shading/wireframe", "Wireframe", "Viewport",
		DfxCoreApplication::getInstance(), QKeySequence(Qt::Key_2), DeepStudioShortcutFunctions::activeViewportWireframeMode);

	DfxShortcutManager::getInstance()->addShortcutAction("viewport/selection/hideSelection", "Hide Selection", "Viewport", DfxCoreApplication::getInstance(),QKeySequence(Qt::CTRL + Qt::Key_H), DeepStudioShortcutFunctions::activeViewportHideSelected);

	DfxShortcutManager::getInstance()->addShortcutAction("viewport/selection/hideUnselected", "Hide Unselected", "Viewport", DfxCoreApplication::getInstance(), QKeySequence(Qt::ALT + Qt::Key_H),DeepStudioShortcutFunctions::activeViewportHideUnselected);

	DfxShortcutManager::getInstance()->addShortcutAction("viewport/selection/hideAll", "Hide All", "Viewport", 
		DfxCoreApplication::getInstance(), QKeySequence(Qt::SHIFT + Qt::Key_H), DeepStudioShortcutFunctions::activeViewportHideAll);

	DfxShortcutManager::getInstance()->addShortcutAction("viewport/selection/unhideAll", "Unhide All", "Viewport", 
		DfxCoreApplication::getInstance(), QKeySequence(Qt::SHIFT + Qt::Key_U), DeepStudioShortcutFunctions::activeViewportUnHideAll);
}

void DeepStudioGlobal::bindShortcuts() {

	QVariant rc;

	QSettings settings;
	//settings.clear();

	rc = settings.value("viewport/navigation/pan", "");
	if (rc.value<QString>().length() <= 0) {
		DfxIntegerTyple tup(Qt::Key_Alt, Qt::MiddleButton);
		settings.setValue("viewport/navigation/pan", QString(tup.toString().c_str()));
		DfxShortcutManager::getInstance()->setViewportPanSequence(std::pair<int, int>(Qt::Key_Alt, Qt::MiddleButton));
	}
	else {
		DfxIntegerTyple tup(rc.value<QString>().toStdString());
		DfxShortcutManager::getInstance()->setViewportPanSequence(std::pair<int, int>(tup.get(0), tup.get(1)));
	}

	rc = settings.value("viewport/navigation/rotate", "");
	if (rc.value<QString>().length() <= 0) {
		DfxIntegerTyple tup(Qt::Key_Alt, Qt::LeftButton);
		settings.setValue("viewport/navigation/rotate", QString(tup.toString().c_str()));
		DfxShortcutManager::getInstance()->setViewportRotateSequence(std::pair<int, int>(Qt::Key_Alt, Qt::LeftButton));
	}
	else {
		DfxIntegerTyple tup(rc.value<QString>().toStdString());
		DfxShortcutManager::getInstance()->setViewportRotateSequence(std::pair<int, int>(tup.get(0), tup.get(1)));
	}

	rc = settings.value("viewport/navigation/zoom", "");
	if (rc.value<QString>().length() <= 0) {
		DfxIntegerTyple tup(Qt::Key_Alt, Qt::RightButton);
		settings.setValue("viewport/navigation/zoom", QString(tup.toString().c_str()));
		DfxShortcutManager::getInstance()->setViewportZoomSequence(std::pair<int, int>(Qt::Key_Alt, Qt::RightButton));
	}
	else {
		DfxIntegerTyple tup(rc.value<QString>().toStdString());
		DfxShortcutManager::getInstance()->setViewportZoomSequence(std::pair<int, int>(tup.get(0), tup.get(1)));
	}

	for (auto action : DfxShortcutManager::getInstance()->getAllShortcutActions()) {
		QString key(action->getName().c_str());

		rc = settings.value(key, "");

		if (rc.value<QString>().length() <= 0) {
			auto dseq = action->getDefualtSequence();

			if (dseq.count() == 1) {
				DfxIntegerTyple tup(dseq[0], -1);
				settings.setValue(key, tup.toString().c_str());

				QKeySequence seq(tup.get(0));
				action->updateShorcut(seq);
			}
			else {
				DfxIntegerTyple tup(dseq[0], dseq[1]);
				settings.setValue(key, tup.toString().c_str());

				QKeySequence seq(tup.get(0) + tup.get(1));
				action->updateShorcut(seq);
			}
		}
		else {
			DfxIntegerTyple tup(rc.value<QString>().toStdString());
			if (tup.get(1) != -1) {
				QKeySequence seq(tup.get(0) + tup.get(1));
				action->updateShorcut(seq);
			}
			else {
				QKeySequence seq(tup.get(0));
				action->updateShorcut(seq);
			}
		}
	}

}

void DeepStudioGlobal::updateLayerManager() {
	((DfxLayersEditorDock*)layerDock)->getView()->updateTree();
}

void DeepStudioGlobal::setRenderPropertiesDock(QDockWidget *dock) {
	renderPropertiesDock = dock;
}

void DeepStudioGlobal::showRenderPropertiesDock() {
	if (renderPropertiesDock->isHidden()) renderPropertiesDock->show();
}