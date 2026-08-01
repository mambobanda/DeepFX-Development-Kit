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
#include "DfxTimelineTrackWidget.h"
#include "DfxGraphEditorManager.h"
#include "DfxPropertyConduit.h"


DfxTimelineTrackWidget::DfxTimelineTrackWidget(int fs, std::string kpath, QWidget *parent) :QWidget(parent) {

	_minFrameScale = fs;

	_trackWidth  = 200;
	_trackHeight = 100;

	_keyFramePath = kpath;

	setFixedHeight(constants::row_height);

	_reflect = false;

	_ghostMode = false;
	_ghostDisplacement = 0;

	setStyleSheet(
		"QWidget{"
		"border-bottom: 2px solid rgb(70,70,70);"
		"background-color: rgba(99,99,99,0);"
		"}");

	QObject::connect(DfxGraphEditorManager::getInstance(), &DfxGraphEditorManager::channelUpdated, this, &DfxTimelineTrackWidget::channelUpdated);
	QObject::connect(DfxPropertyConduit::getInstance(), &DfxPropertyConduit::keyFrameUpdated, this, &DfxTimelineTrackWidget::keyframeUpdated);

}

DfxTimelineTrackWidget::~DfxTimelineTrackWidget() {

}

void DfxTimelineTrackWidget::setTrackWidth(int w) {
	_trackWidth = w;
}

int  DfxTimelineTrackWidget::trackWidth() const{
	return _trackWidth;
}

void DfxTimelineTrackWidget::setTrackHeight(int h) {
	_trackHeight = h;
}

int DfxTimelineTrackWidget::trackHeight() const{
	return _trackHeight;
}

std::vector<int> DfxTimelineTrackWidget::selectedKeys() {
	std::vector<int> sel;

	for (auto p : _selected_keys) {
		sel.push_back(p.first);
	}

	return sel;
}

std::vector<int> DfxTimelineTrackWidget::keys() {

	if (!_keyFramePath.empty()) {
		auto kmap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(_keyFramePath);

		if (kmap != nullptr) return kmap->keys();
	}

	return std::vector<int>();
}

void DfxTimelineTrackWidget::setChannel(DfxTimelineChannelWidget *chn) {
	_channel = chn;
}

void DfxTimelineTrackWidget::getAll(DfxTimelineChannelWidget *chan, std::vector<DfxTimelineTrackWidget*> &arr) {	

	for (auto child : chan->channelChildren()) {

		if (!child->track()->keyframePath().empty()) {
			arr.push_back(child->track());
		}
		getAll(child, arr);
		
	}

}

std::unordered_map<int, bool> DfxTimelineTrackWidget::findAllKeys() {
	std::unordered_map<int, bool> keys;
	auto xchildren = findAll();

	if (!xchildren.empty()) {

		for (auto track : xchildren) {

			for (auto k : track->keys()) {
				auto itr = keys.find(k);

				if (itr != keys.end()) {
					auto selKeys = track->selectedKeys();
					if (std::find(selKeys.begin(), selKeys.end(), k) != selKeys.end()) {
						itr->second = true;
					}
				}
				else {
					keys[k] = false;

					auto selKeys = track->selectedKeys();
					if (std::find(selKeys.begin(), selKeys.end(), k) != selKeys.end()) {
						keys[k] = true;
					}
				}
			}
		}
	}

	return keys;
}

std::vector<DfxTimelineTrackWidget*> DfxTimelineTrackWidget::findAll() {
	std::vector<DfxTimelineTrackWidget*> arr;

	getAll(_channel, arr);

	return arr;
}

void DfxTimelineTrackWidget::addSelectedKey(int k) {
	_selected_keys[k] = 0;
}

void DfxTimelineTrackWidget::paintKeyFrames() {

	QPainter painter(this);
	QBrush  selection_brush(QColor(252,192,21));
	QBrush  ghost_brush(QColor(0, 196, 255, 255));
	QBrush  defualt_brush(QColor(150, 150, 150));

	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::HighQualityAntialiasing);

	auto _startFrame = DfxTimelineManager::getInstance()->firstFrame();
	

	if (!_keyFramePath.empty()) {
		
		auto kmap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(_keyFramePath);

		if (kmap != nullptr) {
			int stepSize = trackWidth() / _minFrameScale;
		
			for (auto k : kmap->keys()) {
				bool drawGhostMode = false;
				int xpos = stepSize * ((k + 1) - _startFrame);
				int d_xpos = stepSize * (((k + _ghostDisplacement) + 1) - _startFrame);
				
				if (_selected_keys.find(k) != _selected_keys.end()) {
					painter.setBrush(selection_brush);

					if (isGhostMode()) drawGhostMode = true;
				}
				else painter.setBrush(defualt_brush);
				
				painter.drawRect(xpos, 0, (stepSize * .45), constants::row_height - 5);

				if (drawGhostMode) {
					painter.setBrush(ghost_brush);
					painter.drawRect(d_xpos, 0, (stepSize * .45), constants::row_height - 5);
				}
			}
		}
	}
	else if (_reflect) {

		auto keys = findAllKeys();

		if (!keys.empty()) {

			int stepSize = trackWidth() / _minFrameScale;
	
			for (auto k : keys) {
				bool drawGhostMode = false;
				int xpos = stepSize * ((k.first + 1) - _startFrame);
				int d_xpos = stepSize * (((k.first + _ghostDisplacement) + 1) - _startFrame);

				if (k.second) {
					painter.setBrush(selection_brush);
					if (isGhostMode()) drawGhostMode = true;
				}
				else {
					painter.setBrush(defualt_brush);
				}

				painter.drawRect(xpos, 0, (stepSize * .45), constants::row_height - 5);

				if (drawGhostMode) {
					painter.setBrush(ghost_brush);
					painter.drawRect(d_xpos, 0, (stepSize * .45), constants::row_height - 5);
				}
			}
		}
	}
}

void DfxTimelineTrackWidget::paintEvent(QPaintEvent *event) {

	//int frameCount = _endFrame - _startFrame;
	int stepSize = trackWidth() / _minFrameScale;
	int pheight = constants::row_height;

	QPainter painter(this);
	painter.setBrush(Qt::NoBrush);
	painter.setPen(QColor(50, 50, 50));

	auto _startFrame = DfxTimelineManager::getInstance()->firstFrame();
	auto _endFrame = DfxTimelineManager::getInstance()->lastFrame();

	//int pwidth = size().width();
	int frame_counter = _startFrame;
	for (int inx = 1; frame_counter <= _endFrame; inx++) {

		int tHeight = constants::row_height;

		painter.drawLine(inx * stepSize, pheight, inx * stepSize, pheight - tHeight);

		frame_counter++;
	}

	paintKeyFrames();
	
	QWidget::paintEvent(event);
}

bool DfxTimelineTrackWidget::isSelected(int frame) {
	
	if (_selected_keys.find(frame) != _selected_keys.end()) return true;
	else return false;

	return false;
}

void DfxTimelineTrackWidget::moveSelection(int from, int to) {
	auto itr = _selected_keys.find(from);

	if (itr != _selected_keys.end()) {
		_selected_keys[to] = itr->second;
		_selected_keys.erase(itr);
	}
}

bool DfxTimelineTrackWidget::alreadySelected(glm::ivec2 pos) {

	auto _startFrame = DfxTimelineManager::getInstance()->firstFrame();
	auto _endFrame = DfxTimelineManager::getInstance()->lastFrame();

	int stepSize = trackWidth() / _minFrameScale;
	int offset = (pos.x / stepSize) - 1;
	int frame = std::max(std::min(_startFrame + offset, _endFrame), _startFrame);

	if (!_keyFramePath.empty()) {

		if (_selected_keys.find(frame) != _selected_keys.end()) return true;
		else return false;
	}
	else if (_reflect) {
		auto keys = findAllKeys();

		if (!keys.empty()) {
			for (auto k : keys) {
				if (k.first == frame && k.second) return true;
			}
		}
	}

	return false;
}

int DfxTimelineTrackWidget::frameFromPosition(glm::ivec2 pos) {

	auto _startFrame = DfxTimelineManager::getInstance()->firstFrame();
	auto _endFrame = DfxTimelineManager::getInstance()->lastFrame();

	int stepSize = trackWidth() / _minFrameScale;
	int offset = (pos.x / stepSize) - 1;
	int frame = std::max(std::min(_startFrame + offset, _endFrame), _startFrame);

	return frame;
}

void DfxTimelineTrackWidget::setReflect(bool val) {
	_reflect = val;
}

void DfxTimelineTrackWidget::setRelevantPaths(std::vector<std::string> rp) {
	_relevantPaths = rp;
}

void DfxTimelineTrackWidget::setMinFrameScale(int scl) {
	_minFrameScale = scl;
}

int DfxTimelineTrackWidget::minScaleFrame() {
	return _minFrameScale;
}

QSize DfxTimelineTrackWidget::sizeHint() const {
	auto _endFrame = DfxTimelineManager::getInstance()->lastFrame();

	int stepSize = trackWidth() / _minFrameScale;
	return QSize((stepSize * (_endFrame)) + (stepSize * 3), constants::row_height);
}

void DfxTimelineTrackWidget::eventProxy(QEvent *evt) {
	event(evt);
}

std::string DfxTimelineTrackWidget::keyframePath() {
	return _keyFramePath;
}

void DfxTimelineTrackWidget::mouseMoveEvent(QMouseEvent *event) {
	DfxGlobal::console::println("Track move event...");
}

void DfxTimelineTrackWidget::mousePressEvent(QMouseEvent *event) {
}

void DfxTimelineTrackWidget::mouseReleaseEvent(QMouseEvent *event) {
	DfxGlobal::console::println("Track Mouse clicked...");
}

void DfxTimelineTrackWidget::updateFrameScale(int value) {
	_minFrameScale = value;
	update();
}

void DfxTimelineTrackWidget::clearSelection() {
	_selected_keys.clear();
}

void DfxTimelineTrackWidget::setGhostMode(bool bl) {
	_ghostMode = bl;
}

bool DfxTimelineTrackWidget::isGhostMode() {
	return _ghostMode;
}

int DfxTimelineTrackWidget::ghostDisplacement() {
	return _ghostDisplacement;
}

void DfxTimelineTrackWidget::setGhostDisplacement(int disp) {
	_ghostDisplacement = disp;
}

bool DfxTimelineTrackWidget::selectKeyFrame(glm::ivec2 pos, int &key, bool toggle) {
	auto _startFrame = DfxTimelineManager::getInstance()->firstFrame();
	auto _endFrame = DfxTimelineManager::getInstance()->lastFrame();

	int stepSize = trackWidth() / _minFrameScale;
	int offset = (pos.x / stepSize) - 1;
	int frame = std::max(std::min(_startFrame + offset, _endFrame), _startFrame);

	if (!_keyFramePath.empty()) {

		if (!_keyFramePath.empty()) {
			auto kmap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(_keyFramePath);

			if (kmap != nullptr) {
				if (kmap->hasKey(frame)) {
					if (toggle) {
						auto k_itr = _selected_keys.find(frame);

						if (k_itr != _selected_keys.end()) {
							_selected_keys.erase(k_itr);
						}
						else {
							_selected_keys[frame] = 0;
						}
					}
					else {
						_selected_keys[frame] = 0;
					}
					return true;
				}
			}
		}
	}
	else if (_reflect) {
		auto xchildren = findAll();
		bool success = false;
		if (!xchildren.empty()) {

			for (auto track : xchildren) {

				for (auto k : track->keys()) {

					if (k == frame) {
						track->addSelectedKey(frame);
						success = true;
					}				
				}
			}
		}

		return success;
	}

	return false;
}

bool DfxTimelineTrackWidget::selectKeyFrames(glm::ivec2 min, glm::ivec2 max, std::vector<int> &keys) {

	int minx = std::min(min.x, max.x);
	int maxx = std::max(min.x, max.x);

	int miny = std::min(min.y, max.y);
	int maxy = std::max(min.y, max.y);

	auto _startFrame = DfxTimelineManager::getInstance()->firstFrame();
	auto _endFrame = DfxTimelineManager::getInstance()->lastFrame();

	int stepSize = trackWidth() / _minFrameScale;
	
	if (!_keyFramePath.empty()) {

		auto kmap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(_keyFramePath);
		for (auto k : kmap->keys()) {
			int minH = geometry().y();
			int maxH = geometry().y() + geometry().height();

			if ((miny <= minH && maxy >= maxH) ||
				(miny >= minH && miny <= maxH) ||
				(maxy >= minH && maxy <= maxH)) {
				
				int minStep = stepSize * k;
				int maxStep = minStep + stepSize;

				if ((minStep >= minx && maxStep <= maxx) ||
					(minx >= minStep && minx <= maxStep) ||
					(maxx >= minStep && maxx <= maxStep)) {

					_selected_keys[k] = 0;
				}
			}
			
		}
	}
	else if (_reflect) {
		auto xchildren = findAll();
		bool success = false;
		if (!xchildren.empty()) {

			for (auto track : xchildren) {

				for (auto k : track->keys()) {

					int minH = geometry().y();
					int maxH = geometry().y() + geometry().height();

					if ((miny <= minH && maxy >= maxH) ||
						(miny >= minH && miny <= maxH) ||
						(maxy >= minH && maxy <= maxH)) {

						int minStep = stepSize * k;
						int maxStep = minStep + stepSize;

						if ((minStep >= minx && maxStep <= maxx) ||
							(minx >= minStep && minx <= maxStep) ||
							(maxx >= minStep && maxx <= maxStep)) {

							track->addSelectedKey(k);
							success = true;
						}
					}
				}
			}
		}

		return success;
	}

	return false;
}

void DfxTimelineTrackWidget::channelUpdated() {
	update();
}

void DfxTimelineTrackWidget::keyframeUpdated(std::string path) {
	if(_keyFramePath == path) update();
}
