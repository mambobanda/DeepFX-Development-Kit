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



#include "DfxPlayBlastPlayer.h"
#include "DfxVideoItem.h"


#include <QtMultimedia/QtMultimedia>

#ifndef QT_NO_OPENGL
# include <QtOpenGL/QGLWidget>
#endif

DfxPlayBlastPlayer::DfxPlayBlastPlayer(QWidget *parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
	, videoItem(0)
	, playButton(0)
	, positionSlider(0)
{

	this->setWindowTitle("Play Blast");

	blastIsPlaying = false;

	idle_tag = false;

	idle_func_timer = nullptr;

	videoItem = new DfxVideoItem;

	current_frame = 0;
	frame_count = 0;

	scene = new QGraphicsScene(this);
	graphicsView = new QGraphicsView(scene);

#ifndef QT_NO_OPENGL
	graphicsView->setViewport(new QGLWidget);
#endif

	scene->addItem(videoItem);

	playButton = new QPushButton;
	stopButton = new QPushButton;

	playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

	fpsLabel = new QLabel("FPS:");
	fps = new QSpinBox();

	fps->setValue(60);
	fps->setMinimum(1);
	fps->setMaximum(999);

	mainMenu = new QMenuBar();
	file = new QMenu("File");
	savePlayBlast = new QAction("Save Playblast Sequence");

	mainMenu->addMenu(file);
	file->addAction(savePlayBlast);

	positionSlider = new QSlider(Qt::Horizontal);
	positionSlider->setRange(0, 0);

	QBoxLayout *controlLayout = new QHBoxLayout;
	controlLayout->setMargin(0);
	controlLayout->addWidget(playButton);
	controlLayout->addWidget(stopButton);
	controlLayout->addWidget(positionSlider);
	controlLayout->addWidget(fpsLabel);
	controlLayout->addWidget(fps);

	QBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(graphicsView);
	layout->addLayout(controlLayout);

	layout->setMenuBar(mainMenu);
	setLayout(layout);

	QObject::connect(playButton, &QPushButton::clicked, this, &DfxPlayBlastPlayer::play);
	QObject::connect(stopButton, &QPushButton::clicked, this, &DfxPlayBlastPlayer::stop);

	QObject::connect(fps, qOverload<int>(&QSpinBox::valueChanged), this, &DfxPlayBlastPlayer::fpsChanged);

	QObject::connect(positionSlider, &QSlider::valueChanged, this, &DfxPlayBlastPlayer::sliderChanged);

	QObject::connect(savePlayBlast, &QAction::triggered, this, &DfxPlayBlastPlayer::saveImages);
}

DfxPlayBlastPlayer::~DfxPlayBlastPlayer()
{
}


void DfxPlayBlastPlayer::play()
{
	if (imageList.size() > 0) {
		if (idle_func_timer == nullptr) {

			idle_func_timer = new QTimer();
			QObject::connect(idle_func_timer, &QTimer::timeout, this, &DfxPlayBlastPlayer::idle_func);

			int milli = (1.f / (float)fps->value()) * 1000.f;
			idle_func_timer->start(milli);

			current_frame = 1;
		}
	}
}

void DfxPlayBlastPlayer::stop()
{
	if (imageList.size() > 0) {
		if (idle_func_timer != nullptr) {
			idle_func_timer->stop();
			delete idle_func_timer;
			idle_func_timer = nullptr;
		}
	}
}


bool DfxPlayBlastPlayer::presentImage(const QImage &image)
{
	QVideoFrame frame(image);

	if (!frame.isValid())
		return false;

	QVideoSurfaceFormat currentFormat = videoItem->surfaceFormat();

	if (frame.pixelFormat() != currentFormat.pixelFormat()
		|| frame.size() != currentFormat.frameSize()) {
		QVideoSurfaceFormat format(frame.size(), frame.pixelFormat());

		if (!videoItem->start(format))
			return false;
	}

	if (!videoItem->present(frame)) {
		videoItem->stop();

		return false;
	}
	else {
		return true;
	}
}

void DfxPlayBlastPlayer::setImageList(std::vector<QImage*> &images) {
	imageList = images;
	frame_count = imageList.size();

	positionSlider->setRange(0, frame_count -  1);
}

void DfxPlayBlastPlayer::idle_func() {

	QApplication::processEvents();

	if (current_frame == imageList.size() + 1) {
		current_frame = 1;
	}

	presentImage(*imageList[current_frame - 1]);
	graphicsView->fitInView(videoItem,Qt::AspectRatioMode::KeepAspectRatio);

	idle_tag = true;

	positionSlider->setValue(current_frame - 1);

	idle_tag = false;

	current_frame++;
}

void DfxPlayBlastPlayer::callIdleFunc() {
	idle_func();
}

bool DfxPlayBlastPlayer::isPlaying() {
	return blastIsPlaying;
}

void DfxPlayBlastPlayer::sliderChanged() {

	if (!idle_tag) {
		stop();


		current_frame = positionSlider->value() + 1;

		presentImage(*imageList[current_frame - 1]);
		graphicsView->fitInView(videoItem, Qt::AspectRatioMode::KeepAspectRatio);
	}
}

void DfxPlayBlastPlayer::fpsChanged(int i) {
	stop();
}

void DfxPlayBlastPlayer::saveImages() {

	QString fileName
		= QFileDialog::getExistingDirectory(nullptr, QString("Save Image Sequence"), "C:/");

	if (fileName.size() > 0 && imageList.size() > 0) {

		QProgressDialog progress("Saving Play Blast...", "Abort", 0, imageList.size(), 0, Qt::WindowCloseButtonHint |Qt::WindowTitleHint);
		progress.setWindowModality(Qt::ApplicationModal);
		progress.setWindowTitle("Saving Play Blast");

		int counter = 1;

		for (auto img : imageList) {
			progress.setValue(counter);

			if (progress.wasCanceled())
				break;

			std::string path = fileName.toStdString() + "/playblast_" + std::to_string(counter) + ".png";
			img->save(path.c_str(), "PNG");

			counter++;
		}

		progress.setValue(imageList.size());
	}
}