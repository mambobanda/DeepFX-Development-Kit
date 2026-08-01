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

#include <DfxGlobal.h>
#include <DfxEntity.h>
#include <DfxViewItem.h>
#include <DfxEntityDB.h>
#include <DfxEntityBasicPropertyTypes.h>

namespace dfxcore {

	class DfxCSimulationEntity : public DfxViewItem {
	public:
		static const DfxPropertyID metaIdentifier = 0x0fff;
	public:
		DfxCSimulationEntity(DfxGlobalEntityName uniquename, size_t itemid = DFX_NULL_ITEM);
		virtual ~DfxCSimulationEntity();

		virtual void preInit();
		virtual void init()  = 0;
		virtual void postInit();
		virtual void clear() = 0;
		virtual void preDone();
		virtual void done()  = 0;
		virtual void postDone();

		virtual void draw();
		virtual DfxBounds bounds();
		virtual void calculateBounds();
		virtual bool selectable();

		static void initialize();
	};
}