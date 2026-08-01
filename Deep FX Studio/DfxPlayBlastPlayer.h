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

#include <QApplication>
#include <QtConcurrent/QtConcurrent>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QProgressDialog>
#include <Qt>
#include <QtGui/QMovie>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDialog>
#include <QSlider>
#include <QTimer>
#include <QLabel>
#include <QSpinBox>
#include <QMenuBar>
#include <QMenu>
#include <QStyle>

class QAbstractButton;
class QAbstractVideoSurface;
class QSlider;

class DfxVideoItem;

class DfxPlayBlastPlayer : public QDialog
{
	Q_OBJECT
public:
	DfxPlayBlastPlayer(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~DfxPlayBlastPlayer();

	QSize sizeHint() const { return QSize(800, 600); }

	void setImageList(std::vector<QImage*> &images);

	void callIdleFunc();
	bool isPlaying();
public slots:
	void stop();
	void play();

	void idle_func();
private slots:
	void sliderChanged();
	void fpsChanged(int i);
	void saveImages();
private:
	bool presentImage(const QImage &image);

	//QMovie movie;
	std::vector<QImage*> imageList;

	DfxVideoItem *videoItem;
	QPushButton *playButton;
	QPushButton *stopButton;
	QSlider *positionSlider;
	QLabel *fpsLabel;
	QSpinBox *fps;
	QMenuBar *mainMenu;
	QMenu *file;
	QAction *savePlayBlast;

	QTimer *idle_func_timer;

	int current_frame;
	int frame_count;

	bool idle_tag;

	bool blastIsPlaying;

	QGraphicsScene *scene;
	QGraphicsView *graphicsView;
};

