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


#include "DfxTimelineChannelWidget.h"

DfxTimelineChannelWidget::DfxTimelineChannelWidget(int lvl, QWidget *parent): QWidget(parent) {

	layout = new QHBoxLayout;
	mainlayout = new QHBoxLayout;

	label = new QLabel("Channel 00");
	check = new QCheckBox();

	treeHandle = new QPushButton(QPixmap(":/icon/spot_icon").scaled(3, 3), "");

	controlWidget = new QWidget;

	_parent = nullptr;
	_track  = nullptr;

	collapsed = false;

	//layout->addWidget(check);
	//layout->addWidget(treeHandle);
	layout->addWidget(label);
	layout->setContentsMargins(3, 0, 0, 0);

	mainlayout->addWidget(treeHandle);
	mainlayout->addWidget(controlWidget);
	mainlayout->setContentsMargins(lvl * 15, 0, 0, 0);
	mainlayout->setSpacing(3);
	
	setFixedHeight(constants::row_height);
	treeHandle->setFixedWidth(12);
	treeHandle->setFixedHeight(constants::row_height);

	QString buttonStyle = "QPushButton { border: 0px; background: rgba(102, 102, 102, 0); padding-left:5px;} ";
	/*QString buttonStyleHover = "QPushButton:hover:!pressed  { border: 0px; background: rgb(150, 150, 150); } ";
	QString buttonStylePressed = "QPushButton:hover:pressed  { border: 0px;  } ";*/
	treeHandle->setStyleSheet(buttonStyle);

	/*treeHandle->setStyleSheet(
		"QPushButton{"
		"background-color:rgb(44, 44, 44);"
		"border-width: 0px;"
		"}");*/

	controlWidget->setStyleSheet(
		"background-color:rgb(44, 44, 44);"
		"border-color:rgb(60, 60, 60);"
		"padding-left: 5px;"
		"border-width: 1px;"
		"border-style: solid;"
		"border-top-left-radius: 5px;"
		"border-bottom-left-radius: 5px;"
		"border-width: 0px;");

	_level = lvl;

	controlWidget->setLayout(layout);

	QObject::connect(treeHandle, &QPushButton::clicked, this, &DfxTimelineChannelWidget::expandCollapse);

	setLayout(mainlayout);
}

DfxTimelineChannelWidget::~DfxTimelineChannelWidget() {

}

void DfxTimelineChannelWidget::paintEvent(QPaintEvent *event) {

}

void DfxTimelineChannelWidget::setText(QString str) {
	label->setText(str);
}

DfxTimelineChannelWidget *DfxTimelineChannelWidget::channelParent() {
	return _parent;
}

void DfxTimelineChannelWidget::setChannelParent(DfxTimelineChannelWidget *pt) {
	_parent = pt;
}

DfxTimelineTrackWidget *DfxTimelineChannelWidget::track() {
	return _track;
}

void DfxTimelineChannelWidget::setTrack(DfxTimelineTrackWidget * trk) {
	_track = trk;
}

void DfxTimelineChannelWidget::addChildChannel(DfxTimelineChannelWidget *chd) {
	if(_children.empty()) treeHandle->setIcon(QPixmap(":/icon/spinner_down").scaled(7, 6));
	_children.push_back(chd);
}

DfxTimelineChannelWidget *DfxTimelineChannelWidget::getChildChannel(int inx) {
	return _children[inx];
}
std::vector<DfxTimelineChannelWidget*> DfxTimelineChannelWidget::channelChildren() {
	return _children;
}

void DfxTimelineChannelWidget::hideTrack(bool val) {

	if (val) {
		_track->hide();
	}
	else {
		_track->show();
	}
}

void DfxTimelineChannelWidget::hideChannel(bool val) {

	if (val) {
		if (!_children.empty()) {
			for (auto chd : _children) {
				chd->hideChannel(true);
				chd->hideTrack(true);
				chd->hide();
				treeHandle->setIcon(QPixmap(":/icon/spinner_right").scaled(6, 7));
			}
		}
	}
	else {
		if (!_children.empty()) {
			for (auto chd : _children) {
				chd->hideChannel(false);
				chd->hideTrack(false);
				chd->show();
				_track->show();
				treeHandle->setIcon(QPixmap(":/icon/spinner_down").scaled(7, 6));
			}
		}
	}
}

void DfxTimelineChannelWidget::setRelevantPaths(std::vector<std::string> rpaths) {
	_track->setRelevantPaths(rpaths);
	_track->setReflect(true);
}

void DfxTimelineChannelWidget::expandCollapse() {

	if (collapsed) {
		hideChannel(false);
		collapsed = false;
	}
	else {
		hideChannel(true);
		collapsed = true;
	}
}