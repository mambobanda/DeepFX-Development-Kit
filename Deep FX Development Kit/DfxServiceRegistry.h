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

#include <unordered_map>
#include "DfxServiceEntry.h"
#include "DfxServiceClass.h"

class DfxServiceRegistry {
public:
	static std::unordered_map<size_t, DfxServiceClass*> services;

	static void registerSerivce(size_t class_id, DfxServiceEntry*);

	static std::vector<DfxServiceEntry*> fetchServiceEntries(size_t class_id);

};
