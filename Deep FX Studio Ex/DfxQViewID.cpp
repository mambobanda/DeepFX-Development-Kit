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

#include "DfxQViewID.h"

std::string viewIDToString(DfxQViewID id) {
	switch(id){
	case DfxQViewID::NONE:
		return "none";
		break;
	case DfxQViewID::DFX_CONSOLE:
		return "console";
		break;
	case DfxQViewID::DFX_ROOT:
		return "root";
		break;
	case DfxQViewID::DFX_SCENE_GRAPH:
		return "scene_graph";
		break;
	case DfxQViewID::DFX_SCENE_VIEW:
		return "scene_view";
		break;
	case DfxQViewID::DFX_TOOLBAR:
		return "toolbar";
		break;
	case DfxQViewID::DFX_SHADER_EDITOR:
		return "shader_editor";
		break;
	case DfxQViewID::DFX_PROPERTIES_EDITOR:
		return "properties_editor";
		break;
	case DfxQViewID::DFX_TIMELINE_EDITOR:
		return "timeline_editor";
		break;
	case DfxQViewID::DFX_GRAPH_EDITOR:
		return "graph_editor";
		break;
	case DfxQViewID::DFX_IPR_VIEW:
		return "ipr";
		break;
	case DfxQViewID::DFX_FLUENT_EDITOR:
		return "fluent_editor";
		break;
	case DfxQViewID::DFX_RESOURCE_BROWSER:
		return "resource_browser";
		break;
	case DfxQViewID::DFX_RENDER_VIEW:
		return "render_view";
		break;
	case DfxQViewID::DFX_TRACK_EDITOR:
		return "track_editor";
		break;
	default:
		return "null";
		break;
	};
}

DfxQViewID stringToViewID(std::string str) {

	if (str == "none") {
		return DfxQViewID::NONE;
	}
	else if (str == "console") {
		return DfxQViewID::DFX_CONSOLE;
	}
	else if (str == "root") {
		return DfxQViewID::DFX_ROOT;
	}
	else if (str == "scene_graph") {
		return DfxQViewID::DFX_SCENE_GRAPH;
	}
	else if (str == "scene_view") {
		return DfxQViewID::DFX_SCENE_VIEW;
	}
	else if (str == "toolbar") {
		return DfxQViewID::DFX_TOOLBAR;
	}
	else if (str == "shader_editor") {
		return DfxQViewID::DFX_SHADER_EDITOR;
	}
	else if (str == "properties_editor") {
		return DfxQViewID::DFX_PROPERTIES_EDITOR;
	}
	else if (str == "timeline_editor") {
		return DfxQViewID::DFX_TIMELINE_EDITOR;
	}
	else if (str == "graph_editor") {
		return DfxQViewID::DFX_GRAPH_EDITOR;
	}
	else if (str == "ipr") {
		return DfxQViewID::DFX_IPR_VIEW;
	}
	else if (str == "fluent_editor") {
		return DfxQViewID::DFX_FLUENT_EDITOR;
	}
	else if (str == "resource_browser") {
		return DfxQViewID::DFX_RESOURCE_BROWSER;
	}
	else if (str == "render_view") {
		return DfxQViewID::DFX_RENDER_VIEW;
	}
	else if (str == "track_editor") {
		return DfxQViewID::DFX_TRACK_EDITOR;
	}
	else {
		return DfxQViewID::NONE;
	}
}