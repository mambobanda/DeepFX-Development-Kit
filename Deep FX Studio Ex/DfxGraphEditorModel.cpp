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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxGraphEditorManager.h"
#include "DfxGraphEditorModel.h"


DfxGraphEditorModel::DfxGraphEditorModel() {

}

DfxGraphEditorModel::~DfxGraphEditorModel() {

}

QVariant DfxGraphEditorModel::data(const QModelIndex &index, int role) const {

	if (!index.isValid())
		return QVariant();

	int ico_size = 18;

	auto item = static_cast<DfxGraphEditorChannel*>(index.internalPointer());
	if (role == Qt::DisplayRole) {

		QString label = "N/A";

		if (index.column() == 0) {

			QString label = item->name().c_str();

			return QVariant(label);
		}
	}

	/*if (role == Qt::DecorationRole) {
		if (item->type() == Resource::ROOT) {
			return QPixmap(":/LaunchPad/root_icon").scaled(ico_size, ico_size);
		}
		else if (item->type() == Resource::DIR) {
			return QPixmap(":/LaunchPad/folder_icon").scaled(ico_size, ico_size);
		}
		else if (item->type() == Resource::FILE) {
			return QPixmap(":/LaunchPad/file_icon").scaled(ico_size, ico_size);
		}
	}*/

	return QVariant();
}

QModelIndex DfxGraphEditorModel::index(int row, int column, const QModelIndex &parent) const {
	DfxGraphEditorChannel *parentItem = nullptr;

	if (!parent.isValid())
		parentItem = DfxGraphEditorManager::getInstance()->rootChannel();
	else {
		parentItem = static_cast<DfxGraphEditorChannel*>(parent.internalPointer());
	}

	auto childItem = parentItem->findChannelByIndex(row);

	if (childItem != nullptr) {
		return createIndex(row, column, childItem);
	}
	else
		return QModelIndex();
}

QModelIndex DfxGraphEditorModel::parent(const QModelIndex &index) const {

	if (!index.isValid())
		return QModelIndex();

	auto childItem = static_cast<DfxGraphEditorChannel*>(index.internalPointer());
	auto parentItem = childItem->parent();

	if (parentItem == nullptr || parentItem->path() == DfxGraphEditorManager::getInstance()->rootChannel()->path())
		return QModelIndex();

	return createIndex(parentItem->relativeIndex(), 0, parentItem);

}

int DfxGraphEditorModel::rowCount(const QModelIndex &parent) const {

	DfxGraphEditorChannel  *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = DfxGraphEditorManager::getInstance()->rootChannel();
	else
		parentItem = static_cast<DfxGraphEditorChannel*>(parent.internalPointer());

	return parentItem->channelCount();

}

int DfxGraphEditorModel::columnCount(const QModelIndex &parent) const {
	return 1;
}

QVariant DfxGraphEditorModel::headerData(int section, Qt::Orientation orientation, int role ) const{
	if (section == 0) return QVariant(tr("Channel"));
	return QVariant();
}

//Qt::ItemFlags DfxGraphEditorModel::flags(const QModelIndex &index) {
//
//}

void DfxGraphEditorModel::beginAppendRowOp(DfxGraphEditorChannel *parent) {
	if (parent->parent() != nullptr) {
		int tail = parent->channelCount();
		beginInsertRows(createIndex(parent->relativeIndex(), 0, parent), tail, tail);
	}
	else {
		int tail = parent->channelCount();
		beginInsertRows(QModelIndex(), tail, tail);
	}
}

void DfxGraphEditorModel::endAppendRowOp() {
	endInsertRows();
}

void DfxGraphEditorModel::beginRemoveRowOp(DfxGraphEditorChannel *parent) {

}

void DfxGraphEditorModel::endRemoveRowOp() {
	endRemoveRows();
}

void DfxGraphEditorModel::beginReset() {
	beginResetModel();
}

void DfxGraphEditorModel::endReset() {
	endResetModel();
}

QModelIndex DfxGraphEditorModel::createExternalIndex(int row, int column, void *ptr) {
	return createIndex(row, column, ptr);
}