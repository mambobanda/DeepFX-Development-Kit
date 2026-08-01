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

#include <QString>
#include <vector>
#include <string>
#include <sstream>
#include <DfxGlobal.h>
#include "DfxEntity.h"
#include "DfxEnum.h"
#include "DfxEntityBasicPropertyTypes.h"

bool entityHasProperty(std::string pro_name, DfxEntity* entity, size_t &);

std::vector < std::pair<std::string, std::string> > mapEntityProperties( DfxEntity *entity);

void mapEntityProperties( DfxEntity *entity, std::vector < std::pair<std::string, std::string>> &properties);
