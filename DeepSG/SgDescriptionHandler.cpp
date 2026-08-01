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

#include "SgDescriptionHandler.h"
#include "SgAssociationHandler.h"

namespace deepsg {

	SgDescriptionHandler::SgDescriptionHandler(fl::Space *spc, fl::Any *dobj) {
		_space = spc;
		_dataObject = dobj;
	}

	SgDescriptionHandler::~SgDescriptionHandler() {

	}

	bool SgDescriptionHandler::fetch(std::string path) {

		auto any = _space->findAbsolute(path);

		if (any != nullptr) {
			_dataObject = any;
			return true;
		}
		else {
			return false;
		}
	}

	std::string SgDescriptionHandler::descriptionPath() {
		return _space->absolutePath(_dataObject);
	}

	std::string SgDescriptionHandler::name() {
		return _dataObject->name();
	}

	std::string SgDescriptionHandler::rename(std::string nm) {
		_dataObject->setName(nm);
		return _dataObject->name();
	}

	SgAssociationHandler SgDescriptionHandler::addAssociation(std::string name, SgAssociation aso) {
		auto any = _space->any("___temp___", "");
		any->newData<SgAssociation>(ASSOCIATION_DESCRIPTION);
		any->rawData<SgAssociation>()->setKind(aso.kind());
		any->rawData<SgAssociation>()->setPath(aso.path());
		any->rawData<SgAssociation>()->setSurface(aso.surface());

		_space->move(any, _dataObject);
		any->setName(name);

		return SgAssociationHandler(_space, any);
	}

	std::vector<SgAssociationHandler> SgDescriptionHandler::addAssociations(std::vector<std::pair<std::string, SgAssociation>> asoArr) {
		std::vector<SgAssociationHandler> output;

		for (auto &aso : asoArr) {
			auto handler = addAssociation(aso.first, aso.second);
			output.push_back(handler);
		}

		return output;
	}

	std::vector<SgAssociationHandler> SgDescriptionHandler::associations() {
		std::vector<SgAssociationHandler> output;

		for (auto ast : _dataObject->rawData<fl::DataObject>()->properties()) {

			if (ast->type() == ASSOCIATION_DESCRIPTION) {
				output.push_back(SgAssociationHandler(_space, ast));
			}
		}

		return output;
	}
}