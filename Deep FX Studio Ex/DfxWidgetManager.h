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

#include <vector>
#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <QObject>
#include <SgNode.h>
#include <SgWidgetNode.h>
#include <SgRootNode.h>
#include <SgXForm.h>
#include <stack>
#include <DfxMode.h>
#include "DfxSceneSelectionModel.h"
#include "DfxWidgetInterface.h"
#include "DfxMeshSelectionModel.h"
#include "DfxWidgetHandler.h"

class DfxWidgetManager: public QObject {
	Q_OBJECT
private:
	static DfxWidgetManager *_instance;
	
	std::stack<deepsg::SgWidgetNode*> widgetStack;
	deepsg::SgWidgetNode *widget;

	DfxWidgetManager();
public:
	~DfxWidgetManager();

	static DfxWidgetManager *getInstance();

	void setWidget(deepsg::SgWidgetNode *);
	deepsg::SgWidgetNode *getWidget();

	deepsg::SgXForm getTransform();

	void pushWidget();
	void popWidget();

	bool isWidgetActive();
	bool isNull();

	void clear();
public slots:
	void updateObjectModeWidgets();
};