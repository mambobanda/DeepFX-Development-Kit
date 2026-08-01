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
#include <DfxGlobal.h>
#include <QMouseEvent>
#include <FlObserver.h>
#include "DfxTimelineFrameIndicator.h"
#include "DfxTimelineUiConstants.h"
#include "DfxSpace.h"
#include "DfxTimelineManager.h"


class DfxTimelineHeaderWidget : public QWidget {
private:

	class CurrentFrameObserver : public fl::Observer {
	public:
		DfxTimelineHeaderWidget *headerPtr;
		void update(fl::Any *any) override;
	};

private:
	int _minFrameScale;

	int _trackWidth;
	int _trackHeight;

	DfxTimelineFrameIndicator *frameIndicatorPtr;

	CurrentFrameObserver * _currentFrameObserver;

	void updateIndicator();
	void drawSimulationState(QPainter &painter, int stepSize);

	int  mousePressframe;
	bool _mousePressed;
	bool _mouseMoved;
public:

	DfxTimelineHeaderWidget(int fs, QWidget *parent = nullptr);
	~DfxTimelineHeaderWidget();

	void setMinFrameScale(int);
	int minScaleFrame();

	void setTrackWidth(int w);
	int trackWidth() const;

	void setTrackHeight(int h);
	int trackHeight() const;

	int length() const;

	void setFrameIndicator(DfxTimelineFrameIndicator *indicator);

	QSize sizeHint() const override;

public slots:
	void updateFrameScale(int value);
	void updateHeader();

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
};