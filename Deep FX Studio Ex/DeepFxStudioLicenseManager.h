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

#include <DfxGlobal.h>
#include <QDateTime>
#include <QCoreApplication> 
#include <DfxRegReader.h>


#include <vector>

class DeepFxStudioLicenseManager {
public:

	struct HostID {
		std::string id;
		std::string name;
	};

	enum {
		LOCKED_STATE = 0xff,
		NODE_LOCKED_STATE,
		FLOATING_STATE,
		TRIAL_MODE_ACTIVE_STATE,
		TRIAL_MODE_EXPIRED_STATE,
	};

	static const int trialDays = 45;
private:
	static std::string nodeLockSalt;

private:
	static std::string _trialDate;
private:

	static unsigned int state;
	static int trailDaysPast;
	static std::vector<HostID> hostids;
public:

	static void init();

	static std::pair<bool, int> tryNodeLockedLicenseMode();

	static bool tryTrialMode();

	static bool tryTrialModeWIN32();

	static unsigned getState();

	static int getTrailDaysPast();

	static std::vector<HostID> getAvailableHostIDs();

	static std::string getTrialDate();

	static void setTrialDate(std::string dt);

	static void clearTrialDate();

};