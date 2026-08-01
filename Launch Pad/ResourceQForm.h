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
#include <QBoxLayout>
#include <QFormLayout>
#include <QItemSelectionModel>
#include "Resource.h"
#include "FileQWidget.h"

class ResourceQForm : public QWidget {
	Q_OBJECT
private:
	QBoxLayout *_layout;
	QWidget *_currentSelection;
public:

	ResourceQForm();
	virtual ~ResourceQForm();

	QWidget *fileForm(Resource *resource);
	QWidget *rootForm(Resource *resource);

public slots:
	void resourceChanged(const QItemSelection &selected, const QItemSelection &deselected);
};