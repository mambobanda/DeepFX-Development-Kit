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

#include <vector>
#include "VCommon.h"


namespace xvault {

	class Node {
	public:
		virtual std::string name() = 0;
		virtual std::vector <Attribute*> &attributes() = 0;
		virtual std::vector <Buffer*> &arrays() = 0;
		virtual std::vector <Node*> &nodes() = 0;
		virtual Attribute *findAttributeByName(std::string) = 0;
		virtual Buffer *findBufferByName(std::string) = 0;
		virtual Node *findNodeByName(std::string) = 0;
		virtual void clear() = 0;
	};
}