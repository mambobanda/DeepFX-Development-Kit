/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <QtGlobal>
#include <string>
#include "DfxExternalToolGlobalState.h"

#ifdef Q_OS_WIN
#define DFX_TOOL_EXPORT __declspec(dllexport)
#else
#define DFX_TOOL_EXPORT
#endif

typedef void *(*CommandInstanceCallback)();

typedef struct {
	const char *commandName;
	CommandInstanceCallback commandInstance;
}ExternalToolDefinition;

typedef const char *(*toolNameCB)();
typedef const char *(*toolCommandCB)();
typedef void (*defineGlobalStateCB)(GlobalState *(*gs)());
typedef ExternalToolDefinition *(*toolDefinitionCB)();

