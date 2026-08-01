#include "DfxQNodeView.h"
#include "DfxQNode.h"
#include <iostream>

DfxQNodeView::DfxQNodeView(QWidget *parent): QGraphicsView(parent) {

	conn = nullptr;

	scene = new QGraphicsScene(this);
	scene->installEventFilter(this);

	DfxQNode *node = new DfxQNode();
	node->setTitle("DfxNode");
	node->addInputSlot("Test 01");
	node->addInputSlot("Test 02");
	node->addOutputSlot("Test Loooooog 03");
	node->addOutputSlot("Test 04");
	node->addOutputSlot("Test 04");
	node->addOutputSlot("Test 04");
	node->addOutputSlot("Test 04");

	DfxQNode *node2 = new DfxQNode();
	node2->setTitle("DfxNode Input");
	node2->addInputSlot("Test 01");
	node2->addInputSlot("Test 02");

	scene->addItem(node);
	scene->addItem(node2);

	setScene(scene);
}

QGraphicsItem* DfxQNodeView::itemAt(const QPointF &pos)
{
	QList<QGraphicsItem*> items = scene->items(QRectF(pos - QPointF(1, 1), QSize(3, 3)));

	foreach(QGraphicsItem *item, items) {
		if (item->type() > QGraphicsItem::UserType)
			return item;
	}

	return 0;
}

bool DfxQNodeView::eventFilter(QObject *o, QEvent *e)
{
	QGraphicsSceneMouseEvent *me = (QGraphicsSceneMouseEvent*)e;

	switch ((int)e->type())
	{
	case QEvent::GraphicsSceneMousePress:
	{

		switch ((int)me->button())
		{
		case Qt::LeftButton:
		{
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item && item->type() == DfxQNodeSlot::Type)
			{
				conn = new DfxQNodeConnection(0);
				scene->addItem(conn);
				conn->setPort1((DfxQNodeSlot*)item);
				conn->setPos1(item->scenePos());
				conn->setPos2(me->scenePos());
				conn->updatePath();

				return true;
			}
			break;
		}
		case Qt::RightButton:
		{
			
			break;
		}
		}
	}
	case QEvent::GraphicsSceneMouseMove:
	{
		if (conn != nullptr)
		{
			conn->setPos2(me->scenePos());
			conn->updatePath();
			return true;
		}
		break;
	}
	case QEvent::GraphicsSceneMouseRelease:
	{
		if (conn != nullptr && me->button() == Qt::LeftButton)
		{
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item && item->type() == DfxQNodeSlot::Type)
			{
				DfxQNodeSlot *port1 = conn->slot1();
				DfxQNodeSlot *port2 = (DfxQNodeSlot*)item;

				if (port1->getParentNode() != port2->getParentNode() && 
					port1->isOutput() != port2->isOutput() &&
				    !port1->isConnected(port2))
				{
					conn->setPos2(port2->scenePos());
					conn->setPort2(port2);
					conn->updatePath();
					conn = nullptr;
					return true;
				}
			}

			delete conn;
			conn = nullptr;
			return true;
		}
		break;
	}
	}
	return QObject::eventFilter(o, e);
}