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

#include <string>
#include "VBuffer.h"

namespace xvault {

	namespace h5 {

		class h5Buffer: public Buffer {
		private:

			unsigned int _type;
			std::string _name;
			size_t _bufferSize;

			int *b_int;
			short *b_short;
			long *b_long;
			long long *b_long_long;
			unsigned int *b_uint;
			unsigned short *b_ushort;
			unsigned long *b_ulong;
			unsigned long long *b_ulong_long;
			float *b_float;
			double *b_double;
			char *b_char;

			bool _autoClear;
		public:

			h5Buffer(bool autoClear=true);
			virtual ~h5Buffer();

			void setAutoClear(bool);

			/// Type ID
			unsigned int type() override;
			void setType(unsigned int) override;

			///  Attribute Name
			std::string name() override;
			void setName(std::string) override;

			/// Buffer Size
			size_t bufferSize() override;
			void setBufferSize(size_t) override;

			/// Retrieve Buffer
			int *bufferInt() override;
			short *bufferShort() override;
			long *bufferLong() override;
			long long *bufferLongLong() override;
			unsigned int *bufferUInt() override;
			unsigned short *bufferUShort() override;
			unsigned long  *bufferULong() override;
			unsigned long long  *bufferULongLong() override;
			float  *bufferFloat() override;
			double *bufferDouble() override;
			char   *bufferChar() override;

			/// Set Value
			void setBufferInt(int *) override;
			void setBufferShort(short *) override;
			void setBufferLong(long *) override;
			void setBufferLongLong(long long *) override;
			void setBufferUInt(unsigned int *) override;
			void setBufferUShort(unsigned short *) override;
			void setBufferULong(unsigned long *) override;
			void setBufferULongLong(unsigned long long *) override;
			void setBufferFloat(float *) override;
			void setBufferDouble(double *) override;
			void setBufferChar(char *) override;

			/// Clear
			void clear() override;
		};
	}
}