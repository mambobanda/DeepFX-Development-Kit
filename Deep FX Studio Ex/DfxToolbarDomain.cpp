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

#include "DfxToolMap.h"
#include "DfxToolbarDomain.h"
#include "DfxToolbarMenu.h"


DfxToolbarDomain::DfxToolbarDomain() {
	setTabPosition(QTabWidget::East);
	styleTabWidget(this);
	
	for (auto domain : DfxToolMap::getInstance()->domains()) {

		for (auto section : domain->getChildren()) {

			DfxToolbarPage *page = new DfxToolbarPage;

			for (auto group : section->getChildren()) {

				DfxToolbarGroup *_group = new DfxToolbarGroup(group->getName().c_str());

				for (auto item : group->getChildren()) {

					if (item->getType() == DfxToolMapNode::Type::ITEM) {
						DfxToolbarItem *titem = new DfxToolbarItem;
						titem->setText(item->getName().c_str());

						QObject::connect(titem, &DfxToolbarItem::clicked, [item]() { DfxGlobal::command::exec(item->getCommand()); });

						_group->addItem(titem);
					}
					else if (item->getType() == DfxToolMapNode::Type::MENU) {
						DfxToolbarMenu *menu = new DfxToolbarMenu;
						menu->setText(item->getName().c_str());

						for (auto mitem : item->getChildren()) {
							if (mitem->getType() == DfxToolMapNode::Type::MENU_ITEM) {
								QAction *maction = new QAction(mitem->getName().c_str());

								QObject::connect(maction, &QAction::triggered, [mitem]() { DfxGlobal::command::exec(mitem->getCommand()); });

								menu->addItem(maction);
							}
						}

						_group->addMenu(menu);
					}
				}

				page->addGroup(_group);
			}

			addTab(page, section->getName().c_str());
		}
	}
}

void DfxToolbarDomain::styleTabWidget(QTabWidget *twidget) {

	QString qpane = 
		"QTabWidget::pane {"
		"background: rgb(25,25,25);"
		"border: 0px;" 
        "}";

	QString tab =
		"QTabBar::tab{"
		"background: rgb(25,25,25);"
		"color: white;"
		"border-top: 1px Solid rgb(50,50,50);"
		"padding: 5px;"
		"border-radius: 2px;"
		"}";

	QString tabSelecteced =
		"QTabBar::tab:selected{"
		"background:  rgb(91,162,213);"
		"}";

	QString tabNotSelecteced =
		"QTabBar::tab:!selected{"
		"margin-right: 2px;"
		"}";

	twidget->setStyleSheet(qpane + tab + tabSelecteced + tabNotSelecteced);
}