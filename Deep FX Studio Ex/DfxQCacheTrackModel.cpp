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


#include <DfxCacheTrackManager.h>
#include "DfxQCacheTrackModel.h"

int DfxQCacheTrackModel::rowCount(const QModelIndex &parent) const {
	return DfxCacheTrackManager::getInstance()->tracks().size();
}

QVariant DfxQCacheTrackModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();

	int ico_size = 18;

	if (role == Qt::DisplayRole) {
		return QVariant("Track");
	}

	return QVariant();
}

void DfxQCacheTrackModel::beginInsert() {

}

void DfxQCacheTrackModel::endInsert() {

}