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
#include "SgAssociation.h"
#include "SgAsset.h"
#include "SgAssetGroup.h"
#include "SgVariants.h"

namespace deepsg {

	class SgDescriptionHandler;
	class SgAssetHandler;
	class SgAssociationHandler;
	class SgVariantsHandler;
	class SgAssetGroupHandler;

	class SgDescriptionHandler {
	protected:
		fl::Any *_dataObject;
		fl::Space *_space;

	public:
		SgDescriptionHandler(fl::Space *_space, fl::Any *_dataObject);
		virtual ~SgDescriptionHandler();

		bool fetch(std::string path);

		std::string descriptionPath();

		std::string name();
		std::string rename(std::string nm);

		std::vector<SgAssociationHandler> associations();

		SgAssociationHandler addAssociation(std::string name, SgAssociation aso);
		std::vector<SgAssociationHandler> addAssociations(std::vector<std::pair<std::string, SgAssociation>> asoArr);
	};

};