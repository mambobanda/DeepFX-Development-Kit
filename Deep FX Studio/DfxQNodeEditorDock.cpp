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

#include "DfxQNodeEditorDock.h"
#include "DfxQNodeView.h"

DfxQNodeEditorDock::DfxQNodeEditorDock(QWidget *parent) : QDockWidget("Scene Graph", parent) {

	uint layersLevel = DfxLayerManager::getInstance()->getLevel();

	mainLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
	buttonLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	navLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	wrapper = new QWidget();
	wrapper->setLayout(mainLayout);

	mainLayout->setMargin(0);
	buttonLayout->setMargin(0);
	buttonLayout->setSpacing(0);

	//fileMenu = new QMenu("File");
	addMenu = new QMenu("Add");
	viewMenu = new QMenu("View");
	layoutMenu = new QMenu("Layout");

	DfxSceneGraphMenuBar menubarSetup;
	menubarSetup.createAddMenu(addMenu);

	/*
	fileMenuButton = new QPushButton("File");
	fileMenuButton->setFixedSize(40, 25);
	fileMenuButton->setFlat(true);
	fileMenuButton->setMenu(fileMenu);
	*/

	homeRootNode = new QPushButton(QIcon(":/layers_tb/home_level"), "");;
	previousRootNode = new QPushButton(QIcon(":/layers_tb/previous_level"), "");
	levelNodeDescription = new QLabel(QString("Level: ") + QString(std::to_string(layersLevel).c_str()));

	homeRootNode->setMaximumSize(30, 30);
	homeRootNode->setMinimumSize(30, 30);

	previousRootNode->setMaximumSize(30, 30);
	previousRootNode->setMinimumSize(30, 30);

	addMenuButton = new QPushButton("Add");
	addMenuButton->setFixedSize(50, 25);
	addMenuButton->setFlat(true);
	addMenuButton->setMenu(addMenu);

	viewMenuButton = new QPushButton("View");
	viewMenuButton->setFixedSize(50, 25);
	viewMenuButton->setFlat(true);
	viewMenuButton->setMenu(viewMenu);

	layoutMenuButton = new QPushButton("Layout");
	layoutMenuButton->setFixedSize(50, 25);
	layoutMenuButton->setFlat(true);
	layoutMenuButton->setMenu(layoutMenu);

	viewAllNodesAction = new QAction("View All Nodes");
	uniformLayoutAction = new QAction("Uniform Layout");

	viewMenu->addAction(viewAllNodesAction);

	layoutMenu->addAction(uniformLayoutAction);

	buttonLayout->setAlignment(Qt::AlignLeft);
	//buttonLayout->addWidget(fileMenuButton);
	buttonLayout->addWidget(addMenuButton);
	buttonLayout->addWidget(viewMenuButton);
	buttonLayout->addWidget(layoutMenuButton);
	
	// navLayout->addWidget(homeRootNode);
	navLayout->addWidget(previousRootNode);
	navLayout->addWidget(levelNodeDescription);

	view = new DfxQNodeView;
	this->setMinimumWidth(300);

	mainLayout->addLayout(buttonLayout);
#if	ENABLE_PROTOTYPE_FEATURES
	mainLayout->addLayout(navLayout);
#endif
	mainLayout->addWidget(view);

	setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea  );
	setWidget( wrapper );

	QObject::connect(viewAllNodesAction, &QAction::triggered, this, &DfxQNodeEditorDock::viewAllNodes);
	QObject::connect(uniformLayoutAction, &QAction::triggered, this, &DfxQNodeEditorDock::uniformLayout);
	QObject::connect(this, &QDockWidget::topLevelChanged, this, &DfxQNodeEditorDock::dockTopLevelChange);

	QObject::connect(previousRootNode, &QPushButton::clicked, view, &DfxQNodeView::previousLayer);
	QObject::connect(homeRootNode, &QPushButton::clicked, view, &DfxQNodeView::homeLayer);

	QObject::connect(DfxLayerManager::getInstance(), &DfxLayerManager::levelUpdated, this, &DfxQNodeEditorDock::updateLevelLabel);
}

DfxQNodeView *DfxQNodeEditorDock::getView() {
	return view;
}

QMenu *DfxQNodeEditorDock::getAddMenu() {
	return addMenu;
}

void DfxQNodeEditorDock::viewAllNodes() {
	view->fitAllNodes();
}

void DfxQNodeEditorDock::uniformLayout() {
	view->uniformLayout(false);
}

void DfxQNodeEditorDock::dockTopLevelChange(bool) {
	if (isFloating()) {
		setWindowFlags( Qt::CustomizeWindowHint | Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
		show();
	}
}

void DfxQNodeEditorDock::updateLevelLabel(unsigned int lvl) {

	uint layersLevel = DfxLayerManager::getInstance()->getLevel();
	levelNodeDescription->setText(QString("Level: ") + QString(std::to_string(layersLevel).c_str()));
}