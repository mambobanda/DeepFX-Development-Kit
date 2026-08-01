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


#include <QMessageBox>
#include "ShortcutConfig.h"

namespace shortcuts {
	void configure() {

		// Main application groups and shortcuts 
		DfxShortcutsDB::getInstance()->addGroup("Application", "Application");

		DfxShortcutsDB::getInstance()->addShortcut("Application", "New Project", []() { 
			QMessageBox msgBox;
			msgBox.setText("Are you sure you want to start a new project?");
			msgBox.setInformativeText("Unsaved data will be lost.");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			msgBox.setDefaultButton(QMessageBox::Yes);
			int ret = msgBox.exec();

			if (ret == QMessageBox::Yes) {
				DfxGlobal::command::exec("clearScene");
			}}
		);

		DfxShortcutsDB::getInstance()->addShortcut("Application", "Open Project", []() { DfxGlobal::command::exec("loadProject"); });
		DfxShortcutsDB::getInstance()->addShortcut("Application", "Save Project", []() { DfxGlobal::command::exec("saveProject"); });
		DfxShortcutsDB::getInstance()->addShortcut("Application", "Save Project As", []() { DfxGlobal::command::exec("saveProjectAs"); });
		
		DfxShortcutsDB::getInstance()->addShortcut("Application", "Quit Application", []() {
			QMessageBox msgBox;
			msgBox.setText("Are you sure you want to exit Deep Fx Studio?");
			msgBox.setInformativeText("Unsaved data will be lost.");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			msgBox.setDefaultButton(QMessageBox::Yes);
			int ret = msgBox.exec();

			if (ret == QMessageBox::Yes) {
				DfxGlobal::command::exec("exitApplication");
			}
		});

		DfxShortcutsDB::getInstance()->addShortcut("Application", "Open Hotkey Editor", []() {DfxGlobal::command::exec("openHotkeyEditor"); });
		//DfxShortcutsDB::getInstance()->addShortcut("Application", "Open Scene View Window", []() { });
		//DfxShortcutsDB::getInstance()->addShortcut("Application", "Open Graph Editor Window", []() { });
		//DfxShortcutsDB::getInstance()->addShortcut("Application", "Open Console Window", []() {});

		DfxShortcutsDB::getInstance()->addShortcut("Application", "Undo", []() { DfxGlobal::command::exec("undo"); });
		DfxShortcutsDB::getInstance()->addShortcut("Application", "Redo", []() { DfxGlobal::command::exec("redo"); });

		// Viewport navigation
		DfxShortcutsDB::getInstance()->addGroup("Viewport", "Viewport");

		DfxShortcutsDB::getInstance()->addShortcut("Viewport", "Pan", []() {});
		DfxShortcutsDB::getInstance()->addShortcut("Viewport", "Zoom", []() {});
		DfxShortcutsDB::getInstance()->addShortcut("Viewport", "Rotate", []() {});

		DfxShortcutsDB::getInstance()->addShortcut("Viewport", "Select", []() {});
		DfxShortcutsDB::getInstance()->addShortcut("Viewport", "Multi Select", []() {});

		DfxShortcutsDB::getInstance()->addShortcut("Viewport", "Context Menu", []() {});

		DfxShortcutsDB::getInstance()->setHotkey("Viewport", "Select", DfxShortcutsDB::KEYBOARD_MOUSE, 0, 0, 0, 0, Qt::LeftButton);
		DfxShortcutsDB::getInstance()->setHotkey("Viewport", "Multi Select", DfxShortcutsDB::KEYBOARD_MOUSE, Qt::Key_Shift, 0, 0, 0, Qt::LeftButton);

		DfxShortcutsDB::getInstance()->setHotkey("Viewport", "Context Menu", DfxShortcutsDB::KEYBOARD_MOUSE, 0, 0, 0, 0, Qt::RightButton);

		// Scene View groups and shortcuts
		DfxShortcutsDB::getInstance()->addGroup("Scene View", "Scene View");

		//DfxShortcutsDB::getInstance()->addShortcut("Scene View", "Switch To Perspective", []() {});
		//DfxShortcutsDB::getInstance()->addShortcut("Scene View", "Switch To Top", []() {});
		//DfxShortcutsDB::getInstance()->addShortcut("Scene View", "Switch To Bottom", []() {});
		//DfxShortcutsDB::getInstance()->addShortcut("Scene View", "Switch To Left", []() {});
		//DfxShortcutsDB::getInstance()->addShortcut("Scene View", "Switch To Right", []() {});
		//DfxShortcutsDB::getInstance()->addShortcut("Scene View", "Switch To Front", []() {});
		//DfxShortcutsDB::getInstance()->addShortcut("Scene View", "Switch To Back", []() {});
		//DfxShortcutsDB::getInstance()->addShortcut("Scene View", "Render Mode Solid", []() {});
		//DfxShortcutsDB::getInstance()->addShortcut("Scene View", "Render Mode Wireframe", []() {});
		//DfxShortcutsDB::getInstance()->addShortcut("Scene View", "Toggle Grid", []() {});
		DfxShortcutsDB::getInstance()->addShortcut("Scene View", "Delete Item / Object / Elements", []() { DfxGlobal::command::exec("delete"); });
		
		// Material Editor
		DfxShortcutsDB::getInstance()->addGroup("Material Editor", "Material Editor");

		// DfxShortcutsDB::getInstance()->addShortcut("Material Editor", "Cut Nodes", []() {});
		DfxShortcutsDB::getInstance()->addShortcut("Material Editor", "Copy Nodes", []() {});
		DfxShortcutsDB::getInstance()->addShortcut("Material Editor", "Paste Nodes", []() {});

		// Compute Node Editor
		DfxShortcutsDB::getInstance()->addGroup("Compute Node Editor", "Compute Node Editor");

		// DfxShortcutsDB::getInstance()->addShortcut("Compute Node Editor", "Cut Nodes", []() {});
		DfxShortcutsDB::getInstance()->addShortcut("Compute Node Editor", "Copy Nodes", []() {});
		DfxShortcutsDB::getInstance()->addShortcut("Compute Node Editor", "Paste Nodes", []() {});

		// Outliner

		// Timeline Editor
		DfxShortcutsDB::getInstance()->addGroup("Outliner", "Outliner");

		DfxShortcutsDB::getInstance()->addShortcut("Outliner", "Delete Item / Object / Elements", []() { DfxGlobal::command::exec("delete"); });

		/*
		DfxShortcutsDB::getInstance()->addGroup("Timeline editor", "Timeline editor");

		DfxShortcutsDB::getInstance()->addShortcut("Timeline editor", "Multi Select", []() {});

		DfxShortcutsDB::getInstance()->setHotkey("Timeline editor", "Multi Select", DfxShortcutsDB::KEYBOARD_MOUSE, Qt::Key_Shift, 0, 0, 0, Qt::LeftButton);
		*/

		load();
	}

	void load() {

		QString appdata = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];

		QDir dir(appdata + "/configuration");
		QFile file(appdata + "/configuration/shortcuts.cfg");

		if (file.exists()) {
			DfxHotkeysSaveLoad::load(appdata + "/configuration/shortcuts.cfg");
		}
		else {
			DfxHotkeysSaveLoad::load(":/config/shortcuts");
		}
	}
}