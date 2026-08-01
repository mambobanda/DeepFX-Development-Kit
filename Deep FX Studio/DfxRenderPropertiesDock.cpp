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

#include "DfxRenderPropertiesDock.h"

DfxRenderPropertiesDock::DfxRenderPropertiesDock(QWidget *parent) : QDockWidget("Render Properties", parent) {

	view = new DfxQRenderProperties;

	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setWidget(view);

}

DfxQRenderProperties *DfxRenderPropertiesDock::getView() {
	return view;
}