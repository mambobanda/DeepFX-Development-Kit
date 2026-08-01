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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxQKeyCheckBox.h"
#include "DfxPropertyConduit.h"
#include "DfxTimelineManager.h"
#include "DfxSpace.h"
#include "DfxPropertyConduit.h"

DfxQKeyCheckBox::DfxQKeyCheckBox(std::string pth, QWidget *parent):QCheckBox(parent) {

	_timeObserver = new TimeObserver(this);
	_pathObserver = new PathObserver(this);

	_path = pth;

	updateState();

	_ignoreSignals = false;

	DfxTimelineManager::getInstance()->addCurrentFrameObserver(_timeObserver);
	setStyleSheet(
		"QCheckBox::indicator"
		"{"
		"border : 1px solid white;"
		"width : 6px;"
		"height : 6px;"
		"border-radius : 4px;"
		"}"
	);

	setStyleSheet(styleSheet() +
		"QCheckBox::indicator:checked"
		"{"
		"background-color: orange;"
		"}"
	);

	QObject::connect(DfxPropertyConduit::getInstance(), &DfxPropertyConduit::keyFrameUpdated, this, &DfxQKeyCheckBox::pathUpdated);
}

DfxQKeyCheckBox::~DfxQKeyCheckBox() {
	delete _timeObserver;
	delete _pathObserver;
}

void DfxQKeyCheckBox::pathUpdated(std::string pth) {
	if (pth == _path) {
		updateState();
	}
}

std::string DfxQKeyCheckBox::path() {
	return _path;
}

void DfxQKeyCheckBox::setPath(std::string pth) {
	_path = pth;
}

bool DfxQKeyCheckBox::ignoreSignals() {
	return _ignoreSignals;
}

void DfxQKeyCheckBox::updateState() {
	auto cframe = DfxTimelineManager::getInstance()->currentFrame();

	auto vany = DfxSpace::getInstance()->findAbsolute(_path);

	if (vany != nullptr) {
		if (vany->type() == fl::type::FLOAT) {
			auto keyMap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(_path);

			if (keyMap != nullptr) {
				if (keyMap->hasKey(cframe)) {
					if (!isChecked()) {
						_ignoreSignals = true;
						setChecked(true);
						QApplication::processEvents();
						_ignoreSignals = false;

					}
					return;
				}
			}
		}
	}

	if (isChecked()) {
		_ignoreSignals = true;
		setChecked(false);
		QApplication::processEvents();
		_ignoreSignals = false;
	}
}

DfxQKeyCheckBox::TimeObserver::TimeObserver(DfxQKeyCheckBox *pt) {
	ptr = pt;
}

void DfxQKeyCheckBox::TimeObserver::update(fl::Any *any) {
	
	if (any->name() == "current_frame") {
		ptr->updateState();
	}

}

DfxQKeyCheckBox::PathObserver::PathObserver(DfxQKeyCheckBox *pt) {

}

void DfxQKeyCheckBox::PathObserver::update(fl::Any *any) {

}