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

class DfxRiptideStageNode;
class DfxRiptideStageHandler;
class DfxRiptideCachePropNode;
class DfxRiptideCachePropHandler;
class DfxRiptideActorNode;
class DfxRiptideActorHandler;
class DfxRiptideAgent;

namespace riptide {
	namespace type {
		enum {
			ACTOR = 100, 

			CACHE = 200, EMITTER = 201, HEATER = 202,
		};
	}
}