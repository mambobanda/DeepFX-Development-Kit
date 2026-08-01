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

#include <DfxLayer.h>
#include "DfxNodeViewCopyPaste.h"


DfxNodeViewCopyPaste::DfxNodeViewCopyPaste() {

}

DfxNodeViewCopyPaste::~DfxNodeViewCopyPaste() {

}

void DfxNodeViewCopyPaste::copySelectedNodes(QGraphicsScene &scene) {

	clear();

	for (auto item : scene.items()) {
		if (item->type() == DfxQBaseNode::Type) {
			if (item->isSelected()) {

				DfxQBaseNode *ptr = (DfxQBaseNode*)item;
				nodes.push_back(ptr);

			}
		}
	}

	for (auto nd : nodes) {
		DfxEntity *ent = nd->getEntity();
		
		DfxLayerItem *layerRef = (DfxLayerItem *)nd->getLayerReference();

		Copy *n_copy = new Copy;
		n_copy->entity = DfxEntityDB::newEntityInstance(ent->UniqueName());
		n_copy->old_id = ent->getInstanceID();
		n_copy->logical_name = ent->getLogicalName();
		n_copy->isViewNode = (layerRef->getNodeType() == DfxLayerNodeType::VIEW_NODE) ? true : false;
		n_copy->isImportNode = (layerRef->getNode()->getNodeType() == DfxQNodeType::IMPORT_NODE) ? true : false;
		n_copy->graphPosX = nd->pos().x();
		n_copy->graphPosY = nd->pos().y();

		//auto _slots = nd->getAllSlots();

		//for (auto slt : _slots) {
		//	if (slt->getSlotType() == DfxQNodeSlotType::OUTPUT_SLOT ||
		//		slt->getSlotType() == DfxQNodeSlotType::GROUP_OUTPUT_SLOT) {

		//		for (size_t i = 0; i < slt->getAllConnections().size(); i++) {
		//			auto conn = slt->getAllConnections()[i];

		//			CopyConnection cc;
		//			cc.output = conn->slot1()->getPropertyID();

		//			if (conn->slot2()->getSlotType() == DfxQNodeSlotType::GROUP_INPUT_SLOT)
		//				cc.input = 1000;
		//			else
		//				cc.input = conn->slot2()->getPropertyID();

		//			cc.source = n_copy->entity->getInstanceID();
		//			//cc.target = conn->slot2()->getParentEntity()->getInstanceID();

		//			n_copy->connections.push_back(cc);
		//		}
		//	}
		//}

		copies.push_back(n_copy);
	}
}

void DfxNodeViewCopyPaste::cutSelectedNodes() {

}

void DfxNodeViewCopyPaste::paste(QGraphicsView *gview) {

	DfxBounds bounds;
	std::vector<DfxQBaseNode*> qnodesLst;

	bounds.empty();
	for (auto cp : copies) {
		
		auto ent = DfxEntityDB::getDataInstance(cp->old_id);

		if (ent != nullptr) {

			DfxLayerItem *nodeitem = new DfxLayerItem;
			nodeitem->setItemType(DfxLayerItemType::NODE_ITEM);
			nodeitem->setNodeType(DfxLayerNodeType::GENERIC_NODE);

			if (!cp->isImportNode) {

				if (cp->entity->UniqueName().compare("2F2X252X_DFX_GROUP_NODE") == 0) {
					DfxQBaseNode *qnode = new DfxQBaseNode(cp->entity, DfxQNodeType::GROUP_NODE);

					qnode->setLayerReference(nodeitem);
					nodeitem->setNode(qnode);
				}
				else {

					DfxQBaseNode *qnode = new DfxQStandardNode(cp->entity);

					qnode->setLayerReference(nodeitem);
					nodeitem->setNode(qnode);
				}

				if(nodeitem->getNode()->getNodeType() == DfxQNodeType::STANDARD_NODE) {
					((DfxQStandardNode*)nodeitem->getNode())->setTitle(cp->logical_name.c_str());
					bounds.add(cp->graphPosX, cp->graphPosY, 0.0);
				}

				//nodeitem->getNode()->setPos(cp->graphPosX, cp->graphPosY);

				if (cp->isViewNode) {
					nodeitem->setNodeType(DfxLayerNodeType::VIEW_NODE);
					DfxScene::getInstance()->addItemNoAnnounce((DfxViewItem*)nodeitem->getNode()->getEntity());
				}


				cp->entity->copy(*ent->getDfxEntity());


				DfxLayerManager::getInstance()->insertNode(nodeitem, DfxLayerManager::getInstance()->getSelectedLayer());

			}
			else {
				nodeitem->setImportNode(true);
			}
		}
	}

	auto mouse_pos = gview->mapFromGlobal(QCursor::pos());
	auto displacementX = bounds.center().x - mouse_pos.x();
	auto displacementY = bounds.center().y - mouse_pos.y();

	for (auto qn : qnodesLst) {
		qn->setPos( qn->pos().x() + displacementX, qn->pos().y() + displacementY);
	}

	for (auto qn : qnodesLst) {
		gview->scene()->selectedItems().append(qn);
		qn->setSelected(true);
	}

	nodes.clear();

	for (auto cp : copies) {
		delete cp;
	}

	copies.clear();
}

void DfxNodeViewCopyPaste::clear() {
	nodes.clear();

	for (auto cp : copies) {
		delete cp->entity;
		delete cp;
	}

	copies.clear();
}