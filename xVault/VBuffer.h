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

#include "VCommon.h"

namespace xvault {

	class Buffer {
	public:

		virtual ~Buffer();
		/// Type ID
		virtual unsigned int type() = 0;
		virtual void setType(unsigned int) = 0;

		///  Attribute Name
		virtual std::string name() = 0;
		virtual void setName(std::string) = 0;

		/// Buffer Size
		virtual size_t bufferSize() = 0;
		virtual void setBufferSize(size_t) = 0;

		/// Retrieve Buffer
		virtual int *bufferInt() = 0;
		virtual short *bufferShort() = 0;
		virtual long *bufferLong() = 0;
		virtual long long *bufferLongLong() = 0;
		virtual unsigned int *bufferUInt() = 0;
		virtual unsigned short *bufferUShort() = 0;
		virtual unsigned long  *bufferULong() = 0;
		virtual unsigned long long  *bufferULongLong() = 0;
		virtual float  *bufferFloat() = 0;
		virtual double *bufferDouble() = 0;
		virtual char   *bufferChar() = 0;

		/// Set Value
		virtual void setBufferInt(int *) = 0;
		virtual void setBufferShort(short *) = 0;
		virtual void setBufferLong(long *) = 0;
		virtual void setBufferLongLong(long long *) = 0;
		virtual void setBufferUInt(unsigned int *) = 0;
		virtual void setBufferUShort(unsigned short *) = 0;
		virtual void setBufferULong(unsigned long *) = 0;
		virtual void setBufferULongLong(unsigned long long *) = 0;
		virtual void setBufferFloat(float *) = 0;
		virtual void setBufferDouble(double *) = 0;
		virtual void setBufferChar(char *) = 0;

		/// Clear
		virtual void clear() = 0;
	};
}