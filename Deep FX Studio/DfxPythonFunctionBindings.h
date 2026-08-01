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


#include "DfxPythonFunctions.h"
#include <pybind11/embed.h>

PYBIND11_EMBEDDED_MODULE(xpy_nodes, m) {
	m.doc() = "Python Node Functions"; // optional module docstring

	m.def("buildPythonNode", &buildPythonNode, "Create new python node");
	m.def("destroyPythonNode", &destroyPythonNode, "Destroy python node");
}