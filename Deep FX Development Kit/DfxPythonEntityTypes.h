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
#include "DfxEntityBasicPropertyTypes.h"

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(xtypes, m) {
	m.doc() = "Entity framework property types, classes & functions."; // optional module docstring

	py::class_<DfxEntityPropertyTypes::Float3>(m, "Float3")
		.def(py::init<>())
		.def_readwrite("x", &DfxEntityPropertyTypes::Float3::x)
		.def_readwrite("y", &DfxEntityPropertyTypes::Float3::x)
		.def_readwrite("z", &DfxEntityPropertyTypes::Float3::x);

	py::class_<DfxEntityPropertyTypes::UID>(m, "UID")
		.def(py::init<>())
		.def("childCount", &DfxEntityPropertyTypes::UID::childCount)
		.def("addChild", &DfxEntityPropertyTypes::UID::addChild)
		.def("removeChild", &DfxEntityPropertyTypes::UID::removeChild)
		.def("getChild", &DfxEntityPropertyTypes::UID::getChild)
		.def("getChildren", &DfxEntityPropertyTypes::UID::getChildren)
		.def("clearChidren", &DfxEntityPropertyTypes::UID::clearChildren)
		.def("setRootID", &DfxEntityPropertyTypes::UID::setRootID)
		.def("getRootID", &DfxEntityPropertyTypes::UID::getRootID);

	py::class_<DfxEnum>(m, "Enum")
		.def(py::init<>())
		.def("add", &DfxEnum::add)
		.def("get", &DfxEnum::get)
		.def("setCurrentIndex", &DfxEnum::setCurrentIndex)
		.def("getCurrentIndex", &DfxEnum::getCurrentIndex)
		.def("size", &DfxEnum::size);

	py::class_<Dfx3dMeshData>(m, "Mesh")
		.def(py::init<>())
		.def("getVertex", &Dfx3dMeshData::_getVertex)
		.def("setVertex", &Dfx3dMeshData::_setVertex)
		.def("getNormal", &Dfx3dMeshData::_getNormal)
		.def("setNormal", &Dfx3dMeshData::_setNormal)
		.def("getColor", &Dfx3dMeshData::_getColor)
		.def("setColor", &Dfx3dMeshData::_setColor)
		.def("getIndex", &Dfx3dMeshData::getIndex)
		.def("setIndex", &Dfx3dMeshData::setIndex)
		.def("vertexCount", &Dfx3dMeshData::getVertexCount)
		.def("indexCount", &Dfx3dMeshData::getIndexCount)
		.def("resizeIndexBuffer", &Dfx3dMeshData::resizeIndexBuffer)
		.def("resizeVertexBuffer", &Dfx3dMeshData::resizeVertexBuffer)
		.def("copy", &Dfx3dMeshData::copy);

	py::class_<DfxEntityPropertyTypes::FloatArray>(m, "FloatArray")
		.def(py::init<>())
		.def("deleteArray", &DfxEntityPropertyTypes::FloatArray::deleteArray)
		.def("resize", &DfxEntityPropertyTypes::FloatArray::resize)
		.def("size", &DfxEntityPropertyTypes::FloatArray::size)
		.def("set", &DfxEntityPropertyTypes::FloatArray::set)
		.def("get", &DfxEntityPropertyTypes::FloatArray::get)
		.def("isNull", &DfxEntityPropertyTypes::FloatArray::isNull);

	py::class_<DfxEntityPropertyTypes::Float3Array>(m, "Float3Array")
		.def(py::init<>())
		.def("deleteArray", &DfxEntityPropertyTypes::Float3Array::deleteArray)
		.def("resize", &DfxEntityPropertyTypes::Float3Array::resize)
		.def("size", &DfxEntityPropertyTypes::Float3Array::size)
		.def("set", &DfxEntityPropertyTypes::Float3Array::set)
		.def("get", &DfxEntityPropertyTypes::Float3Array::get)
		.def("isNull", &DfxEntityPropertyTypes::Float3Array::isNull);


	py::enum_<DfxEntityPropertyTypes::_DfxEntityPropertyTypes>(m, "iTypes", py::arithmetic())
		.value("TraitID", DfxEntityPropertyTypes::_DfxEntityPropertyTypes::TRAIT_UID_TYPE)
		.value("Mesh", DfxEntityPropertyTypes::_DfxEntityPropertyTypes::i3DMESH_TYPE)
		.value("Float", DfxEntityPropertyTypes::_DfxEntityPropertyTypes::iFLOAT_TYPE)
		.value("FloatArray", DfxEntityPropertyTypes::_DfxEntityPropertyTypes::iFLOAT_ARRAY_TYPE)
		.value("Float3", DfxEntityPropertyTypes::_DfxEntityPropertyTypes::iFLOAT3_TYPE)
		.value("Float3Array", DfxEntityPropertyTypes::_DfxEntityPropertyTypes::iFLOAT3_ARRAY_TYPE)
		.value("Enum", DfxEntityPropertyTypes::_DfxEntityPropertyTypes::iENUM_TYPE)
		.export_values();
}