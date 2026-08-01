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
#include <DfxGlobal.h>
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"

namespace compute_node {

	/// If Then Else Node
	inline void ifThenElseMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {
		auto _anyError = new fl::Any();
		auto error = new fl::InterpreterError;
		_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
		returnValues.push_back(_anyError);
		error->hasError = false;

		if (!args.empty()) {

			if (args[0]->type() != fl::type::BOOL) {
				error->hasError = true;
				error->errorMsg = "Unknown Input Type [0]";
				return;
			}
			else if (args[1]->type() != args[2]->type()) {
				error->hasError = true;
				error->errorMsg = "Unknown Input Type [1] / [2]";
				return;
			}
			else if (args[0]->isArray() || args[1]->isArray() || args[2]->isArray() ) {
				error->hasError = true;
				error->errorMsg = "Inputs do not accept arrays.";
				return;
			}

			bool boolean = args[0]->data<bool>();
			fl::Any *retAny = new fl::Any;

			if (args[1]->type() == fl::type::FLOAT) {
				retAny->newData<float>(fl::type::FLOAT);

				(boolean) ? retAny->setData<float>(args[1]->data<float>()) : retAny->setData<float>(args[2]->data<float>());
			}
			else if (args[1]->type() == fl::type::FLOAT_3) {
				retAny->newData<glm::vec3>(fl::type::FLOAT_3);

				(boolean) ? retAny->setData<glm::vec3>(args[1]->data<glm::vec3>()) : retAny->setData<glm::vec3>(args[2]->data<glm::vec3>());

			}
			else if (args[1]->type() == fl::type::SIZE_T) {
				retAny->newData<size_t>(fl::type::SIZE_T);

				(boolean) ? retAny->setData<size_t>(args[1]->data<size_t>()) : retAny->setData<size_t>(args[2]->data<size_t>());
			}
			else if (args[1]->type() == fl::type::BOOL) {
				retAny->newData<bool>(fl::type::BOOL);

				(boolean) ? retAny->setData<bool>(args[1]->data<bool>()) : retAny->setData<bool>(args[2]->data<bool>());
			}
			else if (args[1]->type() == fl::type::STD_STRING) {
				retAny->newData<std::string>(fl::type::STD_STRING);

				(boolean) ? retAny->setData<std::string>(args[1]->data<std::string>()) : retAny->setData<std::string>(args[2]->data<std::string>());
			}
			else if (args[1]->type() == fl::type::ATTRIBUTE_PATH) {
				retAny->newData<std::string>(fl::type::ATTRIBUTE_PATH);

				(boolean) ? retAny->setData<std::string>(args[1]->data<std::string>()) : retAny->setData<std::string>(args[2]->data<std::string>());
			}
			else {
				error->hasError = true;
				error->errorMsg = "Unrecognized input type [1] / [2].";
				return;
			}

			returnValues.push_back(retAny);		
		}
	}

	inline fl::Any *ifThenElseNode(fl::SpaceInterface *space) {

		auto mnode = fl::DataObject::Builder()
			.setName("if_then_else")
			.setLabel("If Then Else")
			.setMode(fl::DataObject::NODE)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
			.addProperty(space, fl::type::ANY, fl::role::OUTPUT, "value", "Value", 0)
			.addProperty(space, fl::type::BOOL, fl::role::INPUT, "bool", "True/False", false)
			.addProperty(space, fl::type::ANY, fl::role::INPUT, "value_0", "Value 1", 0)
			.addProperty(space, fl::type::ANY, fl::role::INPUT, "value_1", "Value 2", 0)
			.addStaticFunction(fl::DataObject::STATIC_MAIN, ifThenElseMethod)
			.build(space);

			mnode->rawData<fl::DataObject>()->property("value")->setAllowAnyType(true);

		return mnode;
	}

	inline void registerIfThenElseNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = "DFX_IF_THEN_ELSE";
		desc->name = "If Then Else";
		desc->description = "If Then Else Comparison. If true pass first valuse, if not pass second value.";
		desc->category = "Comparison";
		desc->placement = "Comparision/If Then Else";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = ifThenElseNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}
}