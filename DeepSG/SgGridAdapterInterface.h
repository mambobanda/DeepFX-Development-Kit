/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include <string>
#include "SgGridChannel.h"

namespace deepsg {

	class SgGridAdapterInterface {
	public:

		virtual ~SgGridAdapterInterface();
		virtual unsigned int gridImplementation() = 0;

		virtual unsigned int channelCount() = 0;
		virtual std::string channelName(unsigned int index) = 0;

		virtual SgGridChannel *channel(unsigned index) = 0;
		virtual SgGridChannel *channel(std::string channelName) = 0;

		virtual void clear() = 0;

	};

}