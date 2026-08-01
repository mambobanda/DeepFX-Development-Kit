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
#include <QBoxLayout>
#include <QSplitter>
#include <QSCrollArea>
#include <QScrollBar>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QSlider>
#include "DfxTimelineFrameIndicator.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxTimelineHeaderWidget.h"
#include "DfxTimelineTrackWidget.h"
#include "DfxTimelineChannelWidget.h"
#include "DfxQStyleScrollBar.h"
#include "DfxTimelineScrollEventFilter.h"
#include "DfxQSceneManager.h"
#include "DfxPropertyConduit.h"
#include "DfxKeyableTypes.h"
#include "DfxQSceneManager.h"
#include "DfxMode.h"
#include "DfxSceneSelectionModel.h"
#include "DfxTimelineKeyTrackOverlay.h"
#include "DfxTimelineTrackEventFilter.h"
#include "DfxTimelineManager.h"

class DfxTimelineEditor: public QWidget {
	Q_OBJECT
private:
	class TimeObserver : public fl::Observer {
	public:
		DfxTimelineEditor *ptr;
		TimeObserver();
		void update(fl::Any *any) override;
	};

private:
	TimeObserver *_timeobserverFrameStart;
	TimeObserver *_timeobserverFrameEnd;


	DfxTimelineTrackEventFilter *trackEventFilter;
	std::vector<DfxTimelineChannelWidget*> _selectedChannels;
public:

	QBoxLayout *button_menu_layout;
	QBoxLayout *button_menu_left_layout;
	QBoxLayout *button_menu_center_layout;
	QBoxLayout *button_menu_right_layout;

	QPushButton *realtimeMode;
	QPushButton *recordCache;
	QPushButton *resetSim;

	QPushButton *play;
	QPushButton *stop;
	QPushButton *nextFrame;
	QPushButton *previousFrame;
	QPushButton *nextKey;
	QPushButton *previousKey;
	QPushButton *start;
	QPushButton *end;
	QPushButton *createKey;
	QPushButton *deleteKey;
	QPushButton *keyMenu;

	QActionGroup *keyablesGroup;
	QMenu *keyableProperties;
	QAction *locRotScl;
	QAction *loc;
	QAction *rot;
	QAction *scl;

	DfxQIntegerSpinnerControl *current_frame;
	DfxQIntegerSpinnerControl *start_frame;
	DfxQIntegerSpinnerControl *end_frame;

public:

	QScrollArea *main_scroll;
	QScrollArea *keyHeader_scroll;
	QScrollArea *key_scroll;
	QScrollArea *channel_scroll;

	QBoxLayout *mainLayout;

	QBoxLayout *keyEditorLayout;
	QBoxLayout *headerKeyTrackLayout;
	QBoxLayout *keyTrackLayout;
	QBoxLayout *centralWidgetLayout;
	QBoxLayout *framescaleHBarLayout;

	QWidget *keyHeaderWidget;
	QWidget *keyTrackWidget;
	QWidget *centralWidget;
	QWidget *keyEditorWidget;
	QWidget *headerKeyTrackWidget;
	QWidget *channelWidget;
	QWidget *buttonMenuWidget;

	QSlider *framescaleSlider;

	QScrollBar *keyTrackHScrollbar;
	QScrollBar *channelKeyTrackVScrollbar;

	QBoxLayout *channelLayout;

	DfxTimelineKeyTrackOverlay *trackOverlay;

	QSplitter *main_splitter;

	DfxTimelineHeaderWidget *keyHeader;

	DfxTimelineScrollEventFilter *scrollAreaEventFilter;

	DfxTimelineFrameIndicator *frameIndicator;
	bool ready;
public:
	DfxTimelineEditor(QWidget *parent = nullptr);
	virtual ~DfxTimelineEditor();

private:
	void stylePlayButtons();
	std::vector<std::string> findKeyFramePaths(fl::Any *any, int level);
	void importDataObject(fl::Any *any, int level, std::vector<std::string> &keyframes, DfxTimelineChannelWidget *parent = nullptr);

	int frameScale;

	DfxTimelineChannelWidget *newChannel(int lvl, std::string path = "");
	void clearChannels();

signals:
	void frameScaleChanged(int scale);
	void shiftPressed();
	void shiftReleased();
	void controlPressed();
	void controlReleased();
public slots:
	void _goToNextFrame();
	void _goToPreviousFrame();
	void _goToFirstFrame();
	void _goToLastFrame();

	void splitterMoved(int pos, int index);
	void key_hscrollRangeChanged(int min, int max);
	void key_vscrollRangeChanged(int min, int max);

	void createKeyAction();
	void deleteKeyAction();
	void setPosRotSclMode();
	void setPosMode();
	void setRotMode();
	void setSclMode();

	void refreshModel();

	void frameScaleSliderChanged(int value);

	void hideShowNoneRealtimeControls();

	void toggleRealtimeMode(bool checked);
protected:
	void resizeEvent(QResizeEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	void enterEvent(QEvent *event) override;
};

class CustomScrollArea : public QScrollArea {
private:
	DfxTimelineEditor *editorPtr;
public:
	CustomScrollArea(DfxTimelineEditor *editor);
protected:
	void resizeEvent(QResizeEvent *event);
};