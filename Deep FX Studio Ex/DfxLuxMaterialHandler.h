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

#include <FlModel.h>
#include "DfxSpace.h"
#include "DfxRenderTarget.h"

class DfxLuxMaterialHandler: public fl::Model {
private:
	static size_t ID_GEN;
	size_t matid;
protected:
	fl::Any *_dataObject;
public:
	static std::string model_id;
public:
	DfxLuxMaterialHandler();
	virtual ~DfxLuxMaterialHandler();

	bool fetch(std::string path);
	fl::Any *dataObject();

	size_t materialID();
	
	fl::Any *createModel(fl::Space *space) override;

	bool create();
	bool destroy();

	static bool clearAllMaterials();
};