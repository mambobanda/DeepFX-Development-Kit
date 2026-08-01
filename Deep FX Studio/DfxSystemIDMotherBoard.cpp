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

#include "DfxSystemIDMotherBoard.h"

DfxSystemIDMotherBoard::DfxSystemIDMotherBoard() {
	result = SUCCESS;
	validSystemID = false;
}


void DfxSystemIDMotherBoard::init() {

	HRESULT result = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	if (FAILED(result)) {
		result = COM_INIT_FAILED;
		return;
	}

	result = CoInitializeSecurity(
		NULL,                           // pSecDesc
		-1,                             // cAuthSvc (COM authentication)
		NULL,                           // asAuthSvc
		NULL,                           // pReserved1
		RPC_C_AUTHN_LEVEL_DEFAULT,      // dwAuthnLevel
		RPC_C_IMP_LEVEL_IMPERSONATE,    // dwImpLevel
		NULL,                           // pAuthList
		EOAC_NONE,                      // dwCapabilities
		NULL                            // Reserved
	);

	if (FAILED(result) && result != RPC_E_TOO_LATE)
	{
		CoUninitialize();

		result = COM_SEC_FAILED;
		return;
	}
}

void DfxSystemIDMotherBoard::unInit() {
	CoUninitialize();
}

void DfxSystemIDMotherBoard::collectBiosInfo() {

	CComBSTR cbsQuery = L"Select * from Win32_BIOS";

	CComPtr<IEnumWbemClassObject> pEnum;

	CComPtr<IWbemClassObject> pWmiObject;

	CComVariant cvtManufac;
	CComVariant cvtSMBios;
	CComVariant cvtSerial;
	CComVariant cvtVersion;

	ULONG uObjectCount = 0;

	CComPtr<IWbemLocator> pWbemLocator;
	HRESULT hr = pWbemLocator.CoCreateInstance(CLSID_WbemLocator);

	if (FAILED(hr)) {
		result = BIO_INFO_FAILED;
		return;
	}

	CComPtr<IWbemServices> pWbemServices;
	hr = pWbemLocator->ConnectServer(CComBSTR(L"root\\cimv2"), NULL, NULL, 0, NULL, 0, NULL, &pWbemServices);

	if (FAILED(hr)) {
		result = BIO_INFO_FAILED;
		return;
	}

	hr = pWbemServices->ExecQuery(CComBSTR("WQL"), cbsQuery, WBEM_FLAG_FORWARD_ONLY, NULL, &pEnum);
	hr = pEnum->Next(WBEM_INFINITE, 1, &pWmiObject, &uObjectCount);

	if (FAILED(hr)) {
		result = BIO_INFO_FAILED;
		return;
	}

	hr = pWmiObject->Get(L"Manufacturer", 0, &cvtManufac, 0, 0);
	bManufacturer = std::string(CW2A(cvtManufac.bstrVal));

	if (FAILED(hr)) {
		result = BIO_INFO_FAILED;
		return;
	}

	hr = pWmiObject->Get(L"SMBIOSBIOSVersion", 0, &cvtSMBios, 0, 0);
	bSMBIOSBIOSVersion = std::string(CW2A(cvtSMBios.bstrVal));

	if (FAILED(hr)) {
		result = BIO_INFO_FAILED;
		return;
	}

	hr = pWmiObject->Get(L"SerialNumber", 0, &cvtSerial, 0, 0);
	bSerialNumber = std::string(CW2A(cvtSerial.bstrVal));

	if (FAILED(hr)) {
		result = BIO_INFO_FAILED;
		return;
	}

	hr = pWmiObject->Get(L"Version", 0, &cvtVersion, 0, 0);
	bVersion = std::string(CW2A(cvtVersion.bstrVal));

	if (FAILED(hr)) {
		result = BIO_INFO_FAILED;
		return;
	}

	/*
	std::cout << "Manufacture: " << bManufacturer.c_str() << std::endl;
	std::cout << "SMBIOSBIOSVersion: " << bSMBIOSBIOSVersion.c_str() << std::endl;
	std::cout << "SerialNumber: " << bSerialNumber.c_str() << std::endl;
	std::cout << "Version: " << bVersion.c_str() << std::endl;
	*/
}

void DfxSystemIDMotherBoard::collectMotherBoardInfo() {
	CComBSTR cbsQuery = L"Select * from Win32_BaseBoard";

	CComPtr<IEnumWbemClassObject> pEnum;

	CComPtr<IWbemClassObject> pWmiObject;

	CComVariant cvtManufac;
	CComVariant cvtName;
	CComVariant cvtSerialNumber;

	ULONG uObjectCount = 0;

	CComPtr<IWbemLocator> pWbemLocator;
	HRESULT hr = pWbemLocator.CoCreateInstance(CLSID_WbemLocator);

	if (FAILED(hr)) {
		result = MOTHERBOARD_INFO_FAILED;
		return;
	}

	CComPtr<IWbemServices> pWbemServices;
	hr = pWbemLocator->ConnectServer(CComBSTR(L"root\\cimv2"), NULL, NULL, 0, NULL, 0, NULL, &pWbemServices);

	if (FAILED(hr)) {
		result = MOTHERBOARD_INFO_FAILED;
		return;
	}

	hr = pWbemServices->ExecQuery(CComBSTR("WQL"), cbsQuery, WBEM_FLAG_FORWARD_ONLY, NULL, &pEnum);
	hr = pEnum->Next(WBEM_INFINITE, 1, &pWmiObject, &uObjectCount);

	if (FAILED(hr)) {
		result = MOTHERBOARD_INFO_FAILED;
		return;
	}

	hr = pWmiObject->Get(L"Manufacturer", 0, &cvtManufac, 0, 0);
	mManufacturer = std::string(CW2A(cvtManufac.bstrVal));

	if (FAILED(hr)) {
		result = MOTHERBOARD_INFO_FAILED;
		return;
	}

	hr = pWmiObject->Get(L"Name", 0, &cvtName, 0, 0);
	mName = std::string(CW2A(cvtName.bstrVal));

	if (FAILED(hr)) {
		result = MOTHERBOARD_INFO_FAILED;
		return;
	}

	hr = pWmiObject->Get(L"SerialNumber", 0, &cvtSerialNumber, 0, 0);
	mSerialNumber = std::string(CW2A(cvtSerialNumber.bstrVal));

	if (FAILED(hr)) {
		result = MOTHERBOARD_INFO_FAILED;
		return;
	}

	/*std::cout << "Manufacturer: " << mManufacturer.c_str() << std::endl;
	std::cout << "Name: " << mName.c_str() << std::endl;
	std::cout << "SerialNumber: " << mSerialNumber.c_str() << std::endl;*/
}

void DfxSystemIDMotherBoard::generateSystemID() {
	init();

	collectBiosInfo();
	collectMotherBoardInfo();

	unInit();

	if (result == SUCCESS) {
		std::stringstream ss;
		std::stringstream ssHex;

		ss << "<<Bios: " << bManufacturer << "-" << bSMBIOSBIOSVersion << "-" << bSerialNumber << "-" << bVersion << ">>"
			<< "<<Motherboard: " << mManufacturer << "-" << mName << "-" << mSerialNumber << ">>";

		auto byteArray = QCryptographicHash::hash(ss.str().c_str(), QCryptographicHash::Md5);
		systemID = formatID(byteArray.toHex().toStdString());

		if (systemID.compare("INVALID ID LENGTH") == 0) {
			validSystemID = false;
		}
		else {
			validSystemID = true;
		}
	}
	else {
		systemID = "SYSTEM ID GEN FAILED: " + std::to_string(result);
		validSystemID = false;
	}
}

void DfxSystemIDMotherBoard::test() {
	init();

	collectBiosInfo();
	collectMotherBoardInfo();

	unInit();
}

std::string DfxSystemIDMotherBoard::getSystemID() {
	return systemID;
}

std::string DfxSystemIDMotherBoard::formatID(std::string hex) {

	
	std::string idStr = hex;
	idStr.resize(16);
	
	if (hex.length() == 32) {
		std::stringstream ss;

		int counter = 0;
		for (int inx = 0; inx < 4; inx++) {

			ss << idStr[counter];
			ss << idStr[counter + 1];
			ss << idStr[counter + 2];
			ss << idStr[counter + 3];

			if (inx != 3) ss << "-";

			counter += 4;
		}

		return ss.str();
	}
	else {
		return "INVALID ID LENGTH";
	}
}

bool DfxSystemIDMotherBoard::hasValidSystemID() {
	return validSystemID;
}