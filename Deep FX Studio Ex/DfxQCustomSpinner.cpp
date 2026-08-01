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

#include "DfxQCustomSpinner.h"

CustomSpinner::CustomSpinner(QWidget *parent) :QDoubleSpinBox(parent) {
	mousePressed = false;
	lastX = 0;
}

QString CustomSpinner::textFromValue(double value) const {
	std::stringstream ss;
	ss << value;

	return QString(ss.str().c_str());
}

double CustomSpinner::valueFromText(const QString &text) const {
	return text.toDouble();
}

void CustomSpinner::stepBy(int steps) {
	setValue(value() + (singleStep() * steps));
	emit editingFinished();
}

void CustomSpinner::mousePressEvent(QMouseEvent *event) {
	QApplication::setOverrideCursor(Qt::BlankCursor);
	mousePressed = true;

	lastX = event->pos().x();
}

void CustomSpinner::mouseReleaseEvent(QMouseEvent *event) {
	QApplication::setOverrideCursor(Qt::ArrowCursor);
	mousePressed = false;
}

void CustomSpinner::mouseMoveEvent(QMouseEvent *event) {
	if (mousePressed) {
		int offX = (event->pos().x() - lastX);

		setValue(value() + (singleStep() * offX));

		lastX = event->pos().x();

		emit editingFinished();
	}
}