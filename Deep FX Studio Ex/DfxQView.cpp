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

#include <QApplication>
#include "DfxQViewManager.h"
#include "DfxQView.h"
#include "DfxQViewMap.h"
#include "DfxGlobal.h"


DfxQView::Tab::Tab(QWidget *parent): QFrame(parent) {

	menu   = new QPushButton(QPixmap(":/icon/view_menu"), "");
	minmax = new QPushButton(QPixmap(":/icon/view_minmax"), "");
	close  = new QPushButton(QPixmap(":/icon/view_close"), "");

	title = new QLabel("< View Title >", this);
	layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight, this);
	layout->setMargin(3);

	QFont font("Arial", 7.5, QFont::Bold);
	title->setFont(font);
	title->setMargin(0);

	layout->addWidget(title);
	layout->addWidget(menu);
	layout->addWidget(minmax);
	layout->addWidget(close);

	layout->setSpacing(2);

	menu->setFixedSize(QSize(14, 14));
	menu->setIconSize(QSize(6, 6));
	minmax->setFixedSize(QSize(14, 14));
	minmax->setIconSize(QSize(6, 6));
	close->setFixedSize(QSize(14, 14));
	close->setIconSize(QSize(6, 6));	

	setFixedHeight(18);
	setLayout(layout);

}

DfxQView::Tab::~Tab() {

}

void DfxQView::Tab::setTitle(QString ttl) {
	title->setText(ttl);
}

DfxQView::DfxQView() {

	setStyleSheet("background-color: rgb(25,25,25)");

	maximised = false;

	container = nullptr;
	centralWidget = nullptr;

	mainLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);

	headerLayout	  = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	headerLayoutLeft  = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	headerLayoutRight = new QBoxLayout(QBoxLayout::Direction::LeftToRight);

	viewTab = new Tab(this);
	constructViewMenu(viewTab);

	headerLayoutLeft->setAlignment(Qt::AlignLeft);
	headerLayoutRight->setAlignment(Qt::AlignRight);

	mainLayout->setMargin(0);
	headerLayout->setMargin(0);
	headerLayoutLeft->setMargin(0);
	headerLayoutRight->setMargin(0);

	mainLayout->addWidget(viewTab);
	mainLayout->setSpacing(0);

	setLayout(mainLayout);

	QObject::connect(viewTab->close, &QPushButton::clicked, this, &DfxQView::closeView);
	QObject::connect(viewTab->minmax, &QPushButton::clicked, this, &DfxQView::minMaxView);
}

DfxQView::~DfxQView() {
	// pass
	int i = 0;
}

void DfxQView::setTitle(QString str) {
	viewTab->setTitle(str);
}

QPushButton *DfxQView::getViewMenuButton() {
	return viewTab->menu;
}

QPushButton *DfxQView::getViewMinMaxButton() {
	return viewTab->minmax;
}

QPushButton *DfxQView::getViewCloseButton() {
	return viewTab->close;
}

DfxQViewContainer *DfxQView::getContainer() {
	return container;
}

void DfxQView::setContainer(DfxQViewContainer *cnt) {
	container = cnt;
}

void DfxQView::clearContainer() {
	container = nullptr;
}

QWidget *DfxQView::getCentralWidget() {
	return centralWidget;
}

void DfxQView::setCentralWidget(QWidget *ctlWidget) {
	mainLayout->addWidget(ctlWidget);
	centralWidget = ctlWidget;
}

void DfxQView::removeCentralWidget() {
	mainLayout->removeWidget(centralWidget);
}

bool DfxQView::isViewMaximised() {
	return maximised;
}

void DfxQView::setViewIsMaximised(bool max) {
	maximised = max;
}

void DfxQView::constructViewMenu(Tab *tab) {

	QString  menuStyle(
		"QMenu::item{"
		"color: rgb(255, 255, 255);"
		"}"

		"QMenu::item:selected{"
		"background-color: rgb(255, 127, 42);"
		"color: rgb(0, 0, 0);"
		"}"
	);

	QMenu *mainMenu = new QMenu(this);
	mainMenu->setStyleSheet(menuStyle);

	QMenu *switchViewMenu = new QMenu("Switch View");
	QMenu *leftRightMenu = new QMenu("Split View Left/Right");
	QMenu *topBottomMenu = new QMenu("Split View Top/Bottom");

	mainMenu->addMenu(switchViewMenu);
	mainMenu->addMenu(leftRightMenu);
	mainMenu->addMenu(topBottomMenu);

	for (auto p : DfxQViewMap::getInstance()->entries()) {
		QAction *leftRightAction = new QAction(p.first.c_str());
		QAction *topBottomAction = new QAction(p.first.c_str());

		QObject::connect(leftRightAction, &QAction::triggered, [this, p]() {
			DfxQView *view = new DfxQView();
			view->setTitle(p.first.c_str());
			view->setCentralWidget(p.second.second());

			DfxQViewManager::getInstance()->splitViewContainer(Qt::Orientation::Horizontal, getContainer(), view, p.second.first, QSize(100, 100));
			
			QApplication::processEvents();
			DfxGlobal::command::exec("updateScene");
		});

		QObject::connect(topBottomAction, &QAction::triggered, [this, p]() {
			DfxQView *view = new DfxQView();
			view->setTitle(p.first.c_str());
			view->setCentralWidget(p.second.second());

			DfxQViewManager::getInstance()->splitViewContainer(Qt::Orientation::Vertical, getContainer(), view, p.second.first, QSize(100, 100));

			QApplication::processEvents();
			DfxGlobal::command::exec("updateScene");
		});

		leftRightMenu->addAction(leftRightAction);
		topBottomMenu->addAction(topBottomAction);
	}

	for (auto p : DfxQViewMap::getInstance()->entries()) {
		auto saction = new QAction(p.first.c_str());

		QObject::connect(saction, &QAction::triggered, [this, p]() {
			auto xcontainer = getContainer();
			auto fview = xcontainer->getView();
			xcontainer->removeView(fview);
			xcontainer->setViewID(DfxQViewID::NONE);

			DfxQView *view = new DfxQView();
			view->setTitle(p.first.c_str());
			view->setCentralWidget(p.second.second());

			xcontainer->setView(view);
			xcontainer->setViewID(p.second.first);

			fview->deleteLater();
		});

		switchViewMenu->addAction(saction);
	}

	tab->menu->setMenu(mainMenu);
}

// Slots /////////////////////////////////////////
void DfxQView::closeView(bool) {
	DfxQViewManager::getInstance()->merge(this);

	QApplication::processEvents();
	DfxGlobal::command::exec("updateScene");
}

void DfxQView::minMaxView(bool bl) {
	DfxQViewManager::getInstance()->minMaxView(this);

	QApplication::processEvents();
	DfxGlobal::command::exec("updateScene");
}

