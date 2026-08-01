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

enum class DfxQViewID 
{ NONE, DFX_ROOT, DFX_SCENE_VIEW, DFX_CONSOLE, DFX_SCENE_GRAPH, DFX_LAYERS, DFX_TOOLBAR, DFX_SHADER_EDITOR, DFX_PROPERTIES_EDITOR,
  DFX_TIMELINE_EDITOR, DFX_IPR_VIEW, DFX_FLUENT_EDITOR, DFX_RESOURCE_BROWSER, DFX_RENDER_VIEW, DFX_TRACK_EDITOR, DFX_GRAPH_EDITOR,
};

std::string viewIDToString(DfxQViewID id);
DfxQViewID stringToViewID(std::string str);