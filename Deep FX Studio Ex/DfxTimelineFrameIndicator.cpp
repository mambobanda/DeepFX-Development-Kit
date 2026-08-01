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

#include "DfxTimelineFrameIndicator.h"

DfxTimelineFrameIndicator::DfxTimelineFrameIndicator(QWidget *parent): QWidget(parent) {
	_x = 0;
	setStyleSheet("background-color:transparent;");
}

DfxTimelineFrameIndicator::~DfxTimelineFrameIndicator() {

}

void DfxTimelineFrameIndicator::paintEvent(QPaintEvent *event) {

	QPainter painter(this);

	int width  = ((QWidget*)parent())->width();
	int height = ((QWidget*)parent())->height();

	QPen pen(QColor(255, 153, 83, 30));
	pen.setWidth(3);

	painter.setBrush(QBrush(QColor(255, 153, 83, 30)));
	painter.setPen(pen);
	painter.drawRect(_x - 1, 0, 2, height);
	//painter.drawLine(_x, 0, _x, height);
	
}

int DfxTimelineFrameIndicator::x() {
	return _x;
}

void DfxTimelineFrameIndicator::setX(int val) {
	_x = val;
}
