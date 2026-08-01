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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <QGraphicsPathItem>
#include <FlCommon.h>
#include <FlAny.h>
#include <FlDataObject.h>
#include "DfxQComputeNodeEnums.h"
#include "DfxQComputeNodeProperty.h"


class DfxQComputeBaseNode : public QGraphicsPathItem {
protected:
	std::vector<DfxQComputeNodeProperty*> nslots;
	fl::Any *_any;
public:
	enum { Type = DfxQNodeEnums::COMPUTE_BASE_NODE };

	DfxQComputeBaseNode(QGraphicsItem *parent = nullptr);
	~DfxQComputeBaseNode();

	int type() const;
	
	fl::Any *any();
	void setAny(fl::Any*);

	std::vector <DfxQComputeNodeProperty*> properties();

	std::vector<DfxQComputeNodeProperty*> &allSlots();

	DfxQComputeNodeProperty *findSlot(std::string propertyPath);

	virtual void updateSlotConnectionPositions();

	static bool isValidProperty(uint32_t);

	void forcePositionUpdate();
};
