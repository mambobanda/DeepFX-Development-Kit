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

#include "DfxRangedLoopNode.h"


namespace DfxNodes {

	DfxRangedLoopNode::DfxRangedLoopNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("Ranged Loop");

		setPropertyState(inFrom, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inTo, DfxEntityPropertyState::INPUT_STATE);

		*(float*)getProperty(inFrom) = 0.f;
		*(float*)getProperty(inTo) = 10.f;

		makeDirty();
	}

	DfxRangedLoopNode::~DfxRangedLoopNode() {

	}

	void DfxRangedLoopNode::compute() {

		int from = (int)*(float*)getProperty(inFrom);
		int to = (int)*(float*)getProperty(inTo);

		auto roots = DfxDelegate::findRoots(this);

		delegateTips.clear();

		if (roots.size() > 0) {
			auto tips = DfxDelegate::findTips(this);			

			for (size_t i = from; i <= to; i++) {

				if (i <= from) {
					DfxDelegate::executeDelegator(this);
				}
				else {
					DfxDelegate::executeDelegator(this, true);
				}
			}

			for (auto t : tips) {
				if (t->hasChildren()) {
					for (auto ent : t->getAllChildren()) {

						delegateTips.push_back(ent);
					}
				}
			}
		}

	}

	void DfxRangedLoopNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void DfxRangedLoopNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxRangedLoopNode::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxRangedLoopNode::isExportable() {
		return false;
	}

	void *DfxRangedLoopNode::creator(DfxGlobalEntityName uniquename) {
		return new DfxRangedLoopNode(uniquename);
	}

	void DfxRangedLoopNode::initialize() {

		DfxEntityDB::createProperty(inFrom, "from", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inTo, "to", new DfxEntityPropertyTypes::FloatPropertyType());
	}
}