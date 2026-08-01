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

#include <glm/vec2.hpp>
#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <DfxGlobal.h>
#include <QMouseEvent>
#include <QCursor>
#include <QScrollBar>
#include <set>
#include "DfxTimelineChannelWidget.h"

class DfxTimelineKeyTrackOverlay : public QWidget {
private:
	enum { MOVE_SELECTED=100, SELECTION_MARQUEE };

	bool mouseButtonPressed;
	bool mouseMoved;
	Qt::MouseButton mouseButton;
	glm::ivec2 mouseOnPressPosition;
	glm::ivec2 mouseLastPosition;
	glm::ivec2 mouseOnMovePosition;
	glm::ivec2 mouseOnReleasePosition;

	glm::ivec2 globalMin;
	glm::ivec2 globalMax;

	bool shiftBtnDown;
	bool controlBtnDown;

	DfxTimelineChannelWidget *focusedChannel;
	glm::ivec2 focusedChannelPosition;
	glm::ivec2 lastMovePosition;

	int mode;
public:
	QWidget *_parent;
	std::vector<DfxTimelineChannelWidget*> *_selectedChannels;
	QScrollBar *vbar;
	QScrollBar *hbar;
public:
	DfxTimelineKeyTrackOverlay(QWidget *parent = nullptr);

	void mouseMoveEventProxy(QMouseEvent *event);
	void mousePressEventProxy(QMouseEvent *event);
	void mouseReleaseEventProxy(QMouseEvent *event);
	void eventProxy(QEvent *);

protected:
	void paintEvent(QPaintEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

public slots:
	void shiftPressed();
	void shiftReleased();
	void controlPressed();
	void controlReleased();
};
