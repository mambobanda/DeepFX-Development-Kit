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

#include <QDialog>
#include <QBoxLayout>
#include <QTextBrowser>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <sstream>

class DfxLicenseInfoDialog {
public:
	static int trialModeInfo();
	static int activationErrorInfo(std::string msg_a, std::string msg_b);
	static int trialCommercialNotCompatible();
	static int trialFileExpired();
	static int trialResolutionExceeded();
};