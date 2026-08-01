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
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"

namespace compute_node {

	static const std::string vectorValue_model_id = "/COMPUTE/VECTOR_VALUE_NODE";
	static const std::string vectorLength_model_id = "/COMPUTE/VECTOR_LENGTH_NODE";


	class VectorValueNodeHandler : public fl::Model {
	public:

		/// Vector Node
		static inline void vectorValueMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::FLOAT_3) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type";
					return;
				}

				auto a = args[0]->data<glm::vec3>();

				auto result = new fl::Any();
				result->newData<glm::vec3>(fl::type::FLOAT_3);
				result->setData<glm::vec3>(a);

				returnValues.push_back(result);
			}
		}

		static inline fl::Any *vectorValueNode(fl::SpaceInterface *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("vector")
				.setLabel("Vector Value")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(vectorValue_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::FLOAT_3, fl::role::INPUT, "a", "A", glm::vec3(0.f))
				.addProperty(space, fl::type::FLOAT_3, fl::role::OUTPUT, "b", "B", glm::vec3(0.f))
				.addStaticFunction(fl::DataObject::STATIC_MAIN, vectorValueMethod)
				.build(space);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return vectorValueNode(space);
		}
	};

	class VectorLengthNodeHandler : public fl::Model {
	public:

		/// Vector Node
		static inline void vectorLengthMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::FLOAT_3 ||
					args[1]->type() != fl::type::FLOAT_3) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0, 1], expected float";
					return;
				}

				if (args[0]->isArray()) {
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

				auto a = args[0]->data<glm::vec3>();
				auto array = args[1]->rawData<std::vector<glm::vec3>>();

				scalarAnswer = glm::length(a);
				for (size_t i = 0; i < array->size(); i++) arrayResults.push_back(glm::length((*array)[i]));

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

		static inline fl::Any *vectorLengthNode(fl::SpaceInterface *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("vector_length")
				.setLabel("Vector Length")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(vectorLength_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "result", "Result", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "result_array", "Result Array", 0.f)
				.addProperty(space, fl::type::FLOAT_3, fl::role::INPUT, "scalar", "Scalar", 0.f)
				.addProperty(space, fl::type::FLOAT_3, fl::role::INPUT, "array", "Array", 0.f)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, vectorLengthMethod)
				.build(space);

			mnode->rawData<fl::DataObject>()->property("array")->newData<std::vector<glm::vec3>>(fl::type::FLOAT_3);
			mnode->rawData<fl::DataObject>()->property("array")->setIsArray(true);

			mnode->rawData<fl::DataObject>()->property("result_array")->newData<std::vector<float>>(fl::type::FLOAT);
			mnode->rawData<fl::DataObject>()->property("result_array")->setIsArray(true);

			auto data = mnode->rawData<fl::DataObject>()->property("result_array")->rawData<std::vector<float>>();

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return vectorLengthNode(space);
		}
	};

	/// Vector Add Node
	inline void vectorAddMethod(fl::SpaceInterface *space, fl::DataObject *dao, fl::InterpreterScope *scope) {
		if (!dao->hasProperty("a")) return;
		if (!dao->hasProperty("b")) return;
		if (!dao->hasProperty("c")) return;

		auto _a = scope->fetchInput(dao->property("a")->absolutePath())[0];
		auto _b = scope->fetchInput(dao->property("b")->absolutePath())[0];

		if (_a->type() != fl::type::FLOAT_3 ||
			_b->type() != fl::type::FLOAT_3) {

			dao->setErrorFlag(true);
			dao->setErrorString("Unknown Input Type");

			return;
		}
		else if (_a->isArray() && _b->isArray()) {
			size_t a_size = _a->rawData<std::vector<glm::vec3>>()->size();
			size_t b_size = _b->rawData<std::vector<glm::vec3>>()->size();

			if (a_size != b_size) {
				dao->setErrorFlag(true);
				dao->setErrorString("Unequal array sizes. Arrays must be the same size.");

				return;
			}
		}

		dao->setErrorFlag(false);

		auto result = new fl::Any();
		if (_a->isArray() || _b->isArray()) {
			size_t length = _a->isArray() ? _a->rawData<std::vector<glm::vec3>>()->size() : _b->rawData<std::vector<glm::vec3>>()->size();

			auto *v_ptr = new std::vector<glm::vec3>();

			if (length > 0) {
				v_ptr->resize(length);
				for (size_t inx = 0; inx < length; inx++) {
					glm::vec3 val00 = _a->isArray() ? (*_a->rawData<std::vector<glm::vec3>>())[inx] : _a->data<glm::vec3>();
					glm::vec3 val01 = _b->isArray() ? (*_b->rawData<std::vector<glm::vec3>>())[inx] : _b->data<glm::vec3>();

					(*v_ptr)[inx] = val00 + val01;
				}
			}

			result->newData<std::vector<glm::vec3>>(fl::type::FLOAT, v_ptr);
			result->setIsArray(true);
		}
		else {
			glm::vec3 a = _a->data<glm::vec3>();
			glm::vec3 b = _b->data<glm::vec3>();

			result->newData<glm::vec3>(fl::type::FLOAT_3);
			result->setData<glm::vec3>(a + b);
		}

		scope->addOutput(dao->property("c")->absolutePath(), result);
	}

	inline fl::Any *vectorAddNode(fl::SpaceInterface *space) {

		auto mnode = fl::DataObject::Builder()
			.setName("add")
			.setLabel("Add")
			.setMode(fl::DataObject::NODE)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
			.addProperty(space, fl::type::FLOAT_3, fl::role::INPUT,  "a", "A", glm::vec3(0.f))
			.addProperty(space, fl::type::FLOAT_3, fl::role::INPUT,  "b", "B", glm::vec3(0.f))
			.addProperty(space, fl::type::FLOAT_3, fl::role::OUTPUT, "c", "C", glm::vec3(0.f))
			.addFunction(fl::DataObject::MAIN, vectorAddMethod)
			.build(space);

		return mnode;
	}

	/// Vector multiply Node
	inline void vectorMultiplyMethod(fl::SpaceInterface *space, fl::DataObject *dao, fl::InterpreterScope *scope) {
		if (!dao->hasProperty("a")) return;
		if (!dao->hasProperty("b")) return;
		if (!dao->hasProperty("c")) return;

		auto _a = scope->fetchInput(dao->property("a")->absolutePath())[0];
		auto _b = scope->fetchInput(dao->property("b")->absolutePath())[0];

		if (_a->type() != fl::type::FLOAT_3 ||
			_b->type() != fl::type::FLOAT_3) {

			dao->setErrorFlag(true);
			dao->setErrorString("Unknown Input Type");

			return;
		}
		else if (_a->isArray() && _b->isArray()) {
			size_t a_size = _a->rawData<std::vector<glm::vec3>>()->size();
			size_t b_size = _b->rawData<std::vector<glm::vec3>>()->size();

			if (a_size != b_size) {
				dao->setErrorFlag(true);
				dao->setErrorString("Unequal array sizes. Arrays must be the same size.");

				return;
			}
		}

		dao->setErrorFlag(false);

		auto result = new fl::Any();
		if (_a->isArray() || _b->isArray()) {
			size_t length = _a->isArray() ? _a->rawData<std::vector<glm::vec3>>()->size() : _b->rawData<std::vector<glm::vec3>>()->size();

			auto *v_ptr = new std::vector<glm::vec3>();

			if (length > 0) {
				v_ptr->resize(length);
				for (size_t inx = 0; inx < length; inx++) {
					glm::vec3 val00 = _a->isArray() ? (*_a->rawData<std::vector<glm::vec3>>())[inx] : _a->data<glm::vec3>();
					glm::vec3 val01 = _b->isArray() ? (*_b->rawData<std::vector<glm::vec3>>())[inx] : _b->data<glm::vec3>();

					(*v_ptr)[inx] = val00 * val01;
				}
			}

			result->newData<std::vector<glm::vec3>>(fl::type::FLOAT_3, v_ptr);
			result->setIsArray(true);
		}
		else {
			glm::vec3 a = _a->data<glm::vec3>();
			glm::vec3 b = _b->data<glm::vec3>();

			result->newData<glm::vec3>(fl::type::FLOAT_3);
			result->setData<glm::vec3>(a * b);
		}

		scope->addOutput(dao->property("c")->absolutePath(), result);
	}

	inline fl::Any *vectorMultiplyNode(fl::SpaceInterface *space) {

		auto mnode = fl::DataObject::Builder()
			.setName("multiply")
			.setLabel("Multiply")
			.setMode(fl::DataObject::NODE)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
			.addProperty(space, fl::type::FLOAT_3, fl::role::INPUT, "a", "A", glm::vec3(0.f))
			.addProperty(space, fl::type::FLOAT_3, fl::role::INPUT, "b", "B", glm::vec3(0.f))
			.addProperty(space, fl::type::FLOAT_3, fl::role::OUTPUT, "c", "C", glm::vec3(0.f))
			.addFunction(fl::DataObject::MAIN, vectorMultiplyMethod)
			.build(space);

		return mnode;
	}

	/// Make Vector Node
	inline void makeVectorMethod(fl::SpaceInterface *space, fl::DataObject *dao, fl::InterpreterScope *scope) {
		if (!dao->hasProperty("x")) return;
		if (!dao->hasProperty("y")) return;
		if (!dao->hasProperty("z")) return;
		if (!dao->hasProperty("vector")) return;

		auto _x = scope->fetchInput(dao->property("x")->absolutePath())[0];
		auto _y = scope->fetchInput(dao->property("y")->absolutePath())[0];
		auto _z = scope->fetchInput(dao->property("z")->absolutePath())[0];

		if (_x->type() != fl::type::FLOAT ||
			_y->type() != fl::type::FLOAT ||
			_z->type() != fl::type::FLOAT) {

			dao->setErrorFlag(true);
			dao->setErrorString("Unknown Input Type");

			return;
		}
		else if (_x->isArray() || _y->isArray() || _z->isArray()) {
		
			dao->setErrorFlag(true);
			dao->setErrorString("X, Y or Z cannot be arrays.");

			return;		
		}

		dao->setErrorFlag(false);

		auto x = _x->data<float>();
		auto y = _y->data<float>();
		auto z = _z->data<float>();

		auto result = new fl::Any();
		result->newData<glm::vec3>(fl::type::FLOAT_3);
		result->setData<glm::vec3>(glm::vec3(x, y, z));

		scope->addOutput(dao->property("vector")->absolutePath(), result);
	}

	inline fl::Any *makeVectorNode(fl::SpaceInterface *space) {

		auto mnode = fl::DataObject::Builder()
			.setName("make_vector")
			.setLabel("Make Vector")
			.setMode(fl::DataObject::NODE)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "x", "X", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "y", "Y", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "z", "Z", 0.f)
			.addProperty(space, fl::type::FLOAT_3, fl::role::OUTPUT, "vector", "Vector", glm::vec3(0.f))
			.addFunction(fl::DataObject::MAIN, makeVectorMethod)
			.build(space);

		return mnode;
	}

	inline void registerCommonVectorNodes() {

		DfxComputeNodeDescription *v_value_desc = new DfxComputeNodeDescription;
		v_value_desc->identifier = vectorValue_model_id;
		v_value_desc->name = "Vector Value";
		v_value_desc->description = "Vector value.";
		v_value_desc->category = "Math";
		v_value_desc->placement = "Math/Vector/Value";
		v_value_desc->major = 1;
		v_value_desc->minor = 0;
		v_value_desc->instance = VectorValueNodeHandler::vectorValueNode;

		DfxComputeNodeDescription *v_length_desc = new DfxComputeNodeDescription;
		v_length_desc->identifier = vectorLength_model_id;
		v_length_desc->name = "Vector Length";
		v_length_desc->description = "Vector Length.";
		v_length_desc->category = "Math";
		v_length_desc->placement = "Math/Vector/Length";
		v_length_desc->major = 1;
		v_length_desc->minor = 0;
		v_length_desc->instance = VectorLengthNodeHandler::vectorLengthNode;

		DfxComputeNodeDescription *add_vector_desc = new DfxComputeNodeDescription;
		add_vector_desc->identifier = "DFX_ADD_VECTORS";
		add_vector_desc->name = "Add Floats";
		add_vector_desc->description = "Sums two float values.";
		add_vector_desc->category = "Math";
		add_vector_desc->placement = "Math/Vector/Add";
		add_vector_desc->major = 1;
		add_vector_desc->minor = 0;
		add_vector_desc->instance = vectorAddNode;

		DfxComputeNodeDescription *multiply_vectors_desc = new DfxComputeNodeDescription;
		multiply_vectors_desc->identifier = "DFX_MULTIPLY_VECTORS";
		multiply_vectors_desc->name = "Multiply Floats";
		multiply_vectors_desc->description = "Multiplies two float values.";
		multiply_vectors_desc->category = "Math";
		multiply_vectors_desc->placement = "Math/Vector/Multiply";
		multiply_vectors_desc->major = 1;
		multiply_vectors_desc->minor = 0;
		multiply_vectors_desc->instance = vectorMultiplyNode;

		DfxComputeNodeDescription *make_vector_desc = new DfxComputeNodeDescription;
		make_vector_desc->identifier = "DFX_MAKE_VECTOR";
		make_vector_desc->name = "Make Vector";
		make_vector_desc->description = "Create a new vector from floats.";
		make_vector_desc->category = "Math";
		make_vector_desc->placement = "Math/Vector/Make Vector";
		make_vector_desc->major = 1;
		make_vector_desc->minor = 0;
		make_vector_desc->instance = makeVectorNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(v_value_desc);
		DfxComputeNodeDB::getInstance()->registerComputeNode(v_length_desc);
		//DfxComputeNodeDB::getInstance()->registerComputeNode(add_vector_desc);
		//DfxComputeNodeDB::getInstance()->registerComputeNode(multiply_vectors_desc);
		//DfxComputeNodeDB::getInstance()->registerComputeNode(make_vector_desc);
	}
}