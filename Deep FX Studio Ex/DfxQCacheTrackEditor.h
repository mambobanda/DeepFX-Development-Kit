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

#include <QWidget>
#include <QListView>
#include <QBoxLayout>
#include <DfxGlobal.h>
#include <DfxQStyleScrollBar.h>
#include <QScrollArea>
#include "DfxCacheTrackManager.h"
#include "DfxQCacheTrackListItem.h"

class DfxQCacheTrackEditor : public QWidget {
	Q_OBJECT
private:
	QWidget *mainWidget;
	QScrollArea *scrollArea;
	QVBoxLayout *trackLayout;
	QVBoxLayout *mainLayout;

	QPushButton *newTrack;

	void styleButtons();
public:

	DfxQCacheTrackEditor(QWidget *parent = Q_NULLPTR);
	virtual ~DfxQCacheTrackEditor();

	void clearList();
public slots:
	void resetList();
	void addTrack();
};