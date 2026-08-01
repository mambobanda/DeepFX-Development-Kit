/*
* The Mailo Today Solutions Software License
*
* Copyright 2017 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#include "DfxCSimulationEntity.h"
#include "DfxSimulationCoreGlobal.h"

namespace dfxcore {

	DfxCSimulationEntity::DfxCSimulationEntity(DfxGlobalEntityName uniquename, size_t itemid) : DfxViewItem(itemid, uniquename) {
		DfxSimulationCoreGlobal::getInstance()->addSimulationEntity(this);

		*(size_t*)getProperty(metaIdentifier) = 10101;
	}

	DfxCSimulationEntity::~DfxCSimulationEntity() {
		DfxSimulationCoreGlobal::getInstance()->removeSimulationEntity(this);
	}

	void DfxCSimulationEntity::draw(){}
	DfxBounds DfxCSimulationEntity::bounds() { return DfxBounds(); }
	void DfxCSimulationEntity::calculateBounds() {  }
	bool DfxCSimulationEntity::selectable() { return false; }

	void DfxCSimulationEntity::preInit() {}

	void DfxCSimulationEntity::postInit() {}

	void DfxCSimulationEntity::preDone() {}

	void DfxCSimulationEntity::postDone() {}

	void DfxCSimulationEntity::initialize() {
		DfxViewItem::initialize();

		DfxEntityDB::createProperty(metaIdentifier, "__meta_sim_identifier__", new DfxEntityPropertyTypes::Size_TPropertyType);
	}
}
