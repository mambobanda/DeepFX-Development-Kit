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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>

class DfxTimelineFrameIndicator: public QWidget {
private:
	int _x;
public:
	DfxTimelineFrameIndicator(QWidget *parent = nullptr);
	~DfxTimelineFrameIndicator();

	int x();
	void setX(int);
	
protected:

	void paintEvent(QPaintEvent *event);
};