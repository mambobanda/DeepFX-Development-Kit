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

#pragma once

#include <vector>
#include <DfxCSimulationEntity.h>
#include <DfxEntityDB.h>

enum class DfxSimulationMode { ACTIVE_MODE, INACTIVE_MODE };

class DfxSimulationCoreGlobal {
private:
	static DfxSimulationCoreGlobal *instance;
	DfxSimulationCoreGlobal();
private:
	DfxSimulationMode simMode;
	bool domainInitialized;
	std::vector<dfxcore::DfxCSimulationEntity*> simEntities;
public:
	static void installNodes();

	static DfxSimulationCoreGlobal *getInstance();

	void initSimulationDomain();

	void deinitializeSimulationDomain();

	bool isDomainInitialized();

	DfxSimulationMode getSimulationMode();
	void setSimulationMode(DfxSimulationMode);

	void addSimulationEntity(dfxcore::DfxCSimulationEntity*);
	void removeSimulationEntity(dfxcore::DfxCSimulationEntity*);

	std::vector<dfxcore::DfxCSimulationEntity*> findSimulationEntitiesOfType(DfxGlobalEntityName uniquename);
	dfxcore::DfxCSimulationEntity *findSimulationEntity(size_t instance_id);
	bool simulationEntityIsOfType(size_t instance_id, DfxGlobalEntityName uniquename);

};

