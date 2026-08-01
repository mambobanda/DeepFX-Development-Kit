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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <QBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QMenu>
#include <QSettings>
#include <QFileDialog.h>
#include "DfxCacheTrackManager.h"

class DfxQCacheTrackListItem: public QWidget {
	Q_OBJECT
private:
	size_t _trackIndex;

	QMenu *_menu;
	QLabel *_label;
	QCheckBox *_enabled;
	QPushButton *_options;

	QLayout *_layout;
public:

	DfxQCacheTrackListItem(int trackIndex = 0, QString color = "", QWidget *parent = nullptr);
	~DfxQCacheTrackListItem();

	void styleButtons();

};