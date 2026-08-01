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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxFluentBigID.h"

size_t DfxFluentBigID::_current_id = 1010;

size_t DfxFluentBigID::id() {
	if (_current_id >= (_max_id - 1)) _current_id = 10101;
	return _current_id++;
}