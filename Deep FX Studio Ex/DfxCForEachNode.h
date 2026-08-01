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

	static const std::string forEach_model_id = "/COMPUTE/FOR_EACH";

	class ForEachNodeHandler : public fl::Model {
	public:
		/// String Node
		static inline void forEachMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {

			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);
			error->hasError = false;

			if (!args.empty()) {

			}

		}

		static inline fl::Any *forEachNode(fl::SpaceInterface *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("for_each")
				.setLabel("For Each")
				.setMode(fl::DataObject::NODE)
				.setAllowDynamicInput(true)
				.setHasModel(true)
				.setModelReference(forEach_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "functor", "Functor", std::string(""))
				.addProperty(space, fl::type::ANY, fl::role::INPUT, "array", "Array", std::string(""))
				.addProperty(space, fl::type::BOOL, fl::role::INPUT, "is_parallel", "Is Parallel", false)
				.addProperty(space, fl::type::BOOL, fl::role::INPUT, "pass_index", "Pass Index", false)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, ForEachNodeHandler::forEachMethod)
				.build(space);

			mnode->rawData<fl::DataObject>()->property("array")->setIsArray(true);
			mnode->rawData<fl::DataObject>()->property("array")->setAllowAnyType(true);
			mnode->rawData<fl::DataObject>()->setIterationType(fl::IterationType::FOR_EACH);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return forEachNode(space);
		}
	};

	inline void registerForEachNode() {
		/// String Node Desc
		DfxComputeNodeDescription *desc_string = new DfxComputeNodeDescription;
		desc_string->identifier = "DFX_FOR_EACH";
		desc_string->name = "For Each";
		desc_string->description = "For Each Loop";
		desc_string->category = "Iteration";
		desc_string->placement = "Iteration/For Each";
		desc_string->major = 1;
		desc_string->minor = 0;
		desc_string->instance = ForEachNodeHandler::forEachNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc_string);

	}
}