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

namespace compute_node {

	/// String Node
	inline void peekMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {

		auto _anyError = new fl::Any();
		auto error = new fl::InterpreterError;
		_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
		returnValues.push_back(_anyError);
		error->hasError = false;

		if (!args.empty()) {

		}

	}

	inline fl::Any *peekNode(fl::SpaceInterface *space) {

		auto mnode = fl::DataObject::Builder()
			.setName("peek")
			.setLabel("Peek")
			.setMode(fl::DataObject::NODE)
			.setAllowDynamicInput(true)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
			.addProperty(space, fl::type::ANY, fl::role::OUTPUT, "out_array", "Array", 0)
			.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "size", "Size", 0)
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "functor", "Functor", std::string(""))
			.addProperty(space, fl::type::ANY, fl::role::INPUT, "array", "Array", 0)
			.addProperty(space, fl::type::BOOL, fl::role::INPUT, "is_parallel", "Is Parallel", false)
			.addProperty(space, fl::type::BOOL, fl::role::INPUT, "pass_index", "Pass Index", false)
			.addStaticFunction(fl::DataObject::STATIC_MAIN, peekMethod)
			.build(space);

		mnode->rawData<fl::DataObject>()->property("array")->setIsArray(true);
		mnode->rawData<fl::DataObject>()->property("array")->setAllowAnyType(true);
		mnode->rawData<fl::DataObject>()->property("out_array")->setIsArray(true);
		mnode->rawData<fl::DataObject>()->property("out_array")->setAllowAnyType(true);

		mnode->rawData<fl::DataObject>()->setIterationType(fl::IterationType::PEEK);

		return mnode;
	}

	inline void registerPeekNode() {
		/// String Node Desc
		DfxComputeNodeDescription *desc_string = new DfxComputeNodeDescription;
		desc_string->identifier = "DFX_PEEK";
		desc_string->name = "PEEK";
		desc_string->description = "Peek Iteration";
		desc_string->category = "Iteration";
		desc_string->placement = "Iteration/Peek";
		desc_string->major = 1;
		desc_string->minor = 0;
		desc_string->instance = peekNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc_string);

	}
}