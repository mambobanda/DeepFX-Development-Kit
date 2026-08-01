/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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
#include <vector>
#include <FlObserver.h>
#include "DfxGraphEditorCurve.h"

class DfxChannelGraphView : public QWidget {
	Q_OBJECT
private:

	class OnClickInfo {
	public:
		enum { NONE=0, ANCHOR_SEL=1, CP1_SEL=2, CP2_SEL=3 };
		int selectionType;
		DfxGraphEditorControlPoint *cpoint;
	};

	class TimeObserver : public fl::Observer {
	public:
		DfxChannelGraphView *ptr;
		TimeObserver(DfxChannelGraphView *ptr);
		void update(fl::Any *any) override;
	};

private:

	int _legendBorder;
	int _cv_point_size;
	int _bar_h;
	int _fontSize;
	int zoomFactorVert;
	int zoomPowerVert; 
	int zoomFactorHoriz;
	int zoomPowerHoriz;
	int first_frame_offset;
	int frame_offsetX;
	int frame_offsetY;
	int line_spacing_x;
	int line_spacing_y;
	int min_spacing;
	int max_spacing;
	int last_spacing;
	int origin_frame_vert;
	int canvas_width;
	int canvas_height;
	bool initialized;

	bool rmbDown;

	int _mode;

	QPoint mmbDownPos;
	int frame_offsetXDown;
	int frame_offsetYDown;
	bool mmbDown;
	
	bool lmbDown;
	QPoint lmbDownPos;
	QPoint lmbDownPosOnMove;
	OnClickInfo lmbClickInfo;
	
	bool shiftDown;

	bool currentFrameUpdate;
	bool mouseMoved;
	bool selectionMarqueeActive;

	bool ignoreChannleUpdate;

	std::vector<DfxGraphEditorCurve> _curves;

	TimeObserver *_currentFrameObserver;
private:

	QFont textFont();
	int lineSpacing(int &zoomFactor, int &zoomPower, int canvas_width, int frames, int min_spacing, int max_spacing);
	float nextZoomFactor(int &factor, int &power, int direction);
	QString removeTrailingZeros(QString input);
	void drawCurves(QPainter &painter, int segments);
	void drawBeizerCurve(QPainter &painter, DfxGraphEditorControlPoint &p1, DfxGraphEditorControlPoint &p2, int segments);
	void drawLinearCurve(QPainter &painter, DfxGraphEditorControlPoint &p1, DfxGraphEditorControlPoint &p2);
	void drawConstantCurve(QPainter &painter, DfxGraphEditorControlPoint &p1, DfxGraphEditorControlPoint &p2);
	void drawCurrentFrameIndicator(QPainter &painter);
	void drawSelectionMarquee(QPainter &painter);

	OnClickInfo lmbClickEvent(QMouseEvent *event);

	void clearSelected();
	void clearSelectedTangents();

	void addKey();
	void buildCurveData(DfxGraphEditorCurve &curve);
protected:

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	void enterEvent(QEvent *event) override;

	void paintEvent(QPaintEvent *event) override;

public:

	enum { NONE = 0, SEL_MODE = 1, ADD_KEY_MODE = 2 , REMOVE_KEY_MODE = 3 };

	DfxChannelGraphView();
	virtual ~DfxChannelGraphView();

	int mode();
	void switchMode(int mode);

public slots:
	
	void channelsUpdated();
	void newChannelCreated();
	void updateCurves(std::vector<std::pair<std::string, QColor>> paths);
	void updateInterpolationType(int type);
	void resetTangents();
};