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
#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"

namespace compute_node {

	static const std::string floatValue_model_id		= "/COMPUTE/FLOAT_VALUE_NODE";    // 0
	static const std::string arithmeticFloat_model_id	= "/COMPUTE/MULTIPLY_FLOAT_NODE"; // 2
	static const std::string mathFunctionFloat_model_id = "/COMPUTE/MATH_FUNCTION_FLOAT_NODE"; // 7
	static const std::string piFloat_model_id			= "/COMPUTE/PI_FLOAT_NODE";  // 13
	static const std::string floatHasValue_model_id		= "/COMPUTE/FLOAT_HAS_VALUE_NODE";    // 0


	inline float arithmeticOp(int op, float a, float b) {
		
		if (op == 0) {
			return a * b;
		}
		else if (op == 1) {
			if (b == 0) return 0.f;
			return a / b;
		}
		else if (op == 2) {
			return a + b;
		}
		else if (op == 3) {
			return a - b;
		}
		else if (op == 4) {
			if (b == 0) return 0.f;
			return std::fmodf(a , b);
		}
		else if (op == 5) {
			return std::powf(a, b);
		}
		
		return 0.f;
	}

	inline float scalarOp(int op, float scl) {

		if (op == 0) {
			return std::sqrtf(scl);
		}
		else if (op == 1) {
			return std::sin(scl);
		}
		else if (op == 2) {
			return std::cos(scl);
		}
		else if (op == 3) {
			return std::tan(scl);
		}
		else if (op == 4) {
			return glm::degrees(scl);
		}
		else if (op == 5) {
			return glm::radians(scl);
		}

		return 0.f;
	}

	class FloatValueNodeHandler: public fl::Model {
	public:

		/// Float Node
		static inline void floatValueMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues)
		{
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::FLOAT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type";

					return;
				}

				float a = args[0]->data<float>();

				auto result = new fl::Any();
				result->newData<float>(fl::type::FLOAT);
				result->setData<float>(a);

				returnValues.push_back(result);
			}
		}

		static inline fl::Any *floatValueNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("float")
				.setLabel("Float Value")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(floatValue_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "a", "a", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "b", "b", 0.f)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, floatValueMethod)
				.build(space);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override{
			return floatValueNode(space);
		}
	};

	class ArithmeticFloatNodeHandler: public fl::Model{
	public:

		/// Arithmetic Floats Node
		static inline void arithmeticFloatMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {

			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);
			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::UNSIGNED_INT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type, expected uint";
					return;
				}

				for (int i = 1; i < args.size(); i++) {
					if (args[i]->type() != fl::type::FLOAT) {
						error->hasError = true;
						error->errorMsg = "Unknown Input Type / Types, expected float";
						return;
					}
				}

				auto operation = args[0]->data<unsigned int>();

				bool allScalar = true;
				bool allArrays = true;

				for (size_t i = 1; i < args.size(); i++) {
					if (args[i]->isArray()) {
						allScalar = false;
					}
					else {
						allArrays = false;
					}
				}

				if (!allScalar && !allArrays) {
					error->hasError = true;
					error->errorMsg = "Inputs must have the same size.";
					return;
				}

				float scalarAnswer = 0.f;
				std::vector<float> arrayResults;
				if (allScalar) {
					scalarAnswer = args[1]->data<float>();
					for (size_t inx = 2; inx < args.size(); inx++) scalarAnswer = arithmeticOp(operation, scalarAnswer, args[inx]->data<float>());
				}
				else {

					auto size = args[1]->rawData<std::vector<float>>()->size();
					bool equalSize = true;
					for (size_t i = 1; i < args.size(); i++) {
						if (size != args[i]->rawData<std::vector<float>>()->size()) {
							equalSize = false;
							break;
						}
					}

					if (!equalSize) {
						error->hasError = true;
						error->errorMsg = "Arrays must have the same size.";
						return;
					}
					else if (size <= 0) {
						return;
					}

					arrayResults = std::vector<float>(*args[1]->rawData<std::vector<float>>());

					for (size_t inx = 0; inx < size; inx++) {			
						for (size_t inj = 2; inj < args.size(); inj++) {
							arrayResults[inx] = arithmeticOp(operation, arrayResults[inx], (*(args[inj]->rawData <std::vector<float>>()))[inx]);
						}
					}
				}

				auto retAny00 = new fl::Any;
				retAny00->newData<float>(fl::type::FLOAT);
				retAny00->setData<float>(scalarAnswer);

				auto retAny01 = new fl::Any;
				retAny01->newData<std::vector<float>>(fl::type::FLOAT, new std::vector<float>(arrayResults));
				retAny01->setIsArray(true);

				returnValues.push_back(retAny00);
				returnValues.push_back(retAny01);
			}
		}

		static inline fl::Any *arithmeticFloatNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("arithmetic")
				.setLabel("Arithmetic")
				.setMode(fl::DataObject::NODE)
				.setAllowDynamicInput(true)
				.setHasModel(true)
				.setModelReference(arithmeticFloat_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "result_scalar", "Result Scalar", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "result_array", "Result Array", 0.f)
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::INPUT, "operation", "Operation", 0)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, arithmeticFloatMethod)
				.build(space);

			std::vector<std::pair<std::string, unsigned int>> operationEnum;
			operationEnum.push_back(std::pair<std::string, unsigned int>("Multiply", 0));
			operationEnum.push_back(std::pair<std::string, unsigned int>("Divide", 1));
			operationEnum.push_back(std::pair<std::string, unsigned int>("Add", 2));
			operationEnum.push_back(std::pair<std::string, unsigned int>("Subtract", 3));
			operationEnum.push_back(std::pair<std::string, unsigned int>("Modulus", 4));
			operationEnum.push_back(std::pair<std::string, unsigned int>("Power", 5));

			mnode->rawData<fl::DataObject>()->property("operation")->setEnumerations(true);
			mnode->rawData<fl::DataObject>()->property("operation")->setEnumArray(operationEnum);

			mnode->rawData<fl::DataObject>()->property("result_array")->newData<std::vector<float>>(fl::type::FLOAT);
			mnode->rawData<fl::DataObject>()->property("result_array")->setIsArray(true);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override{
			return arithmeticFloatNode(space);
		}
	};

	class FloatHasValueNodeHandler : public fl::Model {
	public:

		/// Float Node
		static inline void floatHasValueMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues)
		{
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::FLOAT ||
					args[1]->type() != fl::type::FLOAT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type";
					return;
				}

				if (!args[0]->isArray()) {
					error->hasError = true;
					error->errorMsg = "Unknown array type";
					return;
				}

				if (args[1]->isArray()) {
					error->hasError = true;
					error->errorMsg = "Array not allowed";
					return;
				}

				auto values = args[0]->rawData<std::vector<float>>();
				auto find   = args[1]->data<float>();

				bool found = false;

				for (auto &val: (*values)) {
					if (val == find) {
						found = true;
						break;
					}
				}

				auto result = new fl::Any();
				result->newData<bool>(fl::type::BOOL);
				result->setData<bool>(found);

				returnValues.push_back(result);
			}
		}

		static inline fl::Any *floatHasValueNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("float_has_value")
				.setLabel("Float Has Value")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(floatHasValue_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::BOOL, fl::role::OUTPUT, "result", "Result", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "values", "Values", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "find", "Find", 0.f)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, floatHasValueMethod)
				.build(space);

			mnode->rawData<fl::DataObject>()->property("values")->newData<std::vector<float>>(fl::type::FLOAT);
			mnode->rawData<fl::DataObject>()->property("values")->setIsArray(true);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return floatHasValueNode(space);
		}
	};

	class MathFunctionsNodeHandler : public fl::Model {
	public:

		/// Math Functions Node
		static inline void mathFunctionsMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::FLOAT ||
					args[1]->type() != fl::type::FLOAT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0, 1], expected float";
					return;
				}

				if (args[0]->isArray() || args[2]->isArray()) {
					error->hasError = true;
					error->errorMsg = "Arrays not allowed [0, 2]";
					return;
				}

				if (!args[1]->isArray()) {
					error->hasError = true;
					error->errorMsg = "Expected array [1]";
					return;
				}

				std::vector<float> arrayResults;
				float scalarAnswer = 0.f;

				auto a = args[0]->data<float>();
				auto array = args[1]->rawData<std::vector<float>>();
				auto operation = args[2]->data<unsigned int>();

				scalarAnswer = scalarOp(operation, a);
				for (size_t i = 0; i < array->size(); i++) arrayResults.push_back(scalarOp(operation, (*array)[i]));

				auto retAny00 = new fl::Any;
				retAny00->newData<float>(fl::type::FLOAT);
				retAny00->setData<float>(scalarAnswer);

				auto retAny01 = new fl::Any;
				retAny01->newData<std::vector<float>>(fl::type::FLOAT, new std::vector<float>(arrayResults));
				retAny01->setIsArray(true);

				returnValues.push_back(retAny00);
				returnValues.push_back(retAny01);

			}
		}

		static inline fl::Any *mathFunctionsNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("float_math_functions")
				.setLabel("Math Functions")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(floatValue_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "result", "Result", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "result_array", "Result Array", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "scalar", "Scalar", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "array", "Array", 0.f)
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::INPUT, "operation", "Operation", 0)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, mathFunctionsMethod)
				.build(space);

			std::vector<std::pair<std::string, unsigned int>> operationEnum;
			operationEnum.push_back(std::pair<std::string, unsigned int>("Square Root", 0));
			operationEnum.push_back(std::pair<std::string, unsigned int>("sin", 1));
			operationEnum.push_back(std::pair<std::string, unsigned int>("cos", 2));
			operationEnum.push_back(std::pair<std::string, unsigned int>("tan", 3));
			operationEnum.push_back(std::pair<std::string, unsigned int>("Radians To Degrees", 4));
			operationEnum.push_back(std::pair<std::string, unsigned int>("Degrees To Radians", 5));

			mnode->rawData<fl::DataObject>()->property("operation")->setEnumerations(true);
			mnode->rawData<fl::DataObject>()->property("operation")->setEnumArray(operationEnum);

			mnode->rawData<fl::DataObject>()->property("array")->newData<std::vector<float>>(fl::type::FLOAT);
			mnode->rawData<fl::DataObject>()->property("array")->setIsArray(true);

			mnode->rawData<fl::DataObject>()->property("result_array")->newData<std::vector<float>>(fl::type::FLOAT);
			mnode->rawData<fl::DataObject>()->property("result_array")->setIsArray(true);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return mathFunctionsNode(space);
		}
	};

	class PiFloatNodeHandler : public fl::Model {
	public:

		static inline void piFloatMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			auto result = new fl::Any();
			result->newData<float>(fl::type::FLOAT);
			result->setData<float>(3.14159265359f);

			returnValues.push_back(result);
		}

		static inline fl::Any *piFloatNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("float_pi")
				.setLabel("Pi")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(floatValue_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "pi", "Pi", 3.14159265359f)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, piFloatMethod)
				.build(space);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return piFloatNode(space);
		}
	};
	
	inline void registerCommonFloatNodes() {

		DfxComputeNodeDescription *f_value_desc = new DfxComputeNodeDescription;
		f_value_desc->identifier = floatValue_model_id;
		f_value_desc->name = "Float Value";
		f_value_desc->description = "Float value.";
		f_value_desc->category = "Math";
		f_value_desc->placement = "Math/Float/Value";
		f_value_desc->major = 1;
		f_value_desc->minor = 0;
		f_value_desc->instance = FloatValueNodeHandler::floatValueNode;

		DfxComputeNodeDescription *arithmetic_floats_desc = new DfxComputeNodeDescription;
		arithmetic_floats_desc->identifier = arithmeticFloat_model_id;
		arithmetic_floats_desc->name = "Float Arithmetic";
		arithmetic_floats_desc->description = "Float Arithmetic Opeations.";
		arithmetic_floats_desc->category = "Math";
		arithmetic_floats_desc->placement = "Math/Float/Arithmetic";
		arithmetic_floats_desc->major = 1;
		arithmetic_floats_desc->minor = 0;
		arithmetic_floats_desc->instance = ArithmeticFloatNodeHandler::arithmeticFloatNode;

		DfxComputeNodeDescription *mathFunctions_floats_desc = new DfxComputeNodeDescription;
		mathFunctions_floats_desc->identifier = mathFunctionFloat_model_id;
		mathFunctions_floats_desc->name = "Float Math Functions";
		mathFunctions_floats_desc->description = "Float Math Opeations.";
		mathFunctions_floats_desc->category = "Math";
		mathFunctions_floats_desc->placement = "Math/Float/Math Functions";
		mathFunctions_floats_desc->major = 1;
		mathFunctions_floats_desc->minor = 0;
		mathFunctions_floats_desc->instance = MathFunctionsNodeHandler::mathFunctionsNode;

		DfxComputeNodeDescription *pi_float_desc = new DfxComputeNodeDescription;
		pi_float_desc->identifier = piFloat_model_id;
		pi_float_desc->name = "Float Pi";
		pi_float_desc->description = "Pi.";
		pi_float_desc->category = "Math";
		pi_float_desc->placement = "Math/Float/Pi";
		pi_float_desc->major = 1;
		pi_float_desc->minor = 0;
		pi_float_desc->instance = PiFloatNodeHandler::piFloatNode;

		DfxComputeNodeDescription *f_has_value_desc = new DfxComputeNodeDescription;
		f_has_value_desc->identifier = floatHasValue_model_id;
		f_has_value_desc->name = "Float Has Value";
		f_has_value_desc->description = "Float Has value.";
		f_has_value_desc->category  = "Math";
		f_has_value_desc->placement = "Math/Float/Has value";
		f_has_value_desc->major = 1;
		f_has_value_desc->minor = 0;
		f_has_value_desc->instance = FloatHasValueNodeHandler::floatHasValueNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(f_value_desc);
		DfxComputeNodeDB::getInstance()->registerComputeNode(arithmetic_floats_desc);
		DfxComputeNodeDB::getInstance()->registerComputeNode(mathFunctions_floats_desc);
		DfxComputeNodeDB::getInstance()->registerComputeNode(pi_float_desc);
		DfxComputeNodeDB::getInstance()->registerComputeNode(f_has_value_desc);
	}
}