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

#include <QLineEdit>
#include <qtguiglobal.h>
#include <DfxGlobal.h>
#include <DfxEntityBasicPropertyTypes.h>
#include "DfxQControlBase.h"
#include "DfxEntity.h"


class DfxQStringControl : public QLineEdit, public DfxQControlBase {
	Q_OBJECT
public:
	DfxQStringControl(DfxEntity *ent, size_t propID, std::string initialVal = "N/A", float width = 160.f,
		QWidget *parent = Q_NULLPTR);

	public slots :
	void updateEntity();
};
