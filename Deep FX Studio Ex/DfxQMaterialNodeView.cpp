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

#include "DfxQMaterialNodeView.h"

DfxQMaterialNodeView::DfxQMaterialNodeView() {
	
	linkTemp.source = nullptr;
	linkTemp.destination = nullptr;
	linkTemp.link = nullptr;

	disconnectionLink = nullptr;

	style::styleQScrollBar(horizontalScrollBar());
	style::styleQScrollBar(verticalScrollBar());
}

DfxQMaterialNodeView::~DfxQMaterialNodeView() {

}

void DfxQMaterialNodeView::wheelEvent(QWheelEvent *event) {
	if (items().size() > 0) {

		double scaleFactor = 1.15;

		setTransformationAnchor(AnchorUnderMouse);
		if (event->delta() > 0) {
			scale(scaleFactor, scaleFactor);
		}
		else {
			scale(1.0f / scaleFactor, 1.0f / scaleFactor);
		}
	}

}

QGraphicsItem* DfxQMaterialNodeView::itemAt(const QPointF &pos) {

	if (scene() != nullptr) {
		QList<QGraphicsItem*> items = scene()->items(QRectF(pos - QPointF(1, 1), QSize(3, 3)));

		for (auto item : items) {
			if (item->type() > QGraphicsItem::UserType)
				return item;
		}
	}

	return nullptr;
}

void DfxQMaterialNodeView::notifyIfNodesAreSelected() {
	std::vector<DfxQMaterialBaseNode*> selectedNodes;

	auto items = scene()->selectedItems();
	if (scene() != nullptr) {
		for (auto item : scene()->selectedItems()) {
			if (item->type() == DfxQMaterialNodeEnums::BASE_NODE) {
				selectedNodes.push_back((DfxQMaterialBaseNode*)item);
			}
		}
	}

	emit nodesSelected(selectedNodes);
}

bool DfxQMaterialNodeView::eventFilter(QObject *o, QEvent *e) {
	QGraphicsSceneMouseEvent *me = (QGraphicsSceneMouseEvent*)e;
	QKeyEvent *ke = (QKeyEvent*)e;

	switch ((int)e->type())
	{
	case QEvent::GraphicsSceneMousePress: 
	{
		switch ((int)me->button())
		{
		case Qt::LeftButton:
		{
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item && item->type() == DfxQMaterialNodeEnums::PROPERTY_SLOT) {
				auto slot = (DfxQMaterialNodeSlot*)item;

				if (slot->mode() == DfxQMaterialNodeSlot::SlotMode::INPUT) {
					
					if (slot->hasLinks()) {
						disconnectionLink = slot->links()[0];
						disconnectionLinkPos = me->scenePos();
					}
					
				}
				else if (slot->mode() == DfxQMaterialNodeSlot::SlotMode::OUTPUT) {
					linkTemp.source = slot;
					linkTemp.link = new DfxQMaterialPropertyLink;

					scene()->addItem(linkTemp.link);
					linkTemp.link->updatePosition(linkTemp.source->scenePos(), me->scenePos());
				}
			}
			else {
				if (dragMode() != ScrollHandDrag) {
					setDragMode(RubberBandDrag);
				}
			}
		}
		break;
		case Qt::MiddleButton:
		{
			QGraphicsItem *item = itemAt(me->scenePos());
			if (!item) {
				setDragMode(ScrollHandDrag);
				QMouseEvent evt(me->type(), mapFromGlobal(QCursor::pos()), Qt::LeftButton, Qt::LeftButton, me->modifiers());
				QGraphicsView::mousePressEvent(&evt);
			}
		}
		};
	}
	break;
	case QEvent::GraphicsSceneMouseMove:
	{
		if (linkTemp.link != nullptr) {
			linkTemp.link->updatePosition(linkTemp.source->scenePos(), me->scenePos());
		}
		else if (disconnectionLink != nullptr) {
			qreal dis = QLineF(disconnectionLinkPos, me->scenePos()).length();

			if (dis >= 10) {
				std::vector<DfxQMaterialPropertyLink*> links = {disconnectionLink};
				deleteLinks(links);
				disconnectionLink = nullptr;
			}
		}
	}
	break;
	case QEvent::GraphicsSceneMouseRelease:
	{
		if (me->button() == Qt::LeftButton) {
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item && item->type() == DfxQMaterialNodeEnums::PROPERTY_SLOT) {
				auto space = DfxSpace::getInstance();
				auto slot = (DfxQMaterialNodeSlot*)item;

				if (slot->mode() == DfxQMaterialNodeSlot::SlotMode::INPUT && linkTemp.link != nullptr) {
					deleteLinks(slot->links());

					linkTemp.link->setSlot00(linkTemp.source);
					linkTemp.link->setSlot01(slot);

					auto src_prop = (DfxQMaterialNodeProperty*)linkTemp.source->parentItem();
					auto dest_prop = (DfxQMaterialNodeProperty*)slot->parentItem();
					auto srcNode = (DfxQMaterialNode*)src_prop->parentItem();
					auto destNode = (DfxQMaterialNode*)dest_prop->parentItem();				

					DfxSpace::getInstance()->addBinding(
						linkTemp.source->propertyPath(),
						srcNode->dataObjectPath(),
						slot->propertyPath(),
						destNode->dataObjectPath());

					linkTemp.link->updatePosition();

					linkTemp.link = nullptr;
					linkTemp.source = nullptr;
					linkTemp.destination = nullptr;

					DfxUpdater::getInstance()->materialUpdated();
				}
			}

			if (linkTemp.link != nullptr) {
				scene()->removeItem(linkTemp.link);
				delete linkTemp.link;

				linkTemp.link = nullptr;
				linkTemp.source = nullptr;
				linkTemp.destination = nullptr;
			}

			if (disconnectionLink != nullptr) {
				disconnectionLink = nullptr;
			}

		}

		notifyIfNodesAreSelected();
		setDragMode(NoDrag);
	}
	break;
	case QEvent::KeyPress:
	{
		if (!ke->isAutoRepeat()) {
			if (ke->key() == Qt::Key_Delete) {
				deleteSelected();
			}
		}
	}
	};

	return QObject::eventFilter(o, e);
}

void DfxQMaterialNodeView::deleteLinks(std::vector<DfxQMaterialPropertyLink*> links) {
	auto space = DfxSpace::getInstance();
	bool updateMaterials = false;

	for (auto &lk : links) {

		DfxSpace::getInstance()->removeBinding(lk->slot00()->propertyPath(), lk->slot01()->propertyPath());
		scene()->removeItem(lk);
		delete lk;

		updateMaterials = true;
	}

	if (updateMaterials) DfxUpdater::getInstance()->materialUpdated();
}

void DfxQMaterialNodeView::deleteSelected() {
	auto space = DfxSpace::getInstance();

	std::vector<DfxQMaterialPropertyLink*> links;
	std::vector<DfxQMaterialNode*> nodes;

	for (auto item : scene()->items()) {
		if (item->type() == DfxQMaterialNodeEnums::BASE_NODE) {
			if (item->isSelected()) {
				nodes.push_back((DfxQMaterialNode*)item);
			}
		}
	}

	for (auto item : scene()->items()) {
		if (item->type() == DfxQMaterialNodeEnums::LINK) {
			if (item->isSelected()) {
				links.push_back((DfxQMaterialPropertyLink*)item);
			}
		}
	}

	deleteLinks(links);

	for (auto nd : nodes) {
		auto any = space->findAbsolute(nd->dataObjectPath());
		
		if (any != nullptr) {

			if (any->type() == fl::type::DATA_OBJECT) {
				if (any->rawData<fl::DataObject>()->isProtected())
					continue;
			}

			std::vector<DfxQMaterialPropertyLink*> nlinks;

			for (auto slt : nd->properties())
				for (auto lk : slt->slot()->links()) nlinks.push_back(lk);

			deleteLinks(nlinks);

			space->remove(any);
			((DfxQMaterialNodeCollection*)scene())->removeNode(nd);
			delete nd;
		}
	}
}

void DfxQMaterialNodeView::viewFitAll() {
	if (scene() != nullptr) {
		QRectF bounds = scene()->itemsBoundingRect();
		bounds.setWidth(bounds.width()*0.9);         // to tighten-up margins
		bounds.setHeight(bounds.height()*0.9);       // same as above
		fitInView(bounds, Qt::KeepAspectRatio);
		centerOn(0, 0);
	}
}