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
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/


#include "DfxQDialogWindow.h"


DfxQDialogWindow::DfxQDialogWindow(QWidget *parent, Qt::WindowFlags flags): QMainWindow(parent, flags){

}

DfxQDialogWindow::~DfxQDialogWindow() {

}

void DfxQDialogWindow::closeEvent(QCloseEvent *event) {
	deleteLater();
	QMainWindow::closeEvent(event);
}