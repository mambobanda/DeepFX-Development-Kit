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

#include <QMimeData>
#include <QAbstractListModel>
#include <SgRootNode.h>
#include <QStringListModel> 
#include <DfxItemWidgetType.h>
#include <QPixmap>
#include "DfxMaterialMime.h"
#include "DfxSpace.h"

class DfxQMaterialListModel : public QAbstractListModel {
	Q_OBJECT
private:
	const QString MaterialMIMEType = "deepfx/material.bin";
public:
	DfxQMaterialListModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	bool setData(const QModelIndex &index, const QVariant &value, int role) override;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	void beginAppendRowOp(int row);

	void endAppendRowOp();

protected:
	Qt::ItemFlags flags(const QModelIndex &index) const override;

	QMimeData *mimeData(const QModelIndexList &indexes) const override;

	QStringList mimeTypes() const override;

	/*
	void beginRemoveRowOp(deepsg::SgNode *parent, size_t row);

	void endRemoveRowOp();*/
};