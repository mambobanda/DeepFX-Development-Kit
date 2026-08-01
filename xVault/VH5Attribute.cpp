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

#include "VH5Attribute.h"

namespace xvault {

	namespace h5 {

		h5Attribute::h5Attribute() {
			
			_type = 0;
			_name = "";

			v_int	= 0;
			v_short = 0;
			v_long	= 0;
			v_long_long	= 0;
			v_uint	 = 0;
			v_ushort = 0;
			v_ulong	 = 0;
			v_ulong_long = 0;
			v_float  = 0.f;
			v_double = 0.0;
			v_char	 = 0;
			v_string = "";

		}

		h5Attribute::~h5Attribute() {

		}

		/// Type ID
		unsigned int h5Attribute::type() {
			return _type;
		}

		void h5Attribute::setType(unsigned int typ) {
			_type = typ;
		}

		///  Attribute Name
		std::string h5Attribute::name() {
			return _name;
		}

		void h5Attribute::setName(std::string nme) {
			_name = nme;
		}

		/// Retrieve Value
		int h5Attribute::valueInt() {
			return v_int;
		}

		short h5Attribute::valueShort() {
			return v_short;
		}

		long h5Attribute::valueLong() {
			return v_long;
		}

		long long h5Attribute::valueLongLong() {
			return v_long_long;
		}

		unsigned int h5Attribute::valueUInt() {
			return v_uint;
		}

		unsigned short h5Attribute::valueUShort() {
			return v_ushort;
		}

		unsigned long h5Attribute::valueULong() {
			return v_ulong;
		}

		unsigned long long h5Attribute::valueULongLong() {
			return v_ulong_long;
		}

		float h5Attribute::valueFloat() {
			return v_float;
		}

		double h5Attribute::valueDouble() {
			return v_double;
		}

		char h5Attribute::valueChar() {
			return v_char;
		}

		std::string h5Attribute::valueString() {
			return v_string;
		}

		glm::vec2 h5Attribute::valueFloat2() {
			return v_float2;
		}

		glm::vec3 h5Attribute::valueFloat3() {
			return v_float3;
		}

		glm::vec4 h5Attribute::valueFloat4() {
			return v_float4;
		}

		glm::mat3 h5Attribute::valueFloat9() {
			return v_float9;
		}

		glm::mat4 h5Attribute::valueFloat16() {
			return v_float16;
		}

		/// Set Value
		void h5Attribute::setValueInt(int v) {
			v_int = v;
		} 

		void h5Attribute::setValueShort(short v) {
			v_short = v;
		}

		void h5Attribute::setValueLong(long v) {
			v_long = v;
		}

		void h5Attribute::setValueLongLong(long long v) {
			v_long_long = v;
		}

		void h5Attribute::setValueUInt(unsigned int v) {
			v_uint = v;
		}

		void h5Attribute::setValueUShort(unsigned short v) {
			v_ushort = v;
		}

		void h5Attribute::setValueULong(unsigned long v) {
			v_ulong = v;
		}

		void h5Attribute::setValueULongLong(unsigned long long v) {
			v_ulong_long = v;
		}

		void h5Attribute::setValueFloat(float v) {
			v_float = v;
		}

		void h5Attribute::setValueDouble(double v) {
			v_double = v;
		}

		void h5Attribute::setValueChar(char v) {
			v_char = v;
		}

		void h5Attribute::setValueString(std::string v) {
			v_string = v;
		}

		void h5Attribute::setValueFloat2(glm::vec2 val) {
			v_float2 = val;
		}

		void h5Attribute::setValueFloat3(glm::vec3 val) {
			v_float3 = val;
		}

		void h5Attribute::setValueFloat4(glm::vec4 val) {
			v_float4 = val;
		}

		void h5Attribute::setValueFloat9(glm::mat3 val) {
			v_float9 = val;
		}

		void h5Attribute::setValueFloat16(glm::mat4 val) {
			v_float16 = val;
		}

		// Create Attribute
		h5Attribute *h5Attribute::newAttribute(std::string name, unsigned int type) {

			auto attr = new h5Attribute;
			attr->setType(type);
			attr->setName(name);

			return attr;
		}

		h5Attribute *h5Attribute::newValueInt(std::string name, unsigned int type, int val) {
			auto attr = newAttribute(name, type);
			attr->setValueInt(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueShort(std::string name, unsigned int type, short val) {
			auto attr = newAttribute(name, type);
			attr->setValueShort(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueLong(std::string name, unsigned int type, long val) {
			auto attr = newAttribute(name, type);
			attr->setValueLong(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueLongLong(std::string name, unsigned int type, long long val) {
			auto attr = newAttribute(name, type);
			attr->setValueLongLong(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueUInt(std::string name, unsigned int type, unsigned int val) {
			auto attr = newAttribute(name, type);
			attr->setValueUInt(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueUShort(std::string name, unsigned int type, unsigned short val) {
			auto attr = newAttribute(name, type);
			attr->setValueUShort(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueULong(std::string name, unsigned int type, unsigned long val) {
			auto attr = newAttribute(name, type);
			attr->setValueULong(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueULongLong(std::string name, unsigned int type, unsigned long long val) {
			auto attr = newAttribute(name, type);
			attr->setValueULongLong(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueFloat(std::string name, unsigned int type, float val) {
			auto attr = newAttribute(name, type);
			attr->setValueFloat(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueDouble(std::string name, unsigned int type, double val) {
			auto attr = newAttribute(name, type);
			attr->setValueDouble(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueChar(std::string name, unsigned int type, char val) {
			auto attr = newAttribute(name, type);
			attr->setValueChar(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueString(std::string name, unsigned int type, std::string val) {
			auto attr = newAttribute(name, type);
			attr->setValueString(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueFloat2(std::string name, unsigned int type, glm::vec2 val) {
			auto attr = newAttribute(name, type);
			attr->setValueFloat2(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueFloat3(std::string name, unsigned int type, glm::vec3 val) {
			auto attr = newAttribute(name, type);
			attr->setValueFloat3(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueFloat4(std::string name, unsigned int type, glm::vec4 val) {
			auto attr = newAttribute(name, type);
			attr->setValueFloat4(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueFloat9(std::string name, unsigned int type, glm::mat3 val) {
			auto attr = newAttribute(name, type);
			attr->setValueFloat9(val);

			return attr;
		}

		h5Attribute *h5Attribute::newValueFloat16(std::string name, unsigned int type, glm::mat4 val) {
			auto attr = newAttribute(name, type);
			attr->setValueFloat16(val);

			return attr;
		}
	}
}
