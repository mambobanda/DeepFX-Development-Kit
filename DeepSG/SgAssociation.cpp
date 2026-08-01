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

#include "SgAssociation.h"

namespace deepsg {

	SgAssociation::SgAssociation() :SgDescription(SgDescription::ASSOCIATION) {
		_kind = NONE;
		_surface = "";
		_path = "";
	}

	SgAssociation::~SgAssociation() {

	}

	void SgAssociation::setKind(unsigned int kd) {
		_kind = kd;
	}

	unsigned int SgAssociation::kind() {
		return _kind;
	}

	std::string SgAssociation::path() {
		return _path;
	}

	void SgAssociation::setPath(std::string pth) {
		_path = pth;
	}

	std::string SgAssociation::surface() {
		return _surface;
	}

	void SgAssociation::setSurface(std::string pth) {
		_surface = pth;
	}

	std::string SgAssociation::enumToString(unsigned int em) {
		switch (em) {
		case NONE:
			return "none";
			break;
		case SURFACE_MATERIAL:
			return "surface material";
			break;
		case GROUP_ASSET:
			return "group asset";
			break;
		case VARIANT_ASSET:
			return "variant asset";
			break;
		case ASSET_ASSET:
			return "asset asset";
			break;
		default:
			return "none";
			break;
		};
	}

	unsigned int SgAssociation::stringToEnum(std::string str) {
		if (str == "none") {
			return NONE;
		}
		else if (str == "surface material") {
			return SURFACE_MATERIAL;
		}
		else if (str == "group asset") {
			return GROUP_ASSET;
		}
		else if (str == "variant asset") {
			return VARIANT_ASSET;
		}
		else if (str == "asset asset") {
			return ASSET_ASSET;
		}

		return 1000;
	}
}