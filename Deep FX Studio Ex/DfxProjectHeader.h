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

#include <QDateTime>
#include <RJDocument.h>
#include <RJNode.h>
#include <VH5Attribute.h>
#include <string>

#include "DeepFxStudioLicenseManager.h"
#include "DfxLicenseInfoDialog.h"

namespace DfxProjectHeader {

	static const unsigned int _version = 1;
	static const char *_fileformat = "deepfx.studio.proj";

	static const char *_dateTimeFormat = "yyyy-MM-dd  HH:mm:ss";

	inline xvault::Document *headerXVaultDocument() {
		QDateTime dt = QDateTime::currentDateTime();

		auto attr00 = xvault::h5::h5Attribute::newValueString("modified", xvault::type::STRING, dt.toString(_dateTimeFormat).toStdString());
		auto attr01 = xvault::h5::h5Attribute::newValueString("format", xvault::type::STRING, _fileformat);
		auto attr02 = xvault::h5::h5Attribute::newValueUInt("version", xvault::type::UNSIGNED_INT, DfxProjectHeader::_version);

		auto vdoc = new xvault::rapid::rjDocument();

#if DFX_TRIAL_COMMERCIAL_PROJECT
		auto attr03 = xvault::h5::h5Attribute::newValueUInt("commercial_trial_flag", xvault::type::UNSIGNED_INT, 1010101);
		vdoc->root()->attributes().push_back(attr03);
#endif

		vdoc->root()->attributes().push_back(attr01);
		vdoc->root()->attributes().push_back(attr02);
		vdoc->root()->attributes().push_back(attr00);

		if (DeepFxStudioLicenseManager::getState() != DeepFxStudioLicenseManager::NODE_LOCKED_STATE) {
			std::string tdate = "";

			if (DeepFxStudioLicenseManager::getTrialDate().empty()) {
				tdate = dt.toString(_dateTimeFormat).toStdString();
				DeepFxStudioLicenseManager::setTrialDate(tdate);
			}
			else {
				tdate = DeepFxStudioLicenseManager::getTrialDate();
			}

			auto attr04 = xvault::h5::h5Attribute::newValueString("trial_date", xvault::type::STRING, tdate);
			auto attr05 = xvault::h5::h5Attribute::newValueUInt("trial_file", xvault::type::UNSIGNED_INT, 0);

			vdoc->root()->attributes().push_back(attr04);
			vdoc->root()->attributes().push_back(attr05);
		}

		return vdoc;
	}
};