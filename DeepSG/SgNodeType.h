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

namespace deepsg {

	enum class SgNodeType {
		NILL,
		ROOT,
		TRANSFORM,
		GROUP,
		GRID,
		GEOMETRY,
		WIDGET,
		SHAPE,
		TOOL,
		POINT_CLOUD,
		RENDER,
		RENDER_DELEGATE,
		STAGE,
		STAGE_ACTOR_COLLECTION,
		STAGE_ACTOR,
		STAGE_PROP_COLLECTION,
		STAGE_PROP,
	};

}