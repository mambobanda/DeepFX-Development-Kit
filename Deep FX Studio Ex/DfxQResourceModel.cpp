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

#include "DfxQResourceModel.h"
#include "DfxResourceManager.h"

DfxQResourceModel::DfxQResourceModel() {

}

DfxQResourceModel::~DfxQResourceModel() {

}

QVariant DfxQResourceModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();

	DfxResource *item = static_cast<DfxResource*>(index.internalPointer());

	if (role == Qt::DisplayRole) {
		if (index.column() == 0) {
			return item->name().c_str();
		}
	}
	else if (role == Qt::DecorationRole) {

		if (index.column() == 0) {

			if (item->mode() == resource::mode::FOLDER) {
				return QPixmap(":/icon/folder_icon").scaled(15, 15);
			}
		}
	}

	return QVariant();
}

Qt::ItemFlags DfxQResourceModel::flags(const QModelIndex &index) const {
	if (!index.isValid())
		return 0;

	return QAbstractItemModel::flags(index);
}

QVariant DfxQResourceModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return "";

	return QVariant();
}

QModelIndex DfxQResourceModel::index(int row, int column, const QModelIndex &parent) const {
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	DfxResource *parentItem;

	if (!parent.isValid())
		parentItem = DfxResourceManager::getInstance()->root()->parent();
	else
		parentItem = static_cast<DfxResource*>(parent.internalPointer());

	DfxResource *childItem = parentItem->getChild(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex DfxQResourceModel::parent(const QModelIndex &index) const {
	if (!index.isValid())
		return QModelIndex();

	DfxResource *childItem = static_cast<DfxResource*>(index.internalPointer());
	DfxResource *parentItem = childItem->parent();

	if (parentItem->id() == DfxResourceManager::getInstance()->root()->parent()->id())
		return QModelIndex();

	return createIndex(parentItem->count(), 0, parentItem);
}

int DfxQResourceModel::rowCount(const QModelIndex &parent) const {
	DfxResource *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = DfxResourceManager::getInstance()->root()->parent();
	else
		parentItem = static_cast<DfxResource*>(parent.internalPointer());

	return parentItem->childCount();
}

int DfxQResourceModel::columnCount(const QModelIndex &parent) const {
	return 1;
}

QModelIndex DfxQResourceModel::createExternalIndex(int row, int column, void *ptr) {
	return createIndex(row, column, ptr);
}