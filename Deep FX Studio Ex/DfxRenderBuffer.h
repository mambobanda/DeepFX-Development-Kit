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

#include <QObject>
#include <QImage>
#include <mutex>

class DfxRenderBuffer : public QObject {
Q_OBJECT
private:
	QImage _buffer;
	std::string _bufferName;
	size_t _index;

	std::recursive_mutex _mutex;
public:

	DfxRenderBuffer();
	virtual ~DfxRenderBuffer();

	void setImage(QImage image);
	QImage &getImage();

	std::string bufferName();
	void setBufferName(std::string);

	void setIndex(size_t);
	size_t getIndex();

	void lock();
	void unlock();
signals:
	void updated();
};