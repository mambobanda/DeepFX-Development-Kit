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

#include <FlSpace.h>
#include <FlInterpreterScope.h>
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"
#include "DfxAttributeTree.h"

namespace compute_node {

	/// Get Data Node
	inline void getDataMethod(fl::SpaceInterface *space, fl::DataObject *dao, fl::InterpreterScope *scope) {
		if (!dao->hasProperty("attribute")) return;
		if (!dao->hasProperty("data")) return;

		auto _attribute = scope->fetchInput(dao->property("attribute")->absolutePath())[0];

		std::string attribute_path = _attribute->data<std::string>();

		auto attr = DfxAttributeTree::getInstance()->findAttribute(attribute_path);

		if (attr != nullptr) {	
			if (attr->isReadable()) {
				auto value = attr->getValue();
				scope->addOutput(dao->property("data")->absolutePath(), value);
			}

			dao->setErrorFlag(false);
		}
		else {
			auto result = new fl::Any();
			result->newData<size_t>(fl::type::SIZE_T);
			result->setData<size_t>((size_t)0);

			scope->addOutput(dao->property("data")->absolutePath(), result);

			dao->setErrorFlag(true);
			dao->setErrorString("Failed to find Attribute");
		}
		
	}

	inline fl::Any *getDataNode(fl::SpaceInterface *space) {

		auto mnode = fl::DataObject::Builder()
			.setName("getData")
			.setLabel("Get Data")
			.setMode(fl::DataObject::NODE)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
			.addProperty(space, fl::type::ATTRIBUTE_PATH, fl::role::INPUT, "attribute", "Attribute Path", std::string(""))
			.addProperty(space, fl::type::ANY,  fl::role::OUTPUT, "data", "Data Out", 0.f)
			.addFunction(fl::DataObject::MAIN, getDataMethod)
			.build(space);

		return mnode;
	}

	inline void registerGetDataNode() {
		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = "DFX_GET_DATA";
		desc->name		= "Get Data";
		desc->description = "Get Attribute Value";
		desc->category  = "Data";
		desc->placement = "Data/Get Data";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = getDataNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}
}