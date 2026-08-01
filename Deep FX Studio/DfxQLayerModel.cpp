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

#include "DfxQLayerModel.h"


DfxQLayerModel::DfxQLayerModel(DfxLayerItem *rt) {
	rootItem = rt;
	PM_SIZE = 15;
}

QVariant DfxQLayerModel::data(const QModelIndex &index, int role) const {

	if (!index.isValid())
		return QVariant();

	if (role == Qt::DisplayRole) {

		DfxLayerItem *item = static_cast<DfxLayerItem*>(index.internalPointer());

		if (index.column() == 0) {
			return QVariant(tr(item->getName().c_str()));
		}
		else {
			return QVariant();
		}
	}

	if (role == Qt::DecorationRole) {
		DfxLayerItem *item = static_cast<DfxLayerItem*>(index.internalPointer());

		if (index.column() == 0) {
			
			if (item->getItemType() == DfxLayerItemType::LAYER_ITEM) {
				return (QPixmap(":/layers_tb/layer_icon").scaled(QSize(PM_SIZE,PM_SIZE)));
			}
			else if (item->getItemType() == DfxLayerItemType::NODE_ITEM) {

				if (item->getNode()->getNodeType() == DfxQNodeType::IMPORT_NODE) {
					return (QPixmap(":/layers_tb/import_node_icon_png").scaled(QSize(PM_SIZE, PM_SIZE)));
				}
				else if (item->getNodeType() == DfxLayerNodeType::GENERIC_NODE) {
					return (QPixmap(":/layers_tb/layer_generic_node_icon").scaled(QSize(PM_SIZE, PM_SIZE)));
				}
				else if (item->getNodeType() == DfxLayerNodeType::VIEW_NODE) {
					return (QPixmap(":/layers_tb/layer_viewitem_icon").scaled(QSize(PM_SIZE, PM_SIZE)));
				}
			}
		}

		/*if (index.column() == 1) {
			if (item->getItemType() == DfxLayerItemType::LAYER_ITEM) {
				if (item->isLinked())
					return (QPixmap(":/layers_tb/layer_link_icon").scaled(QSize(PM_SIZE, PM_SIZE)));
				else
					return (QPixmap(":/layers_tb/layer_off_icon").scaled(QSize(PM_SIZE - 10, PM_SIZE - 10)));
			}
		}*/
		if (index.column() == 1) {
			if(item->isVisible())
				return (QPixmap(":/layers_tb/layer_visibility_icon").scaled(QSize(PM_SIZE, PM_SIZE)));
			else
				return (QPixmap(":/layers_tb/layer_off_icon").scaled(QSize(PM_SIZE - 10, PM_SIZE - 10)));
		}
		/*else if (index.column() == 3) {
			if(item->isLocked())
				return (QPixmap(":/layers_tb/layer_lock_icon").scaled(QSize(PM_SIZE, PM_SIZE)));
			else
				return (QPixmap(":/layers_tb/layer_off_icon").scaled(QSize(PM_SIZE - 10, PM_SIZE - 10)));
		}*/
	}

	return QVariant();
}

QModelIndex DfxQLayerModel::index(int row, int column, const QModelIndex &parent) const {

	if (!hasIndex(row, column, parent))
		return QModelIndex();

	DfxLayerItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<DfxLayerItem*>(parent.internalPointer());


	DfxLayerItem *childItem = parentItem->getChild(row);

	if (childItem != nullptr)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex DfxQLayerModel::parent(const QModelIndex &index) const {

	if (!index.isValid())
		return QModelIndex();

	DfxLayerItem *childItem = static_cast<DfxLayerItem *>(index.internalPointer());
	DfxLayerItem *parentItem = childItem->getParent();


	if (parentItem->getID() == rootItem->getID())
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int DfxQLayerModel::rowCount(const QModelIndex &parent) const {
	DfxLayerItem *parentItem;

	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<DfxLayerItem*>(parent.internalPointer());

	return parentItem->childCount();
}

int DfxQLayerModel::columnCount(const QModelIndex &parent) const {
	return 2;
}

void  DfxQLayerModel::beginAppendRowOp(DfxLayerItem *parent) {

	if (parent->getParent() != nullptr) {
		int tail = parent->childCount();
		beginInsertRows(createIndex(parent->row(), 0, parent), tail, tail);
	}
	else {
		int tail = parent->childCount();
		beginInsertRows(QModelIndex(), tail, tail);
	}
}

void  DfxQLayerModel::endAppendRowOp() {
	endInsertRows();
}

void DfxQLayerModel::updateData(DfxLayerItem *item, int column) {

	QModelIndex index = createIndex(item->row(), column, item);
	emit dataChanged(index, index);
}

QVariant DfxQLayerModel::headerData(int section, Qt::Orientation orientation, int role) const {

	/*if (role == Qt::DisplayRole) {
		if (section == 0)
			return QVariant(tr("Description"));
		else if (section == 1)
			return QVariant(tr("Link"));
		else if (section == 2)
			return QVariant(tr("Hide"));
		else if (section == 3)
			return QVariant(tr("Lock"));
	}*/

	if (role == Qt::DisplayRole) {
		if (section == 0)
			return QVariant(tr("Description"));
		else if (section == 1)
			return QVariant(tr("Hide"));
	}
		
	return QVariant();
}

void DfxQLayerModel::beginMoveRowOp(DfxLayerItem *parent, int row) {
	
	QModelIndex root = createIndex(0, 0, rootItem);
	beginMoveRows(QModelIndex(), parent->row(), parent->row(), QModelIndex(), row);
}

void DfxQLayerModel::endMoveRowOp() {
	endMoveRows();
}

void DfxQLayerModel::beginRemoveRowOp(DfxLayerItem *parent, size_t row) {

	if (parent->getParent() != nullptr) {
		beginRemoveRows(createIndex(parent->row(), 0, parent), row, row);
	}
	else {
		int tail = parent->childCount();
		beginRemoveRows(QModelIndex(), row, row);
	}

}

void DfxQLayerModel::endRemoveRowOp() {
	endRemoveRows();
}