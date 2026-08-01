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

#include <QMimeData>
#include <QAbstractItemModel>
#include <SgRootNode.h>
#include <QStringListModel> 
#include <DfxItemWidgetType.h>
#include <SgStagePropCollectionNode.h>
#include <SgStageActorCollectionNode.h>
#include "DfxQDeepSGTreeModelCommon.h"
#include "DfxFluentTypes.h"
#include "DfxSimulationStageBase.h"
#include "DfxSimulationStageActorBase.h"
#include "DfxSimulationStagePropBase.h"


class DfxQDeepSGTreeModel: public QAbstractItemModel {
	Q_OBJECT
private:
	const QString SgNodeMIMEType = "application/sgnode.bin";
public:
	DfxQDeepSGTreeModel();

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

	QModelIndex parent(const QModelIndex &index) const override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;

	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	//bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex()) override;

	//bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

	bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;

	QStringList mimeTypes() const override;

	QMimeData *mimeData(const QModelIndexList &indexes) const override;

	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

	Qt::ItemFlags flags(const QModelIndex &index) const override;

	Qt::DropActions DfxQDeepSGTreeModel::supportedDropActions() const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	void beginAppendRowOp(deepsg::SgNode *parent);

	void endAppendRowOp();

	void beginRemoveRowOp(deepsg::SgNode *parent, size_t row);

	void endRemoveRowOp();

	void beginMoveRowOp(deepsg::SgNode *parent, int row);

	void endMoveRowOp();

	void toggleNodeVisiblity(deepsg::SgNode *node);

	void toggleNodeRenderable(deepsg::SgNode *node);

	QModelIndex createExternalIndex(int row, int column, void *ptr = Q_NULLPTR);

public slots:

	//void updateAll();
};