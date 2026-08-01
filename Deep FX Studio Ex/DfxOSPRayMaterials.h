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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <FlModel.h>
#include "DfxAny.h"
#include "DfxSpace.h"
#include "DfxRenderTarget.h"

namespace ospmat {

	// Materials
	static const std::string principledID = "MATERIAL/OSPRAY_PRINCIPLED_MAT";
	class PrincipledModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *principled(fl::Space *space);
	};

	// Textured
	static const std::string imageTextureID = "MATERIAL/OSPRAY_IMAGE_TEXTURE";
	class ImageTextureModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *imageTexture(fl::Space *space);
	};

}