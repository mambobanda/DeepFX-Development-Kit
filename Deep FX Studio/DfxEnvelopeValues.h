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

#pragma once

#include <DfxEntityBasicPropertyTypes.h>


class DfxEnvelopeNullValue {
public:
	static const int value = 0;
	static const bool isSet = false;
};

class DfxEnvelopeFloatValue {
public:
	float value = 0.0f;
	bool isSet  = false;
};

class DfxEnvelopeFloat3Value {
public:
	DfxEntityPropertyTypes::Float3 value;
	bool isSet = false;
};
