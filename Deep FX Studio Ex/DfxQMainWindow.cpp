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

#include <DfxConsole.h>
#include <QComboBox>
#include <QApplication>
#include "DfxProject.h"
#include "DfxQViewManager.h"

#include "DfxQView.h"
#include "DfxRenderHub.h"

#include "DfxQMainWindow.h"

DfxQMainWindow *DfxQMainWindow::_instance = nullptr;
bool DfxQMainWindow::_isready = false;

DfxQMainWindow::DfxQMainWindow() {

	setWindowTitleInfo();

	setCentralWidget(DfxQViewManager::getInstance()->root());

	setMenuBar(DfxQMainMenu::getInstance());

	DfxViewLayoutManager::findLayoutConfigs();

	layoutWidget = new DfxQLayoutWidget;
	DfxQMainMenu::getInstance()->setCornerWidget(layoutWidget);
	
	QSettings settings;
	auto default_lt = settings.value("~/layout/default_layout", "").toString();

	if (default_lt.isEmpty()) {
		QString appdata = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];
		auto lt = DfxViewLayoutManager::loadLayout((appdata + "/layout_config/default.lt").toStdString());

		if (lt.second != nullptr) {
			DfxViewLayoutManager::switchLayout(*lt.second);
		}
	}
	else {
		DfxViewLayoutManager::switchLayout(default_lt.toStdString());
	}
	
	this->installEventFilter(this);

	setAcceptDrops(false);

	QObject::connect(DfxProject::getInstance(), &DfxProject::projectPathChanged, this, &DfxQMainWindow::setWindowTitleInfo);

	_isready = true;
}

DfxQMainWindow::~DfxQMainWindow() {

}

DfxQMainWindow *DfxQMainWindow::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxQMainWindow;
	}

	return _instance;
}

bool DfxQMainWindow::eventFilter(QObject *obj, QEvent *event) {
	
	if (event->type() == QEvent::KeyPress) {
	
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		
		if(!hasKey(keyEvent->key())){
			// DfxGlobal::console::println("Press: " + std::to_string(keyEvent->key()));

			addKey(keyEvent->key());

			if (DfxShortcutsDB::getInstance()->executeCommand("Application", keysToShortcut())) {
				return true;
			}
		}
		
	}
	else if (event->type() == QEvent::KeyRelease) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

		if (hasKey(keyEvent->key())) {
			// DfxGlobal::console::println("Release: " + std::to_string(keyEvent->key()));
			removeKey(keyEvent->key());
		}
		
	}

	return QMainWindow::eventFilter(obj, event);
}

void DfxQMainWindow::closeEvent(QCloseEvent *event) {
	
	for (int i = 0; i < DfxRenderHub::getInstance()->renderNodeCount(); i++) {
		DfxRenderHub::getInstance()->renderNode(i)->stopRendering();
	}
}

void DfxQMainWindow::setWindowTitleInfo() {
	std::string licence = "";
	if (DeepFxStudioLicenseManager::getState() != DeepFxStudioLicenseManager::NODE_LOCKED_STATE) licence = " TRIAL VERSION ";

	setWindowTitle((DfxVersionNameGlobal::versionNameString() + licence + "  -  " + DfxProject::getInstance()->projectPath()).c_str());
}

void DfxQMainWindow::changeEvent(QEvent *e) {
	if (e->type() == QEvent::WindowStateChange)
	{
		QWindowStateChangeEvent* event = static_cast<QWindowStateChangeEvent*>(e);

		if (event->oldState() & Qt::WindowMinimized)
		{
			DfxGlobal::command::exec("updateScene");
		}
		else if (event->oldState() == Qt::WindowNoState && this->windowState() == Qt::WindowMaximized)
		{
			// DfxGlobal::console::println("Window Maximized!");
		}
	}
}

bool DfxQMainWindow::isReady() {
	return _isready;
}