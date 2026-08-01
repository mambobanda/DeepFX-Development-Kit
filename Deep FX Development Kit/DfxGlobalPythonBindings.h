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

#include <pybind11/embed.h>
#include "DfxGlobal.h"

PYBIND11_EMBEDDED_MODULE(xglobal, m) {
	m.doc() = "Global Deep Fx SDK App Functions."; // optional module docstring

	m.def("print", &DfxGlobal::console::python_print);
	m.def("println", &DfxGlobal::console::python_println, "A function to print stuff on the console");
	m.def("messageBox", &DfxGlobal::info::python_messageBox, "Displays a message box with options.");
	m.def("getFloatValue", &DfxGlobal::input::getFloatValue, "Get value from user.");
	m.def("getStringValue", &DfxGlobal::input::getStringValue, "Get value from user.");
	m.def("command", &DfxGlobal::command::python_exec, "A function to execute in-built commands");
}

