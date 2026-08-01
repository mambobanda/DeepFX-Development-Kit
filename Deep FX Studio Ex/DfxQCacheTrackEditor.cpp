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
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxQCacheTrackEditor.h"

DfxQCacheTrackEditor::DfxQCacheTrackEditor(QWidget *parent) :QWidget(parent) {
	
	mainWidget = new QWidget;
	scrollArea = new QScrollArea;
	trackLayout = new QVBoxLayout;
	newTrack = new QPushButton(QPixmap(":/icon/add_layout_layout").scaledToHeight(8), "");

	mainLayout = new QVBoxLayout(this);
	mainLayout->setAlignment(Qt::AlignTop);
	mainLayout->setContentsMargins(0, 5, 0 ,0 );

	trackLayout->setAlignment(Qt::AlignTop);

	style::styleQScrollBar(scrollArea->horizontalScrollBar());
	style::styleQScrollBar(scrollArea->verticalScrollBar());

	scrollArea->setWidgetResizable(true);

	newTrack->setToolTip("New Track");

	mainWidget->setLayout(trackLayout);

	scrollArea->setWidget(mainWidget);

	mainLayout->addWidget(newTrack);
	mainLayout->addWidget(scrollArea);

	styleButtons();

	setLayout(mainLayout);

	resetList();

	QObject::connect(newTrack, &QPushButton::clicked, this, &DfxQCacheTrackEditor::addTrack);
	QObject::connect(DfxCacheTrackManager::getInstance(), &DfxCacheTrackManager::newTrackInserted, this, &DfxQCacheTrackEditor::resetList);
	QObject::connect(DfxCacheTrackManager::getInstance(), &DfxCacheTrackManager::trackRemoved, this, &DfxQCacheTrackEditor::resetList);

}

DfxQCacheTrackEditor::~DfxQCacheTrackEditor() {

}

void DfxQCacheTrackEditor::styleButtons() {
		QString ButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"padding-left: 2px;"
		"height: 16px;"
		"}";

	QString hoverSty =
		"QPushButton:hover { "
		"background-color: rgb(100,100,100);"
		"}";

	QString hoverPressdSty =
		"QPushButton:hover:pressed { "
		"background-color: rgb(255, 153, 83);"
		"}";

	newTrack->setStyleSheet(ButtonSty + hoverSty + hoverPressdSty);
}

void DfxQCacheTrackEditor::resetList() {
	clearList();

	for (size_t i = 0; i < DfxCacheTrackManager::getInstance()->tracks().size(); i++) {
		QString color;
		if (i == 0 || (i % 2) == 0) {
			color = "rgb(53, 53, 53)";
		}
		else {
			color = "rgb(35, 35, 35)";
		}

		auto item = new DfxQCacheTrackListItem(i, color);
		trackLayout->addWidget(item);
	}

}

void DfxQCacheTrackEditor::clearList() {
	std::vector<QWidget*> items;

	for (int i = 0; i < trackLayout->count(); i++) {
		auto litem = trackLayout->itemAt(i)->widget();
		if (litem != nullptr) items.push_back(litem);
	}

	for (auto item : items) {
		trackLayout->removeWidget(item);
		item->setParent(nullptr);
		delete item;
	}
}

void DfxQCacheTrackEditor::addTrack() {
	auto track = new DfxCacheTrack;
	DfxCacheTrackManager::getInstance()->addTrack(track);
}