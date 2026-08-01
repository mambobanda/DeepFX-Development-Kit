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

#pragma once

#include <QWidget>
#include <QPainter>
#include <QBoxLayout>
#include <Qlabel>
#include <QCheckBox>
#include <QPushButton>
#include <vector>
#include "DfxTimelineCommon.h"
#include "DfxTimelineTrackWidget.h"
#include "DfxTimelineUiConstants.h"

class DfxTimelineChannelWidget : public QWidget {
private:

	QBoxLayout *mainlayout;
	QBoxLayout *layout;

	QWidget *controlWidget;

	QPushButton *treeHandle;

	QLabel *label;
	QCheckBox *check;

	bool collapsed;
private:

	DfxTimelineChannelWidget *_parent;
	
	std::vector<DfxTimelineChannelWidget*> _children;

	int _level;

	int ancestors();

	DfxTimelineTrackWidget *_track;

public:
	QBoxLayout *channelLayoutPtr;
	QBoxLayout *trackLayoutPtr;
public:

	DfxTimelineChannelWidget(int lvl, QWidget *parent = nullptr);
	~DfxTimelineChannelWidget();

	DfxTimelineChannelWidget *channelParent();
	void setChannelParent(DfxTimelineChannelWidget *);

	DfxTimelineTrackWidget *track();
	void setTrack(DfxTimelineTrackWidget *);

	void addChildChannel(DfxTimelineChannelWidget *);
	DfxTimelineChannelWidget *getChildChannel(int inx);

	std::vector<DfxTimelineChannelWidget*> channelChildren();

	void setText(QString str);

	void setRelevantPaths(std::vector<std::string> );

	void hideChannel(bool);

	void hideTrack(bool);

protected:
	void paintEvent(QPaintEvent *event);

public slots:
	void expandCollapse();
};