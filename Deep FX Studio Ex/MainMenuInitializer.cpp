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

#include <DfxQMainWindow.h>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QTextBrowser>
#include "DfxSystemIDGlobal.h"
#include "DfxQDialogWindow.h"
#include "MainMenuInitializer.h"

namespace mainmenu {

	void initialize() {

		// File
		DfxQMainMenu::getInstance()->addAction("File/New Project", []() {
			
			QMessageBox msgBox;
			msgBox.setText("Are you sure you want to start a new project?");
			msgBox.setInformativeText("Unsaved data will be lost.");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			msgBox.setDefaultButton(QMessageBox::Yes);
			int ret = msgBox.exec();

			if (ret == QMessageBox::Yes) {
				DfxGlobal::command::exec("clearScene");
			}
		});
		
		DfxQMainMenu::getInstance()->initRecentSceneMenu();

		DfxQMainMenu::getInstance()->addAction("File/Open Project", []() { DfxGlobal::command::exec("loadProject"); });
		DfxQMainMenu::getInstance()->addAction("File/", nullptr, true);
		DfxQMainMenu::getInstance()->addAction("File/Import/Import Obj", []() { DfxGlobal::command::exec("openObj"); });
		DfxQMainMenu::getInstance()->addAction("File/", nullptr, true);
		DfxQMainMenu::getInstance()->addAction("File/Save Project", []() { DfxGlobal::command::exec("saveProject"); });
		DfxQMainMenu::getInstance()->addAction("File/Save Project As", []() { DfxGlobal::command::exec("saveProjectAs"); });
		//DfxQMainMenu::getInstance()->addAction("File/", nullptr, true);
		//DfxQMainMenu::getInstance()->addAction("File/Package Scene", []() {});
		//DfxQMainMenu::getInstance()->addAction("File/Unpack Scene", []() {});
		//DfxQMainMenu::getInstance()->addAction("File/", nullptr, true);
		//DfxQMainMenu::getInstance()->addAction("File/Load Layout", []() {DfxGlobal::command::exec("loadLayout"); });
		DfxQMainMenu::getInstance()->addAction("File/", nullptr, true);
		DfxQMainMenu::getInstance()->addAction("File/Exit", []() {
			
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

		// Edit
		DfxQMainMenu::getInstance()->addAction("Edit/Undo", []() { DfxGlobal::command::exec("undo"); });
		DfxQMainMenu::getInstance()->addAction("Edit/Redo", []() { DfxGlobal::command::exec("redo"); });
		//DfxQMainMenu::getInstance()->addAction("Edit/", nullptr, true);
		//DfxQMainMenu::getInstance()->addAction("Edit/Cut", []() {});
		//DfxQMainMenu::getInstance()->addAction("Edit/Copy", []() {});
		//DfxQMainMenu::getInstance()->addAction("Edit/Paste", []() {});
		DfxQMainMenu::getInstance()->addAction("Edit/", nullptr, true);
		DfxQMainMenu::getInstance()->addAction("Edit/Hotkeys...", []() {DfxGlobal::command::exec("openHotkeyEditor"); });
		//DfxQMainMenu::getInstance()->addAction("Edit/", nullptr, true);
		///DfxQMainMenu::getInstance()->addAction("Edit/Preferences", []() {});

		// Windows
		for (auto p : DfxQViewMap::getInstance()->entries()) {
			std::string label = "Windows/" + p.first;
			QAction *daction = new QAction(label.c_str());

			DfxQMainMenu::getInstance()->addAction(label.c_str(), [label, p]() {		
				DfxQView *view = new DfxQView();
				view->setTitle(p.first.c_str());
				view->setCentralWidget(p.second.second());

				auto dialog = new DfxQDialogWindow(DfxQMainWindow::getInstance(), Qt::Window | Qt::Widget);
				dialog->setWindowTitle(p.first.c_str());
				dialog->setCentralWidget(view);
				dialog->show();
			});

			
		}

		// Help
		DfxQMainMenu::getInstance()->addAction("Help/View Online Help", []() { QDesktopServices::openUrl(QUrl("https://www.deepfxworld.com/docs")); });
		DfxQMainMenu::getInstance()->addAction("Help/Submit Bug Report", []() { QDesktopServices::openUrl(QUrl("https://www.deepfxworld.com/support")); });

		DfxQMainMenu::getInstance()->addAction("Help/About", []() {

			QDialog dialog(nullptr, Qt::WindowCloseButtonHint);

			QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
			QTextBrowser *browser = new QTextBrowser();

			QLabel *banner = new QLabel("");
			QPixmap img(":/misc/about_banner");
			banner->setPixmap(img);

			std::stringstream ss;
			ss << "<h4> Version: " << DfxVersionNameGlobal::versionNameString() << "</h4>" << std::endl;
			ss << "<h4> Deep Fx Copyright 2015-2021 All Rights Reserved" << "</h4>" << std::endl;
			ss << "<h3> System ID: <span style='color:green;'>" << DfxSystemIDGlobal::getSystemID()->getSystemID() << "</span></h3>" << std::endl;

			browser->setHtml(ss.str().c_str());

			mainLayout->setMargin(0);
			mainLayout->addWidget(banner);
			mainLayout->addWidget(browser);

			dialog.setWindowTitle("About");
			dialog.setMinimumWidth(400);

			dialog.setLayout(mainLayout);

			dialog.exec();
		
		});
	}
}