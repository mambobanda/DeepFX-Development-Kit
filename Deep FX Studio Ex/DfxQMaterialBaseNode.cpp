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

#include <DfxSpace.h>
#include "DfxQMaterialBaseNode.h"

size_t DfxQMaterialBaseNode::ID_GEN = 0xff;

DfxQMaterialBaseNode::DfxQMaterialBaseNode(QGraphicsItem *parent):QGraphicsPathItem(parent) {
	_nid = ID_GEN++;
}

DfxQMaterialBaseNode::~DfxQMaterialBaseNode() {

}

std::vector <DfxQMaterialNodeProperty*> DfxQMaterialBaseNode::properties() {
	return nslots;
}

int DfxQMaterialBaseNode::type() const {
	return Type;
}

size_t DfxQMaterialBaseNode::nodeID() {
	return _nid;
}

std::string DfxQMaterialBaseNode::dataObjectPath() {
	return _dataObjectPath;
}

void DfxQMaterialBaseNode::setDataObjectPath(std::string pth) {
	_dataObjectPath = pth;
}

void DfxQMaterialBaseNode::forcePositionUpdate() {
	if (!_dataObjectPath.empty()) {
		auto any = DfxSpace::getInstance()->findAbsolute(_dataObjectPath);

		if (any != nullptr) {

			if (any->data<fl::DataObject>().hasProperty("xpos")) {
				auto xp = any->data<fl::DataObject>().property("xpos");

				if (pos().x() != xp->data<float>())  xp->setData<float>(pos().x());

			}

			if (any->data<fl::DataObject>().hasProperty("ypos")) {
				auto yp = any->data<fl::DataObject>().property("ypos");

				if (pos().y() != yp->data<float>())  yp->setData<float>(pos().y());
			}

		}
	}
}