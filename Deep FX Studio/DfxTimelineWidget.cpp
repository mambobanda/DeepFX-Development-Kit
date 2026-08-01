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

#include "DfxMainLoop.h"
#include "DfxTimelineWidget.h"
#include "DfxScene.h"


DfxTimelineWidget *DfxTimelineWidget::instance = nullptr;
bool DfxTimelineWidget::playActive = false;
bool DfxTimelineWidget::inProcess = false;

DfxTimelineWidget::DfxTimelineWidget(QWidget *parent): QWidget(parent) {

	fixedHeight = 30;

	setFixedHeight(fixedHeight + 30);

	frameManip = new DfxFrameManipulatorArea(fixedHeight);

	int minwdth = 45;
	int minwdth2 = 50;
	int minwdth3 = 40;

	int minHth = 30;

	play			= new QPushButton( QIcon(":/timeline_btn/play_btn"), "");
	nextFrame		= new QPushButton( QIcon(":/timeline_btn/next_frame_btn"), "");
	previousFrame	= new QPushButton( QIcon(":/timeline_btn/previous_frame_btn"), "");
	nextKey			= new QPushButton( QIcon(":/timeline_btn/next_key_btn"), "");
	previousKey		= new QPushButton( QIcon(":/timeline_btn/previous_key_btn"), "");
	start			= new QPushButton( QIcon(":/timeline_btn/first_frame_btn"), "");
	end				= new QPushButton( QIcon(":/timeline_btn/last_frame_btn"), "");

	play->setFixedWidth(minwdth);
	nextFrame->setFixedWidth(minwdth);
	previousFrame->setFixedWidth(minwdth);
	nextKey->setFixedWidth(minwdth);
	previousKey->setFixedWidth(minwdth);
	start->setFixedWidth(minwdth);
	end->setFixedWidth(minwdth);

	QFont buttonfont = play->font();
	buttonfont.setBold(true);
	
	play->setFont(buttonfont);
	nextFrame->setFont(buttonfont);
	previousFrame->setFont(buttonfont);
	nextKey->setFont(buttonfont);
	previousKey->setFont(buttonfont);
	start->setFont(buttonfont);
	end->setFont(buttonfont);

	buttonLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	buttonLayout->setSpacing(1);
	buttonLayout->setMargin(0);
	buttonLayout->addWidget(start);
	//buttonLayout->addWidget(previousKey);

	buttonLayout->addWidget(previousFrame);
	buttonLayout->addWidget(play);
	buttonLayout->addWidget(nextFrame);
	//buttonLayout->addWidget(nextKey);
	buttonLayout->addWidget(end);

	currentFrame = new QLineEdit();
	frameCount   = new QLineEdit();

	currentFrame->setValidator(new QIntValidator(1, 100000, currentFrame));
	frameCount->setValidator(new QIntValidator(10, 100000, frameCount));

	createKey = new QPushButton("Key");
	removeKey = new QPushButton("Del K");
	autoKey = new QPushButton("Auto K");
	sim = new QPushButton("Sim");
	reset = new QPushButton("Reset");

	currentFrame->setFixedWidth(minwdth2);
	frameCount->setFixedWidth(minwdth2);
	createKey->setFixedWidth(minwdth3);
	removeKey->setFixedWidth(minwdth3);
	autoKey->setFixedWidth(minwdth3);
	sim->setFixedWidth(minwdth3);
	reset->setFixedWidth(minwdth3);

	play->setCheckable(true);
	autoKey->setCheckable(true);
	sim->setCheckable(true);

	QString cstyle01 = sim->styleSheet();
	QString cstyle02 = sim->styleSheet();
	cstyle01 += QString("QPushButton:checked { background-color:green; }");
	cstyle02 += QString("QPushButton:checked { background-color:rgb(255, 102, 0); color: black;}");

	sim->setStyleSheet(cstyle01);
	autoKey->setStyleSheet(cstyle01);
	play->setStyleSheet(cstyle02);

	DfxSimulationCoreGlobal::getInstance()->setSimulationMode(DfxSimulationMode::ACTIVE_MODE);

	if (DfxSimulationCoreGlobal::getInstance()->getSimulationMode() == DfxSimulationMode::ACTIVE_MODE) {
		sim->setChecked(true);
	}
	else {
		sim->setChecked(false);
	}

	QBoxLayout *frameInfoLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	frameInfoLayout->setSpacing(3);
	frameInfoLayout->setMargin(0);
	frameInfoLayout->addWidget(frameCount);
	frameInfoLayout->addWidget(currentFrame);
	frameInfoLayout->addWidget(createKey);
	frameInfoLayout->addWidget(removeKey);
	//frameInfoLayout->addWidget(autoKey);
	frameInfoLayout->addWidget(sim);
	frameInfoLayout->addWidget(reset);

	QBoxLayout *controlLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
	controlLayout->setSpacing(1);
	controlLayout->addLayout(frameInfoLayout);
	controlLayout->addLayout(buttonLayout);

	QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	mainLayout->setMargin(0);
	mainLayout->addWidget(frameManip);
	mainLayout->addLayout(controlLayout);

	setLayout(mainLayout);

	currentFrame->setText(QString::number(DfxTime::getInstance()->getCurrentFrame()));
	frameCount->setText(QString::number(DfxTime::getInstance()->getTotalFrames()));
	setCurrentFrame(DfxTime::getInstance()->getCurrentFrame());
	setFrameCount(DfxTime::getInstance()->getTotalFrames());

	auto selActlist = new DfxSceneActionListener([this](DfxSceneActionInfo info) {
		if (info.actionCode == DfxSceneActionListener::ITEM_SELECTED_ACTION) {
			frameManip->getFrameManipulator()->update();
		}
	});
	
	DfxScene::getInstance()->addActionListener(selActlist);

	QObject::connect(frameManip->getFrameManipulator(), &DfxFrameManipulator::newFrameSet,
					 this, &DfxTimelineWidget::updateCurrentFrameEdit);

	QObject::connect(frameCount, &QLineEdit::editingFinished,
					 this, &DfxTimelineWidget::updateFrameCountEdit);

	QObject::connect(currentFrame, &QLineEdit::editingFinished,
					 this, &DfxTimelineWidget::updateCurrentFrameManipulator);

	QObject::connect(sim, &QPushButton::clicked,
				     this, &DfxTimelineWidget::updateSimulationStateButton);

	QObject::connect(play, &QPushButton::clicked,
		             this, &DfxTimelineWidget::playClickedAction);

	QObject::connect(start, &QPushButton::clicked,
					 this, &DfxTimelineWidget::firstFrameClickedAction);

	QObject::connect(end, &QPushButton::clicked,
					 this, &DfxTimelineWidget::lastFrameClickedAction);

	QObject::connect(nextFrame, &QPushButton::clicked,
					 this, &DfxTimelineWidget::nextFrameClickedAction);

	QObject::connect(previousFrame, &QPushButton::clicked,
					 this, &DfxTimelineWidget::previousFrameClickedAction);

	QObject::connect(reset, &QPushButton::clicked,
					 this, &DfxTimelineWidget::resetClickedAction);

	QObject::connect(createKey, &QPushButton::clicked,
					 this, &DfxTimelineWidget::createKeyClickedAction);

	QObject::connect(removeKey, &QPushButton::clicked,
					 this, &DfxTimelineWidget::deleteKeyClickedAction);
}

DfxTimelineWidget *DfxTimelineWidget::getInstance() {

	if (instance == nullptr) {
		instance = new DfxTimelineWidget();
	}

	return instance;
}

QSize DfxTimelineWidget::sizeHint() const {
	return QSize(400, fixedHeight);
}


void DfxTimelineWidget::paintEvent(QPaintEvent *event) {

}

int DfxTimelineWidget::getFrameCount() {
	return frameManip->getFrameManipulator()->getFrameCount();
}

void DfxTimelineWidget::setFrameCount(int fc) {
	frameManip->getFrameManipulator()->setFrameCount(fc);
}

int DfxTimelineWidget::getCurrentFrame() {
	return frameManip->getFrameManipulator()->getFrameCount();
}

void DfxTimelineWidget::setCurrentFrame(int cf) {
	frameManip->getFrameManipulator()->setCurrentFrame(cf);
}

void DfxTimelineWidget::updateCurrentFrameEdit() {
   currentFrame->setText(QString::number(DfxTime::getInstance()->getCurrentFrame()));
}

void DfxTimelineWidget::updateFrameCountEdit() {
	setFrameCount(frameCount->text().toInt());
}

void DfxTimelineWidget::updateCurrentFrameManipulator() {
	setCurrentFrame(currentFrame->text().toInt());
}

bool DfxTimelineWidget::playIsChecked() {
	return play->isChecked();
}

DfxFrameManipulator *DfxTimelineWidget::getFrameManipulator() {
	return frameManip->getFrameManipulator();
}

void DfxTimelineWidget::updateSimulationStateButton(bool st) {

	if (st)
		DfxSimulationCoreGlobal::getInstance()->setSimulationMode(DfxSimulationMode::ACTIVE_MODE);
	else
		DfxSimulationCoreGlobal::getInstance()->setSimulationMode(DfxSimulationMode::INACTIVE_MODE);
}

void DfxTimelineWidget::setCorrectSimulationFrame() {

	if (DfxSimulationCoreGlobal::getInstance()->getSimulationMode() == DfxSimulationMode::ACTIVE_MODE) {

		int tempCurrent = DfxTime::getInstance()->getCurrentFrame();
		if (tempCurrent != 1) {
			if (!DeepStudioGlobal::getInstance()->isFrameCached(tempCurrent - 1)) {
				int lastFrame = DeepStudioGlobal::getInstance()->getLastCachedFrame();

				if (lastFrame == -1) {
					DfxTime::getInstance()->setNewFrame(1);
				}
				else {
					DfxTime::getInstance()->setNewFrame(lastFrame + 1);
				}
			}
		}
	}
}

void DfxTimelineWidget::nextFrame_() {

	int currentf = DfxTime::getInstance()->getCurrentFrame();

	if (currentf == DfxTime::getInstance()->getTotalFrames()) currentf = 1; else currentf++;

	setCurrentFrame(currentf);

	frameManip->update();
}

void DfxTimelineWidget::previousFrame_() {

	int currentf = DfxTime::getInstance()->getCurrentFrame();

	if (currentf == 1) currentf = DfxTime::getInstance()->getTotalFrames(); else currentf--;

	setCurrentFrame(currentf);
}

void DfxTimelineWidget::nextSimulationFrame() {

	if (!DeepStudioGlobal::getInstance()->allFramesCached()) {

		int lastFrame = DeepStudioGlobal::getInstance()->getLastCachedFrame();

		int currentf = DfxTime::getInstance()->getCurrentFrame();

		if (lastFrame == -1) {
			setCurrentFrame(1);
		}
		else {

			int newFrame = 0;

			if (lastFrame == DfxTime::getInstance()->getTotalFrames()) {
				nextFrame_();
				DfxGlobal::command::exec("syncInterface");
			}
			else {
				newFrame = lastFrame + 1;
				setCurrentFrame(newFrame);
				DfxGlobal::command::exec("syncInterface");
			}
		}
	}
	/*else {
		nextFrame_();
	}*/
}

void DfxTimelineWidget::lastSimulationFrame() {

	int lastFrame = DeepStudioGlobal::getInstance()->getLastCachedFrame();

	int currentf = DfxTime::getInstance()->getCurrentFrame();

	if (lastFrame == -1) {
		setCurrentFrame(1);
	}
	else {
		setCurrentFrame(lastFrame);

	}
}

void DfxTimelineWidget::stepSimulation(DfxEntity *root) {

	if (DfxSimulationCoreGlobal::getInstance()->getSimulationMode() == DfxSimulationMode::ACTIVE_MODE) {

		if (!DeepStudioGlobal::getInstance()->isFrameCached()) {

			root->bfsQuery(root);

			DeepStudioGlobal::getInstance()->cacheFrame();
			DeepStudioGlobal::getInstance()->makeFrameValid();
			update();
		}
	}
}

void DfxTimelineWidget::playFunction(DfxTimelineWidget *pf) {
	playActive = true;

	int mFpsTimer = std::round(1000.f/(float)DfxTime::getInstance()->getFramesPerSec());
	
	std::vector<DfxEntity*> rootNodes = DfxSceneGraph::getInstance()->getGraphRoots();

	while (true) {
		if (!pf->playIsChecked()) break;

		if (DfxSimulationCoreGlobal::getInstance()->getSimulationMode() == DfxSimulationMode::ACTIVE_MODE &&
			!DeepStudioGlobal::getInstance()->allFramesCached()) {

			pf->nextSimulationFrame();
			DfxSimulationCoreGlobal::getInstance()->initSimulationDomain();

			for(auto root: rootNodes)
				pf->stepSimulation(root);
		}
		else {
			pf->nextFrame_();
			QThread::msleep(mFpsTimer);
		}

		DfxGlobal::command::exec("updateViewport");	
	}

	for (auto root : rootNodes) delete root;

	playActive = false;
}

void DfxTimelineWidget::playClickedAction(bool st) {

	/*if (st) {

		play->setIcon(QIcon(":/timeline_btn/pause_btn"));
		playFuture = QtConcurrent::run(DfxTimelineWidget::playFunction, this);
	}
	else {
		
		play->setIcon(QIcon(":/timeline_btn/play_btn"));
		if (playFuture.isRunning()) playFuture.waitForFinished();
	}

	DfxGlobal::command::exec("updateViewport");*/

	if (st) {
		DfxMainLoop::getInstance()->setState(DfxEventLoopState::SIMULATE_STATE);
	}
	else {
		DfxMainLoop::getInstance()->setState(DfxEventLoopState::IDLE_STATE);
	}
}

void DfxTimelineWidget::lastFrameClickedAction(bool st) {
	setCurrentFrame(DfxTime::getInstance()->getTotalFrames());

	DfxGlobal::command::exec("updateViewport");
}

void DfxTimelineWidget::firstFrameClickedAction(bool st) {
	setCurrentFrame(1);

	DfxGlobal::command::exec("updateViewport");
}

void DfxTimelineWidget::nextFrameClickedAction(bool) {
	inProcess = true;

	std::vector<DfxEntity*> rootNodes = DfxSceneGraph::getInstance()->getGraphRoots();
	
	if (DfxSimulationCoreGlobal::getInstance()->getSimulationMode() == DfxSimulationMode::ACTIVE_MODE) {
		nextSimulationFrame();
		DfxSimulationCoreGlobal::getInstance()->initSimulationDomain();

		for (auto root : rootNodes)
			stepSimulation(root);
	}
	else {
		nextFrame_();
	}

	for (auto root : rootNodes) delete root;

	inProcess = false;

	DfxGlobal::command::exec("updateViewport");
}

void DfxTimelineWidget::previousFrameClickedAction(bool) {
	previousFrame_();

	DfxGlobal::command::exec("updateViewport");
}

void DfxTimelineWidget::resetSimulationDomain() {

	if(!playIsChecked() && !inProcess && !playActive){

		DeepStudioGlobal::getInstance()->resetFrameState();
		DfxSimulationCoreGlobal::getInstance()->deinitializeSimulationDomain();
		frameManip->getFrameManipulator()->update();
	}
}

void DfxTimelineWidget::resetClickedAction(bool) {

	getInstance()->resetSimulationDomain();

	DfxGlobal::command::exec("updateViewport");
}

void DfxTimelineWidget::createKeyClickedAction(bool) {

	using namespace DfxEntityPropertyTypes;

	auto list = DfxScene::getInstance()->getSelectedItems();

	for (auto sel : list) {

		DfxEnvelope *posEnv = DfxEnvelopeMap::getInstance()->findEnvelope(sel->getInstanceID(), DfxViewItem::position);
		DfxEnvelope *sclEnv = DfxEnvelopeMap::getInstance()->findEnvelope(sel->getInstanceID(), DfxViewItem::scale);
		DfxEnvelope *rotEnv = DfxEnvelopeMap::getInstance()->findEnvelope(sel->getInstanceID(), DfxViewItem::rotation);

		int frame = DfxTime::getInstance()->getCurrentFrame();

		if (posEnv != nullptr) {

			Float3 f3; 

			f3.x = sel->getPosition().x;
			f3.y = sel->getPosition().y;
			f3.z = sel->getPosition().z;

			posEnv->setValue(frame, f3);
		}

		if (sclEnv != nullptr) {

			Float3 f3;

			f3.x = sel->getScale().x;
			f3.y = sel->getScale().y;
			f3.z = sel->getScale().z;

			sclEnv->setValue(frame, f3);
		}

		if (rotEnv != nullptr) {

			Float3 f3;

			f3.x = sel->getRotation().x;
			f3.y = sel->getRotation().y;
			f3.z = sel->getRotation().z;

			rotEnv->setValue(frame, f3);
		}
	}

	frameManip->getFrameManipulator()->update();
}

void DfxTimelineWidget::deleteKeyClickedAction(bool) {
	using namespace DfxEntityPropertyTypes;

	auto list = DfxScene::getInstance()->getSelectedItems();

	for (auto sel : list) {

		DfxEnvelope *posEnv = DfxEnvelopeMap::getInstance()->findEnvelope(sel->getInstanceID(), DfxViewItem::position);
		DfxEnvelope *sclEnv = DfxEnvelopeMap::getInstance()->findEnvelope(sel->getInstanceID(), DfxViewItem::scale);
		DfxEnvelope *rotEnv = DfxEnvelopeMap::getInstance()->findEnvelope(sel->getInstanceID(), DfxViewItem::rotation);

		int frame = DfxTime::getInstance()->getCurrentFrame();

		if (posEnv != nullptr) posEnv->clearValue(frame);
		if (sclEnv != nullptr) sclEnv->clearValue(frame);
		if (rotEnv != nullptr) rotEnv->clearValue(frame);
		
	}

	frameManip->getFrameManipulator()->update();
}