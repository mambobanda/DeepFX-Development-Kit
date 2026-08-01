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

#include "VH5Buffer.h"

namespace xvault {

	namespace h5 {

		h5Buffer::h5Buffer(bool autoclear) {

			_type = 0;
			_name = "";

			_autoClear = autoclear;

			_bufferSize = 0;

			b_int = nullptr;
			b_short = nullptr;
			b_long = nullptr;
			b_long_long = nullptr;
			b_uint = nullptr;
			b_ushort = nullptr;
			b_ulong = nullptr;
			b_ulong_long = nullptr;
			b_float = nullptr;
			b_double = nullptr;
			b_char = nullptr;

		}

		h5Buffer::~h5Buffer() {
			if(_autoClear) clear();
		}

		void h5Buffer::setAutoClear(bool ot) {
			_autoClear = ot;
		}

		/// Type ID
		unsigned int h5Buffer::type() {
			return _type;
		}

		void h5Buffer::setType(unsigned int t) {
			_type = t;
		}

		///  Attribute Name
		std::string h5Buffer::name() {
			return _name;
		}

		void h5Buffer::setName(std::string nme) {
			_name = nme;
		}

		/// Buffer Size
		size_t h5Buffer::bufferSize() {
			return _bufferSize;
		}

		void h5Buffer::setBufferSize(size_t v) {
			_bufferSize = v;
		}

		/// Retrieve Buffer
		int *h5Buffer::bufferInt() {
			return b_int;
		}

		short *h5Buffer::bufferShort() {
			return b_short;
		}

		long *h5Buffer::bufferLong() {
			return b_long;
		}

		long long *h5Buffer::bufferLongLong() {
			return b_long_long;
		}

		unsigned int *h5Buffer::bufferUInt() {
			return b_uint;
		}

		unsigned short *h5Buffer::bufferUShort() {
			return b_ushort;
		}

		unsigned long  *h5Buffer::bufferULong() {
			return b_ulong;
		}

		unsigned long long  *h5Buffer::bufferULongLong() {
			return b_ulong_long;
		}

		float  *h5Buffer::bufferFloat() {
			return b_float;
		}

		double *h5Buffer::bufferDouble() {
			return b_double;
		}

		char   *h5Buffer::bufferChar() {
			return b_char;
		}

		/// Set Value
		void h5Buffer::setBufferInt(int *b) {
			b_int = b;
		}

		void h5Buffer::setBufferShort(short *b) {
			b_short = b;
		}

		void h5Buffer::setBufferLong(long *b) {
			b_long = b;
		}

		void h5Buffer::setBufferLongLong(long long *b) {
			b_long_long = b;
		}

		void h5Buffer::setBufferUInt(unsigned int *b) {
			b_uint = b;
		}

		void h5Buffer::setBufferUShort(unsigned short *b) {
			b_ushort = b;
		}

		void h5Buffer::setBufferULong(unsigned long *b) {
			b_ulong = b;
		}

		void h5Buffer::setBufferULongLong(unsigned long long *b) {
			b_ulong_long = b;
		}

		void h5Buffer::setBufferFloat(float *b) {
			b_float = b;
		}

		void h5Buffer::setBufferDouble(double *b) {
			b_double = b;
		}

		void h5Buffer::setBufferChar(char *b) {
			b_char = b;
		}

		/// Clear
		void h5Buffer::clear() {

			_type = 0;
			_name = "";

			_bufferSize = 0;

			delete[] b_int;
			delete[] b_short;
			delete[] b_long;
			delete[] b_long_long;
			delete[] b_uint;
			delete[] b_ushort;
			delete[] b_ulong;
			delete[] b_ulong_long;
			delete[] b_float;
			delete[] b_double;
			delete[] b_char;

			b_int = nullptr;
			b_short = nullptr;
			b_long = nullptr;
			b_long_long = nullptr;
			b_uint = nullptr;
			b_ushort = nullptr;
			b_ulong = nullptr;
			b_ulong_long = nullptr;
			b_float = nullptr;
			b_double = nullptr;
			b_char = nullptr;

		}

	}

}