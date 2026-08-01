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

#include "DfxQMenuEditor.h"


QMenu *DfxQMenuEditor::getMenu(QString name, QMenu *menu) {

	auto children = menu->actions();
	for (auto action : children) {

		if (action->isSeparator()) continue;

		if(action->menu())
			if (name.compare(action->menu()->title(), Qt::CaseSensitivity::CaseInsensitive) == 0)
				return action->menu();

	}

	return nullptr;
}

QMenu *DfxQMenuEditor::getMenu(QString name, QMenuBar *menuBar) {

	auto children = menuBar->actions();
	for (auto action: children) {

		if (action->isSeparator()) continue;
	
		if (action->menu())
			if (name.compare(action->menu()->title(), Qt::CaseSensitivity::CaseInsensitive) == 0)
				return action->menu();
		
	}

	return nullptr;
}

void DfxQMenuEditor::addAction(QMenu *menu, QString path, EntryAction act, bool addSeperator) {
	QStringList paths = path.split("/");

	if(paths.size() > 0){ 
		QMenu *currentMenu = menu;
		for (int inx = 0; inx < paths.size(); inx++) {

			QMenu *nextMenu = getMenu(paths[inx], currentMenu);
			if (inx == (paths.size() - 1)) {
				if (addSeperator) {
					currentMenu->addSeparator();
				}
				else if (nextMenu == nullptr) {
					
					QAction *new_action = new QAction(paths[inx]);
					currentMenu->addAction(new_action);
					QObject::connect(new_action, &QAction::triggered, act);
				}
			}
			else {
				if (nextMenu != nullptr) {
					currentMenu = nextMenu;
				}
				else {
					auto tempMenu = new QMenu(paths[inx]);
					tempMenu->setStyleSheet("QMenu::separator { background-color: rgb(50, 50, 50) }");
					currentMenu->addMenu(tempMenu);
					currentMenu = tempMenu;
				}
			}
		}
	}
}

void DfxQMenuEditor::addAction(QMenuBar *menubar, QString path, EntryAction act, bool addSeperator) {
	QStringList paths = path.split("/");

	if (paths.size() > 0) {

		QMenu *currentMenu = getMenu(paths[0], menubar);

		if (currentMenu == nullptr) {
			auto tempMenu = new QMenu(paths[0]);
			tempMenu->setStyleSheet("QMenu::separator { background-color: rgb(50, 50, 50); }");
			menubar->addMenu(tempMenu);
			currentMenu = (tempMenu);
		}

		for (int inx = 1; inx < paths.size(); inx++) {

			QMenu *nextMenu = getMenu(paths[inx], currentMenu);
			if (inx == (paths.size() - 1)) {

				if (addSeperator) {
					currentMenu->addSeparator();
				}
				else if (nextMenu == nullptr) {
					QAction *new_action = new QAction(paths[inx]);
					currentMenu->addAction(new_action);
					QObject::connect(new_action, &QAction::triggered, act);
				}
			}
			else {
				if (nextMenu != nullptr) {
					currentMenu = nextMenu;
				}
				else {
					auto tempMenu = new QMenu(paths[inx]);
					tempMenu->setStyleSheet("QMenu::separator { background-color: rgb(50, 50, 50); }");
					currentMenu->addMenu(tempMenu);
					currentMenu = tempMenu;
				}
			}
		}
		
	}
}

void DfxQMenuEditor::addMenu(QMenuBar *menubar, QMenu *submenu, QString path, bool addSeperator) {
	QStringList paths = path.split("/");

	if (paths.size() > 0) {

		QMenu *currentMenu = getMenu(paths[0], menubar);

		if (currentMenu == nullptr) {
			auto tempMenu = new QMenu(paths[0]);
			tempMenu->setStyleSheet("QMenu::separator { background-color: rgb(50, 50, 50); }");
			menubar->addMenu(tempMenu);
			currentMenu = (tempMenu);
		}

		for (int inx = 1; inx < paths.size(); inx++) {

			QMenu *nextMenu = getMenu(paths[inx], currentMenu);
			if (inx == (paths.size() - 1)) {

				if (addSeperator) {
					currentMenu->addSeparator();
				}
				else if (nextMenu == nullptr) {
					currentMenu->addMenu(submenu);
				}
			}
			else {
				if (nextMenu != nullptr) {
					currentMenu = nextMenu;
				}
				else {
					auto tempMenu = new QMenu(paths[inx]);
					tempMenu->setStyleSheet("QMenu::separator { background-color: rgb(50, 50, 50); }");
					currentMenu->addMenu(tempMenu);
					currentMenu = tempMenu;
				}
			}
		}

	}
}