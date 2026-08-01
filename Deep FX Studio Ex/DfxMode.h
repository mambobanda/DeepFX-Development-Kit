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

#pragma once

#include <QObject>


class DfxMode: public QObject {
	Q_OBJECT
public:
	enum class Mode{ OBJECT_EDIT, VERTEX_EDIT, EDGE_EDIT, FACE_EDIT };
	enum class AnimationMode { KEYFRAME, SIMULATION };
	enum class PlayMode { IDLE, PLAYING };
	enum class TimelineMode { NONE, UPDATING };
private:

	static DfxMode *_instance;

	Mode _mode;
	AnimationMode _animationMode;
	PlayMode _playMode;
	TimelineMode _timelineMode;

	DfxMode();
public:
	virtual ~DfxMode();

	static DfxMode *getInstance();

	DfxMode::Mode mode();
	void setMode(DfxMode::Mode m);

	DfxMode::AnimationMode animationMode();
	void setAnimationMode(DfxMode::AnimationMode animMode);

	DfxMode::PlayMode playMode();
	void setPlayMode(DfxMode::PlayMode playMode);

	DfxMode::TimelineMode timelineMode();
	void setTimelineMode(DfxMode::TimelineMode tMode);

signals:
	void modeChanged();
	void animationModeChanged();
	void playModeChanged();
	void timelineModeChanged();
};