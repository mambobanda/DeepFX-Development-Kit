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

#include <QGraphicsItem>

namespace DfxQMaterialNodeEnums {

	enum { 
		BASE_NODE			= QGraphicsItem::UserType + 1,
		PROPERTY_SLOT		= QGraphicsItem::UserType + 2,
		PROPERTY_SLOT_LABEL = QGraphicsItem::UserType + 3,
		LINK				= QGraphicsItem::UserType + 4,
	};
}