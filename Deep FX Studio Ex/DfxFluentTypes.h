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


namespace DfxFluentTypes {
	enum {

		/// Deep Simplex
		DS_MESH = 5000,
		DS_POINT_COUNT = 5001,

		/// Deep SG
		SG_NODE = 5100,
		SG_GEO_NODE		= 5101,
		SG_WIDGET_NODE	= 5102,
		SG_SHAPE_NODE   = 5103,
		SG_STAGE_NODE   = 5104,
		SG_STAGE_ACTOR_NODE = 5105,
		SG_STAGE_PROP_NODE  = 5106,
		SG_POINT_CLOUD_NODE = 5107,
		SG_TOOL_NODE		= 5108,
		SG_RENDER_NODE		= 5109,
		SG_GROUP_NODE		= 5110,
		SG_NULL_NODE		= 5111,
		SG_RENDER_DELEGATE_NODE = 5112,
		SG_GRID_NODE	= 5113,

		/// Raw Data
		IMAGE = 6000,
		RESOURCE = 6001,
		RESOURCE_POINTER = 6002,
		
	};
};