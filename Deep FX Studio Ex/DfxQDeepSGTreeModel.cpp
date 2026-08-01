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

#include "DfxQDeepSGTreeModel.h"
#include "DfxObjectHandler.h"
#include "DfxPointCloudHandler.h"
#include "DfxLxCameraHandler.h"


DfxQDeepSGTreeModel::DfxQDeepSGTreeModel() {

}

QVariant DfxQDeepSGTreeModel::data(const QModelIndex &index, int role ) const {
	return DfxQDeepSGTreeModelCommon::data(index, role);
}

bool DfxQDeepSGTreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (index.isValid()) {
		if (role == Qt::EditRole) {
			if (index.column() == 0) {
				deepsg::SgNode *item = static_cast<deepsg::SgNode*>(index.internalPointer());
				auto val = value.toString().toStdString();
				
				if (item->getType() != deepsg::SgNodeType::ROOT) {
					auto any = DfxSpace::getInstance()->findAbsolute(item->referencePath());

					if (any != nullptr && !val.empty()) {
						any->setLabel(val);
						return true;
					}
				}
			}
		}
	}
	return false;
}

QModelIndex DfxQDeepSGTreeModel::index(int row, int column, const QModelIndex &parent) const {
	deepsg::SgNode *childItem = DfxQDeepSGTreeModelCommon::chilAtIndex(row, parent);

	if (childItem != nullptr) {
		return createIndex(row, column, childItem);
	}
	else
		return QModelIndex();
}

QModelIndex DfxQDeepSGTreeModel::parent(const QModelIndex &index) const {

	deepsg::SgNode *parentItem = DfxQDeepSGTreeModelCommon::childParent(index);

	if (parentItem == nullptr)
		return QModelIndex();
	else 
		return createIndex(parentItem->relativeIndex(), 0, parentItem);

	return createIndex(parentItem->relativeIndex(), 0, parentItem);
}

int DfxQDeepSGTreeModel::rowCount(const QModelIndex &parent) const {
	return DfxQDeepSGTreeModelCommon::rowCount(parent);
}

int DfxQDeepSGTreeModel::columnCount(const QModelIndex &parent ) const {
	return 3;
}

//bool DfxQDeepSGTreeModel::removeRows(int row, int count, const QModelIndex & parent) {
//	deepsg::SgNode  *parentItem;
//	if (!parent.isValid())
//		parentItem = deepsg::SgRootNode::getInstance();
//	else
//		parentItem = static_cast<deepsg::SgNode*>(parent.internalPointer());
//
//	if (parentItem == nullptr) return false;
//
//	if (count <= 0) return false;
//
//	beginRemoveRows(parent, row, row);
//	auto child = parentItem->childAt(row);
//	parentItem->removeChild(child);
//	endRemoveRows();
//
//	return true;
//}

QStringList DfxQDeepSGTreeModel::mimeTypes() const {

	QStringList types;
	types << SgNodeMIMEType;
	return types;

}

QMimeData *DfxQDeepSGTreeModel::mimeData(const QModelIndexList &indexes) const {

	// make sure there's exactly 1 index and it's valid
	if (indexes.size() != 1 || !indexes[0].isValid())
		return nullptr;

	auto node = static_cast<deepsg::SgNode*>(indexes[0].internalPointer());

	if (node->getType() == deepsg::SgNodeType::STAGE_ACTOR_COLLECTION ||
		node->getType() == deepsg::SgNodeType::STAGE_PROP_COLLECTION  ||
		node->getType() == deepsg::SgNodeType::STAGE_ACTOR ||
		node->getType() == deepsg::SgNodeType::STAGE_PROP ) {

		return nullptr;
	}

	// get the Node pointer from the index's internal pointer, convert it to quintptr, then to a UTF string
	quintptr nodeAddress = (quintptr)indexes[0].internalPointer();

	QByteArray encodedData(QString::number(nodeAddress).toUtf8());

	// allocate mimeData, provide it with encodedData, and return it
	QMimeData *mimeData = new QMimeData();
	mimeData->setData(SgNodeMIMEType, encodedData);

	return mimeData;
}

bool DfxQDeepSGTreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) {
	deepsg::SgNode *parentItem;

	if (!data || action == Qt::IgnoreAction)
		return false;

	if (!parent.isValid())
		return false;
	else
		parentItem = static_cast<deepsg::SgNode*>(parent.internalPointer());

	if(row == -1) 
		row = parentItem->size();
	
	deepsg::SgNode *parentNode = static_cast<deepsg::SgNode*>(parent.internalPointer());

	if (!parentNode) return false;
	
	// get the encoded data of our Node pointer
	QByteArray encodedData = data->data(SgNodeMIMEType);
	deepsg::SgNode *node = (deepsg::SgNode*)encodedData.toULongLong();
	if (!node) return false;

	if (parentNode->getType() == deepsg::SgNodeType::STAGE) {
		DfxGlobal::console::println("Stage parenting not allowed.");
		return false;
	}
	else if (parentNode->getType() == deepsg::SgNodeType::STAGE_ACTOR_COLLECTION){
		auto actorCollectionNode = (deepsg::SgStageActorCollectionNode*)parentNode;
		auto stage = (DfxSimulationStageBase*)actorCollectionNode->stage();

		fl::Any *any = new fl::Any;
		any->newData<deepsg::SgNode>(DfxFluentTypes::SG_NODE, node);

		stage->addActor(any);

		any->setExplicitDelete(false);
		delete any;

		return false;
	}
	else if (parentNode->getType() == deepsg::SgNodeType::STAGE_PROP_COLLECTION) {
		
		auto propCollectionNode = (deepsg::SgStagePropCollectionNode*)parentNode;
		auto stage = (DfxSimulationStageBase*)propCollectionNode->stage();

		fl::Any *any = new fl::Any;
		any->newData<deepsg::SgNode>(DfxFluentTypes::SG_NODE, node);

		stage->addProp(any);

		any->setExplicitDelete(false);
		delete any;

		return false;
	}

	deepsg::SgNode *sourceParentNode = node->parent();
	QModelIndex sourceParent = createIndex(sourceParentNode->relativeIndex(), 0, sourceParentNode);

	moveRow(sourceParent, node->relativeIndex(), parent, row);

	return false;
}

QVariant DfxQDeepSGTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role == Qt::DisplayRole) {
		if (section == 0)
			return QVariant(tr("Name"));
		else if (section == 1)
			return QVariant(tr(""));
		else if (section == 2)
			return QVariant(tr(""));
	}
	
	return QVariant();
}

//bool DfxQDeepSGTreeModel::insertRows(int row, int count, const QModelIndex &parent) {
//	deepsg::SgNode  *parentItem;
//	if (!parent.isValid())
//		parentItem = deepsg::SgRootNode::getInstance();
//	else
//		parentItem = static_cast<deepsg::SgNode*>(parent.internalPointer());
//
//	/*if (parentItem == nullptr) return false;
//
//	if (count <= 0) return false;
//
//	int tail = parentItem->children().size();
//	beginInsertRows(createIndex(parentItem->relativeIndex(), 0, parentItem), tail, tail);
//
//	endInsertRows();*/
//
//	return false;
//}

bool DfxQDeepSGTreeModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) {

	deepsg::SgNode *sourceParentNode = static_cast<deepsg::SgNode*>(sourceParent.internalPointer());
	deepsg::SgNode *destinationParentNode = static_cast<deepsg::SgNode*>(destinationParent.internalPointer());
	deepsg::SgNode *childNode = sourceParentNode->childAt(sourceRow);

	if (sourceParentNode->getID() != destinationParentNode->getID()) {
		beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild);
		if(childNode->parent() != nullptr) childNode->parent()->removeChild(childNode);
		destinationParentNode->addChild(childNode);
		endMoveRows();
	}
	else {
		beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, destinationChild);
		
		if ((childNode->relativeIndex() + 1) < destinationParentNode->children().size()) {

			beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, destinationChild);

			if (destinationChild < sourceRow) {
				destinationParentNode->moveChild(sourceRow, destinationChild);
			}
			else {
				destinationParentNode->moveChild(sourceRow, destinationChild - 1);
			}

			endMoveRows();
		}
		
	}

	return true;
}

Qt::ItemFlags DfxQDeepSGTreeModel::flags(const QModelIndex &index) const {

	//Qt::ItemFlags defaultFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

	if (index.column() == 0) {
		if (index.isValid())
			return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | defaultFlags;
		else
			return Qt::ItemIsDropEnabled | defaultFlags;
	}
	else {
		
		auto _flags = Qt::ItemIsDropEnabled;
		return _flags;
		
	}
	
}

Qt::DropActions DfxQDeepSGTreeModel::supportedDropActions() const {
	return Qt::MoveAction;
}

void  DfxQDeepSGTreeModel::beginAppendRowOp(deepsg::SgNode *parent) {

	if (parent->parent() != nullptr) {
		int tail = parent->children().size();
		beginInsertRows(createIndex(parent->relativeIndex(), 0, parent), tail, tail);
	}
	else {
		int tail = parent->children().size();
		beginInsertRows(QModelIndex(), tail, tail);
	}
}

void  DfxQDeepSGTreeModel::endAppendRowOp() {
	endInsertRows();
}

void DfxQDeepSGTreeModel::beginMoveRowOp(deepsg::SgNode *parent, int row) {

	QModelIndex root = createIndex(0, 0, deepsg::SgRootNode::getInstance()->root());
	beginMoveRows(QModelIndex(), parent->relativeIndex(), parent->relativeIndex(), QModelIndex(), row);
}

void DfxQDeepSGTreeModel::endMoveRowOp() {
	endMoveRows();
}

void DfxQDeepSGTreeModel::beginRemoveRowOp(deepsg::SgNode *parent, size_t row) {

	if (parent->parent() != nullptr) {
		beginRemoveRows(createIndex(parent->relativeIndex(), 0, parent), row, row);
	}
	else {
		int tail = parent->children().size();
		beginRemoveRows(QModelIndex(), row, row);
	}

}

void DfxQDeepSGTreeModel::endRemoveRowOp() {
	endRemoveRows();
}

QModelIndex DfxQDeepSGTreeModel::createExternalIndex(int row, int column, void *ptr) {
	return createIndex(row, column, ptr);
}

void DfxQDeepSGTreeModel::toggleNodeVisiblity(deepsg::SgNode *node) {
	if (node->isVisible()) node->setIsVisible(false);
	else node->setIsVisible(true);

	auto mindex = createExternalIndex(node->relativeIndex(), 1, node);
	emit dataChanged(mindex, mindex);

	DfxGlobal::command::exec("updateScene");
}

void DfxQDeepSGTreeModel::toggleNodeRenderable(deepsg::SgNode *node) {
	if (node->isRenderable()) node->setIsRenderable(false);
	else node->setIsRenderable(true);

	auto mindex = createExternalIndex(node->relativeIndex(), 2, node);
	emit dataChanged(mindex, mindex);
}