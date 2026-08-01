/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include "DfxToolbarMenu.h"

DfxToolbarMenu::DfxToolbarMenu(QWidget *parent) {
	_menu = new QMenu;
	setMenu(_menu);

	QString buttonStyle = "QPushButton { border: 0px; background: rgb(102, 102, 102); } ";
	QString buttonStyleHover = "QPushButton:hover:!pressed  { border: 0px; background: rgb(150, 150, 150); } ";
	QString buttonStylePressed = "QPushButton:hover:pressed  { border: 0px;  } ";
	setStyleSheet(buttonStyle + buttonStyleHover + buttonStylePressed);
}

void DfxToolbarMenu::addItem(QAction *action) {
	_menu->addAction(action);
}