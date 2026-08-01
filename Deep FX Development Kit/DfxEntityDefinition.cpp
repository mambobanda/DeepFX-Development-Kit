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

#include "DfxEntityDefinition.h"

DfxEntityDefintion::DfxEntityDefintion() {
	
}

DfxEntityDefintion::~DfxEntityDefintion() {

}

void DfxEntityDefintion::insertSpecification(const DfxEntityPropertySpec spec) {
	specifications.push_back(spec);
}

DfxEntityDataInstance* DfxEntityDefintion::createInstance() {

	DfxEntityDataInstance* newInstance = new DfxEntityDataInstance;
	
	for (DfxEntityPropertySpec spec : specifications) {
		DfxEntityProperty *newProperty = new DfxEntityProperty(spec.property_id, spec.typeInstance, spec.name, newInstance);
		newInstance->insertProperty(spec.property_id, newProperty);
	}

	return newInstance;
}

DfxEntityDataInstance* DfxEntityDefintion::createInstance(DfxEntity *ent) {

	DfxEntityDataInstance* newInstance = new DfxEntityDataInstance(ent);

	for (DfxEntityPropertySpec spec : specifications) {
		DfxEntityProperty *newProperty = new DfxEntityProperty(spec.property_id, spec.typeInstance, spec.name, newInstance);
		newInstance->insertProperty(spec.property_id, newProperty);
	}

	return newInstance;
}