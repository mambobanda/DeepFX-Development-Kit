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

#include <FlAny.h>
#include "DfxAnyHistoryAction.h"

class DfxTransformHistoryAction: public DfxHistoryAction {
private:
	
	DfxAnyHistoryAction *_posx;
	DfxAnyHistoryAction *_posy;
	DfxAnyHistoryAction *_posz;

	DfxAnyHistoryAction *_rotx;
	DfxAnyHistoryAction *_roty;
	DfxAnyHistoryAction *_rotz;

	DfxAnyHistoryAction *_sclx;
	DfxAnyHistoryAction *_scly;
	DfxAnyHistoryAction *_sclz;

	std::string _transformReference;

	glm::vec3 oldPos;
	glm::vec3 newPos;

	glm::vec3 oldRot;
	glm::vec3 newRot;

	glm::vec3 oldScl;
	glm::vec3 newScl;

public:

	DfxTransformHistoryAction();
	DfxTransformHistoryAction(
		DfxAnyHistoryAction *px, DfxAnyHistoryAction *py, DfxAnyHistoryAction *pz, 
		DfxAnyHistoryAction *rx, DfxAnyHistoryAction *ry, DfxAnyHistoryAction *rz,
		DfxAnyHistoryAction *sx, DfxAnyHistoryAction *sy, DfxAnyHistoryAction *sz);

	virtual ~DfxTransformHistoryAction();

	bool execute() override;
	DfxHistoryAction *inverse() override;

	void setReference(std::string ref);

	void setOldTransform(fl::Any *transform);

	void setNewTransform(fl::Any *transform);

	void buildHistoryActions();
};