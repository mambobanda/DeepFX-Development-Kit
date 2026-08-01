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

#include "DfxSceneSelectionModel.h"

DfxSceneSelectionModel *DfxSceneSelectionModel::_instance = nullptr;

DfxSceneSelectionModel::DfxSceneSelectionModel() {

}

DfxSceneSelectionModel *DfxSceneSelectionModel::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxSceneSelectionModel;
	}

	return _instance;
}

void DfxSceneSelectionModel::addSelection(deepsg::SgNode *node) {
	if(!isSelected(node)) _selected.push_back(node);

	emit selectionChanged();
	notifyPropertiesHub();
}

void DfxSceneSelectionModel::removeSelection(deepsg::SgNode *node) {

	auto itr = _selected.begin();

	while (itr != _selected.end()) {

		if ((*itr)->getID() == node->getID()) {
			itr = _selected.erase(itr);
			emit selectionChanged();
			notifyPropertiesHub();
			return;
		}

		itr++;
	}
	
}

void DfxSceneSelectionModel::addRemoveSelection(deepsg::SgNode *node) {
	if (isSelected(node)) {
		removeSelection(node);
	}
	else {
		addSelection(node);
	}
	// emit selectionChanged();
}

bool DfxSceneSelectionModel::isSelected(deepsg::SgNode *node) {

	auto itr = _selected.begin();

	while (itr != _selected.end()) {

		if ((*itr)->getID() == node->getID()) {
			return true;
		}

		itr++;
	}

	return false;
}

void DfxSceneSelectionModel::clearSelection() {
	_selected.clear();

	emit selectionChanged();
	notifyPropertiesHub();
}

std::vector<deepsg::SgNode*> &DfxSceneSelectionModel::selected() {
	return _selected;
}

void DfxSceneSelectionModel::notifyPropertiesHub() {
	DfxQPropertiesHub::getInstance()->newSceneNodeSelected(_selected);
}

