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

#include <DfxQCacheTrackListItem.h>

DfxQCacheTrackListItem::DfxQCacheTrackListItem(int trackIndex, QString color, QWidget *parent) : QWidget(parent) {
	_trackIndex = trackIndex;

	int icon_btn_sz = 28;
	int icon_sz = 18;

	_menu = new QMenu();
	_label = new QLabel(QString("Track ") + std::to_string(trackIndex).c_str());
	_enabled = new QCheckBox();
	_options = new QPushButton("");

	_layout = new QHBoxLayout;
	_layout->setMargin(0);
	_layout->setSpacing(0);

	_enabled->setFixedWidth(15);
	_enabled->setFixedHeight(17);
	_label->setFixedHeight(17);
	_label->setAlignment(Qt::AlignCenter);

	_layout->addWidget(_label);
	_layout->addWidget(_enabled);
	_layout->addWidget(_options);

	_menu->addAction("Load", [this]() { 
		QSettings settings;
		QString initialDir = settings.value("io/existingCachePath", QDir::homePath()).toString();

		auto fileName = QFileDialog::getOpenFileName(nullptr, tr("Cache"), initialDir, tr("Cache Files (*.flcache)"));

		if (!fileName.isEmpty()) {
			auto track = DfxCacheTrackManager::getInstance()->tracks()[_trackIndex];
			track->setPath(fileName.toStdString());
			settings.setValue("io/existingCachePath", fileName);
		}
		
	});

	_menu->addAction("Clear", [this]() {
		auto track = DfxCacheTrackManager::getInstance()->tracks()[_trackIndex];
		track->clearAll();
	});

	_menu->addAction("Remove", [this]() {
		if (_trackIndex > 0) {
			DfxCacheTrackManager::getInstance()->deleteTrack(_trackIndex);
		}
		else {
			DfxGlobal::console::println("The root track is immutable.");
		}
	});

	_options->setMenu(_menu);
	_options->setFixedWidth(11);

	setStyleSheet(QString("background-color: ") + color + ";");

	styleButtons();

	if (DfxCacheTrackManager::getInstance()->tracks()[trackIndex]->isActive()) _enabled->setChecked(true);

	setLayout(_layout);
}

DfxQCacheTrackListItem::~DfxQCacheTrackListItem() { }

void DfxQCacheTrackListItem::styleButtons() {

	QString hoverSty =
		"QPushButton:hover {"
		"background-color: rgb(100,100,100);"
		"}";

	QString hoverPressdSty =
		"QPushButton:hover:pressed {"
		"background-color: rgb(255, 153, 83);"
		"}";

	QString keyMenuButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border-top-right-radius: 4px;"
		"border-bottom-right-radius: 4px;"
		"width: 11px;"
		"height: 16px;"
		"}";

	_enabled->setStyleSheet(_enabled->styleSheet() +
		"QCheckBox::indicator {"
		"background-color: rgb(20,20,20);"
		"border: 0px solid;"
		"border-color: rgb(50,50,50);"
		"border-width: 2px;"
		"border-style: solid;"
		"border-radius: 5px;"
		"width: 10px;"
		"height: 10px;"
		"}");

	_enabled->setStyleSheet(_enabled->styleSheet() +
		"QCheckBox::indicator:checked {"
		"image: url(:/icon/check);"
		"}");

	_options->setStyleSheet(keyMenuButtonSty + hoverSty + hoverPressdSty);

}