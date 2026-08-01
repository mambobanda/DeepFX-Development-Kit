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
	* @file DfxQPropertiesHub.cpp
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <vector>
#include <QObject.h>
#include "DfxObjectHandler.h"

class DfxQPropertiesHub: public QObject {
	Q_OBJECT
private:
	static DfxQPropertiesHub *_instance;

	DfxQPropertiesHub();
public:

	static DfxQPropertiesHub *getInstance();

	void newSceneNodeSelected(std::vector<deepsg::SgNode*> nodes);
	void newDataObjectSelected(std::string path);

signals:
	void newSceneNodeSelected_signal(std::vector<deepsg::SgNode*> nodes);
	void newDataObjectSelected_signal(std::string path);
};