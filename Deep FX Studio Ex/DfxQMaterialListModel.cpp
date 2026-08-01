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

#include "DfxQMaterialListModel.h"

DfxQMaterialListModel::DfxQMaterialListModel() {

}

int DfxQMaterialListModel::rowCount(const QModelIndex &parent) const {

	auto space = DfxSpace::getInstance();

	auto any = space->findAbsolute("~/MATS");

	if (any != nullptr) {
		return any->data<fl::DataObject>().properties().size();
	}

	return 0;
}

bool DfxQMaterialListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (index.isValid()) {
		if (role == Qt::EditRole) {
			if (index.column() == 0) {
				if (!index.isValid())
					return false;

				auto space = DfxSpace::getInstance();

				auto any = space->findAbsolute("~/MATS");

				if (any == nullptr) return false;

				if (index.row() >= any->data<fl::DataObject>().properties().size()) return false;

				auto val = value.toString().toStdString();

				any->data<fl::DataObject>().properties()[index.row()]->setLabel(val);
			}
		}
	}
	return false;
}

QVariant DfxQMaterialListModel::data(const QModelIndex &index, int role) const  {

	if (!index.isValid())
		return QVariant();

	auto space = DfxSpace::getInstance();

	auto any = space->findAbsolute("~/MATS");

	if (any == nullptr) return QVariant();

	if (index.row() >= any->data<fl::DataObject>().properties().size())
		return QVariant();

	if (role == Qt::DisplayRole)
		return QString(any->data<fl::DataObject>().properties()[index.row()]->label().c_str());
	else if(role == Qt::DecorationRole)
		return QPixmap(":/icon/material_icon").scaled(13, 13);
	else
		return QVariant();
}

QVariant DfxQMaterialListModel::headerData(int section, Qt::Orientation orientation, int role) const  {
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal)
		return QString("Material");

	return QVariant();
}

void DfxQMaterialListModel::beginAppendRowOp(int tail) {
	beginInsertRows(QModelIndex(), tail, tail);
}

void  DfxQMaterialListModel::endAppendRowOp() {
	endInsertRows();
}

Qt::ItemFlags DfxQMaterialListModel::flags(const QModelIndex &index) const {
	Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

	if (index.isValid())
		return Qt::ItemIsDragEnabled | defaultFlags | Qt::ItemIsEditable;
	else
		return Qt::ItemIsDropEnabled | defaultFlags;
}

QStringList DfxQMaterialListModel::mimeTypes() const {
	QStringList types;
	types << MaterialMIMEType;
	return types;
}

QMimeData *DfxQMaterialListModel::mimeData(const QModelIndexList &indexes) const {
	auto space = DfxSpace::getInstance();

	if (indexes.size() > 0) {
		auto any = space->findAbsolute("~/MATS");

		if (any != nullptr) {

			auto path = any->data<fl::DataObject>().properties()[indexes[0].row()]->absolutePath();
			auto *mimeData = new DfxMaterialMime();
			mimeData->setPath(path);
			mimeData->setData(MaterialMIMEType, QByteArray());

			return mimeData;
		}
	}

	return nullptr;
}