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

#include "DfxQDecimalSpinnerControl.h"
#include "DfxPropertyConduit.h"
#include "DfxTimelineManager.h"
#include "DfxGraphEditorManager.h"

DfxQDecimalSpinnerControl::~DfxQDecimalSpinnerControl() {
	delete _observer;
	setStyleSheet("background-color: transparent;");
}

void DfxQDecimalSpinnerControl::styleSpinner(QDoubleSpinBox *spinner, bool keyFramed) {

	if (!keyFramed) {
		spinner->setStyleSheet(
			"QDoubleSpinBox { "
			"color: white;"
			"background-color: rgb(70,70,70);"
			"border: 0px solid;"
			"border-radius: 5px;"
			"padding-left: 2px;"
			"padding-top: 2px;"
			"}");
	}else{
		spinner->setStyleSheet(
			"QDoubleSpinBox { "
			"color: white;"
			"background-color: rgb(0,70, 0);"
			"border: 0px solid;"
			"border-radius: 5px;"
			"padding-left: 2px;"
			"padding-top: 2px;"
			"}");
	}

	spinner->setStyleSheet(spinner->styleSheet() +
		"QDoubleSpinBox:hover { "
		"border-width: 1px;"
		"border-style: solid;"
		"border-color: rgb(255,127,42);"
		"}");

	spinner->setStyleSheet(spinner->styleSheet() +
		"QDoubleSpinBox:focus { "
		"border-width: 1px;"
		"border-style: solid;"
		"border-color: rgb(255,127,42);"
		"}");

	spinner->setStyleSheet(spinner->styleSheet() +
		"QDoubleSpinBox::down-button {"
		"subcontrol-origin: margin;"
		"subcontrol-position: center left;"
		"image: url(:/icon/spinner_left);"
		"padding-left: 6px;"
		"height: 8px;"
		"width: 5px;"
		"}");

	spinner->setStyleSheet(spinner->styleSheet() +
		"QDoubleSpinBox::up-button {"
		"subcontrol-origin: margin;"
		"subcontrol-position: center right;"
		"image: url(:/icon/spinner_right);"
		"padding-right: 6px;"
		"height: 8px;"
		"width: 5px;"
		"}");
}

DfxQDecimalSpinnerControl::DfxQDecimalSpinnerControl(std::string title) {

	_layout = new QHBoxLayout();
	_label = nullptr;
	_spinner = new CustomSpinner();

	_layout->setMargin(0);

	_spinner->setRange(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	_spinner->setDecimals(6);
	_spinner->setSingleStep(.1);
	_spinner->setAlignment(Qt::AlignCenter);

	styleSpinner(_spinner);

	_maxValue = std::numeric_limits<float>::max();
	_minValue = std::numeric_limits<float>::lowest();
}

DfxQDecimalSpinnerControl::DfxQDecimalSpinnerControl(std::string vpath, std::string flabel, bool useLabel, bool keyable, QWidget *parent):DfxBaseControl(parent) {

	_layout = new QHBoxLayout();
	_label = nullptr;
	_spinner = new CustomSpinner();
	_keyToggle = new DfxQKeyCheckBox(vpath,this);
	_layout->setMargin(0);

	_spinner->setRange(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	_spinner->setDecimals(6);
	_spinner->setSingleStep(.1);
	_spinner->setAlignment(Qt::AlignCenter);

	_keyToggle->setFixedWidth(15);

	std::string label = "Control";
	setValuePath(vpath);
	_fixedLabel = flabel;

	auto any = DfxSpace::getInstance()->findAbsolute(vpath);

	if (any != nullptr) {
		label = any->label();
		_spinner->setValue(any->data<float>());
		_observer = new FloatObserver(_spinner);
		any->addObserver(_observer);
	}

	if (_fixedLabel.length() > 0) label = _fixedLabel;

	
	if (useLabel) {
		_label = new QLabel((label + ":").c_str(), this);
		_layout->addWidget(_label);
	}

	_layout->addWidget(_spinner);
	if (keyable) _layout->addWidget(_keyToggle);
	
	setLayout(_layout);

	_maxValue = std::numeric_limits<float>::max();
	_minValue = std::numeric_limits<float>::lowest();

	updateState();

	QObject::connect(_spinner, &QDoubleSpinBox::editingFinished, this, &DfxQDecimalSpinnerControl::updatePath);
	QObject::connect(DfxPropertyConduit::getInstance(), &DfxPropertyConduit::keyFrameUpdated, this, &DfxQDecimalSpinnerControl::pathUpdated);
	QObject::connect(_keyToggle, &DfxQKeyCheckBox::stateChanged, this, &DfxQDecimalSpinnerControl::updateKey);
}

void DfxQDecimalSpinnerControl::updatePath() {

	auto any = DfxSpace::getInstance()->findAbsolute(valuePath());
	auto cframe = DfxTimelineManager::getInstance()->currentFrame();

	if (any != nullptr) {
		
		if (any->type() == fl::type::FLOAT) {

			float oldVal = any->data<float>();
			float val = _spinner->value();

			auto historyAction = new DfxAnyHistoryAction;
			historyAction->setValue<float>(fl::type::FLOAT, any->absolutePath(), val, any->data<float>());
			DfxHistoryManager::getInstance()->pushHistoryAction(historyAction);

			auto keyMap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(valuePath());

			if (keyMap != nullptr) {
				if (keyMap->hasKey(cframe)) {
					auto points = keyMap->controlPoints(cframe);
					
					if (val > oldVal) {
						float diff = val - oldVal;
						points.first.y = points.first.y + diff;
						points.second.y = points.second.y + diff;
					}
					else {
						float diff = oldVal - val;
						points.first.y = points.first.y - diff;
						points.second.y = points.second.y - diff;
					}

					DfxPropertyConduit::getInstance()->updateKeyframeFloatOverride(valuePath(), cframe, val);
					keyMap->updateControlPoints(cframe, points);

					DfxGraphEditorManager::getInstance()->notifyChannelUpdated();
				}
			}		

			any->setData<float>(val);
			any->notifyObservers(_observer);

			DfxGlobal::command::exec("updateScene");

		}
	}
}

void DfxQDecimalSpinnerControl::pathUpdated(std::string pth) {
	if (pth == valuePath()) {
		updateState();
	}
}

void DfxQDecimalSpinnerControl::setValue(float value) {
	_spinner->setValue(value);
	updatePath();
}

float DfxQDecimalSpinnerControl::maxValue() {
	return _maxValue;
}

void DfxQDecimalSpinnerControl::setMaxValue(float mx) {
	_maxValue = mx;
	_spinner->setMaximum(mx);
}

float DfxQDecimalSpinnerControl::minValue() {
	return _minValue;
}

void DfxQDecimalSpinnerControl::setMinValue(float mi) {
	_minValue = mi;
	_spinner->setMinimum(mi);
}

float DfxQDecimalSpinnerControl::minMax(float val) {
	val = std::min(_maxValue, val);
	val = std::max(_minValue, val);

	return val;
}

void DfxQDecimalSpinnerControl::updateState() {
	auto vany = DfxSpace::getInstance()->findAbsolute(valuePath());

	if (vany != nullptr) {
		if (vany->type() == fl::type::FLOAT) {
			auto keyMap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(valuePath());

			if (keyMap != nullptr) {
				styleSpinner(_spinner, true);
				return;
			}
		}
	}

	styleSpinner(_spinner);

}

void DfxQDecimalSpinnerControl::updateKey(int state) {
	auto cframe = DfxTimelineManager::getInstance()->currentFrame();

	bool newChannelCreated = false;

	if (!_keyToggle->ignoreSignals()) {
		if (_keyToggle->isChecked()) {
			
			auto val = DfxSpace::getInstance()->findAbsolute(valuePath())->data<float>();

			if (!DfxPropertyConduit::getInstance()->hasKeyframeOverride(valuePath())) {
				DfxPropertyConduit::getInstance()->createKeyframeFloatOverride(valuePath(), val);
				newChannelCreated = true;
			}

			DfxPropertyConduit::getInstance()->updateKeyframeFloatOverride(valuePath(), cframe, val);

			DfxPropertyConduit::getInstance()->announceNewKeyframeCreated();
		}
		else {

			if (DfxPropertyConduit::getInstance()->hasKeyframeOverride(valuePath())) {
				DfxPropertyConduit::getInstance()->removeKeyframeFloatOverride(valuePath(), cframe);
			}

			if (DfxPropertyConduit::getInstance()->floatKeyframeOverrideSize(valuePath()) == 0) {
				DfxPropertyConduit::getInstance()->destroyframeFloatOverride(valuePath());
				newChannelCreated = true;
			}

			DfxPropertyConduit::getInstance()->announceNewKeyframeCreated();
		}
	}

	if (newChannelCreated) {
		DfxGraphEditorManager::getInstance()->selectionChanged();
	}
	else {
		DfxGraphEditorManager::getInstance()->notifyChannelUpdated();
	}

}

DfxQDecimalSpinnerControl::FloatObserver::FloatObserver(QDoubleSpinBox *sp) {
	spinner = sp;
}

void DfxQDecimalSpinnerControl::FloatObserver::update(fl::Any *any) {

	if (any != nullptr) {
		float val = any->data<float>();
		spinner->setValue(any->data<float>());

	}
}

