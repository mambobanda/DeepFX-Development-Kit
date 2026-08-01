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

#include "ViewMapInitializer.h"

#include <DfxConsole.h>
#include <QStandardPaths.h>
#include <QSettings>
#include "DfxToolbarDomain.h"
#include "DfxQSceneView.h"
#include "DfxSceneGraphEditor.h"
#include "DfxShaderEditor.h"
#include "DfxQPropertiesEditor.h"
#include "DfxQFluentNodeEditor.h"
#include "DfxTimelineEditor.h"
#include "DfxIPRView.h"
#include "DfxRenderView.h"
#include "DfxResourceBrowser.h"
#include "DfxQCacheTrackEditor.h"
#include "DfxGraphEditor.h"

namespace viewmap {
	void initialize() {
		
		DfxQViewMap::getInstance()->addEntry("Tools", DfxQViewID::DFX_TOOLBAR, []() { return new DfxToolbarDomain(); });

		DfxQViewMap::getInstance()->addEntry("Viewport", DfxQViewID::DFX_SCENE_VIEW, []() { return new DfxQSceneView(); });

		DfxQViewMap::getInstance()->addEntry("Console", DfxQViewID::DFX_CONSOLE, []() { 
			auto console = new DfxConsole();
			style::styleQScrollBar(console->textArea()->horizontalScrollBar());
			style::styleQScrollBar(console->textArea()->verticalScrollBar());
			return console; 
		});

		DfxQViewMap::getInstance()->addEntry("Fluent Editor", DfxQViewID::DFX_FLUENT_EDITOR, []() { return new DfxFluentNodeEditor(); });

		DfxQViewMap::getInstance()->addEntry("Outliner", DfxQViewID::DFX_SCENE_GRAPH, []() { return new DfxSceneGraphEditor(); });

		DfxQViewMap::getInstance()->addEntry("Material Editor", DfxQViewID::DFX_SHADER_EDITOR, []() { return new DfxShaderEditor(); });

		DfxQViewMap::getInstance()->addEntry("Properties Editor", DfxQViewID::DFX_PROPERTIES_EDITOR, []() { return new DfxPropertiesEditor(); });

		DfxQViewMap::getInstance()->addEntry("Timeline Editor", DfxQViewID::DFX_TIMELINE_EDITOR, []() { return new DfxTimelineEditor(); });

		DfxQViewMap::getInstance()->addEntry("Graph Editor", DfxQViewID::DFX_GRAPH_EDITOR, []() { return new DfxGraphEditor(); });

		DfxQViewMap::getInstance()->addEntry("Render View", DfxQViewID::DFX_RENDER_VIEW, []() { return new DfxRenderView(); });

		DfxQViewMap::getInstance()->addEntry("Interactive Preview Render", DfxQViewID::DFX_IPR_VIEW, []() { return new DfxIPRView(); });

		// DfxQViewMap::getInstance()->addEntry("Resource Browser", DfxQViewID::DFX_RESOURCE_BROWSER, []() { return new DfxResourceBrowser(); });

		DfxQViewMap::getInstance()->addEntry("Track Manager", DfxQViewID::DFX_TRACK_EDITOR, []() { return new DfxQCacheTrackEditor(); });

	}

	void initResources() {

		QString appdata = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];

		QDir dir(appdata + "/layout_config");
		QDir tmpDir(appdata + "/tmp");
		QFile file(appdata + "/layout_config/default.lt");

		QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "Deep FX", "Deep FX Studio");
		auto rc = settings.value("reset_default_layouts", "none");

		if (!dir.exists()) filesystem::create_directories(filesystem::path((appdata + "/layout_config").toStdString()));
		
		if (!tmpDir.exists()) {
			filesystem::create_directories(filesystem::path((appdata + "/tmp").toStdString()));
		}
		else {
			filesystem::remove_all(filesystem::path((appdata + "/tmp").toStdString()));
			filesystem::create_directories(filesystem::path((appdata + "/tmp").toStdString()));
		}

		if (!file.exists()) QFile::copy(":/layout/default", appdata + "/layout_config/default.lt");
		
		if (rc == "true") {

			QDir defaultDir("layout_config");
			if (defaultDir.exists()) {
				QStringList configs = defaultDir.entryList(QStringList() << "*.lt", QDir::Files);

				for (auto str : configs) {
					QFile cfile("layout_config/" + str);
					QFile ofile(appdata + "/layout_config/" + str);

					if (ofile.exists()) ofile.remove();

					if (cfile.exists()) {
						cfile.copy(ofile.fileName());
					}
				}
			}

			settings.setValue("reset_default_layouts", "false");
		}

		if (!dir.exists()) filesystem::create_directories(filesystem::path((appdata + "/layout_config").toStdString()));

		if (!file.exists()) QFile::copy(":/layout/default", appdata + "/layout_config/default.lt");
	}
}