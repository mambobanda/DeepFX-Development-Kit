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

	class SgAssociationHandler : public SgDescriptionHandler {
	public:
		SgAssociationHandler(fl::Space *_space);
		SgAssociationHandler(fl::Space *_space, fl::Any *_aPtr);
		virtual ~SgAssociationHandler();

		void setKind(unsigned int);
		unsigned int kind();

		std::string path();
		void setPath(std::string pth);

		std::string surface();
		void setSurface(std::string pth);

		SgAssociationHandler addAssociation(std::string name, SgAssociation aso) = delete;
		std::vector<SgAssociationHandler> addAssociations(std::vector<std::pair<std::string, SgAssociation>> asoArr) = delete;
	};

}