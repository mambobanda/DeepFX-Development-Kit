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


#include "DfxRenderViewer.h"


DfxRenderThreadObject::DfxRenderThreadObject(DfxRenderInterface *rint) {
	rinterface = rint;
	fAbort = false;
}

void DfxRenderThreadObject::run(void) {

	QPixmap pmap;

	QTime rTimer;
	rTimer.start();
	
	for (int inx = 1; inx <= rinterface->iterations(); inx++) {

		if (fAbort) {
			emit updateProgress(100);
			rinterface->cleanUp();
			fAbort = false;
			return;
		}

		rinterface->render();

		emit updateRenderTime(rTimer.elapsed(), dfx_render_stats::lastRenderTime);

		int totalDone = ((float)inx / (float)rinterface->iterations()) * 100.f;
		emit updateProgress(totalDone);

		if (inx == 1) {
			pmap.convertFromImage(rinterface->getFinalImage());
			emit update(pmap);
	
		}
		else if ((inx % rinterface->iterationsUpdateTrigger()) == 0) {
			pmap.convertFromImage(rinterface->getFinalImage());
			emit update(pmap);
		
		}
	}

	dfx_render_stats::lastRenderTime = rTimer.elapsed();
	emit updateRenderTime(dfx_render_stats::lastRenderTime, dfx_render_stats::lastRenderTime);

	emit updateProgress(100);

	pmap.convertFromImage(rinterface->getFinalImage());
	emit update(pmap);

	rinterface->cleanUp();

	DfxGlobal::console::println("Radeon Pro Render Done.");
}
void DfxRenderThreadObject::abort() {
	fAbort = true;
}

DfxRenderViewer::DfxRenderViewer() {

	setWindowTitle("Deep FX Studio Render");

	QWidget *cwidget = new QWidget(this);

	mainLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom, this);
	buttonLayout   = new QBoxLayout(QBoxLayout::Direction::LeftToRight, this);
	buttonLayout00 = new QBoxLayout(QBoxLayout::Direction::LeftToRight, this);
	buttonLayout01 = new QBoxLayout(QBoxLayout::Direction::LeftToRight, this);

	mainLayout->setMargin(0);
	buttonLayout->setMargin(4);
	buttonLayout00->setMargin(0);
	buttonLayout01->setMargin(0);
	
	buttonLayout00->setSpacing(2);
	buttonLayout01->setSpacing(2);
	mainLayout->setSpacing(0);

	buttonLayout00->setAlignment(Qt::AlignLeft);
	buttonLayout01->setAlignment(Qt::AlignRight);

	imageArea = new QScrollArea(this);
	image = new QLabel("", this);

	image->setBackgroundRole(QPalette::Base);
	image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	image->setScaledContents(true);

	imageArea->setBackgroundRole(QPalette::Dark);
	imageArea->setWidget(image);

	fileMenuButton = new QPushButton("File", this);
	fileMenuButton->setFixedWidth(55);

	fileMenu	= new QMenu("File", this);
	saveRender  = new QAction("Save Render", this);

	fileMenuButton->setMenu(fileMenu);
	fileMenu->addAction(saveRender);

	zoomButton  = new QPushButton("100%", this);
	zoomButton->setFixedWidth(55);

	zoomMenu	= new QMenu("100%", this);
	zoom10		= new QAction("10%", this);
	zoom20		= new QAction("20%", this);
	zoom30		= new QAction("30%", this);
	zoom40		= new QAction("40%", this);
	zoom50		= new QAction("50%", this);
	zoom80		= new QAction("80%", this);
	zoom100		= new QAction("100%", this);
	zoom200		= new QAction("200%", this);
	zoom300		= new QAction("300%", this);
	zoom400		= new QAction("400%", this);
	zoom500		= new QAction("500%", this);

	zoomButton->setMenu(zoomMenu);
	zoomMenu->addAction(zoom10);
	zoomMenu->addAction(zoom20);
	zoomMenu->addAction(zoom30);
	zoomMenu->addAction(zoom40);
	zoomMenu->addAction(zoom50);
	zoomMenu->addAction(zoom80);
	zoomMenu->addAction(zoom100);
	zoomMenu->addAction(zoom200);
	zoomMenu->addAction(zoom300);
	zoomMenu->addAction(zoom400);
	zoomMenu->addAction(zoom500);

	renderTimeLabel = new QLabel("Time: 00:00:00 | Last: 00:00:00 ");
	renderTimeLabel->setFixedWidth(160);

	progress	= new QProgressBar(this);
	progress->setFixedWidth(100);

	abort		= new QPushButton(QIcon(":/misc/stop_icon"), "", this);
	abort->setIconSize(QSize(15, 15));
	abort->setFixedSize(QSize(21, 21));

	buttonLayout00->addWidget(fileMenuButton);
	buttonLayout00->addWidget(zoomButton);
	buttonLayout01->addWidget(renderTimeLabel);
	buttonLayout01->addWidget(progress);
	buttonLayout01->addWidget(abort);

	buttonLayout->addLayout(buttonLayout00);
	buttonLayout->addLayout(buttonLayout01);

	mainLayout->addLayout(buttonLayout);
	mainLayout->addWidget(imageArea);

	cwidget->setLayout(mainLayout);
	setCentralWidget(cwidget);

	rinterface = nullptr;
	render_thread = nullptr;

	QObject::connect(abort, &QPushButton::clicked, this, &DfxRenderViewer::abortRender);
}

DfxRenderViewer::~DfxRenderViewer() {

}

void DfxRenderViewer::setRenderInterface(DfxRenderInterface *rinter) {
	rinterface = rinter;
}

void DfxRenderViewer::setupRender() {
	if (rinterface != nullptr) {

		std::vector<size_t> uids;
		auto views = DfxLayerManager::getInstance()->findAllViewItems();

		for (auto vit : views) {
			if (vit->isRenderable() ||
				vit->UniqueName().compare("2F2X252X_DFX_CAMERA") == 0 ||
				vit->UniqueName().compare("2F2X252X_DFX_LIGHT") == 0) {

				uids.push_back(vit->getInstanceID());
			}
		}

		rinterface->newFrame(uids);
	}
}

void DfxRenderViewer::render() {

	if (rinterface != nullptr) {

		if (render_thread != nullptr) {
			if (!render_thread->isFinished()) {

				render_thread->quit();
				while (!render_thread->isFinished()) /*wait*/;

			}

			delete render_thread;
			render_thread = nullptr;
		}

		render_thread = new DfxRenderThreadObject(rinterface);

		QObject::connect(render_thread, &DfxRenderThreadObject::update, this, &DfxRenderViewer::updateRenderOutput);
		QObject::connect(render_thread, &DfxRenderThreadObject::updateProgress, this, &DfxRenderViewer::updateProgress);
		QObject::connect(render_thread, &DfxRenderThreadObject::updateRenderTime, this, &DfxRenderViewer::updateTimeStats);

		render_thread->start();
	}
}

void DfxRenderViewer::startRender() {

	if (rinterface != nullptr) {

		progress->reset();
		progress->setMinimum(0);
		progress->setMaximum(100);

		updateTimeStats(0, dfx_render_stats::lastRenderTime);

		setupRender();
		show();

		QSize qsize(rinterface->getWidth() + 8, rinterface->getHeight() + 22);
		resize(qsize);
		
		render();
	}
}

bool DfxRenderViewer::isFinished() {

	if (render_thread == nullptr) {
		return true;
	}
	else {
		if (render_thread->isFinished()) {
			return true;
		}
		else {
			return false;
		}
	}
}

void DfxRenderViewer::updateRenderOutput(QPixmap pmap) {
	image->setPixmap(pmap);
	image->resize(pmap.size());

	qApp->processEvents();
}

void DfxRenderViewer::updateProgress(int value) {
	progress->setValue(value);
	qApp->processEvents();
}

void DfxRenderViewer::abortRender(bool) {

	if (render_thread != nullptr) {
		render_thread->abort();
	}
}

void DfxRenderViewer::updateTimeStats(int time, int last) {
	QTime t = QTime::fromMSecsSinceStartOfDay(time);
	QTime l = QTime::fromMSecsSinceStartOfDay(last);

	renderTimeLabel->setText(QString("Time: ") + t.toString("HH:mm:ss")  + " | " + "Last: " + l.toString("HH:mm:ss"));
	qApp->processEvents();
}