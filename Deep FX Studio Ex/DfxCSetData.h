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

	/// Set Data Node
	inline void setDataMethod(fl::SpaceInterface *space, fl::DataObject *dao, fl::InterpreterScope *scope) {
		if (!dao->hasProperty("attribute")) return;
		if (!dao->hasProperty("data")) return;

		auto _attribute = scope->fetchInput(dao->property("attribute")->absolutePath())[0];

		std::string attribute_path = _attribute->data<std::string>();
		auto _data = scope->fetchInput(dao->property("data")->absolutePath())[0];

		auto attr = DfxAttributeTree::getInstance()->findAttribute(attribute_path);

		if (attr != nullptr) {
			if (attr->isWritable()) {
				auto value = attr->getValue();

				if (attr->type() == _data->type()) {
					scope->addOutput(dao->property("data")->absolutePath(), value);

					if (attr->isArray() && !value->isArray()) {
						dao->setErrorFlag(true);
						dao->setErrorString("Attribute only accepts arrays.");	
						return;
					}

					attr->setValue(_data);
				}
				else {
					dao->setErrorFlag(true);
					dao->setErrorString("Unknown input type.");
					return;
				}
			}
			else {
				dao->setErrorFlag(true);
				dao->setErrorString("Atrribute is not writable.");
				return;
			}
			
		}
		else {

			dao->setErrorFlag(true);
			dao->setErrorString("Failed to find attribute.");

			return;
		}

		dao->setErrorFlag(false);
	}

	inline fl::Any *setDataNode(fl::SpaceInterface *space) {

		auto mnode = fl::DataObject::Builder()
			.setName("setData")
			.setLabel("Set Data")
			.setMode(fl::DataObject::NODE)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
			.addProperty(space, fl::type::ATTRIBUTE_PATH, fl::role::INPUT, "attribute", "Attribute Path", std::string(""))
			.addProperty(space, fl::type::ANY, fl::role::INPUT, "data", "Data In", 0.f)
			.addFunction(fl::DataObject::MAIN, setDataMethod)
			.build(space);

		return mnode;
	}

	inline void registerSetDataNode() {
		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = "DFX_SET_DATA";
		desc->name = "Set Data";
		desc->description = "Set Attribute Value";
		desc->category = "Data";
		desc->placement = "Data/Set Data";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = setDataNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}
}