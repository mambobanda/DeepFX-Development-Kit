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
#include <QBoxLayout>
#include <QCheckbox>
#include <QEvent>
#include <QMouseEvent>
#include <DfxGlobal.h>
#include <sstream>
#include <QColorDialog>
#include "DfxQStyleScrollBar.h"
#include "DfxBaseControl.h"
#include "DfxSpace.h"
#include "DfxAny.h"
#include "FlObserver.h"


class DfxQCheckBoxControl : public DfxBaseControl {
private:

	class BoolObserver : public fl::Observer {
	public:
		QCheckBox *checkbox;

		BoolObserver(QCheckBox *);
		void update(fl::Any *any) override;
	};

private:

	BoolObserver *_observer;

	QCheckBox *_checkbox;

	QHBoxLayout *_layout;

	std::string _fixedLabel;

	static void styleCheckbox(QCheckBox *spinner);
public:
	DfxQCheckBoxControl(std::string vpath);
	DfxQCheckBoxControl(std::string vpath, std::string flabel = "", bool useLabel = true, QWidget *parent = nullptr);
	~DfxQCheckBoxControl();

	
public slots:

	void updatePath(int state);
};