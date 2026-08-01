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

#pragma once

#include <iostream>
#include <QImage>
#include <QDialog>
#include <QPushButton.h>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QSpinbox>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QBoxLayout>
#include <qsettings.h>
#include "ModelFittingEOS.h"
#include "DfxImageViewer.h"

class Global: public QObject{
	Q_OBJECT
private:
	std::string _imagePath;
	QImage *_image;

	QLineEdit *currentSrc;
	QLineEdit *currentDest;
	QDoubleSpinBox *currentScale;
	QButtonGroup *currentButtonGroup;
private:
	static Global *instance;

	Global();
public:
	DfxImageViewer *imageView;

public:

	static Global *getInstance();
	QImage *getImage();
signals:
	void new_image();

public slots:
	void loadImage();
	void findLandmarks();
	void fitModel();
	void fitModelObj();
	void fitModelVideo();

	void getVideoPath();
	void getDestinationPath();
};
