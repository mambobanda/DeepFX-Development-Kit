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

#include "ResourceQModel.h"
#include "ResourceManager.h"

ResourceQModel::ResourceQModel() {

}

QVariant ResourceQModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();

	int ico_size = 18;

	auto item = static_cast<Resource*>(index.internalPointer());
	if (role == Qt::DisplayRole) {

		QString label = "N/A";

		if (index.column() == 0) {

			QString label = item->name().c_str();

			return QVariant(label);
		}
	}

	if (role == Qt::DecorationRole) {
		if (item->type() == Resource::ROOT) {
			return QPixmap(":/LaunchPad/root_icon").scaled(ico_size, ico_size);
		}
		else if (item->type() == Resource::DIR) {
			return QPixmap(":/LaunchPad/folder_icon").scaled(ico_size, ico_size);
		}
		else if (item->type() == Resource::FILE) {
			return QPixmap(":/LaunchPad/file_icon").scaled(ico_size, ico_size);
		}
	}

	return QVariant();
}

bool ResourceQModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (index.isValid()) {
		if (role == Qt::EditRole) {
			if (index.column() == 0) {
				auto item = static_cast<Resource*>(index.internalPointer());
				auto val = value.toString().toStdString();

				item->setName(val);
				return true;
					
			}
		}
	}
	return false;
}

Qt::ItemFlags ResourceQModel::flags(const QModelIndex &index) const {
	Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

	if (index.isValid()) {
		if (index.column() == 0) {
			auto item = static_cast<Resource*>(index.internalPointer());
			if(item->type() != Resource::ROOT) if (index.isValid()) return Qt::ItemIsEditable | defaultFlags;
		}
	}
	
	return defaultFlags;
}

QModelIndex ResourceQModel::index(int row, int column, const QModelIndex &parent) const {
	Resource *parentItem = nullptr;

	if (!parent.isValid())
		parentItem = ResourceManager::getInstance()->root();
	else
		parentItem = static_cast<Resource*>(parent.internalPointer());

	auto childItem = parentItem->findChildByIndex(row);

	if (childItem != nullptr) {
		return createIndex(row, column, childItem);;
	}
	else
		return QModelIndex();
}

QModelIndex ResourceQModel::parent(const QModelIndex &index) const {

	if (!index.isValid())
		return QModelIndex();

	auto childItem = static_cast<Resource*>(index.internalPointer());
	auto parentItem = childItem->parent();

	if (parentItem == nullptr || parentItem->id() == ResourceManager::getInstance()->root()->id())
		return QModelIndex();

	return createIndex(parentItem->count(), 0, parentItem);
}

int ResourceQModel::rowCount(const QModelIndex &parent) const {
	Resource  *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = ResourceManager::getInstance()->root();
	else
		parentItem = static_cast<Resource*>(parent.internalPointer());

	return parentItem->children().size();
}

int ResourceQModel::columnCount(const QModelIndex &parent) const {
	return 1;
}

QVariant ResourceQModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role == Qt::DisplayRole) {
		if (section == 0)
			return QVariant(tr(""));
		else if (section == 1)
			return QVariant(tr(""));
		else if (section == 2)
			return QVariant(tr(""));
	}

	return QVariant();
}

void ResourceQModel::beginReset() {
	beginResetModel();
}

void ResourceQModel::endReset() {
	endResetModel();
}

void  ResourceQModel::beginAppendRowOp(Resource *parent) {

	if (parent->parent() != nullptr) {
		int tail = parent->children().size();
		auto rc = parent->count();
		beginInsertRows(createIndex(parent->count(), 0, parent), tail, tail);
	}
	else {
		int tail = parent->children().size();
		beginInsertRows(QModelIndex(), tail, tail);
	}
}

void  ResourceQModel::endAppendRowOp() {
	endInsertRows();
}

void ResourceQModel::endRemoveRowOp() {
	endRemoveRows();
}

QModelIndex ResourceQModel::createExternalIndex(int row, int column, void *ptr) {
	return createIndex(row, column, ptr);
}