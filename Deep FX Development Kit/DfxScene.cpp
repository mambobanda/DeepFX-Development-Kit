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

#include "DfxScene.h"

DfxScene *DfxScene::instance = nullptr;

DfxScene::DfxScene() {

	transformMode = DfxScene::TRANSLATE_TRANSFORM;
	activeGizmo   = nullptr;
}

DfxScene::~DfxScene() {

}

bool DfxScene::SelectionListComparator::operator() (const std::pair<size_t, size_t> &lhs, const std::pair<size_t, size_t> &rhs) {
	return lhs.first < rhs.first;
}

bool DfxScene::hasItem(DfxViewItem *obj) {

	for (auto it : itemList) {
		if (it->getInstanceID() == obj->getInstanceID()) return true;
	}

	return false;
}

void DfxScene::addItem(DfxViewItem* obj) {
	if (hasItem(obj)) return;

	itemList.push_back(obj);

	DfxSceneActionInfo info;
	info.actionCode = DfxSceneActionListener::ADD_ITEM_ACTION;
	info.item_id = obj->getInstanceID();

	announceAction(info);
}

void DfxScene::addItemNoAnnounce(DfxViewItem* obj) {
	if (hasItem(obj)) return;
	
	itemList.push_back(obj);

	DfxSceneActionInfo info;
	info.actionCode = DfxSceneActionListener::ADD_ITEM_ACTION;
	info.item_id = obj->getInstanceID();
}

void DfxScene::removeItemByID(size_t inst_id) {

	size_t counter = 0;

	for (int inx = 0; inx < itemList.size(); inx++) {

		if (itemList[inx]->getInstanceID() == inst_id) {
			itemList.erase(itemList.begin() + counter);
			return;
		}

		counter++;
	}
}

size_t DfxScene::itemCount() {
	return itemList.size();
}

DfxViewItem *DfxScene::getItem(size_t index) {
	return itemList[index];
}

DfxScene *DfxScene::getInstance() {

	if (instance == nullptr)
		instance = new DfxScene();

	return instance;
}

void DfxScene::itemSelectionEvent(DfxSceneEvent evt) {

	if (evt.getEventCode() == DfxSceneEvent::MOUSE_RELEASE) {

		if (!evt.getScreenRay().isNull()) {

			float tMin = std::numeric_limits<float>::max();
			DfxViewItem *objPtr = nullptr;
			
			for (auto oitem : itemList) {

				if (oitem->selectable()) {

					DfxBounds bounds = ((DfxViewItem*)oitem)->bounds();
					glm::vec2 tMinMax = bounds.intersectsRay(evt.getScreenRay());

					if (tMinMax.x < tMinMax.y && tMinMax.x < tMin) {
						if (!oitem->isHidden()) {
							objPtr = oitem;
							tMin = tMinMax.x;
						}
					}
				}
			}

			if (objPtr != nullptr) {		

				clearSelected();
				selectionList.insert(std::pair<size_t, size_t>(objPtr->getInstanceID(), objPtr->getItemType()));
				objPtr->setSelected(true);

				if (activeGizmo != nullptr) delete activeGizmo;

				if (transformMode == DfxScene::TRANSLATE_TRANSFORM) {
					activeGizmo = new DfxTranslateGizmo(objPtr);
				}
				else if (transformMode == DfxScene::SCALE_TRANSFORM) {
					activeGizmo = new DfxScaleGizmo(objPtr);
				}
				else if (transformMode == DfxScene::ROTATE_TRANSFORM) {
					activeGizmo = new DfxRotateGizmo(objPtr);
				}
				
				DfxSceneActionInfo info;
				info.actionCode = DfxSceneActionListener::ITEM_SELECTED_ACTION;
				info.item_id = objPtr->getInstanceID();

				announceAction(info);
			}
			else {
				clearSelected();
				if (activeGizmo != nullptr) delete activeGizmo;
				activeGizmo = nullptr;
			}

			
		}
	}
}

void DfxScene::itemSelectionEventColorMode(DfxSceneEvent evt) {
	DfxViewItem *objPtr = nullptr;

	if (evt.getEventCode() == DfxSceneEvent::MOUSE_RELEASE) {

		objPtr = DfxViewItem::findViewItemByColorID(evt.getColorID());

		if (objPtr != nullptr) {

			clearSelected();
			selectionList.insert(std::pair<size_t, size_t>(objPtr->getInstanceID(), objPtr->getItemType()));
			objPtr->setSelected(true);

			if (activeGizmo != nullptr) delete activeGizmo;

			if (transformMode == DfxScene::TRANSLATE_TRANSFORM) {
				activeGizmo = new DfxTranslateGizmo(objPtr);
			}
			else if (transformMode == DfxScene::SCALE_TRANSFORM) {
				activeGizmo = new DfxScaleGizmo(objPtr);
			}
			else if (transformMode == DfxScene::ROTATE_TRANSFORM) {
				activeGizmo = new DfxRotateGizmo(objPtr);
			}

			DfxSceneActionInfo info;
			info.actionCode = DfxSceneActionListener::ITEM_SELECTED_ACTION;
			info.item_id = objPtr->getInstanceID();

			announceAction(info);
		}
		else {
			clearSelected();
			if (activeGizmo != nullptr) delete activeGizmo;
			activeGizmo = nullptr;
		}
	}
}

void DfxScene::gizmoSelectionEvent(DfxSceneEvent evt) {

	DfxGizmoEvent gevent;
	gevent.setScreenX(evt.getScreenX());
	gevent.setScreenY(evt.getScreenY());
	gevent.setScreenRay(evt.getScreenRay());
	gevent.setProjectionType(evt.getProjectionType());
	gevent.setScenePos(evt.getScenePos());
	gevent.setView(evt.getView());

	if (evt.getEventCode() == DfxSceneEvent::MOUSE_PRESS) {
		activeGizmo->pressed(gevent);
	}
	else if (evt.getEventCode() == DfxSceneEvent::MOUSE_MOVE) {
		activeGizmo->moving(gevent);
	}
	else if (evt.getEventCode() == DfxSceneEvent::MOUSE_RELEASE) {
		activeGizmo->released(gevent);
	}

}

void DfxScene::clearSelected() {

	for (auto item: itemList) {

		((DfxViewItem*)item)->setSelected(false);
	}

	selectionList.clear();
}

void DfxScene::newSceneEvent(DfxSceneEvent evt) {	
	
	if (activeGizmo != nullptr) {

		if (activeGizmo->handleSelected()) {

			gizmoSelectionEvent(evt);
			return;
		}
	}
	
	itemSelectionEventColorMode(evt);
}

DfxViewItem *DfxScene::getItemByID(size_t id) {

	for ( auto obj : itemList ) {

		if ( obj->getInstanceID() == id ) {
			return obj;
		}
	}

	return nullptr;
}

size_t DfxScene::getTransformMode() {
	return transformMode;
}

void DfxScene::setTransformMode(size_t md) {
	transformMode = md;

	if (!selectionList.empty()) {

		auto objID = selectionList.begin()->first;

		auto objPtr = getItemByID(objID);

		if (objPtr != nullptr) {
			if (activeGizmo != nullptr) delete activeGizmo;

			if (transformMode == DfxScene::TRANSLATE_TRANSFORM) {
				activeGizmo = new DfxTranslateGizmo(objPtr);
			}
			else if (transformMode == DfxScene::SCALE_TRANSFORM) {
				activeGizmo = new DfxScaleGizmo(objPtr);
			}
			else if (transformMode == DfxScene::ROTATE_TRANSFORM) {
				activeGizmo = new DfxRotateGizmo(objPtr);
			}
		}
	}
}

void DfxScene::clearSelectionIfSelected(size_t inx) {

	size_t count = 0;
	size_t cindex = -1;

	for (auto it = selectionList.begin(); it != selectionList.end();) {

		if (it->first == inx) {
			it = selectionList.erase(it);

			if (selectionList.size() <= 0) {
				if (activeGizmo != nullptr) delete activeGizmo;
				activeGizmo = nullptr;
			}
		}
		else {
			++it;
		}
	}
}

DfxAbstractGizmo *DfxScene::getActiveGizmo() {
	return activeGizmo;
}

void DfxScene::setActiveGizmo(DfxAbstractGizmo *ag) {
	activeGizmo = ag;
}

void DfxScene::clearActiveGizmo() {

	if (activeGizmo != nullptr) {
		activeGizmo->done();
		delete activeGizmo;
		activeGizmo = nullptr;
	}
}

void DfxScene::clearSelection() {
	clearSelected();
	clearActiveGizmo();
}

void DfxScene::announceAction(DfxSceneActionInfo info) {

	for (auto action : actionListnerList) {
		action->response(info);
	}
}

std::vector<DfxViewItem*> DfxScene::getSelectedItems() {

	std::vector<DfxViewItem*> its;

	for (auto p : selectionList) {
		DfxViewItem *itv = getItemByID(p.first);

		if (itv != nullptr) its.push_back(itv);
	}

	return its;
}

std::vector<DfxViewItem*> DfxScene::getUnSelectedItems() {

	std::vector<DfxViewItem*> its;

	for (auto it : itemList) {
		bool is_selected = false;

		for (auto p : selectionList) {
			//DfxViewItem *itv = getItemByID(p.first);

			if (p.first == it->getInstanceID()) {
				is_selected = true;
				break;
			}
		}

		if (!is_selected) {
			its.push_back(it);
		}
	}

	return its;
}

void DfxScene::addActionListener(DfxSceneActionListener *actl) {
	actionListnerList.push_back(actl);
}

std::vector<DfxViewItem*> DfxScene::getAllItems() {
	return itemList;
}