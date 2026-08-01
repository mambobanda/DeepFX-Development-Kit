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

#include "DfxQRenderProperties.h"

class DfxRenderPropertiesDock : public QDockWidget {
	Q_OBJECT
private:
	DfxQRenderProperties *view;

public:
	DfxRenderPropertiesDock(QWidget *parent = Q_NULLPTR);

	DfxQRenderProperties *getView();
};