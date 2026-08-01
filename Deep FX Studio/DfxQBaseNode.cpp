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

#include "DfxQBaseNode.h"

DfxQBaseNode::DfxQBaseNode(DfxEntity *ent, DfxQNodeType type, QGraphicsItem *parent): QGraphicsPathItem(parent) {
	entity = ent;
	nodeType = type;
}

DfxQBaseNode::~DfxQBaseNode() {

}

void DfxQBaseNode::rebuild() { }

int DfxQBaseNode::type() const {
	return Type;
}

DfxQNodeType DfxQBaseNode::getNodeType() {
	return nodeType;
}

size_t DfxQBaseNode::ID() {
	if (entity != nullptr) return entity->getInstanceID();
	else return 0;
}

DfxEntity *DfxQBaseNode::getEntity() {
	return entity;
}

void *DfxQBaseNode::getLayerReference() {
	return referenceLayer;
}

void DfxQBaseNode::setLayerReference(void *rl) {
	referenceLayer = rl;
}

void DfxQBaseNode::removeConnections() {
	for (auto slt : nodeSlots) {

		for (auto conn : slt->getAllConnections()) scene()->removeItem(conn);

		if (slt->hasConnections()) slt->removeConnections();

	}
}

void DfxQBaseNode::destroyPropertiesForm() {
	if (propertiesForm != nullptr)delete propertiesForm;

	propertiesForm = nullptr;
}

bool DfxQBaseNode::hasPropertiesForm() {
	if (propertiesForm != nullptr)
		return true;
	else
		return false;
}

QGraphicsProxyWidget *DfxQBaseNode::buildPropertiesForm() {

	if (entity != nullptr && nodeType == DfxQNodeType::STANDARD_NODE) {

		DfxQNodePropertiesPanel *panel = new DfxQNodePropertiesPanel(this);

		DfxEntityDataInstance *dinstance = entity->getDataInstance();

		std::vector<size_t> lst = dinstance->getPropertyList();
		std::sort(lst.begin(), lst.end());

		for (auto id : lst) {

			DfxEntityPropertyState st = dinstance->getPropertyInstance(id)->getState();

			if (DfxEntityPropertyState::INPUT_STATE == dinstance->getPropertyInstance(id)->getState()) {

				QString label = dinstance->getPropertyInstance(id)->getName().c_str();

				if (dinstance->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iFLOAT_TYPE) {

					float val = *((float*)entity->getProperty(id));
					panel->addRow(new QLabel(label), new DfxQDecimalSpinnerControl(entity, id, val));

				}
				else if (dinstance->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iFLOAT3_TYPE) {

					panel->addRow(new QLabel(label), new DfxQDecimal3SpinnerControl(entity, id));

				}
				else if (dinstance->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iENUM_TYPE) {

					panel->addRow(new QLabel(label), new DfxQEnumControl(entity, id));
				}
				else if (dinstance->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iDIR_PATH_TYPE) {

					panel->addRow(new QLabel(label), new DfxQDirectoryPathControl(entity, id));
				}
				else if (dinstance->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iFILE_PATH_TYPE) {

					panel->addRow(new QLabel(label), new DfxQFilePathControl(entity, id));
				}
				else if (dinstance->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iSTRING_TYPE) {

					panel->addRow(new QLabel(label), new DfxQStringControl(entity, id));
				}
			}
		}

		panel->init();
		panel->setTitle(QString(entity->getLogicalName().c_str()) + " Properties");
		propertiesForm = panel;

		return panel;
	}

	return nullptr;
}

QColor DfxQBaseNode::workOutSlotColor(DfxEntity &ent, size_t id) {

	if (ent.getDataInstance()->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iFLOAT3_TYPE ||
		ent.getDataInstance()->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iFLOAT3_ARRAY_TYPE) {
		return QColor(0, 0, 255);
	}
	else if (ent.getDataInstance()->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::TRAIT_UID_TYPE) {
		return QColor(128, 0, 120);
	}
	else if (ent.getDataInstance()->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::i3DMESH_TYPE) {
		return QColor(85, 221, 255);
	}
	else if (ent.getDataInstance()->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iFLOAT_TYPE ||
		ent.getDataInstance()->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iFLOAT_ARRAY_TYPE) {
		return QColor(0, 255, 0);
	}
	else if (ent.getDataInstance()->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iENUM_TYPE) {
		return QColor(255, 255, 0);
	}
	else if (ent.getDataInstance()->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iMATRIX3x3_TYPE) {
		return QColor(128, 51, 64);
	}
	else if (ent.getDataInstance()->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iMATRIX4x4_TYPE ||
		ent.getDataInstance()->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iMATRIX4x4_ARRAY_TYPE) {
		return QColor(68, 85, 0);
	}
	else if (ent.getDataInstance()->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iPROXY_OBJECT_TYPE ||
		ent.getDataInstance()->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iPROXY_OBJECT_ARRAY_TYPE) {
		return QColor(179, 128, 255);
	}
	else if (ent.getDataInstance()->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iSTRING_TYPE ||
		ent.getDataInstance()->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iFILE_PATH_TYPE ||
		ent.getDataInstance()->getPropertyInstance(id)->getType() == DfxEntityPropertyTypes::iDIR_PATH_TYPE) {
		return QColor(244, 33, 133);
	}
	else
	{
		return QColor(255, 0, 0);
	}
}

bool DfxQBaseNode::slotIsArray(DfxEntity *ent, size_t property) {

	if (ent->getPropertyType(property) == DfxEntityPropertyTypes::iFLOAT_ARRAY_TYPE ||
		ent->getPropertyType(property) == DfxEntityPropertyTypes::iFLOAT3_ARRAY_TYPE ||
		ent->getPropertyType(property) == DfxEntityPropertyTypes::iINT_ARRAY_TYPE ||
		ent->getPropertyType(property) == DfxEntityPropertyTypes::iUNSIGNED_INT_ARRAY_TYPE ||
		ent->getPropertyType(property) == DfxEntityPropertyTypes::iSIZE_T_ARRAY_TYPE ||
		ent->getPropertyType(property) == DfxEntityPropertyTypes::iSTRING_ARRAY_TYPE ||
		ent->getPropertyType(property) == DfxEntityPropertyTypes::iCHAR_ARRAY_TYPE ||
		ent->getPropertyType(property) == DfxEntityPropertyTypes::iBOOL_ARRAY_TYPE ||
		ent->getPropertyType(property) == DfxEntityPropertyTypes::iPROXY_OBJECT_ARRAY_TYPE ||
		ent->getPropertyType(property) == DfxEntityPropertyTypes::iMATRIX4x4_ARRAY_TYPE) {

		return true;
	}

	return false;
}

std::vector<DfxQNodeSlot*> DfxQBaseNode::getAllSlots() {
	return nodeSlots;
}

DfxQNodeSlot *DfxQBaseNode::findSlot(size_t id) {

	for (auto slt : nodeSlots) {
		if (slt->getPropertyID() == id) {
			return slt;
		}
	}

	return nullptr;
}

void DfxQBaseNode::cleanEntity() {
	if (nodeType != DfxQNodeType::IMPORT_NODE) {
		DfxEnvelopeMap::getInstance()->removeAllEnvelopes(entity->getInstanceID());
		delete entity;
	}
}

bool DfxQBaseNode::hasSlot(size_t id) {

	for (auto slt : nodeSlots) {
		if (slt->getPropertyID() == id) {
			return true;
		}
	}

	return false;
}

void DfxQBaseNode::setPrefferedWidth(float w) {
	prefferedWidth = w;
}

float DfxQBaseNode::getPrefferedWidth() {
	return prefferedWidth;
}

void DfxQBaseNode::setPrefferedHeight(float h) {
	prefferedHeight = h;
}

float DfxQBaseNode::getPrefferedHeight() {
	return prefferedHeight;
}