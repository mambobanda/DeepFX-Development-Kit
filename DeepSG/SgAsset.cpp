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

#include "SgAsset.h"

namespace deepsg {

	SgAsset::SgAsset() :SgDescription(SgDescription::ASSET) {
		_context = GLOBAL_SPACE;
		_kind = NONE;
		_path = "";
	}

	SgAsset::~SgAsset() {

	}

	void SgAsset::setContext(unsigned int ctx) {
		_context = ctx;
	}

	unsigned int SgAsset::context() {
		return _context;
	}

	void SgAsset::setKind(unsigned int kd) {
		_kind = kd;
	}

	unsigned int SgAsset::kind() {
		return _kind;
	}

	std::string SgAsset::path() {
		return _path;
	}

	void SgAsset::setPath(std::string pth) {
		_path = pth;
	}

	std::string SgAsset::enumToString(unsigned int em) {
		switch (em) {
		case NONE:
			return "none";
			break;
		case ASSET:
			return "asset";
			break;
		case GEOMETRY:
			return "geometry";
			break;
		case MATERIAL:
			return "material";
			break;
		case LIGHT:
			return "light";
			break;
		case CAMERA:
			return "camera";
			break;
		case XFORM:
			return "xform";
			break;
		case GLOBAL_SPACE:
			return "global space";
			break;
		case ASSET_SPACE:
			return "asset space";
			break;
		case REFERENCE:
			return "reference";
			break;
		default:
			return "none";
			break;
		}
	}

	unsigned int SgAsset::stringToEnum(std::string str) {

		if (str == "none") {
			return NONE;
		}
		else if (str == "asset") {
			return ASSET;
		}
		else if (str == "geometry") {
			return GEOMETRY;
		}
		else if (str == "material") {
			return MATERIAL;
		}
		else if (str == "light") {
			return LIGHT;
		}
		else if (str == "camera") {
			return CAMERA;
		}
		else if (str == "xform") {
			return XFORM;
		}
		else if (str == "global space") {
			return GLOBAL_SPACE;
		}
		else if (str == "asset space") {
			return ASSET_SPACE;
		}
		else if (str == "reference") {
			return REFERENCE;
		}

		return 100;
	}

}