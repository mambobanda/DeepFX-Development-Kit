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

#include <atlbase.h>
#include <wbemidl.h> 
#include <iomanip>
#include <iostream>
#include <sstream>
#include <functional>
#include <qstring>
#include <QCryptographicHash>
#include "DfxSystemIDBase.h"

class DfxSystemIDMotherBoard: public DfxSystemIDBase {
private:
	std::string systemID;
private:

	enum
	{
		SUCCESS = 0, COM_INIT_FAILED = 1, COM_SEC_FAILED = 2, WBEM_INSTANCE_FAILED = 3, WBEM_CONNECT_FAILED = 4,
		BIO_INFO_FAILED = 5, MOTHERBOARD_INFO_FAILED = 6,
	};

	size_t result;
	bool validSystemID;

	// BIOS Info
	std::string bManufacturer;
	std::string bSMBIOSBIOSVersion;
	std::string bSerialNumber;
	std::string bVersion;

	// Motherboard
	std::string mManufacturer;
	std::string mName;
	std::string mSerialNumber;

private:

	void init();

	void unInit();

	void collectBiosInfo();

	void collectMotherBoardInfo();

	std::string formatID(std::string hex);
public:

	DfxSystemIDMotherBoard();

	void generateSystemID();

	std::string getSystemID();

	bool hasValidSystemID();

	void test();
};