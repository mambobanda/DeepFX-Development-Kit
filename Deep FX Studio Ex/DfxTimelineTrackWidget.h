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

#include <set>
#include <QWidget>
#include <QPainter>
#include <DfxGlobal.h>
#include <unordered_map>
#include <glm/vec2.hpp>
#include "DfxTimelineCommon.h"
#include "DfxPropertyConduit.h"
#include "DfxTimelineUiConstants.h"
#include "DfxTimelineManager.h"
#include "DfxSpace.h"

class DfxTimelineTrackWidget : public QWidget {
private:
	int _minFrameScale;

	int _trackWidth;
	int _trackHeight;

	std::string _keyFramePath;

	std::vector<std::string> _relevantPaths;

	bool _reflect;

	bool _ghostMode;

	int _ghostDisplacement;

	std::unordered_map<int,int> _selected_keys;

	DfxTimelineChannelWidget *_channel;
private: 
	void getAll(DfxTimelineChannelWidget *chan, std::vector<DfxTimelineTrackWidget*> &);
	void paintKeyFrames();

public:

	DfxTimelineTrackWidget(int fs, std::string kpath = "", QWidget *parent = nullptr);
	~DfxTimelineTrackWidget();

	void setMinFrameScale(int);
	int minScaleFrame();

	void setTrackWidth(int w);
	int trackWidth() const;

	void setTrackHeight(int h);
	int trackHeight() const;

	void setReflect(bool);

	void setRelevantPaths(std::vector<std::string> rp);

	std::string keyframePath();

	QSize sizeHint() const override;

	void eventProxy(QEvent *);

	void clearSelection();

	std::vector<int> selectedKeys();

	void addSelectedKey(int k);

	std::vector<int> keys();

	int frameFromPosition(glm::ivec2 pos);

	void setChannel(DfxTimelineChannelWidget *);

	std::vector<DfxTimelineTrackWidget*> findAll();

	std::unordered_map<int, bool> findAllKeys();

	void setGhostMode(bool);

	bool isGhostMode();

	int ghostDisplacement();

	void setGhostDisplacement(int);

	bool alreadySelected(glm::ivec2 pos);

	void moveSelection(int from, int to);

	bool isSelected(int);

	bool selectKeyFrame(glm::ivec2 pos, int &key, bool toggle = false);

	bool selectKeyFrames(glm::ivec2 min, glm::ivec2 max, std::vector<int> &keys);
public slots:
	void updateFrameScale(int value);
	void channelUpdated();
	void keyframeUpdated(std::string);

protected:
	void paintEvent(QPaintEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
};