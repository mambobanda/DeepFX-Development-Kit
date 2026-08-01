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

#include <QObject>
#include <QApplication>
#include <QEvent>
#include <DfxGlobal.h>
#include <DfxTimelineChannelWidget.h>
#include <DfxTimelineKeyTrackOverlay.h>

class DfxTimelineTrackEventFilter: public QObject {
	Q_OBJECT
public:
	DfxTimelineKeyTrackOverlay *overlay;
	std::vector<DfxTimelineChannelWidget*> *_selectedChannels;
protected:
	bool eventFilter(QObject *obj, QEvent *event);
};