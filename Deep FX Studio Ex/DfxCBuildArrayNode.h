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
#include "DfxQComputeBaseNode.h"
#include "DfxSpaceUtil.h"

namespace compute_node {

	static const std::string buildArray_model_id = "/COMPUTE/BUILD_ARRAY_NODE";

	class BuildArrayNodeHandler : public fl::Model {
	public:

		/// String Node
		static inline void buildArrayMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {

			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);
			error->hasError = false;

			if (!args.empty()) {
				auto baseType = args[0]->type();

				if (!DfxQComputeBaseNode::isValidProperty(baseType)) {
					error->hasError = true;
					error->errorMsg = "Unsupported Base Type";
					return;
				}

				for (auto a : args) {
					if (a->type() != baseType) {
						error->hasError = true;
						error->errorMsg = "Base Type Mismatch: All inputs must be the same type";
						return;
					}
				}

				for (auto a : args) {
					if (a->isArray()) {
						error->hasError = true;
						error->errorMsg = "Arrays not supported";
						return;
					}
				}

				auto anyArray = util::duplicateState(args[0], true, false);
				util::pushArray(anyArray, args);

				returnValues.push_back(anyArray);

			}

		}

		static inline fl::Any *buildArrayNode(fl::Space* space) {
			auto mnode = fl::DataObject::Builder()
				.setName("build_array")
				.setLabel("Build Array")
				.setMode(fl::DataObject::NODE)
				.setAllowDynamicInput(true)
				.setHasModel(true)
				.setModelReference(buildArray_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::ANY, fl::role::OUTPUT, "array", "Array", std::string(""))
				.addStaticFunction(fl::DataObject::STATIC_MAIN, buildArrayMethod)
				.build(space);

			mnode->rawData<fl::DataObject>()->property("array")->setIsArray(true);
			mnode->rawData<fl::DataObject>()->property("array")->setAllowAnyType(true);

			return mnode;
		}

		inline fl::Any *createModel(fl::Space* space) override {
			return buildArrayNode(space);
		}

	};

	inline void registerBuildaArrayNode() {
		/// String Node Desc
		DfxComputeNodeDescription *desc_string = new DfxComputeNodeDescription;
		desc_string->identifier = buildArray_model_id;
		desc_string->name = "Build Array";
		desc_string->description = "Build Array Using Input Slots";
		desc_string->category = "Arrays";
		desc_string->placement = "Array/Build Array";
		desc_string->major = 1;
		desc_string->minor = 0;
		desc_string->instance = BuildArrayNodeHandler::buildArrayNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc_string);

	}
}