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

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include "VCommon.h"

namespace xvault {

	class Attribute {
	public:
		Attribute();
		virtual ~Attribute();

		/// Type ID
		virtual unsigned int type() = 0;
		virtual void setType(unsigned int) = 0;

		///  Attribute Name
		virtual std::string name() = 0;
		virtual void setName(std::string) = 0;

		/// Retrieve Value
		virtual int valueInt() = 0;
		virtual short valueShort() = 0;
		virtual long valueLong() = 0;
		virtual long long valueLongLong() = 0;
		virtual unsigned int valueUInt() = 0;
		virtual unsigned short valueUShort() = 0;
		virtual unsigned long  valueULong() = 0;
		virtual unsigned long long  valueULongLong() = 0;
		virtual float  valueFloat() = 0;
		virtual double valueDouble() = 0;
		virtual char   valueChar() = 0;
		virtual std::string valueString() = 0;
		virtual glm::vec2 valueFloat2() = 0;
		virtual glm::vec3 valueFloat3() = 0;
		virtual glm::vec4 valueFloat4() = 0;
		virtual glm::mat3 valueFloat9() = 0;
		virtual glm::mat4 valueFloat16() = 0;

		/// Set Value
		virtual void setValueInt(int) = 0;
		virtual void setValueShort(short) = 0;
		virtual void setValueLong(long) = 0;
		virtual void setValueLongLong(long long) = 0;
		virtual void setValueUInt(unsigned int) = 0;
		virtual void setValueUShort(unsigned short) = 0;
		virtual void setValueULong(unsigned long) = 0;
		virtual void setValueULongLong(unsigned long long) = 0;
		virtual void setValueFloat(float) = 0;
		virtual void setValueDouble(double) = 0;
		virtual void setValueChar(char) = 0;
		virtual void setValueString(std::string) = 0;
		virtual void setValueFloat2(glm::vec2) = 0;
		virtual void setValueFloat3(glm::vec3) = 0;
		virtual void setValueFloat4(glm::vec4) = 0;
		virtual void setValueFloat9(glm::mat3) = 0;
		virtual void setValueFloat16(glm::mat4) = 0;
	};
}