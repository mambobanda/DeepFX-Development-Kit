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

#include "FlCommon.h"
#include "FlSpaceEventInfo.h"

namespace fl {

	class SpaceObserver {
	private:
		static size_t ID_GEN;
		size_t _id;

		Space *_parent;
	public:

		SpaceObserver();
		virtual ~SpaceObserver();

		size_t id();

		void setParent(Space *parent);

		virtual void update(SpaceEventInfo info) = 0;

	};
}