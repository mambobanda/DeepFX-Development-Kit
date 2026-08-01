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

#include "DfxFrameManipulatorArea.h"


DfxFrameManipulatorArea::DfxFrameManipulatorArea(int fHeight, QWidget *parent) {

	fixedHeight = fHeight;

	frameManipulator = new DfxFrameManipulator(fHeight);

	setWidget(frameManipulator);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setFixedHeight(fixedHeight + 20);

	frameManipulator->setCanvasWidth(size().width());

}

void DfxFrameManipulatorArea::resizeEvent(QResizeEvent *event) {
	frameManipulator->setCanvasWidth(size().width());
}

DfxFrameManipulator *DfxFrameManipulatorArea::getFrameManipulator() {
	return frameManipulator;
}