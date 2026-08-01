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

#include "DfxStandardCompositeNode.h"

namespace DfxNodes {

	DfxStandardCompositeNode::DfxStandardCompositeNode(): DfxBaseCompositeNode(DfxStandardCompositeNode_Identifier) {
		setLogicalName("Composite Node");
		
	}

	DfxStandardCompositeNode::~DfxStandardCompositeNode() {


	}

	void DfxStandardCompositeNode::compute() {
		using namespace DfxEntityPropertyTypes;

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	bool DfxStandardCompositeNode::isLiftable() {
		return true;
	}

	void DfxStandardCompositeNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void DfxStandardCompositeNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxStandardCompositeNode::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxStandardCompositeNode::isExportable() {
		return true;
	}

	void *DfxStandardCompositeNode::creator(DfxGlobalEntityName uniquename) {
		return nullptr;
	}

	void DfxStandardCompositeNode::initialize() {

	}
}