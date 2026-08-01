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

#include <algorithm>
#include <limits>
#include <random>
#include <DfxEntity.h>
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxEntityDB.h"
#include "DfxGlobal.h"

namespace DfxNodes {

	class DfxBaseCompositeNode : public DfxEntity {
	protected:
		std::vector<DfxEntity*> inputMasterNode;
		std::vector<DfxEntity*> outputMasterNode;
		std::vector<std::pair<size_t, std::pair<DfxEntity *, uint>>> liftedSlots;

	public:
		DfxBaseCompositeNode(DfxGlobalEntityName uniquename);
		virtual ~DfxBaseCompositeNode();

		virtual bool isLiftable() = 0;
		virtual void compute() override;

		void addMasterInputNode(DfxEntity *);
		void addMasterOutputNode(DfxEntity *);
		void liftSlot(DfxEntity *, uint slotid);
		void dropSlot(DfxEntity *, uint slotid);

		std::vector<std::pair<size_t, std::pair<DfxEntity *, uint>>> getAllLiftedSlots();
	};
}

