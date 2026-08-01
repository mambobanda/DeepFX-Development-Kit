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

#include <unordered_map>
#include <functional>
#include "FlAny.h"
#include "DfxQMaterialListModel.h"

using DfxMaterialMapInstancer = fl::Any*(*)(fl::Space *space);
using DfxMaterialInitializer = std::function<void()>;

class DfxMaterialManager {
private:
	static DfxMaterialManager *_instance;

	QAbstractItemModel *_materialModel;

	std::unordered_map<std::string, DfxMaterialMapInstancer> materialMap;

	DfxMaterialManager();

public:

	~DfxMaterialManager();

	static DfxMaterialManager *getInstance();

	void newMaterial(DfxMaterialInitializer init);
	void removeMaterial(std::string path);

	std::unordered_map<int, std::string> materialIDMap();

	QAbstractItemModel *materialModel();

	void addMaterialInstance(std::string, DfxMaterialMapInstancer);

	void removeMaterialInstance(std::string );

	fl::Any *createMaterialInstance(std::string);
};