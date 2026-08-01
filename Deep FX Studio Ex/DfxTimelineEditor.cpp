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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxTimelineEditor.h"
#include "DfxGraphEditorManager.h"

DfxTimelineEditor::DfxTimelineEditor(QWidget *parent): QWidget(parent) {

	frameScale = 120;
	ready = false;

	mainLayout = new QVBoxLayout;
	keyEditorLayout = new QVBoxLayout;
	keyTrackLayout  = new QVBoxLayout; 
	channelLayout	= new QVBoxLayout;
	headerKeyTrackLayout = new QHBoxLayout;
	framescaleHBarLayout = new QHBoxLayout;

	button_menu_layout		 = new QHBoxLayout;
	button_menu_left_layout  = new QHBoxLayout;
	button_menu_center_layout = new QHBoxLayout;
	button_menu_right_layout = new QHBoxLayout;
	centralWidgetLayout		 = new QHBoxLayout;
	
	framescaleSlider = new QSlider(Qt::Orientation::Horizontal);

	keyTrackHScrollbar = new QScrollBar(Qt::Orientation::Horizontal);
	channelKeyTrackVScrollbar = new QScrollBar(Qt::Orientation::Vertical);

	main_splitter = new QSplitter(Qt::Orientation::Horizontal);

	keyHeader = new DfxTimelineHeaderWidget(frameScale);

	keyHeaderWidget = new QWidget;
	keyTrackWidget  = new QWidget;
	keyEditorWidget = new QWidget;
	channelWidget	= new QWidget;
	centralWidget	= new QWidget;
	headerKeyTrackWidget = new QWidget;
	buttonMenuWidget = new QWidget;

	keyHeader_scroll = new QScrollArea();
	main_scroll		 = new QScrollArea();	
	channel_scroll	 = new QScrollArea();
	key_scroll		 = new CustomScrollArea(this);

	trackOverlay = new DfxTimelineKeyTrackOverlay(key_scroll);
	trackOverlay->_selectedChannels = &_selectedChannels;
	trackOverlay->_parent = key_scroll;
	trackOverlay->vbar = channelKeyTrackVScrollbar;
	trackOverlay->hbar = keyTrackHScrollbar;
	/*trackEventFilter = new DfxTimelineTrackEventFilter;
	trackEventFilter->overlay = trackOverlay;
	trackEventFilter->_selectedChannels = &_selectedChannels;
	trackOverlay->installEventFilter(trackEventFilter);*/

	scrollAreaEventFilter = new DfxTimelineScrollEventFilter;

	realtimeMode = new QPushButton(QPixmap(":/icon/realtime_mode_icon").scaled(18, 11), "");
	recordCache = new QPushButton(QPixmap(":/icon/record_cache_icon").scaled(8, 8), "");
	resetSim = new QPushButton(QPixmap(":/icon/sim_reset_icon").scaled(13,13), "");
	play = new QPushButton(QPixmap(":/timeline_btn/play_btn").scaled(10,10), "");
	stop = new QPushButton(QPixmap(":/timeline_btn/stop_btn").scaled(10, 10), "");
	nextFrame = new QPushButton(QPixmap(":/timeline_btn/next_frame_btn").scaled(10,10), "");
	previousFrame = new QPushButton(QPixmap(":/timeline_btn/previous_frame_btn").scaled(10,10), "");
	nextKey = new QPushButton(QPixmap(":/timeline_btn/next_key_btn").scaled(10,10), "");
	previousKey = new QPushButton(QPixmap(":/timeline_btn/previous_key_btn").scaled(10,10), "");
	start = new QPushButton(QPixmap(":/timeline_btn/first_frame_btn").scaled(10,10), "");
	end = new QPushButton(QPixmap(":/timeline_btn/last_frame_btn").scaled(10,10), "");
	createKey = new QPushButton(QPixmap(":/icon/old_key_icon").scaled(12, 12), "");
	deleteKey = new QPushButton(QPixmap(":/icon/remove_key").scaled(15, 15), "");
	keyMenu   = new QPushButton("");

	current_frame = new DfxQIntegerSpinnerControl("~/TIME/current_frame", "current", false);
	start_frame   = new DfxQIntegerSpinnerControl("~/TIME/frame_start", "start", false);
	end_frame	  = new DfxQIntegerSpinnerControl("~/TIME/frame_end", "end", false);

	frameIndicator = new DfxTimelineFrameIndicator(keyTrackWidget);

	keyablesGroup = new QActionGroup(this);
	keyableProperties = new QMenu("");
	locRotScl = new QAction("Pos/Rot/Scl");
	loc = new QAction("Position");
	rot = new QAction("Rotation");
	scl = new QAction("Scale");

	framescaleSlider->setFixedWidth(100);
	framescaleSlider->setMaximum(500);
	framescaleSlider->setMinimum(10);
	framescaleSlider->setValue(frameScale);

	keyableProperties->addAction(locRotScl);
	keyableProperties->addAction(loc);
	keyableProperties->addAction(rot);
	keyableProperties->addAction(scl);

	keyablesGroup->addAction(locRotScl);
	keyablesGroup->addAction(loc);
	keyablesGroup->addAction(rot);
	keyablesGroup->addAction(scl);
	keyablesGroup->setExclusive(true);
	
	int keyable = DfxSpace::getInstance()->findAbsolute("~/KEY_OPTIONS/keyable")->data<int>();

	realtimeMode->setCheckable(true);

	locRotScl->setCheckable(true);
	loc->setCheckable(true);
	rot->setCheckable(true);
	scl->setCheckable(true);

	if (keyable == DfxKeyableTypes::LOC_ROT_SCL) locRotScl->setChecked(true);
	else if (keyable == DfxKeyableTypes::LOC) loc->setChecked(true);
	else if (keyable == DfxKeyableTypes::ROT) rot->setChecked(true);
	else if (keyable == DfxKeyableTypes::SCL) scl->setChecked(true);

	keyMenu->setMenu(keyableProperties);

	keyHeader->setFrameIndicator(frameIndicator);

	channel_scroll->viewport()->installEventFilter(scrollAreaEventFilter);
	key_scroll->viewport()->installEventFilter(scrollAreaEventFilter);
	keyHeader_scroll->viewport()->installEventFilter(scrollAreaEventFilter);

	keyHeader_scroll->setFixedHeight(constants::header_height);
	keyHeader_scroll->setFrameShape(QFrame::NoFrame);
	keyHeader_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	keyHeader_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

	key_scroll->setFrameShape(QFrame::NoFrame);
	key_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	key_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

	channel_scroll->setFrameShape(QFrame::NoFrame);
	channel_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	channel_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	channel_scroll->setWidgetResizable(true);

	style::styleQScrollBar(keyTrackHScrollbar);
	style::styleQScrollBar(channelKeyTrackVScrollbar);

	headerKeyTrackLayout->setMargin(0);
	headerKeyTrackLayout->setAlignment(Qt::AlignLeft);

	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	mainLayout->setAlignment(Qt::AlignTop);

	channelLayout->setMargin(0);
	channelLayout->setAlignment(Qt::AlignTop);
	channelLayout->setSpacing(3);

	keyEditorLayout->setMargin(0);
	keyEditorLayout->setAlignment(Qt::AlignTop);
	keyEditorLayout->setSpacing(0);

	keyTrackLayout->setMargin(0);
	keyTrackLayout->setAlignment(Qt::AlignTop);
	keyTrackLayout->setSpacing(3);
	keyTrackLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

	framescaleHBarLayout->setMargin(0);
	framescaleHBarLayout->setSpacing(1);
	framescaleHBarLayout->setAlignment(Qt::AlignLeft);

	button_menu_layout->setContentsMargins(0, 3, 0, 3);

	button_menu_left_layout->setContentsMargins(10, 0, 0, 0);
	button_menu_left_layout->setAlignment(Qt::AlignLeft);

	button_menu_center_layout->setMargin(0);
	button_menu_center_layout->setAlignment(Qt::AlignCenter);
	button_menu_center_layout->setSpacing(1);
	
	button_menu_right_layout->setContentsMargins(0, 0, 10, 0);
	button_menu_right_layout->setAlignment(Qt::AlignRight);

	//channelLayout->addSpacing(constants::header_height);
	/*channelLayout->addWidget(new DfxTimelineChannelWidget);
	channelLayout->addWidget(new DfxTimelineChannelWidget);
	channelLayout->addWidget(new DfxTimelineChannelWidget);
	channelLayout->addWidget(new DfxTimelineChannelWidget);
	channelLayout->addWidget(new DfxTimelineChannelWidget);
	channelLayout->addWidget(new DfxTimelineChannelWidget);
	channelLayout->addWidget(new DfxTimelineChannelWidget);
	channelLayout->addWidget(new DfxTimelineChannelWidget);
	*/
	//channelLayout->addSpacing(constants::header_height);

	/*
	keyTrackLayout->addWidget(new DfxTimelineTrackWidget(startFrame, endFrame, frameScale));
	keyTrackLayout->addWidget(new DfxTimelineTrackWidget(startFrame, endFrame, frameScale));
	keyTrackLayout->addWidget(new DfxTimelineTrackWidget(startFrame, endFrame, frameScale));
	keyTrackLayout->addWidget(new DfxTimelineTrackWidget(startFrame, endFrame, frameScale));
	keyTrackLayout->addWidget(new DfxTimelineTrackWidget(startFrame, endFrame, frameScale));
	keyTrackLayout->addWidget(new DfxTimelineTrackWidget(startFrame, endFrame, frameScale));
	keyTrackLayout->addWidget(new DfxTimelineTrackWidget(startFrame, endFrame, frameScale));
	keyTrackLayout->addWidget(new DfxTimelineTrackWidget(startFrame, endFrame, frameScale));
	*/
	
	keyTrackWidget->setLayout(keyTrackLayout);

	key_scroll->setWidget(keyTrackWidget);
	keyHeader_scroll->setWidget(keyHeader);

	framescaleHBarLayout->addWidget(framescaleSlider);
	framescaleHBarLayout->addWidget(keyTrackHScrollbar);

	keyEditorLayout->addWidget(keyHeader_scroll);
	keyEditorLayout->addWidget(key_scroll);
	keyEditorLayout->addLayout(framescaleHBarLayout);

	keyEditorWidget->setLayout(keyEditorLayout);
	channelWidget->setLayout(channelLayout);

	channel_scroll->setWidget(channelWidget);

	headerKeyTrackLayout->addWidget(keyEditorWidget);
	headerKeyTrackLayout->addWidget(channelKeyTrackVScrollbar);
	headerKeyTrackWidget->setLayout(headerKeyTrackLayout);

	main_splitter->addWidget(channel_scroll);
	main_splitter->addWidget(headerKeyTrackWidget);

	main_splitter->setSizes({ 154, 1762 });

	button_menu_center_layout->addWidget(recordCache);
	button_menu_center_layout->addWidget(realtimeMode);
	button_menu_center_layout->addSpacing(5);
	button_menu_center_layout->addWidget(start);
	button_menu_center_layout->addWidget(previousFrame);
	button_menu_center_layout->addWidget(resetSim);
	button_menu_center_layout->addWidget(stop);
	button_menu_center_layout->addWidget(play);
	button_menu_center_layout->addWidget(nextFrame);
	button_menu_center_layout->addWidget(end);
	button_menu_center_layout->addSpacing(5);
	button_menu_center_layout->addWidget(createKey);
	button_menu_center_layout->addWidget(deleteKey);
	button_menu_center_layout->addWidget(keyMenu);

	button_menu_left_layout->addWidget(start_frame);
	button_menu_right_layout->addWidget(end_frame);

	button_menu_layout->addLayout(button_menu_left_layout);
	button_menu_layout->addLayout(button_menu_center_layout);
	button_menu_layout->addLayout(button_menu_right_layout);

	buttonMenuWidget->setStyleSheet("background-color: transparent;");
	buttonMenuWidget->setLayout(button_menu_layout);

	mainLayout->addWidget(buttonMenuWidget);
	mainLayout->addWidget(main_splitter);

	setLayout(mainLayout);

	stylePlayButtons();
	ready = true;

	hideShowNoneRealtimeControls();

	_timeobserverFrameStart = new TimeObserver;
	_timeobserverFrameEnd   = new TimeObserver;

	_timeobserverFrameStart->ptr = this;
	_timeobserverFrameEnd->ptr   = this;

	DfxSpace::getInstance()->findAbsolute("~/TIME/frame_start")->addObserver(_timeobserverFrameStart);
	DfxSpace::getInstance()->findAbsolute("~/TIME/frame_end")->addObserver(_timeobserverFrameEnd);

	QObject::connect(recordCache, &QPushButton::clicked, DfxTimelineManager::getInstance(), &DfxTimelineManager::recordCache);
	QObject::connect(play, &QPushButton::clicked, DfxTimelineManager::getInstance(), &DfxTimelineManager::play);
	QObject::connect(stop, &QPushButton::clicked, DfxTimelineManager::getInstance(), &DfxTimelineManager::stop);
	QObject::connect(nextFrame, &QPushButton::clicked, this, &DfxTimelineEditor::_goToNextFrame);
	QObject::connect(previousFrame, &QPushButton::clicked, this, &DfxTimelineEditor::_goToPreviousFrame);
	QObject::connect(start, &QPushButton::clicked, this, &DfxTimelineEditor::_goToFirstFrame);
	QObject::connect(end, &QPushButton::clicked, this, &DfxTimelineEditor::_goToLastFrame);

	QObject::connect(key_scroll->horizontalScrollBar(), &QScrollBar::rangeChanged, this, &DfxTimelineEditor::key_hscrollRangeChanged);
	QObject::connect(channel_scroll->verticalScrollBar(), &QScrollBar::rangeChanged, this, &DfxTimelineEditor::key_vscrollRangeChanged);

	QObject::connect(keyTrackHScrollbar, &QScrollBar::valueChanged, keyHeader_scroll->horizontalScrollBar(), &QScrollBar::setValue);
	QObject::connect(keyTrackHScrollbar, &QScrollBar::valueChanged, key_scroll->horizontalScrollBar(), &QScrollBar::setValue);

	QObject::connect(channelKeyTrackVScrollbar, &QScrollBar::valueChanged, key_scroll->verticalScrollBar(), &QScrollBar::setValue);
	QObject::connect(channelKeyTrackVScrollbar, &QScrollBar::valueChanged, channel_scroll->verticalScrollBar(), &QScrollBar::setValue);

	QObject::connect(main_splitter, &QSplitter::splitterMoved, this, &DfxTimelineEditor::splitterMoved);

	QObject::connect(createKey, &QPushButton::clicked, this, &DfxTimelineEditor::createKeyAction);
	QObject::connect(deleteKey, &QPushButton::clicked, this, &DfxTimelineEditor::deleteKeyAction);

	QObject::connect(locRotScl, &QAction::triggered, this, &DfxTimelineEditor::setPosRotSclMode);
	QObject::connect(loc, &QAction::triggered, this, &DfxTimelineEditor::setPosMode);
	QObject::connect(rot, &QAction::triggered, this, &DfxTimelineEditor::setRotMode);
	QObject::connect(scl, &QAction::triggered, this, &DfxTimelineEditor::setSclMode);

	QObject::connect(DfxPropertyConduit::getInstance(), &DfxPropertyConduit::newKeyFrameCreated, this, &DfxTimelineEditor::refreshModel);
	QObject::connect(DfxSceneSelectionModel::getInstance(), &DfxSceneSelectionModel::selectionChanged, this, &DfxTimelineEditor::refreshModel);

	QObject::connect(framescaleSlider, &QSlider::valueChanged, this, &DfxTimelineEditor::frameScaleSliderChanged);
	QObject::connect(this, &DfxTimelineEditor::frameScaleChanged, keyHeader, &DfxTimelineHeaderWidget::updateFrameScale);

	QObject::connect(this, &DfxTimelineEditor::shiftPressed, trackOverlay, &DfxTimelineKeyTrackOverlay::shiftPressed);
	QObject::connect(this, &DfxTimelineEditor::controlPressed, trackOverlay, &DfxTimelineKeyTrackOverlay::controlPressed);
	QObject::connect(this, &DfxTimelineEditor::shiftReleased, trackOverlay, &DfxTimelineKeyTrackOverlay::shiftReleased);
	QObject::connect(this, &DfxTimelineEditor::controlReleased, trackOverlay, &DfxTimelineKeyTrackOverlay::controlReleased);

	QObject::connect(realtimeMode, &QPushButton::clicked, this, &DfxTimelineEditor::toggleRealtimeMode);
	QObject::connect(resetSim, &QPushButton::clicked, DfxSimulationManager::getInstance(), &DfxSimulationManager::resetSimulationState);
	QObject::connect(DfxMode::getInstance(), &DfxMode::animationModeChanged, this, &DfxTimelineEditor::hideShowNoneRealtimeControls);
}

DfxTimelineEditor::~DfxTimelineEditor() {
	delete _timeobserverFrameStart;
	delete _timeobserverFrameEnd;
}

void DfxTimelineEditor::splitterMoved(int pos, int index) {
	// printf("%i, %i\n", main_splitter->sizes()[0], main_splitter->sizes()[1]);
}

void DfxTimelineEditor::key_hscrollRangeChanged(int min, int max) {
	keyTrackHScrollbar->setRange(min, max);
}

void DfxTimelineEditor::key_vscrollRangeChanged(int min, int max) {
	channelKeyTrackVScrollbar->setRange(min, max);
}

void DfxTimelineEditor::resizeEvent(QResizeEvent *event) {
	channel_scroll->setFixedHeight(height() - 40);
	frameIndicator->setFixedHeight(keyTrackWidget->height());
	//trackOverlay->resize(key_scroll->width(), key_scroll->height() + constants::row_height);
}

void DfxTimelineEditor::keyPressEvent(QKeyEvent *event) {

	if (event->key() == Qt::Key_Shift) {
		emit shiftPressed();
	}
	else if (event->key() == Qt::Key_Control) {
		emit controlPressed();
	}
}

void DfxTimelineEditor::keyReleaseEvent(QKeyEvent *event) {

	if (event->key() == Qt::Key_Shift) {
		emit shiftReleased();
	}
	else if (event->key() == Qt::Key_Control) {
		emit controlReleased();
	}

}

void DfxTimelineEditor::createKeyAction() {
	DfxGlobal::command::exec("createKey");
}

void DfxTimelineEditor::deleteKeyAction() {

	for (auto trl : _selectedChannels) {
		if (!trl->track()->keyframePath().empty()) {

			for (auto frame : trl->track()->selectedKeys()) {
				DfxPropertyConduit::getInstance()->removeKeyframeFloatOverride(trl->track()->keyframePath(), frame);
			}

			if (DfxPropertyConduit::getInstance()->floatKeyframeOverrideSize(trl->track()->keyframePath()) == 0) {
				DfxPropertyConduit::getInstance()->destroyframeFloatOverride(trl->track()->keyframePath());
			}
		}
	}

	trackOverlay->update();
	DfxGraphEditorManager::getInstance()->notifyChannelUpdated();

}

void DfxTimelineEditor::enterEvent(QEvent *event) {
	setFocus();
}

std::vector<std::string>  DfxTimelineEditor::findKeyFramePaths(fl::Any *any, int level) {
	DfxTimelineChannelWidget *chanParent = nullptr;
	std::vector<std::string> paths;

	if (any != nullptr) {
		if (any->type() == fl::type::DATA_OBJECT) {
			for (auto ap00 : any->data<fl::DataObject>().properties()) {

				if (ap00->type() == fl::type::DATA_OBJECT) {
					auto n_paths = findKeyFramePaths(ap00, level + 1);
					for (auto n : n_paths) paths.push_back(n);
				}
				else {

					if (level >= 0) {

						auto a_path = ap00->absolutePath();
						if (DfxPropertyConduit::getInstance()->hasKeyframeOverride(a_path)) {
							paths.push_back(a_path);
						}
					}
				}
			}
		}

	}

	return paths;
}

void DfxTimelineEditor::importDataObject(fl::Any *any, int level, std::vector<std::string> &keyframes, DfxTimelineChannelWidget *parent) {
	
	if (any != nullptr) {

		DfxTimelineChannelWidget *chanParent = nullptr;
		if (any->type() == fl::type::DATA_OBJECT) {
			std::vector<std::string> selectedPaths;

			if (level >= 0) {

				bool create = false;
				auto a_path = any->absolutePath();
				for (auto p : keyframes) {
					if (p.find(a_path) != std::string::npos) {
						create = true;
						selectedPaths.push_back(p);
					}
				}

				if (create) {
					chanParent = newChannel(level);
					chanParent->setText(any->label().c_str());
					chanParent->setRelevantPaths(selectedPaths);

					if (parent != nullptr) {
						chanParent->setChannelParent(parent);
						parent->addChildChannel(chanParent);
					}
				}
			}

			for (auto ap00 : any->data<fl::DataObject>().properties()) {

				if (ap00->type() == fl::type::DATA_OBJECT) {
					importDataObject(ap00, level + 1, keyframes, chanParent);
				}
				else {

					if (level >= 0) {

						auto a_path = ap00->absolutePath();
						if (DfxPropertyConduit::getInstance()->hasKeyframeOverride(a_path)) {
							auto chan = newChannel(level + 1, a_path);
							chan->setText(ap00->label().c_str());
							chan->setChannelParent(chanParent);
							chanParent->addChildChannel(chan);
						}
					}
				}

			}

			if (chanParent != nullptr) chanParent->expandCollapse();
		}
	}
}

DfxTimelineChannelWidget *DfxTimelineEditor::newChannel(int lvl, std::string path) {

	DfxTimelineTrackWidget *track = new DfxTimelineTrackWidget(frameScale, path);
	DfxTimelineChannelWidget *chan = new DfxTimelineChannelWidget(lvl);

	chan->setTrack(track);
	track->setChannel(chan);

	channelLayout->addWidget(chan);
	keyTrackLayout->addWidget(track);

	_selectedChannels.push_back(chan);

	chan->channelLayoutPtr = channelLayout;
	chan->trackLayoutPtr = keyTrackLayout;

	QObject::connect(this, &DfxTimelineEditor::frameScaleChanged, track, &DfxTimelineTrackWidget::updateFrameScale);

	track->update();

	return chan;
}

void DfxTimelineEditor::clearChannels() {

	for (auto chan : _selectedChannels) {
		auto track = chan->track();

		keyTrackLayout->removeWidget(track);
		delete track;

		channelLayout->removeWidget(chan);
		delete chan;
	}

	delete channelLayout->takeAt(0);
	delete channelLayout->takeAt(0);

	_selectedChannels.clear();
}

void DfxTimelineEditor::refreshModel() {
	clearChannels();

	channelLayout->addSpacing(constants::header_height);
	if (DfxMode::getInstance()->mode() == DfxMode::Mode::OBJECT_EDIT) {

		if (!DfxSceneSelectionModel::getInstance()->selected().empty()) {

			for (auto node : DfxSceneSelectionModel::getInstance()->selected()) {

				auto any = DfxSpace::getInstance()->findAbsolute(node->referencePath());

				auto k_paths = findKeyFramePaths(any, 0);
				
				importDataObject(any, 0, k_paths);
				resize(geometry().width() + 1, geometry().height() + 1);
				resize(geometry().width() - 1, geometry().height() - 1);

			}
		}
	}
	channelLayout->addSpacing(10);
}

void DfxTimelineEditor::setPosRotSclMode() {
	DfxSpace::getInstance()
		->findAbsolute("~/KEY_OPTIONS/keyable")
		->setData<int>(DfxKeyableTypes::LOC_ROT_SCL);
}

void DfxTimelineEditor::setPosMode() {
	DfxSpace::getInstance()
		->findAbsolute("~/KEY_OPTIONS/keyable")
		->setData<int>(DfxKeyableTypes::LOC);
}

void DfxTimelineEditor::setRotMode() {
	DfxSpace::getInstance()
		->findAbsolute("~/KEY_OPTIONS/keyable")
		->setData<int>(DfxKeyableTypes::ROT);
}

void DfxTimelineEditor::setSclMode() {
	DfxSpace::getInstance()
		->findAbsolute("~/KEY_OPTIONS/keyable")
		->setData<int>(DfxKeyableTypes::SCL);
}

void DfxTimelineEditor::frameScaleSliderChanged(int value) {
	frameScale = value;
	emit frameScaleChanged(value);
	key_scroll->resize(key_scroll->size().width()+1, key_scroll->size().height()+1);
	key_scroll->resize(key_scroll->size().width()-1, key_scroll->size().height()-1);
}

void DfxTimelineEditor::hideShowNoneRealtimeControls() {

	if (DfxMode::getInstance()->animationMode() == DfxMode::AnimationMode::SIMULATION) {
		start->show();
		resetSim->show();
		previousFrame->hide();
		nextFrame->hide();
		end->show();
		createKey->hide();
		keyMenu->hide();
		deleteKey->hide();
		realtimeMode->setChecked(true);
	}
	else if(DfxMode::getInstance()->animationMode() == DfxMode::AnimationMode::KEYFRAME){
		resetSim->hide();
		start->show();
		previousFrame->show();
		nextFrame->show();
		end->show();
		createKey->show();
		keyMenu->show();
		deleteKey->show();
		realtimeMode->setChecked(false);
	}
}

void DfxTimelineEditor::toggleRealtimeMode(bool checked) {
	if (DfxMode::getInstance()->animationMode() == DfxMode::AnimationMode::KEYFRAME) {
		DfxMode::getInstance()->setAnimationMode(DfxMode::AnimationMode::SIMULATION);
	}
	else if (DfxMode::getInstance()->animationMode() == DfxMode::AnimationMode::SIMULATION) {
		DfxMode::getInstance()->setAnimationMode(DfxMode::AnimationMode::KEYFRAME);
	}
}

void DfxTimelineEditor::stylePlayButtons() {
	QString leftButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"border-bottom-left-radius: 4px;"
		"border-top-left-radius: 4px;"
		"padding-left: 2px;"
		"height: 16px;"
		"}";

	QString ButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"padding-left: 2px;"
		"height: 16px;"
		"}";

	QString rightButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"border-bottom-right-radius: 4px;"
		"border-top-right-radius: 4px;"
		"padding-left: 2px;"
		"height: 16px;"
		"}";

	QString hoverSty =
		"QPushButton:hover { "
		"background-color: rgb(100,100,100);"
		"}";

	QString hoverPressdSty =
		"QPushButton:hover:pressed { "
		"background-color: rgb(255, 153, 83);"
		"}";

	QString keyButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border-top-left-radius: 4px;"
		"border-bottom-left-radius: 4px;"
		"}";

	QString keyMenuButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border-top-right-radius: 4px;"
		"border-bottom-right-radius: 4px;"
		"width: 11px;"
		"height: 16px;"
		"}";

	QString realtimeButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"border-radius: 4px;"
		"padding-left: 2px;"
		"height: 16px;"
		"}";

	QString realtimeButtonCheckedSty =
		"QPushButton:checked { "
		"background-color: green;"
		"}";

	start->setStyleSheet(leftButtonSty + hoverSty + hoverPressdSty);
	previousFrame->setStyleSheet(ButtonSty + hoverSty + hoverPressdSty);
	resetSim->setStyleSheet(ButtonSty + hoverSty + hoverPressdSty);
	stop->setStyleSheet(ButtonSty + hoverSty + hoverPressdSty);
	play->setStyleSheet(ButtonSty + hoverSty + hoverPressdSty);
	nextFrame->setStyleSheet(ButtonSty + hoverSty + hoverPressdSty);
	end->setStyleSheet(rightButtonSty + hoverSty + hoverPressdSty);
	createKey->setStyleSheet(keyButtonSty + hoverSty + hoverPressdSty);
	deleteKey->setStyleSheet(ButtonSty + hoverSty + hoverPressdSty);
	keyMenu->setStyleSheet(keyMenuButtonSty + hoverSty + hoverPressdSty);
	realtimeMode->setStyleSheet(realtimeButtonSty + hoverSty + hoverPressdSty + realtimeButtonCheckedSty);
	recordCache->setStyleSheet(realtimeButtonSty + hoverSty + hoverPressdSty + realtimeButtonCheckedSty);
}

DfxTimelineEditor::TimeObserver::TimeObserver() {}

void DfxTimelineEditor::TimeObserver::update(fl::Any *any) {

	if (any->name() == "frame_start") {
		auto cs = DfxTimelineManager::getInstance()->currentFrame();
		auto fs = any->data<int>();

		if (fs > cs) {
			DfxTimelineManager::getInstance()->goToFrame(fs);
		}
	}

	if (any->name() == "frame_end") {
		auto cs = DfxTimelineManager::getInstance()->currentFrame();
		auto es = any->data<int>();

		if (es < cs) {
			DfxTimelineManager::getInstance()->goToFrame(es);
		}
	}

	ptr->keyHeader->update();
}

void DfxTimelineEditor::_goToNextFrame() {
	DfxMode::getInstance()->setTimelineMode(DfxMode::TimelineMode::UPDATING);
	DfxTimelineManager::getInstance()->goToNextFrame();
	DfxMode::getInstance()->setTimelineMode(DfxMode::TimelineMode::NONE);
}

void DfxTimelineEditor::_goToPreviousFrame() {
	DfxMode::getInstance()->setTimelineMode(DfxMode::TimelineMode::UPDATING);
	DfxTimelineManager::getInstance()->goToPreviousFrame();
	DfxMode::getInstance()->setTimelineMode(DfxMode::TimelineMode::NONE);
}

void DfxTimelineEditor::_goToFirstFrame() {
	DfxMode::getInstance()->setTimelineMode(DfxMode::TimelineMode::UPDATING);
	DfxTimelineManager::getInstance()->goTofirstFrame();
	DfxMode::getInstance()->setTimelineMode(DfxMode::TimelineMode::NONE);
}

void DfxTimelineEditor::_goToLastFrame() {
	DfxMode::getInstance()->setTimelineMode(DfxMode::TimelineMode::UPDATING);
	DfxTimelineManager::getInstance()->goToLastFrame();
	DfxMode::getInstance()->setTimelineMode(DfxMode::TimelineMode::NONE);
}

void CustomScrollArea::resizeEvent(QResizeEvent *event) {

	if (editorPtr->ready) {

		QWidget *temp00 = takeWidget();
		QWidget *temp01 = editorPtr->keyHeader_scroll->takeWidget();

		editorPtr->keyHeader->setTrackWidth(width());
		for (int i = 0; i < editorPtr->keyTrackLayout->count(); i++) {
			auto item = editorPtr->keyTrackLayout->itemAt(i);
			
			if (item->widget() != nullptr) {
			 ((DfxTimelineTrackWidget*)item->widget())->setTrackWidth(width());
			}
		}

		temp00->resize(editorPtr->keyHeader->length(), temp00->height());
		temp01->resize(editorPtr->keyHeader->length(), constants::header_height);

		setWidget(temp00);
		editorPtr->keyHeader_scroll->setWidget(temp01);

		editorPtr->frameIndicator->setFixedWidth(editorPtr->keyHeader->length());
		editorPtr->trackOverlay->resize(editorPtr->key_scroll->width(), editorPtr->key_scroll->height() + constants::row_height);
	}

	QScrollArea::resizeEvent(event);
}

CustomScrollArea::CustomScrollArea(DfxTimelineEditor *editor) {
	editorPtr = editor;
}

