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
#include "DfxGenericPythonEntity.h"

namespace py = pybind11;

class DfxGenericPythonEntityTrampoline : public DfxGenericPythonEntity {
	using DfxGenericPythonEntity::DfxGenericPythonEntity;

	void  compute() override {
		PYBIND11_OVERLOAD_PURE(
			void, /* Return type */
			DfxGenericPythonEntity,      /* Parent class */
			compute,          /* Name of function in C++ (must match Python name) */
		);
	}

	void  define() override {
		PYBIND11_OVERLOAD_PURE(
			void, /* Return type */
			DfxGenericPythonEntity,      /* Parent class */
			define,          /* Name of function in C++ (must match Python name) */
		);
	}

	void  setup() override {
		PYBIND11_OVERLOAD_PURE(
			void, /* Return type */
			DfxGenericPythonEntity,      /* Parent class */
			setup,          /* Name of function in C++ (must match Python name) */
			);
	}
};

PYBIND11_EMBEDDED_MODULE(xnode, m) {
	m.doc() = "Entity Framework classes and framework."; // optional module docstring

	py::class_<DfxPyEntityDefinition>(m, "EntityDefinition")
		.def(py::init<>())
		.def("addProperty", &DfxPyEntityDefinition::addProperty);

	py::class_<DfxGenericPythonEntity, DfxGenericPythonEntityTrampoline>(m, "GenericEntity")
		.def(py::init<>())
		.def("compute", &DfxGenericPythonEntity::compute)
		.def("define", &DfxGenericPythonEntity::define)
		.def("setup", &DfxGenericPythonEntity::setup)
		.def("setName", &DfxGenericPythonEntity::setName)
		.def("define", &DfxGenericPythonEntity::define)
		.def("addProperty", &DfxGenericPythonEntity::addProperty)
		.def("instanceID", &DfxGenericPythonEntity::_getInstanceID)
		.def("setPropertyType", &DfxGenericPythonEntity::setPropetryType)
		.def("setFloatProperty", &DfxGenericPythonEntity::setFloatProperty)
		.def("getFloatProperty", &DfxGenericPythonEntity::getFloatProperty)
		.def("setFloat3Property", &DfxGenericPythonEntity::setFloat3Property)
		.def("getFloat3Property", &DfxGenericPythonEntity::getFloat3Property)
		.def("getFloatArrayProperty", &DfxGenericPythonEntity::getFloatArrayProperty)
		.def("getFloat3ArrayProperty", &DfxGenericPythonEntity::getFloat3ArrayProperty)
		.def("getMeshProperty", &DfxGenericPythonEntity::getMeshProperty)
		.def("getEnumProperty", &DfxGenericPythonEntity::getEnumProperty )
		.def("getUIDProperty", &DfxGenericPythonEntity::getUIDProperty)
		.def("cleanEntity", &DfxGenericPythonEntity::_cleanEntity)
		.def("dirtyChildren", &DfxGenericPythonEntity::_dirtyChildren);

}