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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "SgAssociationHandler.h"

namespace deepsg {

	SgAssociationHandler::SgAssociationHandler(fl::Space *_space) : SgDescriptionHandler(_space, nullptr) {

	}

	SgAssociationHandler::SgAssociationHandler(fl::Space *_space, fl::Any *_aPtr) : SgDescriptionHandler(_space, _aPtr) {

	}

	SgAssociationHandler::~SgAssociationHandler() {

	}



	void SgAssociationHandler::setKind(unsigned int knd) {
		_dataObject->rawData<SgAssociation>()->setKind(knd);
	}

	unsigned int SgAssociationHandler::kind() {
		return _dataObject->rawData<SgAssociation>()->kind();
	}

	std::string SgAssociationHandler::path() {
		return _dataObject->rawData<SgAssociation>()->path();
	}

	void SgAssociationHandler::setPath(std::string pth) {
		_dataObject->rawData<SgAssociation>()->setPath(pth);
	}

	std::string SgAssociationHandler::surface() {
		return _dataObject->rawData<SgAssociation>()->surface();
	}

	void SgAssociationHandler::setSurface(std::string pth) {
		_dataObject->rawData<SgAssociation>()->setSurface(pth);
	}

}