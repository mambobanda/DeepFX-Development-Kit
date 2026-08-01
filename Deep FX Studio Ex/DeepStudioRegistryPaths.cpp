
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

#include "DeepStudioRegistryPaths.h"


namespace registry {

	void initRegistry() {

		DeepStudioRegistry::Entry app_settings;
		app_settings.String = "appdata/settings";

		DeepStudioRegistry::Entry app_temp;
		app_temp.String = "appdata/temp";

		DeepStudioRegistry::Entry py_tools;
		py_tools.String = "scripts/py_tools";

		DeepStudioRegistry::Entry py_nodes;
		py_nodes.String = "scripts/py_nodes";

		DeepStudioRegistry::insertEntry(_$APP_SETTINGS_DIR, app_settings);
		DeepStudioRegistry::insertEntry(_$APP_TEMP_DIR, app_temp);
		DeepStudioRegistry::insertEntry(_$PYTHON_NODE_DIR, py_nodes);
		DeepStudioRegistry::insertEntry(_$PYTHON_TOOL_DIR, py_tools);
	}

	void printRegistry() {
		DeepStudioRegistry::Entry out;

		if (DeepStudioRegistry::findEntry(_$APP_SETTINGS_DIR, out)) {
			std::cout << _$APP_SETTINGS_DIR << ": " << out.String << std::endl;
		}
		else {
			std::cout << "Entry Doesn't Exist. \n";
		}

		if (DeepStudioRegistry::findEntry(_$APP_TEMP_DIR, out)) {
			std::cout << _$APP_TEMP_DIR << ": " << out.String << std::endl;
		}
		else {
			std::cout << "Entry Doesn't Exist. \n";
		}

		if (DeepStudioRegistry::findEntry(_$PYTHON_NODE_DIR, out)) {
			std::cout << _$PYTHON_NODE_DIR << ": " << out.String << std::endl;
		}
		else {
			std::cout << "Entry Doesn't Exist. \n";
		}

		if (DeepStudioRegistry::findEntry(_$PYTHON_TOOL_DIR, out)) {
			std::cout << _$PYTHON_TOOL_DIR << ": " << out.String << std::endl;
		}
		else {
			std::cout << "Entry Doesn't Exist. \n";
		}
	}
}