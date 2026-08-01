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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxQDeepSGSelectionTreeModel.h"
#include "DfxObjectHandler.h"
#include "DfxPointCloudHandler.h"
#include "DfxLxCameraHandler.h"


DfxQDeepSGSelectionTreeModel::DfxQDeepSGSelectionTreeModel() {

}

QVariant DfxQDeepSGSelectionTreeModel::data(const QModelIndex &index, int role) const {
	return DfxQDeepSGTreeModelCommon::data(index, role);
}

QModelIndex DfxQDeepSGSelectionTreeModel::index(int row, int column, const QModelIndex &parent) const {
	deepsg::SgNode *childItem = DfxQDeepSGTreeModelCommon::chilAtIndex(row, parent);

	if (childItem != nullptr) {
		return createIndex(row, column, childItem);
	}
	else
		return QModelIndex();
}

QModelIndex DfxQDeepSGSelectionTreeModel::parent(const QModelIndex &index) const {

	deepsg::SgNode *parentItem = DfxQDeepSGTreeModelCommon::childParent(index);

	if (parentItem == nullptr)
		return QModelIndex();
	else
		return createIndex(parentItem->relativeIndex(), 0, parentItem);

	return createIndex(parentItem->relativeIndex(), 0, parentItem);
}

int DfxQDeepSGSelectionTreeModel::rowCount(const QModelIndex &parent) const {
	return DfxQDeepSGTreeModelCommon::rowCount(parent);
}

int DfxQDeepSGSelectionTreeModel::columnCount(const QModelIndex &parent) const {
	return 1;
}

QVariant DfxQDeepSGSelectionTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {

	if (role == Qt::DisplayRole) {
		if (section == 0)
			return QVariant(tr("Name"));
		else if (section == 1)
			return QVariant(tr("Visibility"));
	}

	return QVariant();
}

QModelIndex DfxQDeepSGSelectionTreeModel::createExternalIndex(int row, int column, void *ptr) {
	return createIndex(row, column, ptr);
}