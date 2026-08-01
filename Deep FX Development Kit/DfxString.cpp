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

#include "DfxString.h"


DfxString::DfxString() {

}

DfxString::DfxString(const char*str) {
	append(str);
}

DfxString::~DfxString() {

}


int DfxString::toInt() {
	int val = 0;

	try
	{
		val = std::stoi(c_str());
	}
	catch (const std::exception& ex)
	{
		val = 0;
	} 
		
	return val;
}

float DfxString::toFloat() {
	float val = 0.0f;

	try
	{
		val = std::stof(c_str());
	}
	catch (const std::exception& ex)
	{
		val = 0.0f;
	}

	return 0.0f;
}

double DfxString::toDouble() {
	float val = 0.0;

	try
	{
		val = std::stod(c_str());
	}
	catch (const std::exception& ex)
	{
		val = 0.0;
	}

	return 0.0;
}

bool DfxString::toBool() {
	DfxString strTrue("true");

	if (compareLowerCase(strTrue) == 0)
		return true;
	else
		return false;
}

DfxString DfxString::toLower() {
	std::string tmp = c_str();
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);

	return DfxString(tmp.c_str());
}

DfxString DfxString::toUpper() {
	std::string tmp = c_str();
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);

	return DfxString(tmp.c_str());
}

int DfxString::compareLowerCase(DfxString other) {
	DfxString tmpOther = other.toLower();

	return compare(tmpOther.c_str());
}

int DfxString::compareUpperCase(DfxString other) {
	DfxString tmpOther = other.toUpper();

	return compare(tmpOther.c_str());
}