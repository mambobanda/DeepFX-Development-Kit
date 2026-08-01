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


#include <exception>
#include <string>
#include <pybind11/embed.h>
#include <Python.h>
#include "DfxGlobal.h"

namespace py = pybind11;

class DfxPythonGlobal {
private:
	static DfxPythonGlobal *instance;
	DfxPythonGlobal();

	py::scoped_interpreter *guard;

	bool enabled;
private:
	std::string app_name;
	std::wstring python_dir;
public:

	~DfxPythonGlobal();

	static DfxPythonGlobal *getInstance();

	void initPythonInterpreter();
	void setPythonDir(std::wstring);
	
	void exec(std::string script);

	bool isPythonEnabled();
	void  enablePython(bool);
	void helloWorldTest();
};