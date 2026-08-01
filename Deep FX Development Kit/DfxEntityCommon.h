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

#include <string>

// Forward Declarations
class DfxEntityDefintion;
class DfxEntityProperty;
class DfxEntityDataInstance;
class DfxEntityDB;
class DfxEntity;

// Enumerations
enum class DfxEntityPropertyState { NORMAL_STATE = 0x100, INPUT_STATE, OUTPUT_STATE, DATA_STORE_STATE };

//Definitions
using DfxGlobalEntityName = std::string;

// Dfx Entity Item Types
static size_t DFX_OBJECT_ITEM = 255;
static size_t DFX_CAMERA_ITEM = 256;
static size_t DFX_LIGHT_ITEM  = 257;
static size_t DFX_CUSTOM_ITEM = 258;
static size_t DFX_POINT_ITEM  = 259;
static size_t DFX_NULL_ITEM   = 260;
