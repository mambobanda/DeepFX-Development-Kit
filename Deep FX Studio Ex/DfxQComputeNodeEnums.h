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

#include <QGraphicsItem>

namespace DfxQNodeEnums {

	enum {
		COMPUTE_BASE_NODE		= QGraphicsItem::UserType + 1,
		COMPUTE_PROPERTY_SLOT	= QGraphicsItem::UserType + 2,
		COMPUTE_PROPERTY_SLOT_LABEL = QGraphicsItem::UserType + 3,
		COMPUTE_LINK			= QGraphicsItem::UserType + 4,
		COMPUTE_NODE			= QGraphicsItem::UserType + 5,
		COMPUTE_COMPOUND_NODE	= QGraphicsItem::UserType + 6,
		COMPUTE_COMPOUND_IN_NODE  = QGraphicsItem::UserType + 7,
		COMPUTE_COMPOUND_OUT_NODE = QGraphicsItem::UserType + 8,
		COMPUTE_PROPERTY          = QGraphicsItem::UserType + 9,
		COMPUTE_ERROR_ICON        = QGraphicsItem::UserType + 10,
		COMPUTE_COMPOUND_FUNCTIONAL_ICON = QGraphicsItem::UserType + 11,
	};
}