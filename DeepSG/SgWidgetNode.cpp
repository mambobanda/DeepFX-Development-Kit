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

#include "SgWidgetNode.h"


namespace deepsg {

	std::string SgWidgetNode::_uniqueName = "UID_WIDGET_NODE";

	SgWidgetNode::SgWidgetNode():SgNode(SgNodeType::WIDGET) {
		_widget = nullptr;
	}

	SgWidgetNode::~SgWidgetNode() {
		delete _widget;
	}

	std::string SgWidgetNode::uniqueName() {
		return _uniqueName;
	}

	void SgWidgetNode::setWidget(SgWidget *sg) {
		_widget = sg;
	}

	SgWidget *SgWidgetNode::widget() {
		return _widget;
	}

}