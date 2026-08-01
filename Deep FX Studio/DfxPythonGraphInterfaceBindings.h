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

#include <pybind11/stl.h>
#include <pybind11/embed.h>
#include "DfxPythonGraphInterface.h"

PYBIND11_EMBEDDED_MODULE(xgraph, m) {
	m.doc() = "Deep FX SDK Graph Functions."; // optional module docstring

	// Create Destroy Functions
	m.def("createNode",		&createNode, "Create new node from unique id");
	m.def("createViewNode", &createViewNode, "Create new view node from unique id");

	// Manipulation Functions
	m.def("connectProperties", &connectProperties, "Connect two node properties (output->input)");
	m.def("connectToGroup", &connectToGroup, "Connect trait ID to group.");
	m.def("disconnectProperties", &disconnectProperties, "Disconnect two node properties ");

	// Node Property values Get/Set
	m.def("setString", &setString, "Set node string property value");
	m.def("getString", &getString, "Get node string property value");
	m.def("setFloat", &setFloat, "Set node float property value");
	m.def("getFloat", &getFloat, "Get node float property value");
	m.def("setFloat3", &setFloat3, "Set node float3 property value");
	m.def("getFloat3", &getFloat3, "Get node float3 property value");
	m.def("setEnumIndex", &setEnumIndex, "Set node enum property value");
	m.def("getEnumIndex", &getEnumIndex, "Get node emum property value");

	// Graph UI Functions
	m.def("getNodesSelectedInGraph", &getNodesSelectedInGraph, "Get selected nodes in UI");
	m.def("uniformLayoutSelected", &uniformLayoutSelected, "Layout nodes in uniform, takes selection into account");
	m.def("uniformLayout", &uniformLayout, "Layout nodes in uniform");
	m.def("viewAllNodes", &viewAllNodes, "View all nodes in the ui");

	// Query Functions	
	m.def("getNodeUniqueName",	&getNodeUniqueName, "Returns node's unique name");
	m.def("getPropertyID",		&getPropertyID, "Returns property id for property name");
	m.def("printNodeProperties",&printNodeProperties, "Print all node properties");
	m.def("hasProperty",		&hasProperty, "Check if node has named property");
	m.def("hasPropertyID",		&hasPropertyID, "Check if node and name match property id");
	m.def("findAllNodes",		py::overload_cast<>(&findAllNodes), "Find all node ids");
	m.def("findAllNodes",		py::overload_cast<const char*>(&findAllNodes), "Find Find all node ids for specifc type of node");
	m.def("propertyStateType",	&propertyStateType, "Query property state type (e.g Input, Output etc...)");
	m.def("propertyIsInput",	&propertyIsInput, "Is node property an Input type");
	m.def("propertyIsOutput",	&propertyIsOutput, "Is node property an Output type");
	m.def("propertyIsDatastore",&propertyIsDatastore, "Is node property a Datasource type");
	m.def("propertyHasParent",	&propertyHasParent, "Check if property has a parent");
	m.def("propertyHasChildren", &propertyHasChildren, "Check if property has children");
	m.def("getPropertyParent",	&getPropertyParent, "Get node property parent");
	m.def("getPropertyChildren",&getPropertyChildren, "Get node property children");
	m.def("viewHasSelection",	&viewHasSelection, "Check if object is selected in the viewport.");
	m.def("getViewSelection",	&getViewSelection, "Get first selected object in the viewort.");
	m.def("graphHasSelection", &graphHasSelection, "Graph has selection.");
	m.def("getGraphSelection", &getGraphSelection, "Get graph selection.");
	m.def("hasMeshData",		&hasMeshData, "Check if node has mesh data");
	m.def("hasProxyMeshData",	&hasProxyMeshData, "Check if node has proxy mesh data");
	m.def("hasChildren", &hasChildren, "Check if node has children");
	m.def("getAllChildren", &getAllChildren, "Get all nodes children");

}
