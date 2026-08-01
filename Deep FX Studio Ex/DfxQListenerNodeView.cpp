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

#include "DfxQListenerNodeView.h"

DfxQListenerNodeView::DfxQListenerNodeView() {
	style::styleQScrollBar(horizontalScrollBar());
	style::styleQScrollBar(verticalScrollBar());

	
}

DfxQListenerNodeView::~DfxQListenerNodeView() {

}

void DfxQListenerNodeView::wheelEvent(QWheelEvent *event) {
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

QGraphicsItem* DfxQListenerNodeView::itemAt(const QPointF &pos) {

	if (scene() != nullptr) {
		QList<QGraphicsItem*> items = scene()->items(QRectF(pos - QPointF(1, 1), QSize(3, 3)));

		for (auto item : items) {
			if (item->type() > QGraphicsItem::UserType)
				return item;
		}
	}

	return nullptr;
}


bool DfxQListenerNodeView::eventFilter(QObject *o, QEvent *e) {
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
			if (item == nullptr) {
				if (dragMode() != ScrollHandDrag) {
					setDragMode(RubberBandDrag);
				}
			}

		}
		break;
		case Qt::MiddleButton:
		{
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item == nullptr) {
				setDragMode(ScrollHandDrag);
				QMouseEvent evt(me->type(), mapFromGlobal(QCursor::pos()), Qt::LeftButton, Qt::LeftButton, me->modifiers());
				QGraphicsView::mousePressEvent(&evt);
			}
		}
		break;
		};
	}
	break;
	case QEvent::GraphicsSceneMouseRelease:
	{
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

void DfxQListenerNodeView::deleteSelected() {
	auto space = DfxSpace::getInstance();

	std::vector<DfxQListenerNode*> nodes;

	for (auto item : scene()->items()) {
		if (item->type() == DfxQNodeEnums::LISTENER_NODE) {
			if (item->isSelected()) {
				nodes.push_back((DfxQListenerNode*)item);
			}
		}
	}

	for (auto node : nodes) {
		auto handler = node->deleteAllItems();

		if (handler.dataObject() != nullptr) {
			scene()->removeItem(node);
			handler.destroy();
		}
	}
}