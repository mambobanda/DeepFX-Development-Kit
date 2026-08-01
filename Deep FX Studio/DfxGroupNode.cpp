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

#include "DfxGroupDB.h"
#include "DfxGroupNode.h"

namespace DfxNodes {


	// Group Node
	DfxGroupNode::DfxGroupNode(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("Group");

		DfxEntityPropertyTypes::UID *uid01 = (DfxEntityPropertyTypes::UID*)getProperty(out);
		uid01->setRootID(getInstanceID());

		setPropertyState(out, DfxEntityPropertyState::OUTPUT_STATE);

		*(std::string*)getProperty(inName) = "default";

		DfxGroupDBEntry ent;
		ent.uid = getInstanceID();

		DfxGroupDB::getInstance()->addEntry(ent);
	}

	DfxGroupNode::~DfxGroupNode() {
		DfxGroupDB::getInstance()->removeEntry(getInstanceID());
	}

	void DfxGroupNode::compute() {
		using namespace DfxEntityPropertyTypes;

		cleanAllProperties();
		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void DfxGroupNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void DfxGroupNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxGroupNode::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxGroupNode::isExportable() {
		return true;
	}

	void *DfxGroupNode::creator(DfxGlobalEntityName uniquename) {
		return new DfxGroupNode(uniquename);
	}

	void DfxGroupNode::initialize() {
		DfxEntityDB::createProperty(out, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inName, "name", new DfxEntityPropertyTypes::StringPropertyType());
	}

	void DfxGroupNode::addUID(DfxEntityPropertyTypes::UID* uid) {
		using namespace DfxEntityPropertyTypes;

		UID* _uid = (UID*)getProperty(out);
		_uid->addChild(uid);
	}

	void DfxGroupNode::removeUID(DfxEntityPropertyTypes::UID* uid) {

		using namespace DfxEntityPropertyTypes;

		UID* _uid = (UID*)getProperty(out);
		_uid->removeChild(uid);
	}
}