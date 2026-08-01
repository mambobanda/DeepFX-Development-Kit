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

#include <QAbstractItemModel>
#include <QPixMap>
#include "DfxLayer.h"

class DfxQLayerModel : public QAbstractItemModel {
private:
	DfxLayerItem *rootItem;
private:
	int PM_SIZE;
public:
	DfxQLayerModel(DfxLayerItem *rt);

	QVariant data(const QModelIndex &index, int role) const override;
	
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	
	QModelIndex parent(const QModelIndex &index) const override;
	
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	void beginAppendRowOp(DfxLayerItem *parent);

	void endAppendRowOp();

	void beginRemoveRowOp(DfxLayerItem *parent, size_t row);

	void endRemoveRowOp();

	void beginMoveRowOp(DfxLayerItem *parent, int row);

	void endMoveRowOp();

	void updateData(DfxLayerItem *item, int column);

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};