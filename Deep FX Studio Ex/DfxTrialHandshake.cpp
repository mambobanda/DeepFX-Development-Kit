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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxTrialHandshake.h"

bool DfxTrialHandshake::waitingForResponse = false;
QString DfxTrialHandshake::responseMsg = "";
int DfxTrialHandshake::responseCode = 0;
QNetworkReply *DfxTrialHandshake::reply = nullptr;

std::pair<bool, int> DfxTrialHandshake::askForPermission() {
	waitingForResponse = true;

	std::unique_ptr<QNetworkAccessManager> networkManager(new QNetworkAccessManager());

	QUrlQuery postData;
	postData.addQueryItem("secret", "OurFatherWhoArtInHeavenHallowedBeThyName");

	QNetworkRequest request(QUrl("https://www.deepfxworld.com/trial.php"));
	request.setRawHeader("User-Agent", "MyAppName/1.0 (Nokia; Qt)");
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	QObject::connect(networkManager.get(), &QNetworkAccessManager::finished, &DfxTrialHandshake::replyFinished);

	reply = networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

	while (waitingForResponse) {
		QApplication::processEvents();
	}

	std::pair<bool, int> answer;
	answer.second = responseCode;

	if (responseMsg == "amen") answer.first = true;
	else answer.first = false;

	return answer;
}

void DfxTrialHandshake::replyFinished(QNetworkReply *reply) {
	QByteArray bytes = reply->readAll();
	responseMsg = QString::fromUtf8(bytes.data(), bytes.size());
	responseCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	waitingForResponse = false;
}