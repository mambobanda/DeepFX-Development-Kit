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

#pragma

#include <QBoxlayout.h>
#include <QScrollArea>
#include <QPushButton.h>
#include <QLineEdit>
#include <QWidget.h>
#include <QPainter.h>
#include <QIntValidator>
#include <DfxGlobal.h>
#include <QtConcurrent\QTConcurrentRun>
#include <DfxRoot.h>
#include "DeepStudioGlobal.h"
#include "DfxTime.h"
#include "DfxEnvelopeMap.h"
#include "DfxSceneGraph.h"
#include "DfxFrameManipulatorArea.h"
#include "DfxFrameManipulator.h"

class DfxTimelineWidget: public QWidget {
private:

	static DfxTimelineWidget *instance;

	DfxTimelineWidget(QWidget *parent = Q_NULLPTR);

protected:
	void paintEvent(QPaintEvent *event) override;
	int fixedHeight;

	DfxFrameManipulatorArea *frameManip;

	QBoxLayout *buttonLayout;

	QPushButton *play;
	QPushButton *nextFrame;
	QPushButton *previousFrame;
	QPushButton *nextKey;
	QPushButton *previousKey;
	QPushButton *start;
	QPushButton *end;

	QLineEdit *currentFrame;
	QLineEdit *frameCount;

	QPushButton *createKey;
	QPushButton *removeKey;
	QPushButton *autoKey;
	QPushButton *sim;
	QPushButton *reset;

	static bool playActive;
	static bool inProcess;

	QFuture<void> playFuture;

	static void playFunction( DfxTimelineWidget *pf );
public:

	static DfxTimelineWidget *getInstance();

	virtual QSize sizeHint() const override;

	int getFrameCount();

	void setFrameCount(int fc);

	int getCurrentFrame();

	void setCurrentFrame(int cf);
	
	bool playIsChecked();

	void setCorrectSimulationFrame();

	void nextFrame_();

	void previousFrame_();

	void nextSimulationFrame();

	void lastSimulationFrame();

	void stepSimulation(DfxEntity *ent);

	void resetSimulationDomain();

	DfxFrameManipulator *getFrameManipulator();

public slots:
	void updateCurrentFrameEdit();
	void updateFrameCountEdit();
	void updateCurrentFrameManipulator();
	void updateSimulationStateButton(bool st);
	void playClickedAction(bool st);
	void lastFrameClickedAction(bool);
	void firstFrameClickedAction(bool);
	void nextFrameClickedAction(bool);
	void previousFrameClickedAction(bool);
	void resetClickedAction(bool);
	void createKeyClickedAction(bool);
	void deleteKeyClickedAction(bool);
};