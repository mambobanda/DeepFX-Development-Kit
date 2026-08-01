/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include <QAbstractItemModel>
#include "DfxGraphEditorChannel.h"

class DfxGraphEditorModel : public QAbstractItemModel {
	Q_OBJECT
private:

public:

	DfxGraphEditorModel();
	virtual ~DfxGraphEditorModel();

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

	QModelIndex parent(const QModelIndex &index) const override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;

	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	//Qt::ItemFlags flags(const QModelIndex &index) const override;

	void beginAppendRowOp(DfxGraphEditorChannel *parent);

	void endAppendRowOp();

	void beginRemoveRowOp(DfxGraphEditorChannel *parent);

	void endRemoveRowOp();

	void beginReset();

	void endReset();

	QModelIndex createExternalIndex(int row, int column, void *ptr = Q_NULLPTR);
};