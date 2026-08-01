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

#include "DfxSceneGraph.h"

DfxSceneGraph *DfxSceneGraph::instance = nullptr;

DfxSceneGraph::DfxSceneGraph() {
	
	DfxSceneActionListener *addItemListener = new DfxSceneActionListener([](DfxSceneActionInfo info) {

		if (info.actionCode == DfxSceneActionListener::ADD_ITEM_ACTION) {

			DfxEntity *item = DfxScene::getInstance()->getItemByID(info.item_id);

			if(!DfxEnvelopeMap::getInstance()->hasEnvelope(item->getInstanceID(), DfxViewItem::position)){
				DfxEnvelope *positionEnvelope = new DfxEnvelope(DfxEnvelope_Identifier, item, DfxViewItem::position);
				DfxEnvelopeMap::getInstance()->addEnvelope(item->getInstanceID(), positionEnvelope);
			}

			if (!DfxEnvelopeMap::getInstance()->hasEnvelope(item->getInstanceID(), DfxViewItem::rotation)) {
				DfxEnvelope *rotationEnvelope = new DfxEnvelope(DfxEnvelope_Identifier, item, DfxViewItem::rotation);
				DfxEnvelopeMap::getInstance()->addEnvelope(item->getInstanceID(), rotationEnvelope);
			}

			if (!DfxEnvelopeMap::getInstance()->hasEnvelope(item->getInstanceID(), DfxViewItem::scale)) {
				DfxEnvelope *scaleEnvelope = new DfxEnvelope(DfxEnvelope_Identifier, item, DfxViewItem::scale);
				DfxEnvelopeMap::getInstance()->addEnvelope(item->getInstanceID(), scaleEnvelope);
			}

			DfxLayerManager::getInstance()->addViewNode((DfxViewItem*)item);

		}
	});

	DfxScene::getInstance()->addActionListener(addItemListener);
}

DfxSceneGraph::~DfxSceneGraph() {

}

DfxSceneGraph *DfxSceneGraph::getInstance() {

	if (instance == nullptr) {
		instance = new DfxSceneGraph();
	}

	return instance;
}

void DfxSceneGraph::initGraphUI() {
	graphUI = new DfxQNodeEditorDock();
}

DfxQNodeEditorDock *DfxSceneGraph::getGraphUI() {
	return graphUI;
}

std::vector<DfxEntity*> DfxSceneGraph::getGraphRoots() {
	std::vector<DfxEntity*> roots;

	for (size_t i = 0; i < DfxLayerManager::getInstance()->layerCount(); i++) {
		
		
		DfxLayerItem *layer = DfxLayerManager::getInstance()->getLayer(i);

		if (layer->isLinked()) {

			DfxRoot *root = new DfxRoot("0");

			for (size_t j = 0; j < layer->childCount(); j++) {

				if (!layer->getChild(j)->getNode()->getEntity()->hasParent()) {
					root->addChild(layer->getChild(j)->getNode()->getEntity());
				}

			}

			roots.push_back(root);
		}
	}

	return roots;
}

//std::vector<DfxQNode*> DfxSceneGraph::getNodeList() {
//	std::vector<DfxQNode*> list;
//
//	auto gitem = graphUI->getView()->getScene()->items();
//
//	for (auto it : gitem) {
//
//		if (it && it->type() == DfxQNode::Type) {
//
//			list.push_back((DfxQNode*)it);
//		}
//	}
//
//	return list;
//}

//void DfxSceneGraph::parentUnparentedNodes(DfxEntity *parent) {
//	
//	auto gitem = graphUI->getView()->getScene()->items();
//
//	for (auto it : gitem) {
//
//		if (it && it->type() == DfxQNode::Type) {
//
//			DfxEntity *ent = ((DfxQNode*)it)->getEntity();
//
//			if ( !ent->hasParent() ) {
//				parent->addChild(ent);
//			}
//		}
//	}
//}

void DfxSceneGraph::resetSimulation() {

	auto gitem = graphUI->getView()->getScene()->items();

	for (auto it : gitem) {

		if (it && it->type() == DfxQBaseNode::Type) {

			DfxEntity *ent = ((DfxQBaseNode*)it)->getEntity();

			ent->reset();
		}
	}
}