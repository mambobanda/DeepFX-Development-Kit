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
#include <FlModel.h>
#include "DfxFluentSet.h"
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"
#include "DfxFluentSetManager.h"
#include "DfxFluentBigID.h"

namespace compute_node {

	static const std::string setSize_model_id = "/COMPUTE/SET_SIZE_NODE";

	class SetSizeNodeHandler : public fl::Model {
	public:

		static inline void setSizeMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues)
		{
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::STD_STRING) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0], expected string";
					return;
				}
				

				if (args[0]->isArray() ) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}

				auto setName = args[0]->data<std::string>();		

				float size = 0.f;

				if (!setName.empty()) {
					auto set = DfxFluentSetManager::getInstance()->getSet(setName);

					if (set != nullptr) {
						size = (float)set->indices().size();
					}
					else {
						error->hasError = true;
						error->errorMsg = "Set Doesn't Exist.";
						return;
					}
				}

				auto rany01 = new fl::Any();
				rany01->newData<float>(fl::type::FLOAT);
				rany01->setData(size);
				returnValues.push_back(rany01);

			}
		}

		static inline fl::Any *setSizeNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("set_size")
				.setLabel("Set Size")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(setSize_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "size", "Size", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "set", "Set", std::string())
				.addStaticFunction(fl::DataObject::STATIC_MAIN, setSizeMethod)
				.build(space);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return setSizeNode(space);
		}
	};

	inline void registerSetSizeNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = setSize_model_id;
		desc->name = "Set Size";
		desc->description = "The number of elements in a set.";
		desc->category = "Sets";
		desc->placement = "Sets/Set Size";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = SetSizeNodeHandler::setSizeNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}

}