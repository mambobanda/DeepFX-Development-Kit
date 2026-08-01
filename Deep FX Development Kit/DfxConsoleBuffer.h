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

#include <QObject>
#include <QtWidgets\qwidget.h>
#include <QString>
#include <DfxString.h>
#include <string>
#include <mutex>
#include <sstream>

class DfxConsoleBuffer: public QObject {
	Q_OBJECT
private:
	static std::mutex _guard;
private:
	static DfxConsoleBuffer *instance;
	std::stringstream buffer;

	size_t _maxBufferSize;

	DfxConsoleBuffer();
public:

	static DfxConsoleBuffer *getInstance();

	void appendBuffer(DfxString);
	void appendBuffer(std::string);
	void appendBuffer(const char*);

	void setMaxBufferSize(size_t);
	size_t maxBufferSize();

	void cullBuffer();

	void saveBufferToFile(DfxString, DfxString);

	std::string toString();
signals:
	void bufferUpdateEvent(const QString &);
};
