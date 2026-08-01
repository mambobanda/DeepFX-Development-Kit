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

#include <DfxGlobal.h>
#include "DfxQMainMenu.h"

DfxQMainMenu *DfxQMainMenu::_instance = nullptr;

DfxQMainMenu::DfxQMainMenu() {
	
}

DfxQMainMenu *DfxQMainMenu::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxQMainMenu;
	}

	return _instance;
}

void DfxQMainMenu::initRecentSceneMenu() {
	recentProjects = new QMenu("Recent Projecs");
	DfxQMenuEditor::addMenu(this, recentProjects, "File/Recent Projects");
	
	initRecentSceneList();
}

void DfxQMainMenu::addAction(QString path, DfxQMenuEditor::EntryAction act, bool addSeparator) {
	DfxQMenuEditor::addAction(this, path, act, addSeparator);
}

void DfxQMainMenu::initRecentSceneList() {
	QSettings settings;

	list = settings.value("main_menu/recentFileList", QStringList()).value<QStringList>();
	
	for (int inx = 0; inx < list.size(); inx++) {

		QAction *new_action = new QAction(list[inx]);
		recentProjects->addAction(new_action);
		QObject::connect(new_action, &QAction::triggered, [this, inx] { DfxGlobal::command::exec("loadProject " + list[inx].toStdString()); });

		recentProjects->addAction(new_action);

	}
}

void DfxQMainMenu::updateRecentOpenedSceneList(std::string path) {
	auto actions = recentProjects->actions();
	for (auto act : actions) recentProjects->removeAction(act);

	QSettings settings;
	list = settings.value("main_menu/recentFileList", QStringList()).value<QStringList>();

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

	settings.setValue("main_menu/recentFileList", list);

	initRecentSceneList();
}