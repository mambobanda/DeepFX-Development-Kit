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

#include "DfxTimelineKeyTrackOverlay.h"
#include "DfxGraphEditorManager.h"

DfxTimelineKeyTrackOverlay::DfxTimelineKeyTrackOverlay(QWidget *parent): QWidget(parent) {
	
	mode = 0;
	mouseButtonPressed = false;
	shiftBtnDown = false;
	controlBtnDown = false;
	mouseMoved = false;

	focusedChannel = nullptr;
	focusedChannelPosition = glm::ivec2(0);

	setStyleSheet("background-color: transparent;");
}

void DfxTimelineKeyTrackOverlay::paintEvent(QPaintEvent *event) {
	QBrush brush(QColor(255, 255, 255, 30));

	int width  = mouseLastPosition.x - mouseOnPressPosition.x;
	int height = mouseLastPosition.y - mouseOnPressPosition.y;

	if (mouseButtonPressed && mode == SELECTION_MARQUEE) {
		QPainter p(this);
		p.setBrush(brush);
		p.drawRect(mouseOnPressPosition.x, mouseOnPressPosition.y, width, height);
	}
}

void DfxTimelineKeyTrackOverlay::mouseMoveEvent(QMouseEvent *event) {
	mouseLastPosition = glm::ivec2(event->x(), event->y());

	QPoint parent_pos = _parent->mapFromGlobal(QCursor::pos());
	QPoint disp(hbar->value(), vbar->value());

	bool updateLastPosition = false;

	if (mode == MOVE_SELECTED) {
		
		for (auto trl : *_selectedChannels) {

			//auto kmap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(trl->track()->keyframePath());
			auto okey = trl->track()->frameFromPosition(lastMovePosition);
			auto mkey = trl->track()->frameFromPosition(glm::ivec2(parent_pos.x() + disp.x(), parent_pos.y() + disp.y()));

			int offset = mkey - okey;

			trl->track()->setGhostDisplacement(offset);
		}

	}

	mouseMoved = true;
	update();
}

void DfxTimelineKeyTrackOverlay::mousePressEvent(QMouseEvent *event) {

	mouseButtonPressed = true;
	mouseButton = event->button();
	mouseOnPressPosition = glm::ivec2(event->x(), event->y());
	mouseLastPosition = mouseOnPressPosition;

	QPoint parent_pos = _parent->mapFromGlobal(QCursor::pos());
	globalMin = glm::ivec2(parent_pos.x(), parent_pos.y());

	QPoint disp(hbar->value(), vbar->value());

	for (auto trl : *_selectedChannels) {
		if (!trl->track()->isHidden()) {
			if (trl->track()->geometry().contains((parent_pos + disp))) {
				int i = 0;
				if (trl->track()->alreadySelected(glm::ivec2(parent_pos.x() + disp.x(), parent_pos.y() + disp.y()))) {
					mode = MOVE_SELECTED;
					focusedChannel = trl;
					focusedChannelPosition = glm::ivec2(parent_pos.x() + disp.x(), parent_pos.y() + disp.y());
					lastMovePosition = focusedChannelPosition;
					break;
				}
			}
		}
	}

	if (mode == 0) {
		if(!shiftBtnDown) for (auto trl : *_selectedChannels) trl->track()->clearSelection();
		for (auto trl : *_selectedChannels) {

			if (!trl->track()->isHidden()) {
				if (trl->track()->geometry().contains((parent_pos + disp))) {
					int i = 0;
					bool toggle = shiftBtnDown ? true : false;
					if (trl->track()->selectKeyFrame(glm::ivec2(parent_pos.x() + disp.x(), parent_pos.y() + disp.y()), i)){
						mode = MOVE_SELECTED;
						lastMovePosition = glm::ivec2(parent_pos.x() + disp.x(), parent_pos.y() + disp.y());
						break;
					}
		
				}
			}
		}
	}

	if (mode == MOVE_SELECTED) {
		for (auto trl : *_selectedChannels) {
			trl->track()->setGhostMode(true);
			trl->track()->setGhostDisplacement(0);
		}
	}

	if (mode == 0) mode = SELECTION_MARQUEE;

	update();
}

void DfxTimelineKeyTrackOverlay::mouseReleaseEvent(QMouseEvent *event) {

	std::vector<int> keys;
	mouseButton = event->button();
	mouseOnReleasePosition = glm::ivec2(event->x(), event->y());
	
	QPoint parent_pos = _parent->mapFromGlobal(QCursor::pos());
	globalMax = glm::ivec2(parent_pos.x(), parent_pos.y());

	glm::ivec2 disp(hbar->value(),vbar->value());

	if (mode == SELECTION_MARQUEE) {
		/// for (auto trl : *_selectedChannels) trl->track()->clearSelection();
		for (auto trl : *_selectedChannels) {
			if (!trl->track()->isHidden()) {
				trl->track()->selectKeyFrames(globalMin + disp, globalMax + disp, keys);
			}
		}
	}

	if (mode == MOVE_SELECTED) {
		for (auto trl : *_selectedChannels) {

			std::unordered_map<int, float> oldKeys;
			auto kmap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(trl->track()->keyframePath());
			auto okey = trl->track()->frameFromPosition(lastMovePosition);
			auto mkey = trl->track()->frameFromPosition(glm::ivec2(parent_pos.x() + disp.x, parent_pos.y() + disp.y));

			int offset = mkey - okey;

			if (offset != 0) {
				if (kmap != nullptr) {
					for (auto k : kmap->keys()) {
						if (trl->track()->isSelected(k)) {
							int newKey = k + offset;

							if (newKey != k) {

								if (kmap->hasKey(newKey)) {
									oldKeys[newKey] = kmap->value(newKey);
								}

								auto oiter = oldKeys.find(k);
								if (oiter != oldKeys.end()) {
									kmap->insertValue(newKey, oiter->second);
									trl->track()->addSelectedKey(newKey);
								}
								else {

									auto points = kmap->controlPoints(k);
									points.first.x = newKey - (float(k) - points.first.x);
									points.second.x = newKey + (points.second.x - float(k));

									kmap->moveKey(k, newKey);
									kmap->updateControlPoints(newKey, points);
									trl->track()->moveSelection(k, newKey);
								}

								DfxGraphEditorManager::getInstance()->notifyChannelUpdated();
							}
						}
					}
				}
			}
		}
	}

	if (!mouseMoved && focusedChannel != nullptr) {
		int i = 0;
		
		if (!shiftBtnDown) {
			for (auto trl : *_selectedChannels) trl->track()->clearSelection();
			focusedChannel->track()->selectKeyFrame(focusedChannelPosition, i);
		}
		else {
			focusedChannel->track()->selectKeyFrame(focusedChannelPosition, i, true);
		}
	}

	for (auto trl : *_selectedChannels) {
		trl->track()->setGhostMode(false);
	}
	
	mouseButtonPressed = false;
	mouseMoved = false;
	focusedChannel = nullptr;
	focusedChannelPosition = glm::ivec2(0);
	mode = 0;

	update();
}

void DfxTimelineKeyTrackOverlay::mouseMoveEventProxy(QMouseEvent *event) {
	mouseMoveEvent(event);
}

void DfxTimelineKeyTrackOverlay::mousePressEventProxy(QMouseEvent *event) {
	mousePressEvent(event);
}

void DfxTimelineKeyTrackOverlay::mouseReleaseEventProxy(QMouseEvent *event) {
	mouseReleaseEvent(event);
}

void DfxTimelineKeyTrackOverlay::eventProxy(QEvent *evt) {
	event(evt);
}

void DfxTimelineKeyTrackOverlay::shiftPressed() {
	shiftBtnDown = true;
}

void DfxTimelineKeyTrackOverlay::shiftReleased() {
	shiftBtnDown = false;
}

void DfxTimelineKeyTrackOverlay::controlPressed() {
	controlBtnDown = true;
}

void DfxTimelineKeyTrackOverlay::controlReleased() {
	controlBtnDown = false;
}
