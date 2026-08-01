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

#include "DfxViewItem.h"

DfxColorCounter DfxViewItem::color_id_counter;
std::unordered_map<std::string, DfxViewItem*> DfxViewItem::color_map;

DfxViewItem::DfxViewItem(size_t itemType, DfxGlobalEntityName uniquename): DfxEntity(uniquename) {

	*((size_t*)getProperty(item_id)) = getInstanceID();
	*((size_t*)getProperty(item_type)) = itemType;

	*((size_t*)getProperty(__meta_type__)) = 0xaaff;

	selected = false;

	setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	setScale(glm::vec3(1.f, 1.f, 1.f));

	((DfxEnum*)getProperty(rotationOrder))->add("XYZ");
	((DfxEnum*)getProperty(rotationOrder))->add("XZY");
	((DfxEnum*)getProperty(rotationOrder))->add("YXZ");
	((DfxEnum*)getProperty(rotationOrder))->add("YZX");
	((DfxEnum*)getProperty(rotationOrder))->add("ZXY");
	((DfxEnum*)getProperty(rotationOrder))->add("ZYX");

	color_id_counter.increment();
	colorID = DfxColorID(color_id_counter.redi(), color_id_counter.bluei(), color_id_counter.greeni());

	color_map.insert(std::pair<std::string, DfxViewItem*>(colorID.float_str(), this));

	hidden = false;
}

DfxViewItem::~DfxViewItem() {
	color_map.erase(colorID.float_str());
}

void DfxViewItem::compute() {
	// Do Nothing
	getDataInstance()->makeClean();
}

bool DfxViewItem::preprocessViewItem() {
	return false;
}

void DfxViewItem::preprocess() {
	
}

void DfxViewItem::onDataChanged(DfxEntityDataInstance *instance) {
	// Do Nothing
}

void DfxViewItem::setPosition(glm::vec3 pos) {

	((DfxEntityPropertyTypes::Float3*)getProperty(position))->x = pos.x;
	((DfxEntityPropertyTypes::Float3*)getProperty(position))->y = pos.y;
	((DfxEntityPropertyTypes::Float3*)getProperty(position))->z = pos.z;

	makePropertyBindingDirty(position);
	makeChildrenDirty();
	makeDirty();
}

glm::vec3 DfxViewItem::getPosition() {

	return glm::vec3(((DfxEntityPropertyTypes::Float3*)getProperty(position))->x,
					 ((DfxEntityPropertyTypes::Float3*)getProperty(position))->y,
					 ((DfxEntityPropertyTypes::Float3*)getProperty(position))->z);

}

void DfxViewItem::setRotation(glm::vec3 rot) {

	((DfxEntityPropertyTypes::Float3*)getProperty(rotation))->x = rot.x;
	((DfxEntityPropertyTypes::Float3*)getProperty(rotation))->y = rot.y;
	((DfxEntityPropertyTypes::Float3*)getProperty(rotation))->z = rot.z;

	makePropertyBindingDirty(rotation);
	makeChildrenDirty();
	makeDirty();
}

glm::vec3 DfxViewItem::getRotation() {

	return glm::vec3(((DfxEntityPropertyTypes::Float3*)getProperty(rotation))->x,
					 ((DfxEntityPropertyTypes::Float3*)getProperty(rotation))->y,
					 ((DfxEntityPropertyTypes::Float3*)getProperty(rotation))->z);
}

unsigned int DfxViewItem::getRotationOrder() {
	auto val = (unsigned int)((DfxEnum*)getProperty(rotationOrder))->getCurrentIndex();
	return (unsigned int)((DfxEnum*)getProperty(rotationOrder))->getCurrentIndex();
}

void DfxViewItem::setRotationOrder(unsigned int ord) {
	if (ord > 5) {
		((DfxEnum*)getProperty(rotationOrder))->setCurrentIndex(0);
	}
	else {
		((DfxEnum*)getProperty(rotationOrder))->setCurrentIndex(ord);
	}
}

void DfxViewItem::setScale(glm::vec3 scl) {
	((DfxEntityPropertyTypes::Float3*)getProperty(scale))->x = scl.x;
	((DfxEntityPropertyTypes::Float3*)getProperty(scale))->y = scl.y;
	((DfxEntityPropertyTypes::Float3*)getProperty(scale))->z = scl.z;

	makePropertyBindingDirty(scale);
	makeChildrenDirty();
	makeDirty();
}

glm::vec3 DfxViewItem::getScale() {

	return glm::vec3(((DfxEntityPropertyTypes::Float3*)getProperty(scale))->x,
					 ((DfxEntityPropertyTypes::Float3*)getProperty(scale))->y,
					 ((DfxEntityPropertyTypes::Float3*)getProperty(scale))->z);
}

bool DfxViewItem::isSelected() {
	return selected;
}

bool DfxViewItem::isHidden() {
	size_t __id__ = this->getInstanceID();
	return hidden;
}

void DfxViewItem::hide(bool b) {
	hidden = b;
}

void DfxViewItem::drawSelection() {

}

void DfxViewItem::setSelected(bool sel) {
	selected = sel;
}

size_t DfxViewItem::getItemType() {
	return *((size_t*)getProperty(item_type));
}

glm::mat4 DfxViewItem::transformationMatrix() {

	glm::mat4 transMat = glm::translate(glm::mat4(1.f), glm::vec3(getPosition().x, getPosition().y, getPosition().z));

	DfxRotation rotation(getRotation(), getRotationOrder());

	transMat *= rotation.rotationMatrix();

	transMat *= glm::scale(glm::mat4(1.f), glm::vec3(getScale().x, getScale().y, getScale().z));

	return transMat;
}

DfxColorID DfxViewItem::getColorID() {
	return colorID;
}

bool DfxViewItem::isRenderable() {
	return false;
}

DfxViewItem *DfxViewItem::findViewItemByColorID(DfxColorID cid) {
	
	auto it = color_map.find(cid.float_str());

	if (it != color_map.end()) {
		return it ->second;
	}

	return nullptr;
}

void DfxViewItem::initialize() {

	DfxEntityDB::createProperty(__meta_type__, "__meta_type__", new DfxEntityPropertyTypes::Size_TPropertyType());
	DfxEntityDB::createProperty(item_id, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
	DfxEntityDB::createProperty(item_type, "item_type", new DfxEntityPropertyTypes::Size_TPropertyType());
	DfxEntityDB::createProperty(position,"in_position", new DfxEntityPropertyTypes::Float3PropertyType());
	DfxEntityDB::createProperty(rotation, "in_rotation", new DfxEntityPropertyTypes::Float3PropertyType());
	DfxEntityDB::createProperty(scale, "in_scale", new DfxEntityPropertyTypes::Float3PropertyType());
	DfxEntityDB::createProperty(rotationOrder, "in_rotation_order", new DfxEntityPropertyTypes::DfxEnumPropertyType());
}