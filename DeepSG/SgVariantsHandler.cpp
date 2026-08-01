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

#include "SgVariantsHandler.h"
#include "SgVariants.h"

namespace deepsg {

	SgVariantsHandler::SgVariantsHandler(fl::Space *_space) : SgDescriptionHandler(_space, nullptr) {
	}

	SgVariantsHandler::SgVariantsHandler(fl::Space *_space, fl::Any *_aPtr) : SgDescriptionHandler(_space, _aPtr) {
	}

	void SgVariantsHandler::setKind(unsigned int knd) {
		_dataObject->rawData<fl::DataObject>()->value()->rawData<SgVariants>()->setKind(knd);
	}

	unsigned int SgVariantsHandler::kind() {
		return _dataObject->rawData<fl::DataObject>()->value()->rawData<SgVariants>()->kind();
	}

}