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

#include <DfxLayerManager.h>
#include <DfxSelectLayerDialog.h>
#include <iostream>
#include "DeepStudioGlobal.h"
#include "DfxQNodeView.h"
#include "DfxQBaseNode.h"

static bool dragScrollMode = false;
static bool rightContextMenu = false;

DfxQNodeView::DfxQNodeView(QWidget *parent): QGraphicsView(parent) {

	conn = nullptr;
	disConn = nullptr;
	propertyPanelPtr = nullptr;
	selectedSlot = nullptr;
	
	scene = DfxLayerManager::getInstance()->getSelectedLayer()->getScene();
	scene->installEventFilter(this);
	//setDragMode(RubberBandDrag);

	setScene(scene);

	renameNodeAction     = new QAction("Rename Node");
	exportNodeAction     = new QAction("Export Node");
	copySelectedAction   = new QAction("Copy");
	cutSelectedAction    = new QAction("Cut");
	pasteSelectedAction  = new QAction("Paste");
	deleteSelectedAction = new QAction("Delete");
	selectAllAction      = new QAction("Select All");

	//Slot Memory actions
	liftSlotAction	= new QAction("Lift Slot");
	dropSlotAction  = new QAction("Drop Slot");
	
	//Prototype Actions
	createRangedLoopAction = new QAction("Create Ranged Loop");

	rightMouseButtonDown = false;
	rightMouseButtonStartX = 0;
	rightMouseButtonStartY = 0;

	setBackgroundBrush(QBrush(QColor(81, 85, 89), Qt::SolidPattern));

	QObject::connect(renameNodeAction, &QAction::triggered, this, &DfxQNodeView::renameSelected);
	QObject::connect(exportNodeAction, &QAction::triggered, this, &DfxQNodeView::exportSelectedNodes);
	QObject::connect(copySelectedAction,   &QAction::triggered, this, &DfxQNodeView::copySelected);
	QObject::connect(cutSelectedAction,    &QAction::triggered, this, &DfxQNodeView::cutSelected);
	QObject::connect(pasteSelectedAction,  &QAction::triggered, this, &DfxQNodeView::pasteSelected);
	QObject::connect(deleteSelectedAction, &QAction::triggered, this, &DfxQNodeView::deleteSelected);
	QObject::connect(selectAllAction,      &QAction::triggered, this, &DfxQNodeView::selectAll);
	QObject::connect( DfxLayerManager::getInstance(), &DfxLayerManager::layerSelected, this, &DfxQNodeView::updateViewableLayer );
	QObject::connect( DfxLayerManager::getInstance(), &DfxLayerManager::removingLayer, this, &DfxQNodeView::setSceneToNull);

	QObject::connect(liftSlotAction, &QAction::triggered, this, &DfxQNodeView::liftSlot);
	QObject::connect(dropSlotAction, &QAction::triggered, this, &DfxQNodeView::dropSlot);

	QObject::connect(createRangedLoopAction, &QAction::triggered, this, &DfxQNodeView::createRangedLoop);

}

QGraphicsItem* DfxQNodeView::itemAt(const QPointF &pos)
{
	QList<QGraphicsItem*> items = scene->items(QRectF(pos - QPointF(1, 1), QSize(3, 3)));

	for(auto item: items) {
		if (item->type() > QGraphicsItem::UserType)
			return item;
	}

	return 0;
}



bool DfxQNodeView::eventFilter(QObject *o, QEvent *e)
{
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
			if (item && item->type() == DfxQNodeSlot::Type)
			{
				DfxQNodeSlot *itemPtr = (DfxQNodeSlot*)item;

				if (itemPtr->isOutput()) {

					conn = new DfxQNodeConnection(0);
					scene->addItem(conn);
					conn->setPort1((DfxQNodeSlot*)item);
					conn->setPos1(item->scenePos());
					conn->setPos2(me->scenePos());
					conn->updatePath();
					return true;
				}
				else if (itemPtr->isInput()) {

					if (itemPtr->hasConnections()) {
						disConn = itemPtr->getConnection(0);
						disConnPos = me->scenePos();
					}
				}
			}
			else if (item && item->type() == DfxQNodePropertiesPanel::Type) {

			}
			else if (item && item->type() == DfxQBaseNode::Type) {
				auto p = me->pos();

				if (propertyPanelPtr == nullptr) {
					moveToTop((DfxQBaseNode*)item);
				}
				else {
					if (!propertyPanelPtr->boundingRect().contains(p)) {
						moveToTop((DfxQBaseNode*)item);
					}
				}
			}
			else {
				if (!rightMouseButtonDown) {
					dragScrollMode = true;
					setDragMode(ScrollHandDrag);
				}
				else {
					setDragMode(RubberBandDrag);
				}
			}
			break;
		}
		case Qt::RightButton:
		{
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item && item->type() == DfxQBaseNode::Type)
			{
				item->setSelected(true);

				QMenu menu;

				if (getScene()->selectedItems().size() > 0) {
					menu.addAction(renameNodeAction);
				}

				menu.addAction(copySelectedAction);
				//menu.addAction(cutSelectedAction);
				//menu.addAction(pasteSelectedAction);
				menu.addSeparator();
				menu.addAction(deleteSelectedAction);
#if	ENABLE_PROTOTYPE_FEATURES
				menu.addAction(createRangedLoopAction);
#endif
				menu.exec(QCursor::pos());
			}
			else if (item && item->type() == DfxQNodeSlot::Type) {

				if (!DfxLayerManager::getInstance()->getSelectedLayer()->isTop()) {

					QMenu menu;
					auto qn = (DfxQNodeSlot*)item;
					selectedSlot = qn;

					if (qn->isLifted()) {
						menu.addAction(dropSlotAction);
					}
					else {
						menu.addAction(liftSlotAction);
					}

					menu.exec(QCursor::pos());
				}
			}
			else {

				if (item == Q_NULLPTR) {
					rightMouseButtonDown = true;
					rightContextMenu = true;
					QMouseEvent evt(me->type(), mapFromGlobal(QCursor::pos()), Qt::LeftButton, Qt::LeftButton, me->modifiers());
					QGraphicsView::mousePressEvent(&evt);
					
				}
			}

			break;
		}
		case Qt::MiddleButton:
		{

		}
		}
		break;
	}
	case QEvent::GraphicsSceneMouseDoubleClick:
	{
		QGraphicsItem *item = itemAt(me->scenePos());
		if (item && item->type() == DfxQBaseNode::Type)
		{
			if (((DfxQBaseNode*)item)->getNodeType() != DfxQNodeType::COMPOSITE_NODE) {

				auto p = me->pos();
				if (propertyPanelPtr != nullptr) {

					propertyPanelPtr->destroyPropertiesForm();

					if (propertyPanelPtr->getEntity()->getInstanceID() ==
						((DfxQBaseNode*)item)->getEntity()->getInstanceID()) {

						propertyPanelPtr = nullptr;
					}
					else {
						((DfxQBaseNode*)item)->buildPropertiesForm();
						propertyPanelPtr = (DfxQBaseNode*)item;
					}

				}
				else {

					((DfxQBaseNode*)item)->buildPropertiesForm();
					propertyPanelPtr = (DfxQBaseNode*)item;
				}
			}
			else {

				auto layerItem = (DfxLayerItem*)((DfxQBaseNode*)item)->getLayerReference();
				DfxLayerManager::getInstance()->switchSelectedLayer(layerItem);
				DfxLayerManager::getInstance()->incrementLevel();

				if (scene != nullptr)
					scene->removeEventFilter(this);

				scene = DfxLayerManager::getInstance()->getSelectedLayer()->getScene();
				scene->installEventFilter(this);

				setScene(scene);

				((DfxQCompositeNode*)DfxLayerManager::getInstance()->getSelectedLayer()->getNode())
					->rebuildChildCompositeNodes();

				/*if (item->getItemType() == DfxLayerItemType::LAYER_ITEM) {
					if (scene->items().size()) {
						centerOn(scene->items()[scene->items().size() - 1]);
					}
				}*/
			}

		}
		break;
	}
	case QEvent::GraphicsSceneMouseMove:
	{
		rightContextMenu = false;
		if (conn != nullptr)
		{
			conn->setPos2(me->scenePos());
			conn->updatePath();
			return true;
		}
		else if (disConn != nullptr) {

			qreal dis = QLineF(disConnPos, me->scenePos()).length();

			if (dis >= 5) {

				delete disConn;
				disConn = nullptr;

				DfxGlobal::command::exec("updateViewport");
			}
		}
		else {

			/*if (horizontalScrollBar()->maximum() == horizontalScrollBar()->value() || 
				horizontalScrollBar()->minimum() == horizontalScrollBar()->value()) {
				qreal x = QGraphicsView::scene()->sceneRect().x();
				qreal y = QGraphicsView::scene()->sceneRect().y();
				qreal w = QGraphicsView::scene()->sceneRect().width() + 1000;
				qreal h = QGraphicsView::scene()->sceneRect().height();

				QGraphicsView::scene()->setSceneRect(x, y, w, h);
			}

			if (verticalScrollBar()->maximum() == verticalScrollBar()->value() ||
				verticalScrollBar()->minimum() == verticalScrollBar()->value()) {
				qreal x = QGraphicsView::scene()->sceneRect().x();
				qreal y = QGraphicsView::scene()->sceneRect().y();
				qreal w = QGraphicsView::scene()->sceneRect().width();
				qreal h = QGraphicsView::scene()->sceneRect().height() + 1000;

				QGraphicsView::scene()->setSceneRect(x, y, w, h);
			}*/
			
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
				DfxQNodeSlot *itemPtr = (DfxQNodeSlot*)item;

				if (itemPtr->isInput()) {

					if (itemPtr->getSlotType() == DfxQNodeSlotType::GROUP_INPUT_SLOT &&
						conn->slot1()->getPropertyType() == DfxEntityPropertyTypes::TRAIT_UID_TYPE) {

						DfxQNodeSlot *port1 = conn->slot1();
						DfxQNodeSlot *port2 = (DfxQNodeSlot*)item;

						if(port1->getParentNode() != port2->getParentNode() &&
						   port1->getParentNode()->ID() != port2->getParentNode()->ID() &&
						   port1->isOutput() != port2->isOutput() ) {

							conn->setPos2(port2->scenePos());
							conn->setPort2(port2);
							conn->updatePath();
							conn->setConnectionType(DfxQNodeConnectionType::GROUP);
							conn->bind();
							conn = nullptr;

							DfxGlobal::command::exec("updateViewport");

							return true;
						}
					}
					else {

						DfxQNodeSlot *port1 = conn->slot1();
						DfxQNodeSlot *port2 = (DfxQNodeSlot*)item;

						if (port1->getParentNode() != port2->getParentNode() &&
							port1->getParentNode()->ID() != port2->getParentNode()->ID() &&
							port1->getPropertyType() == port2->getPropertyType() &&
							port1->isOutput() != port2->isOutput() &&
							!port1->isConnected(port2))
						{
							if (itemPtr->hasConnections())
								itemPtr->removeConnections();


							conn->setPos2(port2->scenePos());
							conn->setPort2(port2);
							conn->updatePath();
							conn->bind();
							conn = nullptr;


							DfxGlobal::command::exec("updateViewport");

							return true;
						}
					}
				}
			}

			delete conn;
			conn = nullptr;
			return true;
		}
		else if (disConn != nullptr) {

			disConn = nullptr;
		}
		else if (me->button() == Qt::RightButton) {
			
			if (rightMouseButtonDown) {

				rightMouseButtonDown = false;
			
				if (rightContextMenu) {
					QGraphicsItem *item = itemAt(me->scenePos());
					if (item == Q_NULLPTR)
					{
						QMenu menu;
						//menu.addAction(exportNodeAction);
						menu.addAction(pasteSelectedAction);
						menu.exec(QCursor::pos());
					}
					rightContextMenu = false;
				}

				QMouseEvent evt(me->type(), mapFromGlobal(QCursor::pos()), Qt::LeftButton, Qt::LeftButton, me->modifiers());
				QGraphicsView::mouseReleaseEvent(&evt);
			}
		}
		setDragMode(NoDrag);
		dragScrollMode = false;
		delete conn;
		conn = nullptr;
		break;
	}
	case QEvent::KeyPress:
	{
		if (!ke->isAutoRepeat()) {
			if (ke->key() == Qt::Key_Delete) {
				deleteSelected();
			}
		}
	}
		break;
	case QEvent::KeyRelease:
		break;
	}

	return QObject::eventFilter(o, e);
}

void DfxQNodeView::wheelEvent(QWheelEvent *event) {

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

QGraphicsScene *DfxQNodeView::getScene() {
	return scene;
}


void DfxQNodeView::renameSelected() {

	if (getScene()->selectedItems().size() > 0) {
		QGraphicsItem *item = getScene()->selectedItems()[0];

		if (item && item->type() == DfxQBaseNode::Type) {

			auto qn = (DfxQBaseNode*)item;

			QDialog dialog;
			dialog.move(QCursor::pos());

			QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
			QPushButton  *ok = new QPushButton("Rename");
			QPushButton  *cancel = new QPushButton("Cancel");

			QFormLayout *flayout = new QFormLayout();
			QLineEdit *lineedit = new QLineEdit(qn->getEntity()->getLogicalName().c_str());

			buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);
			buttonBox->addButton(cancel, QDialogButtonBox::RejectRole);

			flayout->addRow(QString("&New Name:"), lineedit);

			flayout->addWidget(buttonBox);

			dialog.setWindowTitle("Rename Node");
			dialog.setMinimumWidth(200);

			dialog.setLayout(flayout);

			QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
			QObject::connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);

			int rc = dialog.exec();

			if (rc) {

				if (qn->getNodeType() == DfxQNodeType::STANDARD_NODE) {
					if (lineedit->text().length() > 0) {
						((DfxQStandardNode*)qn)->setTitle(lineedit->text());
						qn->getEntity()->setLogicalName(lineedit->text().toStdString());
					}
				}
			}
		}
	}
}

void DfxQNodeView::copySelected() {
	copyPaste.copySelectedNodes(*scene);
}

void DfxQNodeView::cutSelected() {

}

void DfxQNodeView::pasteSelected() {
	copyPaste.paste(this);
}

void DfxQNodeView::deleteSelected() {

	if (propertyPanelPtr != nullptr) {
		propertyPanelPtr->destroyPropertiesForm();
		propertyPanelPtr = nullptr;
	}

	DfxGlobal::command::exec("clearSelection");

	std::vector<DfxQNodeConnection*> connections;
	std::vector<DfxQBaseNode*> nodes;

	for (auto item : scene->items()) {
		if (item->type() == DfxQBaseNode::Type) {
			if (item->isSelected()) {

				DfxQBaseNode *ptr = (DfxQBaseNode*)item;			
				nodes.push_back(ptr);

				
			}
		}
	}

	if (nodes.size() > 0) {
		DeepStudioGlobal::getInstance()->resetSimulation();
	}

	for (auto nd : nodes) {
		DfxLayerManager::getInstance()->removeNode(nd);
	}

	for (auto item : scene->items()) {
		if (item->type() == DfxQNodeConnection::Type) {
			if (item->isSelected()) {

				DfxQNodeConnection *ptr = (DfxQNodeConnection*)item;
				connections.push_back(ptr);

			}
		}
	}

	for (auto nd : connections) {
		/*nd->unbind();
		nd->slot1()->removeConnection(nd->getID());
		nd->slot2()->removeConnection(nd->getID());*/
		delete nd;
	}
}

void DfxQNodeView::selectAll() {

}

void DfxQNodeView::updateViewableLayer(DfxLayerItem *item) {
	if(scene!= nullptr)
		scene->removeEventFilter(this);
	
	scene = DfxLayerManager::getInstance()->getSelectedLayer()->getScene();
	scene->installEventFilter(this);

	setScene(scene);

	if (item->getItemType() == DfxLayerItemType::LAYER_ITEM) {
		if (scene->items().size()) {
			centerOn(scene->items()[scene->items().size() - 1 ]);
		}
	}
}

void DfxQNodeView::setSceneToNull() {
	scene = nullptr;
}

void DfxQNodeView::moveToTop(DfxQBaseNode *node) {
	
	for (auto item : node->scene()->items()) {

		if (item && item->type() == DfxQBaseNode::Type){
			DfxQBaseNode *other = (DfxQBaseNode*)item;

			if (other->getEntity()->getInstanceID() != node->getEntity()->getInstanceID()) {
				other->setZValue(0.f);
			}
		}
	}

	node->setZValue(1.f);
}

void DfxQNodeView::exportSelectedNodes(){

	std::vector<DfxQBaseNode *> nodes;

	for (auto item : scene->items()) {
		if (item->type() == DfxQBaseNode::Type) {
			if (item->isSelected()) {

				DfxQBaseNode *ptr = (DfxQBaseNode*)item;

				nodes.push_back(ptr);
			}
		}
	}

	DfxSelectLayerDialog([nodes](DfxLayerItem *itm) {

		if (itm != nullptr) {

			for (auto node : nodes) {

				DfxLayerItem *item = (DfxLayerItem *)node->getLayerReference();
				DfxLayerManager::getInstance()->addImportNode(item, itm);
			}
		}
	}).exec();
}

void DfxQNodeView::fitAllNodes() {

	auto sc = QGraphicsView::scene();

	if (sc != Q_NULLPTR) {
		fitInView(sc->itemsBoundingRect(), Qt::KeepAspectRatio);
	}
}

std::vector<DfxEntity*> DfxQNodeView::getNodesSelectedInGraph() {
	std::vector<DfxEntity*> nodes;

	if (scene->selectedItems().size() > 0) {

		for (auto node : scene->selectedItems()) {

			if (node->type() == DfxQBaseNode::Type) {
				nodes.push_back(((DfxQBaseNode*)node)->getEntity());
			}
		}
	}

	return nodes;
}

void DfxQNodeView::uniformLayout(bool everything) {
	auto sc = QGraphicsView::scene();

	if (scene->items().size() > 0) {

		std::vector<DfxQBaseNode*> bnodes;
		DfxQHierarchicalLayoutAlpha lalgo;

		if (everything) {
			for (auto node : scene->items()) {

				if (node->type() == DfxQBaseNode::Type) {
					lalgo.addNodeReference((DfxQBaseNode*)node);
					bnodes.push_back((DfxQBaseNode*)node);
				}
			}
		}
		else {
			if (scene->selectedItems().size() > 0) {

				for (auto node : scene->selectedItems()) {

					if (node->type() == DfxQBaseNode::Type) {
						lalgo.addNodeReference((DfxQBaseNode*)node);
						bnodes.push_back((DfxQBaseNode*)node);
					}
				}
			}
			else {
				for (auto node : scene->items()) {

					if (node->type() == DfxQBaseNode::Type) {
						lalgo.addNodeReference((DfxQBaseNode*)node);
						bnodes.push_back((DfxQBaseNode*)node);
					}
				}
			}
		}

		if (bnodes.size() >= 2) {
			lalgo.solveLayout();
			
			for (auto bn : bnodes) {

				float nx = lalgo.getNewX(bn->getEntity()->getInstanceID());
				float ny = lalgo.getNewY(bn->getEntity()->getInstanceID());

				bn->setPos(QPointF(nx, ny));

				/*std::stringstream ss;
				ss << "Name: " << bn->getEntity()->getLogicalName() << std::endl;
				ss << "New Pos: " << nx << ", " << ny << std::endl;
				ss << "Actual Pos: " << bn->pos().x() << ", " << bn->pos().y() << std::endl << std::endl;

				DfxGlobal::console::println(ss.str());*/
			}
		}
	}
}

void DfxQNodeView::previousLayer() {

	DfxLayerItem *previousSelectedLayer = DfxLayerManager::getInstance()->getSelectedLayer();

	if (!previousSelectedLayer->isTop()) {
		
		DfxQBaseNode *cnode = previousSelectedLayer->getNode();
		cnode->rebuild();
	}

	if (DfxLayerManager::getInstance()->previousLayer()) {

		if (scene != nullptr)
			scene->removeEventFilter(this);

		scene = DfxLayerManager::getInstance()->getSelectedLayer()->getScene();
		scene->installEventFilter(this);

		setScene(scene);
	}
}

void DfxQNodeView::homeLayer() {

	if (DfxLayerManager::getInstance()->homeLayer()) {

		if (scene != nullptr)
			scene->removeEventFilter(this);

		scene = DfxLayerManager::getInstance()->getSelectedLayer()->getScene();
		scene->installEventFilter(this);

		if (DfxLayerManager::getInstance()->getSelectedLayer()->isTop()) {

			for (auto item : scene->items()) {

				if (item && item->type() == DfxQBaseNode::Type) {
					auto qnode = (DfxQBaseNode*)item;
					if (qnode->getNodeType() == DfxQNodeType::COMPOSITE_NODE) {
						((DfxQCompositeNode*)qnode)->rebuild();
					}
				}
			}
		}
		
		setScene(scene);
	}
}

void DfxQNodeView::liftSlot() {

	if (selectedSlot != nullptr) {

		auto cnode = (DfxNodes::DfxBaseCompositeNode*)DfxLayerManager::getInstance()->getSelectedLayer()->getNode()->getEntity();

		if (cnode->isLiftable()) {

			cnode->liftSlot(selectedSlot->getParentEntity(), selectedSlot->getPropertyID());
			selectedSlot->setLifted(true);
			selectedSlot->update();
		}
	}

	selectedSlot = nullptr;
}

void DfxQNodeView::dropSlot() {

	if (selectedSlot != nullptr) {

		auto cnode = (DfxNodes::DfxBaseCompositeNode*)DfxLayerManager::getInstance()->getSelectedLayer()->getNode()->getEntity();

		if (cnode->isLiftable()) {

			cnode->dropSlot(selectedSlot->getParentEntity(), selectedSlot->getPropertyID());
			selectedSlot->setLifted(false);
			selectedSlot->update();
		}	
	}

	selectedSlot = nullptr;
}

void DfxQNodeView::createRangedLoop() {

	if (scene->selectedItems().size() > 0) {

		std::vector<DfxQBaseNode*> bnodes;
		std::vector<DfxEntity*> entities;

		if (scene->selectedItems().size() > 0) {

			for (auto node : scene->selectedItems()) {

				if (node->type() == DfxQBaseNode::Type) {
					bnodes.push_back((DfxQBaseNode*)node);
					entities.push_back(((DfxQBaseNode*)node)->getEntity());
				}
			}
		}

		if (bnodes.size() > 0) {

			QRectF bRect = bnodes[0]->mapRectToScene(bnodes[0]->childrenBoundingRect());

			for (auto node : bnodes) {
				bRect = bRect.united(node->mapRectToScene(node->childrenBoundingRect()));
			}

			auto layerItem = DfxLayerManager::getInstance()->addRangedLoop(entities, bRect);

			if (layerItem != nullptr) {
				auto rloop = (DfxQRangedLoopNode*)layerItem->getNode();

				for (auto node : bnodes) rloop->addDelagate(node);
			}
		}
	}
}