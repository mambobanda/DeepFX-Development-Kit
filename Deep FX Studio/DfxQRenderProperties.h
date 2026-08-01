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

#include <QDialog>
#include <QListWidget>
#include <QBoxLayout>
#include <QSpinbox>
#include <QFormLayout>
#include <QComboBox>
#include <QMainWindow>
#include <QImage>
#include <QColor>
#include <QPushButton>
#include <QDockWidget>
#include "DfxRenderViewer.h"
#include "DfxQKeymapEditor.h"

class DfxQRenderProperties : public QWidget {
	Q_OBJECT
private:

	QScrollArea *scrollArea;
	QBoxLayout *mainLayout;
	QBoxLayout *groupLayout;

	QGroupBox *renderer;
	QWidget	  *renRefresh;
	QBoxLayout *rendererLayout;
	QFormLayout *rendererForm;
	QComboBox *rendererList;
	QPushButton *refresh;

	QGroupBox *resolution;
	QFormLayout *resolutionForm;
	QSpinBox *width;
	QSpinBox *height;

	QGroupBox *frames;
	QFormLayout *framesForm;
	QSpinBox *startFrame;
	QSpinBox *endFrame;

	QBoxLayout *renderButtonLayout;
	QPushButton *renderFrame;
	QPushButton *renderAnimation;

	DfxRenderViewer renderViewer;
private:

	void saveImage(std::string path, int width, int height, unsigned char *data, unsigned int size);
public:

	DfxQRenderProperties(QWidget *parent = nullptr);
	~DfxQRenderProperties();

public slots:
	void refreshRendererList();
	void renderSlot();
	void renderAnimationSlot();
};