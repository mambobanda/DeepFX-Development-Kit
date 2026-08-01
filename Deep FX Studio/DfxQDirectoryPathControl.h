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

#include <QBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <qtguiglobal.h>
#include <DfxGlobal.h>
#include <DfxEntity.h>
#include <DfxEntityBasicPropertyTypes.h>
#include <limits>
#include "DfxQControlBase.h"

class DfxQDirectoryPathControl : public QWidget, public DfxQControlBase {
	Q_OBJECT
protected:

private:
	QLineEdit *path;
	QPushButton *selectPath;

	QBoxLayout *layout;
public:
	DfxQDirectoryPathControl(DfxEntity *ent, size_t propID, std::string initial_value = "C:/deepfx_content_dir", float width = 160.f, QWidget *parent = Q_NULLPTR);

public slots :
	void updateEntity();
	void updateEdited();
};