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

#include "SgVariants.h"

namespace deepsg {

	SgVariants::SgVariants() :SgDescription(SgDescription::VARIANTS) {
		_kind = NONE;
	}

	SgVariants::~SgVariants() {

	}

	void SgVariants::setKind(unsigned int kd) {
		_kind = kd;
	}

	unsigned int SgVariants::kind() {
		return _kind;
	}

	std::string SgVariants::enumToString(unsigned int em) {
		switch (em) {
		case NONE:
			return "none";
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
		default:
			return "none";
			break;
		};
	}

	unsigned int SgVariants::stringToEnum(std::string str) {
		if (str == "none") {
			return NONE;
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

		return 1000;
	}
}