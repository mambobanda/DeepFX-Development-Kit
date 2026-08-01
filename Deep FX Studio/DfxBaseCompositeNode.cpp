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

#include "DfxBaseCompositeNode.h"


namespace DfxNodes {

	DfxBaseCompositeNode::DfxBaseCompositeNode(DfxGlobalEntityName uniquename) {
		ORIGINAL_NAME = uniquename;
		this->globalID = hash(uniquename);
		logicalName = "Base Composition Entity";

		instanceID = ++ENTITY_INSTANCE_ID_GEN;

		// _createDataInstance();
		// _init();

		// getDataInstance()->makeDirty();
	}
	
	DfxBaseCompositeNode::~DfxBaseCompositeNode() {

	}

	void DfxBaseCompositeNode::compute() {

	}

	void DfxBaseCompositeNode::addMasterInputNode(DfxEntity *) {

	}

	void DfxBaseCompositeNode::addMasterOutputNode(DfxEntity *) {

	}

	void DfxBaseCompositeNode::liftSlot(DfxEntity *ent, uint slotid) {
		std::pair<DfxEntity *, uint> p00(ent, slotid);
		std::pair<size_t, std::pair<DfxEntity *, uint>> p01(ent->getInstanceID(), p00);

		liftedSlots.push_back(p01);

		ent->hide();
	}

	void DfxBaseCompositeNode::dropSlot(DfxEntity *ent, uint slotid) {

		for (size_t i = 0; i < liftedSlots.size(); i++) {

			if (ent->getInstanceID() == liftedSlots[i].first) {
				std::pair<DfxEntity *, uint> p = liftedSlots[i].second;
				
				if (p.second == slotid) {
					liftedSlots.erase(liftedSlots.begin() + i);
					break;
				}
			}
		}

		ent->unhide();
	}

	std::vector<std::pair<size_t, std::pair<DfxEntity *, uint>>> DfxBaseCompositeNode::getAllLiftedSlots() {
		return liftedSlots;
	}

}