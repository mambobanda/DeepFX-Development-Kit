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

#include <Vector>
#include <DfxEntityDB.h>
#include <QString>
#include <Stack>
#include "DfxLayerManager.h"

// Creation / Destruction Functions
size_t createNode(const char* uniqueid);
size_t createViewNode(const char* uniqueid);

// Manipulation Functions
void connectToGroup(size_t source_id, size_t source_property_id, size_t groupid);
void connectProperties(size_t source_id, size_t source_property_id, size_t destination_id, size_t destination_property_id);
void disconnectProperties(size_t source_id, size_t source_property_id, size_t destination_id, size_t destination_property_id);

// Node Property values Get/Set
void  setString(size_t source_id, const char* name, const char *);
const char *getString(size_t source_id, const char* name);
void  setFloat(size_t source_id, const char* name, float);
float getFloat(size_t source_id, const char* name);
void  setFloat3(size_t source_id, const char* name, float x , float y, float z);
std::vector<float> getFloat3(size_t source_id, const char* name);
void  setEnumIndex(size_t source_id, const char* name, int index);
int   getEnumIndex(size_t source_id, const char* name);


// Graph UI Functions
std::vector<size_t> getNodesSelectedInGraph();
void uniformLayoutSelected();
void uniformLayout();
void viewAllNodes();

// Traversal Functions

// Query Functions
const char *getNodeUniqueName(size_t uid);
size_t getPropertyID(size_t uid, const char* name);
void printNodeProperties(size_t intanceid);
bool hasProperty(size_t uid, const char* name);
bool hasPropertyID(size_t uid, const char* name, size_t propid);
std::vector<size_t>findAllNodes();
std::vector<size_t>findAllNodes(const char* uniqueid);
const char *propertyStateType(size_t uid, size_t propid);
bool propertyIsInput(size_t uid, size_t propid);
bool propertyIsOutput(size_t uid, size_t propid);
bool propertyIsDatastore(size_t uid, size_t propid);
bool propertyHasParent(size_t uid, size_t propid);
bool propertyHasChildren(size_t uid, size_t propid);
size_t getPropertyParent(size_t uid, size_t propid);
std::vector<size_t> getPropertyChildren(size_t uid, size_t propid);
bool viewHasSelection();
size_t getViewSelection();
bool graphHasSelection();
std::vector<size_t> getGraphSelection();
bool hasMeshData(size_t uid);
bool hasProxyMeshData(size_t uid);
bool hasChildren(size_t uid);
std::vector<size_t> getAllChildren(size_t uid);
//bool propertyHasParent(size_t uid, size_t propid);
//bool propertyHasChildren(size_t uid, size_t propid);

