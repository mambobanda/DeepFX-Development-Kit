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
  * @file FlCommon.h
  *
  * @brief Global definition, enums and forward class declarations defined here.
  *
  * @author Mambo Banda
  *
  */

#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <random>
#include <limits>
#include <queue>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

namespace fl {

	/// Common Types that may be used by an fl::Any class 
	namespace type {
		enum {
			NONE			= 0,
			DATA_OBJECT		= 1,
			FLOW			= 2,
			COMPOUND_PORT   = 3,
			FLOAT			= 4,
			DOUBLE			= 5,
			INT				= 6,
			UNSIGNED_INT	= 7,
			SIZE_T			= 8,
			CHAR			= 9,
			UNSIGNED_CHAR	= 10,
			BOOL			= 11,
			STD_STRING		= 12,
			FLOAT_2			= 13,
			FLOAT_3			= 14,
			FLOAT_4			= 15,
			DOUBLE_2		= 16,
			DOUBLE_3		= 17,
			DOUBLE_4		= 18,
			INT_2			= 19,
			INT_3			= 20,
			INT_4			= 21,
			MAT_2			= 22,
			MAT_3			= 23,
			MAT_4			= 24,
			SHAPE			= 25,
			USERDATA		= 26,
			ANY				= 27,
			ATTRIBUTE_PATH  = 28,
			INTERPRETER_ERROR = 29,
		};
	}

	/// The role of an fl::Any Class
	namespace role {
		enum {
			NONE   = 880,
			INPUT  = 881,
			OUTPUT = 882,
			HIDDEN_INPUT  = 883,
			HIDDEN_OUTPUT = 884,
			CUSTOM_INPUT  = 885,
			CUSTOM_OUTPUT = 886,
			COMPOUND_INPUT  = 887,
			COMPOUND_OUTPUT = 888
		};
	}

	typedef struct _InterpreterError {
		bool hasError;
		std::string errorMsg;
	}InterpreterError;

	/// Forward Declaration of Classes
	class Any;
	class DataObject;
	class DataObjectProperty;
	class Binding;
	class Observer;
	class Space;
	class SpaceObserver;
	class SpaceEventInfo;
	class InterpreterScope;
	class Dispatcher;
	class DispatchManager;
	struct Link;

	template<typename Key, typename Value>
	class AnyMap;

	using SpaceRemoveAnyCallBack = void(*)(fl::Any*);
}
