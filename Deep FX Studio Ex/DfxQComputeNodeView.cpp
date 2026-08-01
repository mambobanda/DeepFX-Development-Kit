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

#include "DfxQComputeNodeView.h"
#include "DfxQComputeCompoundInNode.h"
#include "DfxQComputeCompoundOutNode.h"
#include "DfxQComputeCompoundNode.h"

DfxQComputeNodeView::DfxQComputeNodeView(DfxFluentNodeEditor *nEditor) {
	_nodeEditor = nEditor;

	_nodePopupMenu = new DfxQComputePopupMenu(this, _nodeEditor);
	_nodePopupMenu->init();

	linkTemp.source = nullptr;
	linkTemp.destination = nullptr;
	linkTemp.link = nullptr;

	disconnectionLink = nullptr;

	style::styleQScrollBar(horizontalScrollBar());
	style::styleQScrollBar(verticalScrollBar());
}

DfxQComputeNodeView::~DfxQComputeNodeView() {

}

void DfxQComputeNodeView::enterEvent(QEvent *event) {
	if (!hasFocus()) {
		setFocus();
	}
}

void DfxQComputeNodeView::wheelEvent(QWheelEvent *event) {
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

QGraphicsItem* DfxQComputeNodeView::itemAt(const QPointF &pos) {

	if (scene() != nullptr) {
		QList<QGraphicsItem*> items = scene()->items(QRectF(pos - QPointF(1, 1), QSize(3, 3)));

		for (auto item : items) {
			if (item->type() > QGraphicsItem::UserType)
				return item;
		}
	}

	return nullptr;
}

void DfxQComputeNodeView::notifyIfNodesAreSelected() {
	std::vector<DfxQComputeBaseNode*> selectedNodes;

	auto items = scene()->selectedItems();
	if (scene() != nullptr) {
		for (auto item : scene()->selectedItems()) {
			if (item->type() == DfxQNodeEnums::COMPUTE_NODE) {
				selectedNodes.push_back((DfxQComputeBaseNode*)item);
			}
		}
	}


	emit nodesSelected(selectedNodes);
}

bool DfxQComputeNodeView::eventFilter(QObject *o, QEvent *e) {
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
			if (item && item->type() == DfxQNodeEnums::COMPUTE_PROPERTY_SLOT) {
				auto slot = (DfxQComputeNodeSlot*)item;

				if (slot->mode() == DfxQComputeNodeSlot::SlotMode::INPUT) {

					if (slot->hasLinks()) {
						disconnectionLink = slot->links()[0];
						disconnectionLinkPos = me->scenePos();
					}

				}
				else if (slot->mode() == DfxQComputeNodeSlot::SlotMode::OUTPUT) {
					linkTemp.source = slot;
					linkTemp.link = new DfxQComputePropertyLink;

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
		break;
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
				std::vector<DfxQComputePropertyLink*> links = { disconnectionLink };
				deleteLinks(links);
				disconnectionLink = nullptr;
			}
		}
		else if (!scene()->selectedItems().empty()) {
			for (auto itm : scene()->selectedItems()) {
				if (itm->type() == DfxQNodeEnums::COMPUTE_NODE ||
					itm->type() == DfxQNodeEnums::COMPUTE_COMPOUND_NODE||
					itm->type() == DfxQNodeEnums::COMPUTE_COMPOUND_OUT_NODE ||
					itm->type() == DfxQNodeEnums::COMPUTE_COMPOUND_IN_NODE) {

					((DfxQComputeBaseNode*)itm)->updateSlotConnectionPositions();
				}
			}
		}
	}
	break;
	case QEvent::GraphicsSceneMouseRelease:
	{
		if (me->button() == Qt::LeftButton) {
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item && item->type() == DfxQNodeEnums::COMPUTE_PROPERTY_SLOT) {
				auto space = DfxSpace::getInstance();
				auto slot = (DfxQComputeNodeSlot*)item;

				if (slot->mode() == DfxQComputeNodeSlot::SlotMode::INPUT && linkTemp.link != nullptr) {
					auto src_prop = (DfxQComputeNodeProperty*)linkTemp.source->parentItem();
					auto dest_prop = (DfxQComputeNodeProperty*)slot->parentItem();

					if (src_prop->slotMode() == DfxQComputeNodeProperty::SlotMode::NORMAL) {

						if (dest_prop->slotMode() == DfxQComputeNodeProperty::SlotMode::NORMAL) {

							if (slot->shape() == DfxQComputeNodeSlot::SlotShape::SQUARE || slot->shape() == DfxQComputeNodeSlot::SlotShape::CIRCLE) {
								deleteLinks(slot->links());

								linkTemp.link->setSlot00(linkTemp.source);
								linkTemp.link->setSlot01(slot);

								if (util::areBindable(linkTemp.source->propertyPath(), slot->propertyPath())) {
									auto srcNode  = (DfxQComputeNode*)src_prop->parentItem();
									auto destNode = (DfxQComputeNode*)dest_prop->parentItem();

									DfxSpace::getInstance()->addBinding(
										linkTemp.source->propertyPath(),
										srcNode->any()->absolutePath(),
										slot->propertyPath(),
										destNode->any()->absolutePath());

									linkTemp.link->updatePosition();

									linkTemp.link = nullptr;
									linkTemp.source = nullptr;
									linkTemp.destination = nullptr;
								}
							}
							else if (slot->shape() == DfxQComputeNodeSlot::SlotShape::CIRCLE_PLUS) {
								auto computeAny = DfxSpace::getInstance()->findAbsolute(slot->propertyPath());
								auto sourceAny  = DfxSpace::getInstance()->findAbsolute(linkTemp.source->propertyPath());
								
								if (computeAny != nullptr && sourceAny != nullptr) {
									auto srcNode  = (DfxQComputeNode*)src_prop->parentItem();
									auto destNode = (DfxQComputeNode*)dest_prop->parentItem();
									
									auto nAttr = util::duplicateState(sourceAny);
									nAttr->setName("value");
									nAttr->setRole(fl::role::INPUT);
									nAttr->setIsDynamic(true);

									DfxSpace::getInstance()->move(nAttr, computeAny);
									nAttr->setLabel(nAttr->name());

									destNode->addSlot(nAttr, destNode->allSlots().size() - 1);
									destNode->insertSlots();

									auto newProperty = destNode->findSlot(nAttr->absolutePath());

									if (newProperty != nullptr) {
										linkTemp.link->setSlot00(linkTemp.source);
										linkTemp.link->setSlot01(newProperty->slot());

										DfxSpace::getInstance()->addBinding(
											linkTemp.source->propertyPath(),
											srcNode->any()->absolutePath(),
											newProperty->slot()->propertyPath(),
											destNode->any()->absolutePath());

										linkTemp.link->updatePosition();

										linkTemp.link = nullptr;
										linkTemp.source = nullptr;
										linkTemp.destination = nullptr;
									}
								}
							}
						}
						else if (dest_prop->slotMode() == DfxQComputeNodeProperty::SlotMode::APPEND_COMPOUND_OUTPUT) {

							if (src_prop->slotMode() == DfxQComputeNodeProperty::SlotMode::NORMAL) {
								auto any = DfxSpace::getInstance()->findAbsolute(linkTemp.source->propertyPath());

								if (any != nullptr) {
									auto new_prop = dest_prop->outNode()->appendOutput(linkTemp.source->propertyPath(), any->label());
									((DfxQComputeNodeCollection*)scene())->restoreCompoundOutNodeLinks(dest_prop->outNode());
								}
							}
							else if (src_prop->slotMode() == DfxQComputeNodeProperty::SlotMode::COMPOUND_OUTPUT_PROXY) {
								auto new_prop = dest_prop->outNode()->appendOutput(src_prop->slot()->propertyPath(), src_prop->label()->label());
								((DfxQComputeNodeCollection*)scene())->restoreCompoundOutNodeLinks(dest_prop->outNode());
							}
						}
					}
					else if (src_prop->slotMode() == DfxQComputeNodeProperty::SlotMode::APPEND_COMPOUND_INPUT) {
					

						if (dest_prop->slotMode() == DfxQComputeNodeProperty::SlotMode::NORMAL) {
							auto any = DfxSpace::getInstance()->findAbsolute(slot->propertyPath());

							if (any != nullptr) {
								auto new_prop = src_prop->inNode()->appendInput(slot->propertyPath(), any->label());
								((DfxQComputeNodeCollection*)scene())->restoreCompoundInNodeLinks(src_prop->inNode());
							}
						}
						else if (dest_prop->slotMode() == DfxQComputeNodeProperty::SlotMode::COMPOUND_INPUT_PROXY) {
							auto new_prop = src_prop->inNode()->appendInput(dest_prop->slot()->propertyPath(), dest_prop->label()->label());
							((DfxQComputeNodeCollection*)scene())->restoreCompoundInNodeLinks(src_prop->inNode());
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
				if (itm->type() == DfxQNodeEnums::COMPUTE_NODE ||
					itm->type() == DfxQNodeEnums::COMPUTE_COMPOUND_NODE ||
					itm->type() == DfxQNodeEnums::COMPUTE_COMPOUND_OUT_NODE ||
					itm->type() == DfxQNodeEnums::COMPUTE_COMPOUND_IN_NODE) {

					((DfxQComputeBaseNode*)itm)->updateSlotConnectionPositions();
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
			else if (ke->key() == Qt::Key_Tab) {
				_nodePopupMenu->showMenuPopup();
			}
		}	
	}
	break;
	};

	return QObject::eventFilter(o, e);
}

void DfxQComputeNodeView::deleteLinks(std::vector <DfxQComputePropertyLink*> links) {

	DfxQComputeCompoundInNode *inNode = nullptr;
	DfxQComputeCompoundOutNode *outNode = nullptr;
	std::vector<std::string> inPaths;
	std::vector<std::string> outPaths;

	for (auto &lk : links) {

		if (lk->linkType() == DfxQComputePropertyLink::NORMAL) {
			auto p_prop = (DfxQComputeNodeProperty*)lk->slot01()->parentItem();
			auto cnode  = (DfxQComputeNode*)p_prop->parentItem();

			auto destAny = DfxSpace::getInstance()->findAbsolute(p_prop->slot()->propertyPath());

			DfxSpace::getInstance()->removeBinding(lk->slot00()->propertyPath(), lk->slot01()->propertyPath());
			scene()->removeItem(lk);
			delete lk;

			if (destAny != nullptr) {

				if (destAny->isDynamic()) {
					auto sindex = cnode->slotIndex(p_prop->slot()->propertyPath());
					if (sindex > -1) {
						cnode->eraseSlot(sindex);
						cnode->insertSlots();
						cnode->updateLinkPositions();
						DfxSpace::getInstance()->remove(destAny);
					}

				}
			}
			
		}
		else if (lk->linkType() == DfxQComputePropertyLink::COMPOUND_INPUT_LINK) {
			inNode = ((DfxQComputeNodeProperty*)lk->slot00()->parentItem())->inNode();

			if (inNode != nullptr) {
				inPaths.push_back(lk->slot00()->propertyPath());
			}
		}
		else if (lk->linkType() == DfxQComputePropertyLink::COMPOUND_OUTPUT_LINK) {
			outNode = ((DfxQComputeNodeProperty*)lk->slot01()->parentItem())->outNode();

			auto lnks = DfxSpace::getInstance()->findAssociatedLinks(lk->slot00()->propertyPath());


			for (auto lk00 : lnks) {

				bool skip = false;
				
				for (auto lk01 : lk->slot00()->links()) {
					if (lk01->slot00()->propertyPath() == lk00.output_path &&
						lk01->slot01()->propertyPath() == lk00.input_path) {
						skip = true;
					}
				}

				if(!skip) DfxSpace::getInstance()->removeBinding(lk00.output_path, lk00.input_path);
			}

			if (outNode != nullptr) {
				outPaths.push_back(lk->slot01()->propertyPath());
			}
		}
	}

	if (inNode != nullptr) {
		for (auto p : inPaths) {
			inNode->removeInput(p);
		}

		((DfxQComputeNodeCollection*)scene())->restoreCompoundInNodeLinks(inNode);
	}

	if (outNode != nullptr) {
		for (auto p : outPaths) outNode->removeOutput(p);
		((DfxQComputeNodeCollection*)scene())->restoreCompoundOutNodeLinks(outNode);
	}
}

void DfxQComputeNodeView::deleteSelected() {
	auto space = DfxSpace::getInstance();

	std::vector<DfxQComputePropertyLink*> links;
	std::vector<DfxQComputeBaseNode*> nodes;

	for (auto item : scene()->items()) {
		if (item->type() == DfxQNodeEnums::COMPUTE_NODE ||
			item->type() == DfxQNodeEnums::COMPUTE_COMPOUND_NODE) {
			if (item->isSelected()) {
				nodes.push_back((DfxQComputeBaseNode*)item);
			}
		}
	}

	for (auto item : scene()->items()) {
		if (item->type() == DfxQNodeEnums::COMPUTE_LINK) {
			if (item->isSelected()) {
				links.push_back((DfxQComputePropertyLink*)item);
			}
		}
	}

	deleteLinks(links);

	for (auto nd : nodes) {
		if (nd->any() != nullptr) {

			if (nd->any()->type() == fl::type::DATA_OBJECT) {
				if (nd->any()->rawData<fl::DataObject>()->isProtected())
					continue;
			}

			std::vector<DfxQComputePropertyLink*> nlinks;

			for (auto slt : nd->allSlots())
				for (auto lk : slt->slot()->links()) nlinks.push_back(lk);

			deleteLinks(nlinks);

			space->remove(nd->any());
			scene()->removeItem(nd);
			delete nd;
		}
	}
}

void DfxQComputeNodeView::viewFitAll() {
	if (scene() != nullptr) {
		QRectF bounds = scene()->itemsBoundingRect();
		bounds.setWidth(bounds.width()*0.9);         // to tighten-up margins
		bounds.setHeight(bounds.height()*0.9);       // same as above
		fitInView(bounds, Qt::KeepAspectRatio);
		centerOn(0, 0);
	}
}