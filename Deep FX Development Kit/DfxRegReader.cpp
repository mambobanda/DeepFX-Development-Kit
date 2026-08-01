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

#ifdef WIN32

#include "DfxRegReader.h"

DfxRegReader::DfxRegReader() {
	keyOpen = false;
}

void DfxRegReader::open(HKEY root, const std::wstring &strValueName) {
	errorCode = RegOpenKeyExW(root, strValueName.c_str(), 0, KEY_READ, &hKey);

	if (errorCode == ERROR_SUCCESS) keyOpen = true;
}

void DfxRegReader::close() {
	if (keyOpen) {
		errorCode = RegCloseKey(hKey);

		if (errorCode == ERROR_SUCCESS) keyOpen = false;
	}
}

uint32_t DfxRegReader::readUInt32(std::wstring name) {

	DWORD type = REG_DWORD;
	DWORD cbData = sizeof(uint32_t);
	uint32_t value;

	errorCode = RegQueryValueEx(hKey, name.c_str(), NULL, &type, (LPBYTE)&value, &cbData);

	return value;
}

uint64_t DfxRegReader::readUInt64(std::wstring name) {

	DWORD type = REG_QWORD;
	DWORD cbData = sizeof(uint64_t);
	uint64_t value;

	errorCode = RegQueryValueEx(hKey, name.c_str(), NULL, &type, (LPBYTE)&value, &cbData);

	return value;
}

std::string DfxRegReader::readString(std::wstring name, int buffersize) {

	WCHAR szBuffer[2080];
	DWORD cbData = sizeof(szBuffer);

	errorCode = RegQueryValueExW(hKey, name.c_str(), 0, NULL, (LPBYTE)&szBuffer, &cbData);

	std::wstring wstr(szBuffer);

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::string rt;

	rt = cv.to_bytes(wstr.c_str());

	return rt;
}

bool DfxRegReader::hasError() {
	if (errorCode == ERROR_SUCCESS) return false;
	else return true;
}

LONG DfxRegReader::getErrorCode() {
	return errorCode;
}


#endif