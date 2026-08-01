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
#include "DfxFluentChannel.h"
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"

namespace compute_node {

	static const std::string valueString_model_id	= "/COMPUTE/VALUE_STRING_NODE";
	static const std::string toString_model_id		= "/COMPUTE/TO_STRING_NODE";
	static const std::string concatString_model_id	= "/COMPUTE/CONCAT_STRING_NODE";

	class ValueStringNodeHandler : public fl::Model {
	public:

		static inline void valueStringMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {

			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			if (!args.empty()) {

				if (args[0]->type() != fl::type::STD_STRING) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type";

					return;
				}
				else if (args[0]->isArray()) {
					error->hasError = true;
					error->errorMsg = "Input 'a' does not accept arrays.";

					return;
				}

				std::string str = args[0]->data<std::string>();

				auto result = new fl::Any();
				result->newData<std::string>(fl::type::STD_STRING);
				result->setData<std::string>(std::string(str));

				error->hasError = false;

				returnValues.push_back(result);
			}
			else {
				error->hasError = true;
				error->errorMsg = "No arguments found";
			}
		}

		static inline fl::Any *valueStringNode(fl::SpaceInterface *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("string")
				.setLabel("String")
				.setMode(fl::DataObject::NODE)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "a", "a", std::string(""))
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "string", "String", std::string(""))
				.addStaticFunction(fl::DataObject::STATIC_MAIN, valueStringMethod)
				.build(space);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return valueStringNode(space);
		}
	};

	class ToStringNodeHandler : public fl::Model {
	public:

		static inline void toStringMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {
				
				if (args[0]->isArray()) {
					error->hasError = true;
					error->errorMsg = "Input does not accept arrays.";

					return;
				}

				if (!args[1]->isArray()) {
					error->hasError = true;
					error->errorMsg = "Input expected array.";

					return;
				}

				std::string out00 = "";
				std::string out01 = "";

				if (args[0]->type() == fl::type::FLOAT) {
					out00 = std::to_string(args[0]->data<float>());
				}
				else if (args[0]->type() == fl::type::FLOAT_3) {
					auto x = std::to_string(args[0]->data<glm::vec3>().x);
					auto y = std::to_string(args[0]->data<glm::vec3>().y);
					auto z = std::to_string(args[0]->data<glm::vec3>().z);

					out00 = "[ " + x + ", " + y + ", " + z + " ]";
				}
				else if (args[0]->type() == fl::type::SIZE_T) {
					out00 = std::to_string(args[0]->data<size_t>());
				}
				else if (args[0]->type() == fl::type::STD_STRING) {
					out00 = args[0]->data<std::string>();
				}
				else if (args[0]->type() == fl::type::BOOL) {
					out00 = args[0]->data<bool>() ? "True" : "False";
				}

				if (args[1]->type() == fl::type::FLOAT) {
					auto arr = args[1]->rawData<std::vector<float>>();
					std::stringstream ss;
					for (size_t i = 0; i < arr->size(); i++) {
						ss << std::to_string((*arr)[i]) << std::endl;
					}

					out01 = ss.str();
				}
				else if (args[1]->type() == fl::type::FLOAT_3) {
					auto arr = args[1]->rawData<std::vector<glm::vec3>>();
					std::stringstream ss;
					for (size_t i = 0; i < arr->size(); i++) {
						auto x = std::to_string((*arr)[i].x);
						auto y = std::to_string((*arr)[i].y);
						auto z = std::to_string((*arr)[i].z);
						ss << "[ " + x + ", " + y + ", " + z + " ]" << std::endl;
					}

					out01 = ss.str();
				}
				else if (args[1]->type() == fl::type::SIZE_T) {
					auto arr = args[1]->rawData<std::vector<size_t>>();
					std::stringstream ss;
					for (size_t i = 0; i < arr->size(); i++) {
						ss << std::to_string((*arr)[i]) << std::endl;
					}

					out01 = ss.str();
				}
				else if (args[1]->type() == fl::type::STD_STRING) {
					auto arr = args[1]->rawData<std::vector<std::string>>();
					std::stringstream ss;
					for (size_t i = 0; i < arr->size(); i++) {
						ss <<(*arr)[i] << std::endl;
					}

					out01 = ss.str();
				}
				else if (args[1]->type() == fl::type::BOOL) {
					auto arr = args[1]->rawData<std::vector<bool>>();
					std::stringstream ss;
					for (size_t i = 0; i < arr->size(); i++) {
						ss << ((*arr)[i] ? "True" : "False") << std::endl;
					}

					out01 = ss.str();
				}

				auto result00 = new fl::Any();
				result00->newData<std::string>(fl::type::STD_STRING);
				result00->setData<std::string>(out00);

				auto result01 = new fl::Any();
				result01->newData<std::string>(fl::type::STD_STRING);
				result01->setData<std::string>(out01);

				returnValues.push_back(result00);
				returnValues.push_back(result01);
			}
		}
	
		static inline fl::Any *toStringNode(fl::SpaceInterface *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("to_string")
				.setLabel("To String")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(toString_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "value_string", "Value String", std::string(""))
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "array_string", "Array String", std::string(""))
				.addProperty(space, fl::type::ANY, fl::role::INPUT, "value", "Value", 0.f)
				.addProperty(space, fl::type::ANY, fl::role::INPUT, "array", "Array", 0.f)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, toStringMethod)
				.build(space);


			mnode->rawData<fl::DataObject>()->property("array")->setIsArray(true);
			mnode->rawData<fl::DataObject>()->property("value")->setAllowAnyType(true);
			mnode->rawData<fl::DataObject>()->property("array")->setAllowAnyType(true);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return toStringNode(space);
		}
	};

	class ConcatStringNodeHandler : public fl::Model {
	public:
		
		static inline void concactStringsMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {

			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::STD_STRING ||
					args[1]->type() != fl::type::STD_STRING) {

					error->hasError = true;
					error->errorMsg = "Unknown Input Type";

					return;
				}
				else if (args[0]->isArray() || args[1]->isArray()) {
					error->hasError = true;
					error->errorMsg = "Inputs do not accept arrays.";

					return;
				}

				std::string sa = args[0]->data<std::string>();
				std::string sb = args[1]->data<std::string>();

				auto result = new fl::Any();
				result->newData<std::string>(fl::type::STD_STRING);
				result->setData<std::string>(std::string(sa + sb));

				returnValues.push_back(result);
			}
		}

		static inline fl::Any *concactStringsNode(fl::SpaceInterface *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("concact_string")
				.setLabel("Combine Strings")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(concatString_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "a", "String A", std::string(""))
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "b", "String B", std::string(""))
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "string", "String", std::string(""))
				.addStaticFunction(fl::DataObject::STATIC_MAIN, concactStringsMethod)
				.build(space);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return concactStringsNode(space);
		}
	};

	inline void registerCommonStringNodes() {

		// String Node Desc
		DfxComputeNodeDescription *desc_string = new DfxComputeNodeDescription;
		desc_string->identifier = valueString_model_id;
		desc_string->name = "String value";
		desc_string->description = "String value.";
		desc_string->category = "Strings";
		desc_string->placement = "Strings/String Value";
		desc_string->major = 1;
		desc_string->minor = 0;
		desc_string->instance = ValueStringNodeHandler::valueStringNode;

		// Decimal To String Node Desc
		DfxComputeNodeDescription *desc_to_string = new DfxComputeNodeDescription;
		desc_to_string->identifier = toString_model_id;
		desc_to_string->name = "To String";
		desc_to_string->description = "Convert a value to a string";
		desc_to_string->category = "Strings";
		desc_to_string->placement = "Strings/To String";
		desc_to_string->major = 1;
		desc_to_string->minor = 0;
		desc_to_string->instance = ToStringNodeHandler::toStringNode;

		// Concact Strings Node Desc
		DfxComputeNodeDescription *desc_concact_strings = new DfxComputeNodeDescription;
		desc_concact_strings->identifier = concatString_model_id;
		desc_concact_strings->name = "Concact String";
		desc_concact_strings->description = "Combine Two Strings";
		desc_concact_strings->category = "String";
		desc_concact_strings->placement = "Strings/Concact Strings";
		desc_concact_strings->major = 1;
		desc_concact_strings->minor = 0;
		desc_concact_strings->instance = ConcatStringNodeHandler::concactStringsNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc_string);
		DfxComputeNodeDB::getInstance()->registerComputeNode(desc_to_string);
		DfxComputeNodeDB::getInstance()->registerComputeNode(desc_concact_strings);
		// DfxComputeNodeDB::getInstance()->registerComputeNode(desc_i_to_string);

	}
}