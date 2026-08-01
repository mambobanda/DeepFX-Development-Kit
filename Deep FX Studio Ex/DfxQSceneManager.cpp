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

#include "DfxQSceneManager.h"

DfxQSceneManager *DfxQSceneManager::_instance = nullptr;

DfxQSceneManager::DfxQSceneManager() {
	_model = new DfxQDeepSGTreeModel;
	_selectionModel = new QItemSelectionModel(_model);

	ignoreSceneSelectionModelSignal = false;
	ignoreModelSelectionModelSignal = false;

	QObject::connect(_selectionModel, &QItemSelectionModel::selectionChanged, this, &DfxQSceneManager::modelSelectionChanged);
	QObject::connect(DfxSceneSelectionModel::getInstance(), &DfxSceneSelectionModel::selectionChanged, this, &DfxQSceneManager::sceneSelectionChanged);
}

DfxQSceneManager::~DfxQSceneManager() {
}

DfxQSceneManager *DfxQSceneManager::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxQSceneManager;
	}

	return _instance;
}

DfxQDeepSGTreeModel *DfxQSceneManager::model() {
	return _model;
}

QItemSelectionModel *DfxQSceneManager::selectionModel() {
	return _selectionModel;
}

void DfxQSceneManager::moveNode(deepsg::SgNode *node, deepsg::SgNode *to) {
	if (node->isMovable()) {
		if (node->parent() == nullptr) {
			model()->beginAppendRowOp(to);
			to->addChild(node);
			model()->endAppendRowOp();

			emit sceneUpdated();
		}
		else {
			node->parent()->removeChild(node);
			to->addChild(node);

			emit sceneUpdated();
		}
	}
}

void DfxQSceneManager::removeNode(deepsg::SgNode *node) {

	if (node->parent() != nullptr) {
		int rel = node->relativeIndex();
		model()->beginRemoveRowOp(node->parent(), node->relativeIndex());
		node->parent()->removeChild(node);
		model()->endRemoveRowOp();

		emit sceneUpdated();
	}

}

void DfxQSceneManager::modelSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
	
	if (!ignoreModelSelectionModelSignal) {
		
		ignoreSceneSelectionModelSignal = true;

		DfxSceneSelectionModel::getInstance()->clearSelection();
		
		// Select 
		for (auto index : _selectionModel->selectedIndexes()) {

			if (index.isValid()) {
				deepsg::SgNode *ndPtr = static_cast<deepsg::SgNode*>(index.internalPointer());
	
				DfxSceneSelectionModel::getInstance()->addSelection(ndPtr);
				
			}
		}

		ignoreSceneSelectionModelSignal = false;
		DfxGlobal::command::exec("updateScene");

	}
}

void DfxQSceneManager::sceneSelectionChanged() {

	if (!ignoreSceneSelectionModelSignal) {

		ignoreModelSelectionModelSignal = true;

		_selectionModel->select(QModelIndex(), QItemSelectionModel::Clear);

		// Select 
		for (auto nd : DfxSceneSelectionModel::getInstance()->selected()) {
			auto uid = nd->getID();

			auto mIndex = _model->createExternalIndex(nd->relativeIndex(), 0, nd);
			_selectionModel->select(mIndex, QItemSelectionModel::Select);
		
		}

		ignoreModelSelectionModelSignal = false;

	}
}

void DfxQSceneManager::removeAll(deepsg::SgNode *node) {
	while(!node->children().empty()) {
		removeAll(node->children()[0]);
		removeNode(node->children()[0]);
	}
}

void DfxQSceneManager::removeAll() {
	removeAll(deepsg::SgRootNode::getInstance());
}

void DfxQSceneManager::toggleVisibilty(deepsg::SgNode *node) {
	_model->toggleNodeVisiblity(node);
}

void DfxQSceneManager::toggleRenderable(deepsg::SgNode *node) {
	_model->toggleNodeRenderable(node);
}