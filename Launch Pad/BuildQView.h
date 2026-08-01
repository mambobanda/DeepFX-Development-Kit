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
#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>
#include "ResourceManager.h"
#include "FileQWidget.h"

class BuildQView: public QWidget {
private:
	QBoxLayout *layout;
	QFormLayout *formLayout;
	QLineEdit *name;
	FileQWidget *buildDir;
	FileQWidget *binaryCreator;

	QSpinBox *major;
	QSpinBox *minor;
	QSpinBox *patch;
	
	QPushButton *build;
public:

	BuildQView();
	virtual ~BuildQView();

public slots:
	void startBuild();

	void updateName();
	void updateBuildDir();
	void updateBinaryCreator();
	void updateMajor(int);
	void updateMinor(int);
	void updatePatch(int);
	void updateBuild();
};