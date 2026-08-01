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

#include "DfxPythonGlobal.h"

DfxPythonGlobal *DfxPythonGlobal::instance = nullptr;

DfxPythonGlobal::DfxPythonGlobal() {
	python_dir = std::wstring(L"python/");
	app_name = "Deep Fx SDK";
	guard = nullptr;
	enabled = false;
}

DfxPythonGlobal::~DfxPythonGlobal() {
	if (guard != nullptr) {
		delete guard;
	}
}

DfxPythonGlobal *DfxPythonGlobal::getInstance() {

	if (instance == nullptr) {
		instance = new DfxPythonGlobal();
	}

	return instance;
}

void DfxPythonGlobal::initPythonInterpreter() {
	if (enabled) {
		Py_SetProgramName(L"Deep Fx SDK");
		Py_SetPythonHome((wchar_t*)python_dir.c_str());

		guard = new py::scoped_interpreter();
	}
}

void DfxPythonGlobal::setPythonDir(std::wstring path) {
	python_dir = path;
}

void DfxPythonGlobal::helloWorldTest() {

	py::print("Hello, World!");
}

void DfxPythonGlobal::exec(std::string script) {

	if (enabled) {
		try {

			py::exec(script.c_str());
			DfxGlobal::console::println("< Finished executing Python Script >");
		}
		catch (std::exception &ex) {
			DfxGlobal::console::println(ex.what());
		}
	}
	else {
		DfxGlobal::console::println("< Python Is Disabled. >");
	}

}

bool DfxPythonGlobal::isPythonEnabled() {
	return enabled;
}

void  DfxPythonGlobal::enablePython(bool val) {
	enabled = val;
}