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

#include "SgDescriptionHandler.h"

namespace deepsg {

	class SgAssetHandler : public SgDescriptionHandler {
	public:

		SgAssetHandler(fl::Space *_space);
		SgAssetHandler(fl::Space *_space, fl::Any *_aPtr);

		virtual ~SgAssetHandler();

		void setContext(unsigned int);
		unsigned int context();

		void setKind(unsigned int);
		unsigned int kind();

		std::string path();
		void setPath(std::string pth);

		std::vector<SgAssetHandler> assets();

		std::vector<SgVariantsHandler> variants();

		std::vector<SgAssetGroupHandler> groups();

		SgAssetHandler newAsset(std::string name, unsigned int type);

		SgVariantsHandler newVariant(std::string name, unsigned int kind);

		SgAssetGroupHandler newGroup(std::string name);
	};
}