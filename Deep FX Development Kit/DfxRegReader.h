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

#ifdef WIN32

#include <Windows.h>
#include <string>
#include <locale>      
#include <codecvt>  

class DfxRegReader {
private:
	HKEY hKey;
	LONG errorCode;

	bool keyOpen;
public:

	DfxRegReader();

	void open(HKEY root, const std::wstring &strValueName);

	void close();

	uint32_t readUInt32(std::wstring name);

	uint64_t readUInt64(std::wstring name);

	std::string readString(std::wstring name, int buffersize);

	bool hasError();

	LONG getErrorCode();
};

#endif