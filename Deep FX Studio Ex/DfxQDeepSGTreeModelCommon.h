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
#include <QAbstractItemModel>
#include <SgRootNode.h>
#include <QStringListModel> 
#include <DfxItemWidgetType.h>
#include <SgStagePropCollectionNode.h>
#include <SgStageActorCollectionNode.h>
#include "DfxFluentTypes.h"
#include "DfxSimulationStageBase.h"
#include "DfxSimulationStageActorBase.h"
#include "DfxSimulationStagePropBase.h"

class DfxQDeepSGTreeModelCommon {
public:

	static QVariant data(const QModelIndex &index, int role = Qt::DisplayRole);

	static deepsg::SgNode *chilAtIndex(int row, const QModelIndex &parent = QModelIndex());

	static deepsg::SgNode *childParent(const QModelIndex &index);

	static int rowCount(const QModelIndex &parent = QModelIndex());

	static int columnCount(const QModelIndex &parent = QModelIndex());
};