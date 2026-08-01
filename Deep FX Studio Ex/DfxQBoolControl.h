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

#include <limits>
#include <QApplication>
#include <QComboBox>
#include <QBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <DfxGlobal.h>
#include "DfxBaseControl.h"
#include "DfxSpace.h"
#include "DfxAny.h"
#include "FlObserver.h"


class DfxQBoolControl : public DfxBaseControl {
private:

	class BoolObserver : public fl::Observer {
	public:
		QComboBox *combo;

		BoolObserver(QComboBox *);
		void update(fl::Any *any) override;
	};


private:
	std::string _fixedLabel;

	BoolObserver *_observer;
	QHBoxLayout *_layout;
	QLabel *_label;
	QComboBox *_combobox;
public:

	DfxQBoolControl(std::string flabel, QWidget *parent = nullptr);
	DfxQBoolControl(std::string vpath, std::string flabel, bool useLabel = true, QWidget *parent = nullptr);
	~DfxQBoolControl();

	QComboBox *comboBox();

	void addItem(int index, QString name);
	int selectedIndex();

	static void styleComboBox(QComboBox *_combobox);
public slots:
	void updatePath(const QString &text);
};