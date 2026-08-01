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

#include "DfxItem.h"



DfxItem::DfxItem(size_t itemType, DfxGlobalEntityName uniquename): DfxEntity(uniquename) {
	*((size_t*)getProperty(item_type)) = itemType; 
}

DfxItem::~DfxItem() {

}

void DfxItem::compute() {

}

void DfxItem::onDataChanged(DfxEntityDataInstance *instance) {

}


size_t DfxItem::getItemType() {
	return *((size_t*)getProperty(item_type));
}

void DfxItem::initialize() {
	DfxEntityDB::createProperty(item_type, "item_type", new DfxEntityPropertyTypes::Size_TPropertyType());
}
