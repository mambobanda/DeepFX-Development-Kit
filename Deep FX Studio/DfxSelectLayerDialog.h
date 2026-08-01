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
#include <QListview>
#include <QStandardItemModel>
#include <QBoxLayout>
#include <functional>
#include <QPushButton>
#include "DfxLayerManager.h"

using LayerSelectDialogCallback = std::function<void(DfxLayerItem *)>;

class DfxSelectLayerDialog : public QDialog {
private:
	QBoxLayout *boxLayout;
	QBoxLayout *buttonLayout;
	QListView *list;
	QStandardItemModel *model;
	QPushButton *ok;
	QPushButton *cancel;

	LayerSelectDialogCallback callback;
	DfxLayerItem *result;
public:
	
	DfxSelectLayerDialog(LayerSelectDialogCallback, DfxLayerItem *excludeitem = nullptr);
};