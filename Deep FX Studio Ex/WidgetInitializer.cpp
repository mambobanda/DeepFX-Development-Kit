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

#include "WidgetInitializer.h"
#include "DfxWidgetHandler.h"

namespace widget {

	void initialize() {
		DfxWidgetHandler handler;
		handler.create();

		DfxTranslateWidget *twidget = new DfxTranslateWidget;
		deepsg::SgWidgetNode *twidgetNode = handler.widgetNode();
		twidgetNode->setWidget(twidget);

		DfxWidgetManager::getInstance()->setWidget(twidgetNode);
	}
}