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


#include "DfxImageViewer.h"

DfxImageViewer::DfxImageViewer(QWidget *wgt) : QWidget(wgt) {

	imageLabel = new QLabel;
	scrollArea = new QScrollArea();

	layout = new QBoxLayout(QBoxLayout::Direction::Down);
	layout->setMargin(0);

	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);

	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(imageLabel);
	scrollArea->setVisible(true);
	
	layout->addWidget(scrollArea);

	setLayout(layout);
}

void DfxImageViewer::setImage(QImage &image) {
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->resize(image.size());
}