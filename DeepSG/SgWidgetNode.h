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

#include <glm/vec3.hpp>
#include "SgNode.h"
#include "SgWidget.h"
#include "SgTransform.h"

namespace deepsg {

	class SgWidgetNode: public SgNode {
	private:
		SgWidget *_widget;

		static std::string _uniqueName;
	public:
		SgWidgetNode();
		virtual ~SgWidgetNode();

		std::string uniqueName();

		void setWidget(SgWidget *);
		SgWidget *widget();
	};

}