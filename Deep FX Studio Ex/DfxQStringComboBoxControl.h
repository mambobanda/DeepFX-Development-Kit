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

#pragma once

#include <limits>
#include <QApplication>
#include <QComboBox>
#include <QBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <DfxGlobal.h>
#include <QStackedWidget>
#include <QListView>
#include "DfxQStyleScrollBar.h"
#include "DfxBaseControl.h"
#include "DfxSpace.h"
#include "DfxAny.h"
#include "FlObserver.h"


class DfxQStringComboBoxControl : public DfxBaseControl {
private:

	class StringObserver : public fl::Observer {
	public:
		DfxQStringComboBoxControl *combo;

		StringObserver(DfxQStringComboBoxControl *, std::vector<std::string> _enumList);
		void update(fl::Any *any) override;
	};

	std::vector<std::string> _enumList;
	std::unordered_map<std::string, unsigned int> _enumerators;
private:
	std::string _fixedLabel;

	StringObserver *_observer;
	QHBoxLayout *_layout;
	QLabel *_label;
	QComboBox *_combobox;

public:

	DfxQStringComboBoxControl(std::string flabel, QWidget *parent = nullptr);
	DfxQStringComboBoxControl(std::string vpath, std::string flabel, std::vector<std::string> enums, bool useLabel = true, QWidget *parent = nullptr);
	~DfxQStringComboBoxControl();

	QComboBox *comboBox();

	void addItem(QString name);
	void selectByEnum(QString txt);
	unsigned int selectionEnum(QString str);
	int selectedIndex();

	static void styleComboBox(QComboBox *_combobox);

public slots:
	void updatePath(const QString &text);
	void updateStackedWidget(int index);
};