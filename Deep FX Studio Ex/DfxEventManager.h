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
#include <QFuture>
#include <QtConcurrent/QtConcurrent> 
#include <QApplication>
#include "DfxSpace.h"
#include "DfxGlobal.h"
#include "DfxEventEnums.h"
#include "DfxFlowHandler.h"
#include "FlInterpreter.h"

class DfxEventManager: public QObject {
	Q_OBJECT
private:
	static DfxEventManager *_instance;
	DfxEventManager();

	void annouceToFluentEventListeners(unsigned int);
public:
	~DfxEventManager();

	static DfxEventManager *getInstance();

	void announceEvent(unsigned int);

signals:
	void announceEventSignal(unsigned int);
};