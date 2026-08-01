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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include "FluentHelperFunctionInitializer.h"

namespace flhelpers {
	
	void init() {

		/// Float Type
		fl::AnyHelperMap::_copyMap[fl::type::FLOAT] = [](fl::Any *from, fl::Any *to) {
			if (from->type() == fl::type::FLOAT) {
				fl::clear(to);

				if (!from->isArray()) {
					to->newData<float>(fl::type::FLOAT);
					to->setData<float>(from->data<float>());
				}
				else{
					auto _arr = new std::vector<float>(*from->rawData<std::vector<float>>());
					to->newData<std::vector<float>>(fl::type::FLOAT, _arr);
				}
			}
		};

		fl::AnyHelperMap::_clearMap[fl::type::FLOAT] = [](fl::Any *data) {
			if (data->type() == fl::type::FLOAT) {
				if (!data->isArray()) {
					auto _ptr = (float*)data->voidData();
					delete _ptr;
					data->nullify();
				}
				else {
					auto _ptr = (std::vector<float>*)data->voidData();
					delete _ptr;
					data->nullify();
				}
			}
		};

		/// Std String
		fl::AnyHelperMap::_copyMap[fl::type::STD_STRING] = [](fl::Any *from, fl::Any *to) {
			if (from->type() == fl::type::STD_STRING) {
				fl::clear(to);

				if (!from->isArray()) {
					to->newData<std::string>(fl::type::STD_STRING);
					to->setData<std::string>(from->data<std::string>());
				}
				else {
					auto _arr = new std::vector<std::string>(*from->rawData<std::vector<std::string>>());
					to->newData<std::vector<std::string>>(fl::type::STD_STRING, _arr);
				}
			}
		};

		fl::AnyHelperMap::_clearMap[fl::type::STD_STRING] = [](fl::Any *data) {
			if (data->type() == fl::type::STD_STRING) {
				if (!data->isArray()) {
					auto _ptr = (std::string*)data->voidData();
					delete _ptr;
					data->nullify();
				}
				else {
					auto _ptr = (std::vector<std::string>*)data->voidData();
					delete _ptr;
					data->nullify();
				}
			}
		};

		/// Interpreter Error
		fl::AnyHelperMap::_copyMap[fl::type::INTERPRETER_ERROR] = [](fl::Any *from, fl::Any *to) {
			if (from->type() == fl::type::INTERPRETER_ERROR) {
				fl::clear(to);

				if (!from->isArray()) {
					to->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR);
					to->setData<fl::InterpreterError>(from->data<fl::InterpreterError>());
				}
				else {
					auto _arr = new std::vector<fl::InterpreterError>(*from->rawData<std::vector<fl::InterpreterError>>());
					to->newData<std::vector<fl::InterpreterError>>(fl::type::INTERPRETER_ERROR, _arr);
				}
			}
		};

		fl::AnyHelperMap::_clearMap[fl::type::INTERPRETER_ERROR] = [](fl::Any *data) {
			if (data->type() == fl::type::INTERPRETER_ERROR) {
				if (!data->isArray()) {
					auto _ptr = (fl::InterpreterError*)data->voidData();
					delete _ptr;
					data->nullify();
				}
				else {
					auto _ptr = (std::vector<fl::InterpreterError>*)data->voidData();
					delete _ptr;
					data->nullify();
				}
			}
		};

		/// Float 3 Type
		fl::AnyHelperMap::_copyMap[fl::type::FLOAT_3] = [](fl::Any *from, fl::Any *to) {
			if (from->type() == fl::type::FLOAT_3) {
				fl::clear(to);

				if (!from->isArray()) {
					to->newData<glm::vec3>(fl::type::FLOAT_3);
					to->setData<glm::vec3>(from->data<glm::vec3>());
				}
				else {
					auto _arr = new std::vector<glm::vec3>(*from->rawData<std::vector<glm::vec3>>());
					to->newData<std::vector<glm::vec3>>(fl::type::FLOAT_3, _arr);
				}
			}
		};

		fl::AnyHelperMap::_clearMap[fl::type::FLOAT_3] = [](fl::Any *data) {
			if (data->type() == fl::type::FLOAT_3) {
				if (!data->isArray()) {
					auto _ptr = (glm::vec3*)data->voidData();
					delete _ptr;
					data->nullify();
				}
				else {
					auto _ptr = (std::vector<glm::vec3>*)data->voidData();
					delete _ptr;
					data->nullify();
				}
			}
		};

		/// size_t Type
		fl::AnyHelperMap::_copyMap[fl::type::SIZE_T] = [](fl::Any *from, fl::Any *to) {
			if (from->type() == fl::type::SIZE_T) {
				fl::clear(to);

				if (!from->isArray()) {
					to->newData<size_t>(fl::type::SIZE_T);
					to->setData<size_t>(from->data<size_t>());
				}
				else {
					auto _arr = new std::vector<size_t>(*from->rawData<std::vector<size_t>>());
					to->newData<std::vector<size_t>>(fl::type::SIZE_T, _arr);
				}
			}
		};

		fl::AnyHelperMap::_clearMap[fl::type::SIZE_T] = [](fl::Any *data) {
			if (data->type() == fl::type::SIZE_T) {
				if (!data->isArray()) {
					auto _ptr = (size_t*)data->voidData();
					delete _ptr;
					data->nullify();
				}
				else {
					auto _ptr = (std::vector<size_t>*)data->voidData();
					delete _ptr;
					data->nullify();
				}
			}
		};

		/// Bool Type
		fl::AnyHelperMap::_copyMap[fl::type::BOOL] = [](fl::Any *from, fl::Any *to) {
			if (from->type() == fl::type::BOOL) {
				fl::clear(to);

				if (!from->isArray()) {
					to->newData<bool>(fl::type::BOOL);
					to->setData<bool>(from->data<bool>());
				}
				else {
					auto _arr = new std::vector<bool>(*from->rawData<std::vector<bool>>());
					to->newData<std::vector<bool>>(fl::type::BOOL, _arr);
				}
			}
		};

		fl::AnyHelperMap::_clearMap[fl::type::BOOL] = [](fl::Any *data) {
			if (data->type() == fl::type::BOOL) {
				if (!data->isArray()) {
					auto _ptr = (bool*)data->voidData();
					delete _ptr;
					data->nullify();
				}
				else {
					auto _ptr = (std::vector<bool>*)data->voidData();
					delete _ptr;
					data->nullify();
				}
			}
		};

		    /*
			DOUBLE = 5,
			INT = 6,
			UNSIGNED_INT = 7,
			CHAR = 9,
			UNSIGNED_CHAR = 10,
			FLOAT_2 = 13,
			FLOAT_4 = 15,
			DOUBLE_2 = 16,
			DOUBLE_3 = 17,
			DOUBLE_4 = 18,
			INT_2 = 19,
			INT_3 = 20,
			INT_4 = 21,
			MAT_2 = 22,
			MAT_3 = 23,
			MAT_4 = 24,
			ATTRIBUTE_PATH = 28,
			INTERPRETER_ERROR = 29,*/
	}

}