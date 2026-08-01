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

#include <qgraphicsview.h>
#include <DfxRoot.h>
#include "DfxLayerManager.h"
#include "DfxPythonObjectInstanceMap.h"


DfxLayerManager *DfxLayerManager::instance = nullptr;

DfxLayerManager::DfxLayerManager() {

	DfxLayerItem *default = new DfxLayerItem;
	default->setItemType(DfxLayerItemType::LAYER_ITEM);
	default->setScene(new QGraphicsScene());
	default->setName("default");
	default->setIsTop(true);
	
	root = new DfxLayerItem();
	root->setName("root");
	root->setParent(nullptr);
	root->addChild(default);

	model = new DfxQLayerModel(root);

	selectedLayer = default;

	use_maximise_scene = true;

	level = 0;
}

DfxLayerManager::~DfxLayerManager() {

}

DfxLayerManager *DfxLayerManager::getInstance() {

	if (instance == nullptr) {
		instance = new DfxLayerManager();
	}

	return instance;
}

DfxLayerItem *DfxLayerManager::getRoot() {
	return root;
}

size_t DfxLayerManager::layerCount() {
	return root->childCount();
}

DfxLayerItem *DfxLayerManager::getLayerItem(size_t inx) {
	return root->getChild(inx);
}

DfxLayerItem *DfxLayerManager::getSelectedLayer() {
	return selectedLayer;
}

DfxQLayerModel *DfxLayerManager::getModel() {
	return model;
}

QGraphicsScene *DfxLayerManager::getScene(size_t inx) {
	return root->getChild(inx)->getScene();
}

void DfxLayerManager::setUseMaximiseScene(bool val) {
	use_maximise_scene = val;
}

void DfxLayerManager::forceMaximise() {
	/*if (selectedLayer != nullptr) {
		auto _scene = selectedLayer->getScene();
		if (_scene->sceneRect().height() < 30000 ||
			_scene->sceneRect().width() < 30000) {

			qreal x = _scene->sceneRect().x();
			qreal y = _scene->sceneRect().y();
			qreal w = 30000;
			qreal h = 30000;

			_scene->setceneRect(x, y, w, h);
			
		}
	}*/
}

void DfxLayerManager::maximiseScene(QGraphicsScene *_scene, DfxQBaseNode *centerOn) {

	if (use_maximise_scene) {
		/*if (_scene->sceneRect().height() < 30000 ||
			_scene->sceneRect().width() < 30000) {*/
		if(_scene->items().size()  == 1){
			qreal x = _scene->sceneRect().x();
			qreal y = _scene->sceneRect().y();
			qreal w = 30000;
			qreal h = 30000;

			_scene->setSceneRect(x, y, w, h);
			centerOn->setPos(QPointF(w / 2, w / 2));

			for (auto gview : _scene->views()) {

				gview->centerOn(centerOn);
			}
		}
		else {
			for (auto gview : _scene->views()) {

				auto w = gview->viewport()->rect().width();
				auto h = gview->viewport()->rect().height();
				auto center = gview->viewport()->rect().center();
				auto new_pos = gview->mapToScene(center);

				std::mt19937 mt01(centerOn->getEntity()->getInstanceID());
				std::mt19937 mt02(centerOn->getEntity()->getInstanceID() + 1);
				std::uniform_real_distribution<double> rndH(-(h / 2), (h / 2));
				std::uniform_real_distribution<double> rndW(-(w / 2), (w / 2));

				qreal x = new_pos.x() + rndW(mt01);
				qreal y = new_pos.y() + rndH(mt02);

				centerOn->setPos(x, y);
			}
		}
	}
}

void DfxLayerManager::moveLayerUp() {
	
	if (selectedLayer != nullptr) {

		if (selectedLayer->row() != 0) {

			model->beginMoveRowOp(selectedLayer, selectedLayer->row() - 1);

			root->moveChild(selectedLayer->row(), selectedLayer->row() - 1);

			model->endMoveRowOp();
		}
	}
}

void DfxLayerManager::moveLayerDown() {

	if (selectedLayer != nullptr) {

		if (selectedLayer->row() != selectedLayer->getParent()->childCount() - 1) {

			model->beginMoveRowOp(selectedLayer, selectedLayer->row() + 2); 

			root->moveChild(selectedLayer->row(), selectedLayer->row() + 1);

			model->endMoveRowOp();
		}
	}
}

void DfxLayerManager::addNode(DfxEntity *node) {

	if (selectedLayer != nullptr) {

		model->beginAppendRowOp(selectedLayer);

		DfxQBaseNode *qnode = new DfxQStandardNode(node);
		selectedLayer->getScene()->addItem(qnode);

		DfxLayerItem *nodeitem = new DfxLayerItem;
		nodeitem->setItemType(DfxLayerItemType::NODE_ITEM);
		nodeitem->setNodeType(DfxLayerNodeType::GENERIC_NODE);
		nodeitem->setNode(qnode);
		qnode->setLayerReference(nodeitem);

		selectedLayer->addChild(nodeitem);

		model->endAppendRowOp();
		
		maximiseScene(selectedLayer->getScene(), qnode);
	}
}

void DfxLayerManager::insertNode(DfxLayerItem *item, DfxLayerItem *layer) {
	if (layer != nullptr) {

		layer->getScene()->addItem(item->getNode());

		model->beginAppendRowOp(layer);
		layer->addChild(item);
		model->endAppendRowOp();

		maximiseScene(layer->getScene(), item->getNode());
	}
}

void DfxLayerManager::addViewNode(DfxViewItem *node) {

	if (selectedLayer != nullptr) {

		DfxScene::getInstance()->addItemNoAnnounce(node);

		model->beginAppendRowOp(selectedLayer);

		DfxQBaseNode *qnode = new DfxQStandardNode(node);
		selectedLayer->getScene()->addItem(qnode);

		DfxLayerItem *nodeitem = new DfxLayerItem;
		nodeitem->setItemType(DfxLayerItemType::NODE_ITEM);
		nodeitem->setNodeType(DfxLayerNodeType::VIEW_NODE);
		nodeitem->setNode(qnode);
		qnode->setLayerReference(nodeitem);

		selectedLayer->addChild(nodeitem);

		model->endAppendRowOp();

		maximiseScene(selectedLayer->getScene(), qnode);

		DfxGlobal::command::exec("updateViewport");
	}
}

void DfxLayerManager::addCompositeNode(DfxEntity *node) {

	if (selectedLayer != nullptr) {

		model->beginAppendRowOp(selectedLayer);

		DfxQBaseNode *qnode = new DfxQCompositeNode(node);
		selectedLayer->getScene()->addItem(qnode);

		DfxLayerItem *nodeitem = new DfxLayerItem;
		nodeitem->setItemType(DfxLayerItemType::COMPOSITE_ITEM);
		nodeitem->setNodeType(DfxLayerNodeType::VIEW_NODE);
		nodeitem->setNode(qnode);
		nodeitem->setScene(new QGraphicsScene());
		qnode->setLayerReference(nodeitem);

		selectedLayer->addChild(nodeitem);

		model->endAppendRowOp();

		maximiseScene(selectedLayer->getScene(), qnode);
	}
}

DfxLayerItem *DfxLayerManager::addImportNode(DfxLayerItem *item, DfxLayerItem *parent) {

	if (parent != nullptr) {		

		/*DfxEntity *ent = item->getNode()->getEntity();
		DfxQNode *qnode = new DfxQNode(ent, DfxQNodeType::IMPORT_NODE);
		parent->getScene()->addItem(qnode);

		DfxLayerItem *nodeitem = new DfxLayerItem;
		nodeitem->setItemType(DfxLayerItemType::NODE_ITEM);
		nodeitem->setNodeType(DfxLayerNodeType::GENERIC_NODE);
		nodeitem->setNode(qnode);
		qnode->setLayerReference(nodeitem);

		item->addImportItem(nodeitem);

		model->beginAppendRowOp(parent);
		parent->addChild(nodeitem);
		model->endAppendRowOp();*/
	
		return nullptr;
	}

	return nullptr;
}

DfxLayerItem *DfxLayerManager::addGroupNode(DfxEntity *) {
	
	if (selectedLayer != nullptr) {
		model->beginAppendRowOp(selectedLayer);

		DfxEntity *ent = new DfxNodes::DfxGroupNode(DfxNodes::DfxGroupNode_Identifier);
		DfxQBaseNode *qnode = new DfxQGroupNode(ent);
		selectedLayer->getScene()->addItem(qnode);

		DfxLayerItem *nodeitem = new DfxLayerItem;
		nodeitem->setItemType(DfxLayerItemType::NODE_ITEM);
		nodeitem->setNodeType(DfxLayerNodeType::GENERIC_NODE);
		nodeitem->setNode(qnode);
		qnode->setLayerReference(nodeitem);

		selectedLayer->addChild(nodeitem);

		model->endAppendRowOp();

		maximiseScene(selectedLayer->getScene(), qnode);

		return nodeitem;
	}

	return nullptr;
}

DfxLayerItem *DfxLayerManager::addRangedLoop(std::vector<DfxEntity*> delegates, QRectF boundingRect) {

	if (selectedLayer != nullptr) {

		model->beginAppendRowOp(selectedLayer);

		DfxEntity *ent = new DfxNodes::DfxRangedLoopNode(DfxNodes::DfxRangedLoopNode_Identifier);

		for (auto delg : delegates) {
			ent->addDelegatee(delg);
		}

		DfxQBaseNode *qnode = new DfxQRangedLoopNode(ent, boundingRect);
		selectedLayer->getScene()->addItem(qnode);

		DfxLayerItem *nodeitem = new DfxLayerItem;
		nodeitem->setItemType(DfxLayerItemType::NODE_ITEM);
		nodeitem->setNodeType(DfxLayerNodeType::GENERIC_NODE);
		nodeitem->setNode(qnode);
		qnode->setLayerReference(nodeitem);

		selectedLayer->addChild(nodeitem);

		model->endAppendRowOp();

		//maximiseScene(selectedLayer->getScene(), qnode);

		return nodeitem;
	}

	return nullptr;
}

DfxLayerItem *DfxLayerManager::addNewLayer() {
	if (root != nullptr) {

		QGraphicsScene *_scene = new QGraphicsScene();

		DfxLayerItem *new_item = new DfxLayerItem();
		new_item->setItemType(DfxLayerItemType::LAYER_ITEM);
		new_item->setScene(_scene);
		new_item->setName("New Layer");

		model->beginAppendRowOp(root);
		root->addChild(new_item);
		model->endAppendRowOp();

		return new_item;
	}

	return nullptr;
}

void DfxLayerManager::removeLayer() {

	if (selectedLayer != nullptr) {

		while (selectedLayer->childCount() > 0) {
			removeNode(selectedLayer->getChild(0)->getNode());
		}

		emit removingLayer();

		model->beginRemoveRowOp(selectedLayer->getParent(), selectedLayer->row());
		root->removeChild(selectedLayer);
		model->endRemoveRowOp();

		if (root->childCount() > 0)
			selectedLayer = root->getChild(0);
		else
			selectedLayer = nullptr;

		emit removingLayerDone();
	}
}

void DfxLayerManager::removeLayer(size_t inx) {

	if (inx >= 0 && inx < root->childCount())  {

		DfxLayerItem *layerPtr = root->getChild(inx);

		if (layerPtr != nullptr) {

			while (layerPtr->childCount() > 0) {
				removeNode(layerPtr->getChild(0)->getNode());
			}

			emit removingLayer();

			model->beginRemoveRowOp(layerPtr->getParent(), layerPtr->row());
			root->removeChild(layerPtr);
			model->endRemoveRowOp();
			layerPtr = nullptr;

			emit removingLayerDone();
		}
	}
}

void DfxLayerManager::removeNode(DfxQBaseNode *qnode, bool cleanEntity) {

	std::vector<DfxQNodeConnection *> connections;
	std::vector<DfxLayerItem *> importList;

	DfxLayerItem *reference = (DfxLayerItem *)qnode->getLayerReference();

	if (reference == nullptr) 
		return;

	DfxLayerItem *parent = reference->getParent();	

	if ( parent != nullptr ) {

		QGraphicsScene *qscene = parent->getScene();

		if (qnode->getNodeType() != DfxQNodeType::IMPORT_NODE) {

			for (auto imp : reference->getImportList()) {	

				imp->setItemType(DfxLayerItemType::NULL_ITEM);
				imp->getNode()->removeConnections();
				importList.push_back(imp);

			}
		}

		for (auto slt : qnode->getAllSlots()) {

			for (auto conn : slt->getAllConnections()) {
				connections.push_back(conn);
			}
		}

		/*for (auto conn : connections) {
			conn->unbind();
			conn->slot1()->removeConnection(conn->getID());
			conn->slot2()->removeConnection(conn->getID());
		}*/

		for (auto conn : connections) {
			qscene->removeItem(conn);
			delete conn;
			conn = nullptr;
		}

		if (reference->getNodeType() == DfxLayerNodeType::VIEW_NODE) {
			DfxScene::getInstance()->removeItemByID(reference->getNode()->getEntity()->getInstanceID());
		}

		if (qnode->getEntity()->UniqueName().compare("2F2X252X_DFX_DYNAMIC_CUSTOM_PYTHON_NODE") == 0) {
			qscene->removeItem(qnode);
			DfxPythonObjectInstanceMap::removeAndDestroy(qnode->getEntity()->getInstanceID());
		}
		else {
			qscene->removeItem(qnode);
			if (cleanEntity)qnode->cleanEntity();
		}
		
		model->beginRemoveRowOp(parent, reference->row());
		parent->removeChild(reference);
		model->endRemoveRowOp();

		for (auto impt : importList) {
			removeNode(impt->getNode());
		}

	}
}

void DfxLayerManager::removeimportNode() {

}

std::vector<DfxLayerItem*> DfxLayerManager::findImportNodes(DfxQBaseNode *qnode) {
	std::vector<DfxLayerItem*> imps;

	for (size_t inx = 0; inx < root->childCount(); inx++) {

		for (size_t inx2 = 0; inx2 < root->getChild(inx)->childCount(); inx2++) {

			if (root->getChild(inx)->getChild(inx2)->getNode()->getEntity()->getInstanceID() ==
				qnode->getEntity()->getInstanceID()) {

				if (root->getChild(inx)->getChild(inx2)->getNode()->getNodeType() == 
					DfxQNodeType::IMPORT_NODE) {

					imps.push_back(root->getChild(inx)->getChild(inx2));
				}
			}
		}
	}


	return imps;
}

DfxLayerItem *DfxLayerManager::findLayer(DfxQBaseNode *qnode) {

	for (size_t inx = 0; inx < root->childCount(); inx++) {

		for (size_t inx2 = 0; inx2 < root->getChild(inx)->childCount(); inx2++) {

			if (root->getChild(inx)->getChild(inx2)->getNode()->getEntity()->getInstanceID() ==
			    qnode->getEntity()->getInstanceID()) {

				return root->getChild(inx);
			}
		}
	}

	return nullptr;
}

DfxLayerItem *DfxLayerManager::getLayer(size_t index) {
	return root->getChild(index);
}

void DfxLayerManager::renameLayer(std::string name, DfxLayerItem *layer) {

	if (layer != nullptr) {
		
		if (layer->getItemType() == DfxLayerItemType::LAYER_ITEM) {

			layer->setName(name);

			model->updateData(layer, 0);
		}
	}
}

void DfxLayerManager::updateState(const QModelIndex &itemindex) {

	DfxLayerItem *item = static_cast<DfxLayerItem*>(itemindex.internalPointer());

	if ( item->getItemType() == DfxLayerItemType::LAYER_ITEM ) {

		if (selectedLayer != nullptr) {
				selectedLayer = item;
				emit layerSelected(selectedLayer);
			
		}
		else {
			selectedLayer = item;
			emit layerSelected(selectedLayer);
		}
	}
	else {
		
		if (selectedLayer != nullptr) {
		
				selectedLayer = item->getParent();
				emit layerSelected(selectedLayer);
		}
		else {
			selectedLayer = item;
			emit layerSelected(selectedLayer);
		}
	}

	if (item->getItemType() != DfxLayerItemType::LAYER_ITEM) {
		if (itemindex.column() == 1) {
			item->hideUnhide();
			model->updateData(item, 1);
		}
	}
	/*if (itemindex.column() == 1) {
		item->linkUnlink();
		model->updateData(item, 1);
	}

	if (itemindex.column() == 2) {
		item->hideUnhide();
		model->updateData(item, 2);
	}

	if (itemindex.column() == 3) {
		item->lockUnlock();
		model->updateData(item, 3);
	}*/
}

DfxLayerItem *DfxLayerManager::findNodeByOldID(size_t oid) {

	for (int i = 0; i < root->childCount(); i++) {

		DfxLayerItem *nodePtr = root->getChild(i);
		for (int j = 0; j < nodePtr->childCount(); j++) {

			if (nodePtr->getChild(j)->getOldInstanceID() == oid) {
				return nodePtr->getChild(j);
			}
		}
	}

	return nullptr;
}

std::vector<DfxViewItem*> DfxLayerManager::findAllViewItems() {

	std::vector<DfxViewItem*> vitems;
	std::stack<DfxLayerItem*> stck;

	for (size_t inx = 0; inx < root->childCount(); inx++) {
		
		if (root->getChild(inx)->getItemType() == DfxLayerItemType::LAYER_ITEM) {
			stck.push(root->getChild(inx));
		}
	}

	while (stck.size() > 0) {
		auto top = stck.top();
		stck.pop();

		for (size_t inx = 0; inx < top->childCount(); inx++) {

			if (top->getChild(inx)->getItemType() == DfxLayerItemType::NODE_ITEM &&
				top->getChild(inx)->getNodeType() == DfxLayerNodeType::VIEW_NODE) {

				vitems.push_back((DfxViewItem*)top->getChild(inx)->getNode()->getEntity());
			}
			else if (top->getChild(inx)->getItemType() == DfxLayerItemType::LAYER_ITEM ||
					 top->getChild(inx)->getItemType() == DfxLayerItemType::COMPOSITE_ITEM) {

				stck.push(top->getChild(inx));
			}
		}
	}

	return vitems;
}

DfxLayerItem *DfxLayerManager::findNodeByID(size_t id) {

	for (int i = 0; i < root->childCount(); i++) {

		DfxLayerItem *nodePtr = root->getChild(i);
		for (int j = 0; j < nodePtr->childCount(); j++) {

			if (nodePtr->getChild(j)->getNode()->getEntity()->getInstanceID() == id) {
				return nodePtr->getChild(j);
			}
		}
	}

	return nullptr;
}

void DfxLayerManager::clear() {

	while ( root->childCount() > 0 ) {
		removeLayer(0);
	}

	selectedLayer = nullptr;
}

void DfxLayerManager::resetSelectedLayer() {

	if (root->childCount() > 0) {
		selectedLayer = root->getChild(0);
	}
	else {
		selectedLayer = nullptr;
	}
}

void DfxLayerManager::reset() {
	clear();

	DfxLayerItem *default = new DfxLayerItem;
	default->setItemType(DfxLayerItemType::LAYER_ITEM);
	default->setScene(new QGraphicsScene());
	default->setName("default");

	root->addChild(default);

	selectedLayer = default;
}

void DfxLayerManager::switchSelectedLayer(DfxLayerItem *item) {
	selectedLayer = item;
}

bool DfxLayerManager::previousLayer() {

	if (!selectedLayer->isTop()) {
		selectedLayer = selectedLayer->getParent();
		decrementLevel();
		return true;
	}

	return false;    
}

bool DfxLayerManager::homeLayer() {
	
	if (!selectedLayer->isTop()) {

		while (!selectedLayer->isTop()) {
			selectedLayer = selectedLayer->getParent();
		}
		
		level = 0;
		levelUpdated(level);
		return true;
	}

	return false;
}


void DfxLayerManager::incrementLevel() {
	level++;
	levelUpdated(level);
}

void DfxLayerManager::decrementLevel() {
	level--;
	levelUpdated(level);
}

unsigned int DfxLayerManager::getLevel() {
	return level;
}

void DfxLayerManager::updateView() {
	emit model->dataChanged(QModelIndex(), QModelIndex());
}