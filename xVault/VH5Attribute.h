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

#include "VAttribute.h"

namespace xvault {

	namespace h5 {

		class h5Attribute : public Attribute {
		private:
			
			unsigned int _type;
			std::string _name;

			int v_int;
			short v_short;
			long v_long;
			long long v_long_long;
			unsigned int v_uint;
			unsigned short v_ushort;
			unsigned long v_ulong;
			unsigned long long v_ulong_long;
			float v_float;
			double v_double;
			char v_char;
			std::string v_string;
			glm::vec2 v_float2;
			glm::vec3 v_float3;
			glm::vec4 v_float4;
			glm::mat3 v_float9;
			glm::mat4 v_float16;

		private:
			static h5Attribute *newAttribute(std::string name, unsigned int type);

		public:

			h5Attribute();
			virtual ~h5Attribute();

			/// Type ID
			unsigned int type() override;
			void setType(unsigned int) override;

			///  Attribute Name
			std::string name() override;
			void setName(std::string) override;

			/// Retrieve Value
			int valueInt() override;
			short valueShort() override;
			long valueLong() override;
			long long valueLongLong() override;
			unsigned int valueUInt() override;
			unsigned short valueUShort() override;
			unsigned long  valueULong() override;
			unsigned long long  valueULongLong() override;
			float  valueFloat() override;
			double valueDouble() override;
			char   valueChar() override;
			std::string valueString() override;
			glm::vec2 valueFloat2() override;
			glm::vec3 valueFloat3() override;
			glm::vec4 valueFloat4() override;
			glm::mat3 valueFloat9() override;
			glm::mat4 valueFloat16() override;

			/// Set Value
			void setValueInt(int) override;
			void setValueShort(short) override;
			void setValueLong(long) override;
			void setValueLongLong(long long) override;
			void setValueUInt(unsigned int) override;
			void setValueUShort(unsigned short) override;
			void setValueULong(unsigned long) override;
			void setValueULongLong(unsigned long long) override;
			void setValueFloat(float) override;
			void setValueDouble(double) override;
			void setValueChar(char) override;
			void setValueString(std::string) override;
			void setValueFloat2(glm::vec2) override;
			void setValueFloat3(glm::vec3) override;
			void setValueFloat4(glm::vec4) override;
			void setValueFloat9(glm::mat3) override;
			void setValueFloat16(glm::mat4) override;

			static h5Attribute *newValueInt(std::string name, unsigned int type,int);
			static h5Attribute *newValueShort(std::string name, unsigned int type, short);
			static h5Attribute *newValueLong(std::string name, unsigned int type,long);
			static h5Attribute *newValueLongLong(std::string name, unsigned int type,long long);
			static h5Attribute *newValueUInt(std::string name, unsigned int type,unsigned int);
			static h5Attribute *newValueUShort(std::string name, unsigned int type,unsigned short);
			static h5Attribute *newValueULong(std::string name, unsigned int type,unsigned long);
			static h5Attribute *newValueULongLong(std::string name, unsigned int type, unsigned long long);
			static h5Attribute *newValueFloat(std::string name, unsigned int type, float);
			static h5Attribute *newValueDouble(std::string name, unsigned int type, double);
			static h5Attribute *newValueChar(std::string name, unsigned int type, char);
			static h5Attribute *newValueString(std::string name, unsigned int type, std::string);
			static h5Attribute *newValueFloat2(std::string name, unsigned int type, glm::vec2);
			static h5Attribute *newValueFloat3(std::string name, unsigned int type, glm::vec3);
			static h5Attribute *newValueFloat4(std::string name, unsigned int type, glm::vec4);
			static h5Attribute *newValueFloat9(std::string name, unsigned int type, glm::mat3);
			static h5Attribute *newValueFloat16(std::string name, unsigned int type, glm::mat4);

		};
	}
}