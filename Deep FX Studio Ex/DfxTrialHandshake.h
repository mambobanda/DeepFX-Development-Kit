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

#include <QNetworkRequest> 
#include <QNetworkAccessManager> 
#include <QUrlQuery>
#include <QNetworkReply>
#include <QApplication>
#include <algorithm>

class DfxTrialHandshake {
private:
	static bool waitingForResponse;
	static QString responseMsg;
	static int responseCode;
	static QNetworkReply *reply;
private:
	static void replyFinished(QNetworkReply *reply);
public:
	static std::pair<bool,int> askForPermission();
};