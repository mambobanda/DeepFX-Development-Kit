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


class DfxQDeepSGSelectionTreeModel : public QAbstractItemModel {
	Q_OBJECT
private:
	const QString SgNodeMIMEType = "application/sgnode.bin";
public:
	DfxQDeepSGSelectionTreeModel();

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

	QModelIndex parent(const QModelIndex &index) const override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;

	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	QModelIndex createExternalIndex(int row, int column, void *ptr = Q_NULLPTR);

public slots:

	//void updateAll();
};