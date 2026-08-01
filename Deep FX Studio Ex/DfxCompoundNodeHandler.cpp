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

#include "DfxCompoundNodeHandler.h"

std::string DfxCompoundNodeHandler::model_id = "/FLUENT/COMPOUND_NODE_INFO";

fl::Any *DfxCompoundNodeHandler::createModel(fl::Space *space) {
	return fl::DataObject::Builder()
		.setName("compound")
		.setLabel("Compound Node")
		.setMode(fl::DataObject::COMPOUND_NODE)
		.setHasModel(true)
		.setModelReference(model_id)
		.setIsFunctional(false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "in_xpos", "Input X-Position", -200.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "in_ypos", "Input Y-Position", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "out_xpos", "Output X-Position", 200.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "out_ypos", "Output Y-Position", 0.f)
		.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "path", "Compound Path", std::string(""))
		.build(space);
}