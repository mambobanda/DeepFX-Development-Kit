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
#include <QStackedWidget>
#include <QListView>
#include "DfxBaseControl.h"
#include "DfxQStyleScrollBar.h"
#include "DfxSpace.h"
#include "DfxAny.h"
#include "FlObserver.h"


class DfxQComboBoxControl : public DfxBaseControl {
private:

	class IntegerObserver : public fl::Observer {
	public:
		QComboBox *combo;
		std::vector<std::pair<std::string, unsigned int>> enumList;

		IntegerObserver(QComboBox *, std::vector<std::pair<std::string, unsigned int>> _enumList);
		void update(fl::Any *any) override;
	};

	std::vector<std::pair<std::string, unsigned int>> _enumList;
	std::unordered_map<std::string, unsigned int> _enumerators;
private:
	std::string _fixedLabel;

	IntegerObserver *_observer;
	QHBoxLayout *_layout;
	QLabel *_label;
	QComboBox *_combobox;

public:

	DfxQComboBoxControl(std::string flabel, QWidget *parent = nullptr);
	DfxQComboBoxControl(std::string vpath, std::string flabel, std::vector<std::pair<std::string, unsigned int>> enums, bool useLabel = true, QWidget *parent = nullptr);
	~DfxQComboBoxControl();

	QComboBox *comboBox();

	void addItem(int index, QString name);
	void selectByEnum(unsigned int);
	static void selectByEnum(unsigned int, QComboBox *, std::vector<std::pair<std::string, unsigned int>> &enums);
	unsigned int selectionEnum(QString str);
	int selectedIndex();

	static void styleComboBox(QComboBox *_combobox);


public slots:
	void updatePath(const QString &text);
	void updateStackedWidget(int index);
};