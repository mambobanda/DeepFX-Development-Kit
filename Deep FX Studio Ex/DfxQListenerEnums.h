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
		LISTENER_NODE		 = QGraphicsItem::UserType + 1,
		LISTENER_NODE_ITEM	 = QGraphicsItem::UserType + 2,
		LISTENER_EDIT_FLOW	 = QGraphicsItem::UserType + 3,
		LISTENER_NEW_FLOW	 = QGraphicsItem::UserType + 4,
		LISTENER_DELETE_FLOW = QGraphicsItem::UserType + 5,
		LISTENER_LABEL		 = QGraphicsItem::UserType + 6,
		LISTENER_ORDER_FLOW  = QGraphicsItem::UserType + 7,
	};
}