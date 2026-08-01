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

#include "DfxQFlowNodeView.h"
#include "DfxQFlowConnector.h"
#include "DfxQFlowLink.h"

DfxQFlowNodeView::DfxQFlowNodeView() {
	style::styleQScrollBar(horizontalScrollBar());
	style::styleQScrollBar(verticalScrollBar());

	linkTemp.source		 = nullptr;
	linkTemp.destination = nullptr;
	linkTemp.link		 = nullptr;

}

DfxQFlowNodeView::~DfxQFlowNodeView() {

}

void DfxQFlowNodeView::wheelEvent(QWheelEvent *event) {
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

QGraphicsItem* DfxQFlowNodeView::itemAt(const QPointF &pos) {

	if (scene() != nullptr) {
		QList<QGraphicsItem*> items = scene()->items(QRectF(pos - QPointF(1, 1), QSize(3, 3)));

		for (auto item : items) {
			if (item->type() > QGraphicsItem::UserType)
				return item;
		}
	}

	return nullptr;
}

void DfxQFlowNodeView::notifyIfNodesAreSelected() {
	std::vector<DfxQFlowBaseNode*> selectedNodes;

	auto items = scene()->selectedItems();
	if (scene() != nullptr) {
		for (auto item : scene()->selectedItems()) {
			if (item->type() == DfxQNodeEnums::FLOW_BREAK_NODE    ||
				item->type() == DfxQNodeEnums::FLOW_DECISION_NODE || 
				item->type() == DfxQNodeEnums::FLOW_PROCESS_NODE  ||
				item->type() == DfxQNodeEnums::FLOW_START_NODE ) {
				selectedNodes.push_back((DfxQFlowBaseNode*)item);
			}
		}
	}


	emit nodesSelected(selectedNodes);
}

std::string DfxQFlowNodeView::flowPath() {
	return _flowPath;
}

void DfxQFlowNodeView::setFlowPath(std::string fpth) {
	_flowPath = fpth;
}

bool DfxQFlowNodeView::eventFilter(QObject *o, QEvent *e) {
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
			if (item && item->type() == DfxQNodeEnums::FLOW_CONNECTOR) {
				auto conn = (DfxQFlowConnector*)item;

				if (conn->state() != DfxQFlowConnector::INPUT) {

					auto collection = (DfxQFlowCollection*)scene();
					auto connector = (DfxQFlowConnector*)item;

					if (connector->type() != DfxQFlowConnector::INPUT) {

						linkTemp.source = connector->parentNode();
						linkTemp.link = new DfxQFlowLink(collection->router());
						linkTemp.link->setSrcState(connector->state());

						scene()->addItem(linkTemp.link);
						linkTemp.link->updatePosition(linkTemp.source->center(), me->scenePos());
					}
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
		break;
		};
	}
	break;
	case QEvent::GraphicsSceneMouseMove:
	{

		if (linkTemp.link != nullptr) {
			linkTemp.link->updatePosition(linkTemp.source->center(), me->scenePos());
		}
		else if (!scene()->selectedItems().empty()) {
			for (auto itm : scene()->selectedItems()) {
				if (itm->type() == DfxQNodeEnums::FLOW_BREAK_NODE   ||
					itm->type() == DfxQNodeEnums::FLOW_START_NODE   ||
					itm->type() == DfxQNodeEnums::FLOW_PROCESS_NODE ||
					itm->type() == DfxQNodeEnums::FLOW_DECISION_NODE) {

					((DfxQFlowBaseNode*)itm)->updateLinkPositions();
				}
			}
		}
	}
	break;
	case QEvent::GraphicsSceneMouseRelease:
	{
		if (me->button() == Qt::LeftButton) {
			QGraphicsItem *item = itemAt(me->scenePos());

			if (item != nullptr) {
				if (item->type() == DfxQNodeEnums::FLOW_CONNECTOR) {
					auto collection = (DfxQFlowCollection*)scene();
					auto connector = (DfxQFlowConnector*)item;

					if (connector->state() == DfxQFlowConnector::INPUT) {

						auto space = DfxSpace::getInstance();

						if (linkTemp.link != nullptr) {
							auto existingLink = DfxQFlowLink::findExistingLink(linkTemp.link->srcState(), linkTemp.source);

							if (existingLink != nullptr) {
								existingLink->unbind();
								scene()->removeItem(existingLink);
								delete existingLink;
							}

							linkTemp.link->setNode00(linkTemp.source);
							linkTemp.link->setNode01(connector->parentNode());
							linkTemp.link->bind();
							//DfxSpace::getInstance()->addBinding(linkTemp.source->propertyPath(), slot->propertyPath());

							linkTemp.link->updatePosition();

							linkTemp.link = nullptr;
							linkTemp.source = nullptr;
							linkTemp.destination = nullptr;
						}
					}
				}
			}

			if (linkTemp.link != nullptr) {

				scene()->removeItem(linkTemp.link);
				delete linkTemp.link;

				linkTemp.link = nullptr;
				linkTemp.source = nullptr;
				linkTemp.destination = nullptr;
			}

		}

		if (!scene()->selectedItems().empty()) {
			for (auto itm : scene()->selectedItems()) {
				if (itm->type() == DfxQNodeEnums::FLOW_BREAK_NODE ||
					itm->type() == DfxQNodeEnums::FLOW_START_NODE ||
					itm->type() == DfxQNodeEnums::FLOW_PROCESS_NODE ||
					itm->type() == DfxQNodeEnums::FLOW_DECISION_NODE) {

					((DfxQFlowBaseNode*)itm)->updateLinkPositions();
				}
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
	break;
	};

	return QObject::eventFilter(o, e);
}


void DfxQFlowNodeView::deleteSelected() {

	std::vector<DfxQFlowLink*> links;
	std::vector<DfxQFlowBaseNode*> nodes;

	for (auto item : scene()->items()) {
		if (item->type() == DfxQNodeEnums::FLOW_BREAK_NODE    ||
			item->type() == DfxQNodeEnums::FLOW_DECISION_NODE || 
			item->type() == DfxQNodeEnums::FLOW_PROCESS_NODE) {
			if (item->isSelected()) {
				nodes.push_back((DfxQFlowBaseNode*)item);
			}
		}
	}

	for (auto item : scene()->items()) {
		if (item->type() == DfxQNodeEnums::FLOW_LINK) {
			if (item->isSelected()) {
				links.push_back((DfxQFlowLink*)item);
			}
		}
	}

	for (auto lk : links) {
		lk->unbind();
		scene()->removeItem(lk);
		delete lk;
	}
	
	for (auto nd : nodes) {
		nd->removeAndUnbindAllLinks();
		nd->removeFromFlow();
		scene()->removeItem(nd);
		delete nd;
	}
}

void DfxQFlowNodeView::updatePositions() {
	for (auto itm : scene()->items()) {
		if (itm->type() == DfxQNodeEnums::FLOW_BREAK_NODE ||
			itm->type() == DfxQNodeEnums::FLOW_START_NODE ||
			itm->type() == DfxQNodeEnums::FLOW_PROCESS_NODE ||
			itm->type() == DfxQNodeEnums::FLOW_DECISION_NODE) {

			((DfxQFlowBaseNode*)itm)->updateLinkPositions();
		}
	}
}