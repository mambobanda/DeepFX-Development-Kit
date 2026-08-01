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
#include <FlModel.h>
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"

namespace compute_node {

	static const std::string floatLessThan_model_id = "/COMPUTE/FLOAT_LESS_THAN_NODE";
	static const std::string floatEqualTo_model_id  = "/COMPUTE/FLOAT_EQUAL_TO_NODE";

	class FloatLessThanNodeHandler : public fl::Model {
	public:
		/// Float Less Than Node
		static inline void floatLessThanMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {

			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);
			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::FLOAT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0]";

					return;
				}
				if (args[1]->type() != fl::type::FLOAT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [1]";

					return;
				}

				if (args[2]->type() != fl::type::BOOL) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [2]";

					return;
				}
		
				if (args[0]->isArray() || args[1]->isArray() || args[2]->isArray()) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}


				float a = args[0]->data<float>();
				float b = args[1]->data<float>();
				bool isEqual = args[2]->data<bool>();

				bool boolean = false;

				if (isEqual) boolean = (a <= b);
				else boolean = (a < b);

				auto rany = new fl::Any();
				rany->newData<std::vector<bool>>(fl::type::BOOL);
				rany->setData(boolean);

				returnValues.push_back(rany);

			}
		}

		static inline fl::Any *floatLessThanNode(fl::SpaceInterface *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("float_less_than")
				.setLabel("Less Than")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(floatLessThan_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::BOOL, fl::role::OUTPUT, "bool", "True/False", false)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "a", "a", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "b", "b", 0.f)
				.addProperty(space, fl::type::BOOL, fl::role::INPUT, "and_equal", "And Equal", false)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, floatLessThanMethod)
				.build(space);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return floatLessThanNode(space);
		}
	};

	class FloatEqualToNodeHandler : public fl::Model {
	public:
		/// Float Equal To
		static inline void floatEqualToMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {

			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);
			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::FLOAT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0]";

					return;
				}
				if (args[1]->type() != fl::type::FLOAT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [1]";

					return;
				}

				if (args[2]->type() != fl::type::BOOL) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [2]";

					return;
				}

				else if (args[0]->isArray() || args[1]->isArray() || args[2]->isArray()) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}


				float a = args[0]->data<float>();
				float b = args[1]->data<float>();
				bool isNotEqual = args[2]->data<bool>();

				bool boolean = false;

				if (isNotEqual) boolean = (a != b);
				else boolean = (a == b);

				auto rany = new fl::Any();
				rany->newData<std::vector<bool>>(fl::type::BOOL);
				rany->setData(boolean);

				returnValues.push_back(rany);

			}
		}

		static inline fl::Any *floatEqualToNode(fl::SpaceInterface *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("float_equal_to")
				.setLabel("Equal To")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(floatEqualTo_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::BOOL, fl::role::OUTPUT, "bool", "True/False", false)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "a", "a", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "b", "b", 0.f)
				.addProperty(space, fl::type::BOOL, fl::role::INPUT, "not", "Not", false)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, floatEqualToMethod)
				.build(space);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return floatEqualToNode(space);
		}
	};

	inline void registerFloatComparisonNodes() {
		DfxComputeNodeDescription *desc00 = new DfxComputeNodeDescription;
		desc00->identifier = floatLessThan_model_id;
		desc00->name = "Float Less Than";
		desc00->description = "Less Than comparision operator.";
		desc00->category = "Math";
		desc00->placement = "Math/Float Comparison/Less Than";
		desc00->major = 1;
		desc00->minor = 0;
		desc00->instance = FloatLessThanNodeHandler::floatLessThanNode;

		DfxComputeNodeDescription *desc01 = new DfxComputeNodeDescription;
		desc01->identifier = floatEqualTo_model_id;
		desc01->name = "Float Equal To";
		desc01->description = "Equal To comparision operator.";
		desc01->category = "Math";
		desc01->placement = "Math/Float Comparison/Equal To";
		desc01->major = 1;
		desc01->minor = 0;
		desc01->instance = FloatEqualToNodeHandler::floatEqualToNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc00);
		DfxComputeNodeDB::getInstance()->registerComputeNode(desc01);
	}
}