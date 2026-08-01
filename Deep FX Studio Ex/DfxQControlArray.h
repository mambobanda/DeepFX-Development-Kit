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
#include <QLabel>
#include <QFormLayout>


class DfxQControlArray : public QWidget {
private:
	QFormLayout *_formLayout;
public:
	enum {INTEGER=0, DECIMAL};

	~DfxQControlArray();

	DfxQControlArray(QWidget *parent = nullptr);

	void addControl(std::string label, QWidget *control);

	void addControl(QWidget *control, std::string label);

	void addControl(QWidget *control);

	void addSpacer(int width, int height);
};
