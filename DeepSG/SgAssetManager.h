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

#include <FlSpace.h>
#include <iostream>
#include "SgAssetHandler.h"

namespace deepsg {

	class SgAssetManager {
	private:
		static SgAssetManager *_instance;
		fl::Space *_assetSpace;

		SgAssetManager();
	public:

		virtual ~SgAssetManager();

		static SgAssetManager *getInstance();
		static void newInstance();

		void clear();
		
		std::vector<SgAssetHandler> assets();

		SgAssetHandler newAsset(std::string name, unsigned int type);

	};

}