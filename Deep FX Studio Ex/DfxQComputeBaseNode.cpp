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
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/


#include "DfxQComputeBaseNode.h"

DfxQComputeBaseNode::DfxQComputeBaseNode(QGraphicsItem *parent) :QGraphicsPathItem(parent) {

}

DfxQComputeBaseNode::~DfxQComputeBaseNode() {

}

std::vector <DfxQComputeNodeProperty*> DfxQComputeBaseNode::properties() {
	return nslots;
}

int DfxQComputeBaseNode::type() const {
	return Type;
}

std::vector<DfxQComputeNodeProperty*> &DfxQComputeBaseNode::allSlots() {
	return nslots;
}

void DfxQComputeBaseNode::updateSlotConnectionPositions() {}

DfxQComputeNodeProperty *DfxQComputeBaseNode::findSlot(std::string propertyPath) {

	for (auto cp: nslots) {
		if (cp->slot()->propertyPath() == propertyPath) return cp;
	}

	return nullptr;
}

bool DfxQComputeBaseNode::isValidProperty(uint32_t typ) {

	if (typ == fl::type::FLOAT ||
		typ == fl::type::FLOAT_3 ||
		typ == fl::type::SIZE_T ||
		typ == fl::type::STD_STRING ||
		typ == fl::type::BOOL ||
		typ == fl::type::ATTRIBUTE_PATH ||
		typ == fl::type::ANY) {

		return true;
	}


	return false;
}

fl::Any *DfxQComputeBaseNode::any() {
	return _any;
}

void DfxQComputeBaseNode::setAny(fl::Any *an) {
	_any = an;
}

void DfxQComputeBaseNode::forcePositionUpdate() {
	
	if (_any != nullptr) {

		if (_any->data<fl::DataObject>().hasProperty("xpos")) {
			auto xp = _any->data<fl::DataObject>().property("xpos");

			if (pos().x() != xp->data<float>())  xp->setData<float>(pos().x());

		}

		if (_any->data<fl::DataObject>().hasProperty("ypos")) {
			auto yp = _any->data<fl::DataObject>().property("ypos");

			if (pos().y() != yp->data<float>())  yp->setData<float>(pos().y());
		}

	}
	
}