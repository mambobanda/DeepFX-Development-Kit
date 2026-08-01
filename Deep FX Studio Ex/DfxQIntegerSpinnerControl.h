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
#include <QSpinBox>
#include <QBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <DfxGlobal.h>
#include "DfxBaseControl.h"
#include "DfxSpace.h"
#include "DfxAny.h"
#include "FlObserver.h"


class DfxQIntegerSpinnerControl : public DfxBaseControl {
private:

	class IntegerObserver : public fl::Observer {
	public:
		QSpinBox *spinner;
		unsigned int _type;

		IntegerObserver(QSpinBox *, unsigned int type = INT);
		void update(fl::Any *any) override;
	};

	class CustomSpinner : public QSpinBox {
	private:
		bool mousePressed;
		int lastX;
	protected:
		/*QString textFromValue(double value) const override;
		double  valueFromText(const QString &text) const override;*/

		void stepBy(int steps) override;
	public:
		CustomSpinner(QWidget *parent = nullptr);

	protected:
		void mousePressEvent(QMouseEvent *event) override;
		void mouseReleaseEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;
	};

private:
	std::string _fixedLabel;

	unsigned int _type;

	IntegerObserver *_observer;
	QHBoxLayout *_layout;
	QLabel *_label;
	CustomSpinner *_spinner;

	int _maxValue;
	int _minValue;

	void styleSpinner(QSpinBox *spinner);
public:

	enum {INT=0, UNSIGNED_INT=1, SIZE_T=2};

	DfxQIntegerSpinnerControl(std::string vpath, std::string flabel = "", bool useLabel = true, QWidget *parent = nullptr, unsigned int type = INT);
	~DfxQIntegerSpinnerControl();

	int maxValue();
	void setMaxValue(int mx);

	int minValue();
	void setMinValue(int min);

	void setValue(int value);
public slots:
	void updatePath();
};