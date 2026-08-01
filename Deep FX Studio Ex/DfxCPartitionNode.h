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
	inline void partitionMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {

		auto _anyError = new fl::Any();
		auto error = new fl::InterpreterError;
		_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
		returnValues.push_back(_anyError);
		error->hasError = false;

		if (!args.empty()) {

		}

	}

	inline fl::Any *partitionNode(fl::SpaceInterface *space) {

		auto mnode = fl::DataObject::Builder()
			.setName("partition")
			.setLabel("Partition")
			.setMode(fl::DataObject::NODE)
			.setAllowDynamicInput(true)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
			.addProperty(space, fl::type::ANY, fl::role::OUTPUT, "out_array0", "Array 1", 0)
			.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "size0", "Size 1", 0.f)
			.addProperty(space, fl::type::ANY, fl::role::OUTPUT, "out_array1", "Array 2", 0)
			.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "size1", "Size 2", 0.f)
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "functor", "Functor", std::string(""))
			.addProperty(space, fl::type::ANY, fl::role::INPUT, "array", "Array", 0)
			.addProperty(space, fl::type::BOOL, fl::role::INPUT, "is_parallel", "Is Parallel", false)
			.addProperty(space, fl::type::BOOL, fl::role::INPUT, "pass_index", "Pass Index", false)
			.addStaticFunction(fl::DataObject::STATIC_MAIN, partitionMethod)
			.build(space);

		mnode->rawData<fl::DataObject>()->property("array")->setIsArray(true);
		mnode->rawData<fl::DataObject>()->property("array")->setAllowAnyType(true);
		
		mnode->rawData<fl::DataObject>()->property("out_array0")->setIsArray(true);
		mnode->rawData<fl::DataObject>()->property("out_array0")->setAllowAnyType(true);
	
		mnode->rawData<fl::DataObject>()->property("out_array1")->setIsArray(true);
		mnode->rawData<fl::DataObject>()->property("out_array1")->setAllowAnyType(true);

		mnode->rawData<fl::DataObject>()->setIterationType(fl::IterationType::PARTITION);

		return mnode;
	}

	inline void registerPartitionNode() {
		/// String Node Desc
		DfxComputeNodeDescription *desc_string = new DfxComputeNodeDescription;
		desc_string->identifier = "DFX_PARTITION";
		desc_string->name = "Partition";
		desc_string->description = "Partition Iteration";
		desc_string->category = "Iteration";
		desc_string->placement = "Iteration/Partition";
		desc_string->major = 1;
		desc_string->minor = 0;
		desc_string->instance = partitionNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc_string);

	}
}