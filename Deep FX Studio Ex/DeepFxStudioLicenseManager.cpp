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

#ifndef _DEBUG
#include <LicenseVerifier.h>
#endif // !_DEBUG

#include "DfxSystemIDGlobal.h" 
#include "DeepFxStudioLicenseManager.h"

std::string DeepFxStudioLicenseManager::nodeLockSalt = "deep-fx-studio-2019";

unsigned int DeepFxStudioLicenseManager::state = DeepFxStudioLicenseManager::LOCKED_STATE;
int DeepFxStudioLicenseManager::trailDaysPast = 0;
std::vector<DeepFxStudioLicenseManager::HostID> DeepFxStudioLicenseManager::hostids;

std::string DeepFxStudioLicenseManager::_trialDate = "";

void DeepFxStudioLicenseManager::init() {

}

std::pair<bool, int> DeepFxStudioLicenseManager::tryNodeLockedLicenseMode() {


	bool validNodeLicenseFile = false;
	int rc = -1;
#ifndef _DEBUG
	if (DfxSystemIDGlobal::getSystemID()->hasValidSystemID()) {

		LicenseVerifier verf;
		verf.licenseDirectory = QCoreApplication::applicationDirPath().toStdString();
		verf.productKey = DfxSystemIDGlobal::getSystemID()->getSystemID();
		verf.salt = nodeLockSalt;

		rc = verf.validLicense();

		if (rc == VALID_LICENSE) {
			validNodeLicenseFile = true;
			state = NODE_LOCKED_STATE;
		}
		else if (rc == INVALID_LICENSE) {
			DfxGlobal::console::println("No valid license was found.");
			state = LOCKED_STATE;
		}
		else if (rc == NO_LICENSE_FOUND) {
			DfxGlobal::console::println("Can't find license file. Make sure license file is in the correct directory.");
			state = LOCKED_STATE;
		}
		else if (rc == NO_PUBLIC_KEY_FOUND) {
			DfxGlobal::console::println("Can't find stub file. Make sure stub file is in the correct directory.");
			state = LOCKED_STATE;
		}
		else if (rc == INVALID_PUBLIC_KEY) {
			DfxGlobal::console::println("Wrong stub file found. Make sure you have the correct stub file.");
			state = LOCKED_STATE;
		}

	}
	else {
		state = LOCKED_STATE;
	}

#endif

	return std::pair<bool, int>(validNodeLicenseFile, rc);
}

bool DeepFxStudioLicenseManager::tryTrialMode()
{
#ifdef WIN32
	return tryTrialModeWIN32();
#else
	state = LOCKED_STATE;
	return false;
#endif 

}

bool DeepFxStudioLicenseManager::tryTrialModeWIN32() {

#ifdef WIN32

	bool keyFound = false;
	DfxRegReader reg;

	uint zero	= 0; bool hasZero	= false;
	uint one	= 0; bool hasOne	= false;
	uint two	= 0; bool hasTwo	= false;
	uint three	= 0; bool hasThree	= false;
	uint four	= 0; bool hasFour	= false;
	uint five	= 0; bool hasFive	= false;
	uint six	= 0; bool hasSix	= false;

	reg.open(HKEY_LOCAL_MACHINE, L"Software\\Oplympus\\Microsoft\\Windows NT\\CurrentVersion\\Ports");

	if (reg.hasError()) {
		reg.open(HKEY_LOCAL_MACHINE, L"Software\\WOW6432Node\\Oplympus\\Microsoft\\Windows NT\\CurrentVersion\\Ports");

		if (!reg.hasError()) {
			keyFound = true;
		}
		else {
			state = LOCKED_STATE;
			return false;
		}
	}
	else {
		keyFound = true;
	}

	if (keyFound) {

		zero = reg.readUInt32(L"Zero");
		if (!reg.hasError()) hasZero = true;

		one = reg.readUInt32(L"One");
		if (!reg.hasError()) hasOne = true;

		two = reg.readUInt32(L"Two");
		if (!reg.hasError()) hasTwo = true;

		three = reg.readUInt32(L"Three");
		if (!reg.hasError()) hasThree = true;

		four = reg.readUInt32(L"Four");
		if (!reg.hasError()) hasFour = true;

		five = reg.readUInt32(L"Five");
		if (!reg.hasError()) hasFive = true;

		six = reg.readUInt32(L"Six");
		if (!reg.hasError()) hasSix = true;

		if (hasZero && hasOne && hasTwo && hasThree &&
			hasFour && hasFive && hasSix) {

			QDate dt(two, one, zero);
			QTime tm(four, five, six, 0);

			auto now = QDateTime::currentDateTime();
			QDateTime past(dt, tm);

			int daysPast = past.daysTo(now);

			if (daysPast < 0 || daysPast >= trialDays) {
				trailDaysPast = daysPast;
				state = TRIAL_MODE_EXPIRED_STATE;
				return false;
			}
			else {
				trailDaysPast = daysPast;
				state = TRIAL_MODE_ACTIVE_STATE;
				return true;
			}

		}
		else {
			state = LOCKED_STATE;
			return false;
		}
	}
	else {
		state = LOCKED_STATE;
		return false;
	}

#endif 
}

unsigned DeepFxStudioLicenseManager::getState() {
	return state;
}

int DeepFxStudioLicenseManager::getTrailDaysPast() {
	return trailDaysPast;
}

std::vector<DeepFxStudioLicenseManager::HostID> DeepFxStudioLicenseManager::getAvailableHostIDs() {
	return hostids;
}

std::string DeepFxStudioLicenseManager::getTrialDate() {
	return _trialDate;
}

void DeepFxStudioLicenseManager::setTrialDate(std::string dt) {
	_trialDate = dt;
}

void DeepFxStudioLicenseManager::clearTrialDate() {
	_trialDate = "";
}
