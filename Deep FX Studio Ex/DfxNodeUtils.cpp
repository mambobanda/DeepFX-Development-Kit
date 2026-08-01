/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include "DfxNodeUtils.h"

namespace util {
	void copyDataObjectProperties(fl::DataObject &from, fl::DataObject &to) {

		auto &fromProperties = from.propertiesOriginalOrder();
		auto &toProperties = to.propertiesOriginalOrder();

		for (auto inx = 0; inx < fromProperties.size(); inx++) {
			if (fromProperties[inx]->name() == toProperties[inx]->name()) {
				fl::copy(fromProperties[inx], toProperties[inx]);
			}
		}
	}
}