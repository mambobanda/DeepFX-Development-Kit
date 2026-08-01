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

#include "DfxQListenerNode.h"

QSvgRenderer *DfxQListenerNode::renderer_icon = nullptr;

DfxQListenerNode::DfxQListenerNode(QGraphicsItem *parent):QGraphicsPathItem(parent) {

	if (renderer_icon == nullptr) renderer_icon = new QSvgRenderer(QLatin1String(":/icon/listener_header_icon"));

	icon = new QGraphicsSvgItem(this);
	menu = new DfxQListenerMenuButton(this);

	_label = new QGraphicsTextItem("Listener Label 00", this);
	_label->setDefaultTextColor(QColor(Qt::black));
	_label->setFont(headerFont());
	
	icon->setSharedRenderer(renderer_icon);

	menu->setNode(this);

	_width = 300;
	_height = 40;

	xpos = 0;
	ypos = 0;

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	initial_layout = true;
	//loadTestData();
}

DfxQListenerNode::~DfxQListenerNode() {

}

int DfxQListenerNode::type() const {
	return Type;
}

void DfxQListenerNode::draw() {

	QPainterPath p;
	QPainterPath ptest;

	p.addRect(0, 0, _width, _height);
	_header = p;

	/*ptest.addRect(4, 5, 25, 10);
	_test = ptest;*/

	setPath(p);
}

void DfxQListenerNode::layoutUi() {
	_width = 25 + _label->boundingRect().size().width() + 40;

	float heightCounter = _height + 4;
	for (auto item : _items) {
		item->setPos(3.f, heightCounter);
		heightCounter += item->boundingRect().size().height();

		_width = std::max(_width, item->calcWidth());
	}

	for (auto item : _items) item->setItemWidth(_width);

	menu->setPos(_width - 30, 4);
	icon->setPos(4, 5);
	_label->setPos(30, 8);

	if (initial_layout) {
		DfxListenerHandler handler;

		if (handler.fetch(listenerPath())) {
			auto any = handler.dataObject();

			float xpos = 0.f;
			float ypos = 0.f;

			if (any->data<fl::DataObject>().hasProperty("xpos")) xpos = any->data<fl::DataObject>().property("xpos")->data<float>();
			if (any->data<fl::DataObject>().hasProperty("ypos")) ypos = any->data<fl::DataObject>().property("ypos")->data<float>();

			setPos(xpos, ypos);
		}

		initial_layout = false;
	}
}

QFont DfxQListenerNode::headerFont() {

	QFont font;
	font.setPointSize(10);
	font.setBold(true);

	return font;
}

void DfxQListenerNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	layoutUi();

	draw();

	if (isSelected()) {
		painter->setPen(QPen(QColor(Qt::yellow), 1));
		painter->setBrush(QColor(Qt::yellow));
	}
	else {
		painter->setPen(QPen(QColor(190, 190, 190), 1));
		painter->setBrush(QColor(255, 255, 255));
	}

	painter->drawPath(_header);
}

QString DfxQListenerNode::labelText() {
	return _label->toPlainText();
}

void DfxQListenerNode::setEditorPtr(DfxFluentNodeEditor *ptr) {
	_editor = ptr;
}

void DfxQListenerNode::setLabelText(QString txt) {
	_label->setPlainText(txt);
}

void DfxQListenerNode::newItem(DfxFlowHandler &fl, std::string name) {
	auto item = new DfxQListenerItem(this);
	item->setLabelText(fl.dataObject()->label().c_str());
	item->setFlowPath(fl.dataObject()->absolutePath());
	item->setEditorPtr(_editor);
	item->setParent(this);
	item->setFlowName(name);
	addItem(item);
}

void DfxQListenerNode::deleteItem(DfxFlowHandler &handler, bool addExistingItems) {
	clear();
	DfxListenerHandler l_handler;
	if (l_handler.fetch(listenerPath())) {
		if(l_handler.removeFlowPath(handler.dataObject()->absolutePath()))
			handler.destroy();
	}

	if(addExistingItems)addExisting();
}

DfxListenerHandler DfxQListenerNode::deleteAllItems() {
	DfxListenerHandler handler;

	if (handler.fetch(listenerPath())) {

		if (handler.dataObject() != nullptr) {
			auto paths = handler.dataObject()->rawData<fl::DataObject>()->property("paths");

			if (paths != nullptr) {
				for (auto p : paths->data<fl::DataObject>().propertiesOriginalOrder()) {
					if (p->type() == fl::type::STD_STRING) {
						DfxFlowHandler fhandler;

						if (fhandler.fetch(p->data<std::string>())) {
							deleteItem(fhandler, false);
						}
					}
				}
			}
		}
	}

	return handler;
}

void DfxQListenerNode::addItem(DfxQListenerItem *itm) {
	_items.push_back(itm);
}

void DfxQListenerNode::removeItem(DfxQListenerItem *itm) {

	auto itr = _items.begin();

	while(itr != _items.end()) {
		
		if ((*itr)->uid() == itm->uid()) {
			_items.erase(itr);
			break;
		}

		itr++;
	}
}

void DfxQListenerNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	DfxListenerHandler handler;
	// Set XY Pos
	if (handler.fetch(listenerPath())) {
		auto any = handler.dataObject();

		if (any != nullptr) {

			if (any->data<fl::DataObject>().hasProperty("xpos")) {
				auto xp = any->data<fl::DataObject>().property("xpos");

				if (pos().x() != xp->data<float>())  xp->setData<float>(pos().x());

			}

			if (any->data<fl::DataObject>().hasProperty("ypos")) {
				auto yp = any->data<fl::DataObject>().property("ypos");

				if (pos().y() != yp->data<float>())  yp->setData<float>(pos().y());
			}

		}
	}
	
	QGraphicsPathItem::mouseReleaseEvent(event);
}

void DfxQListenerNode::addExisting() {
	DfxListenerHandler handler;
	
	if (handler.fetch(listenerPath())) {

		if (handler.dataObject() != nullptr) {
			auto paths = handler.dataObject()->rawData<fl::DataObject>()->property("paths");

			if (paths != nullptr) {
				for (auto p : paths->data<fl::DataObject>().propertiesOriginalOrder()) {
					if (p->type() == fl::type::STD_STRING) {
						DfxFlowHandler fhandler;

						if (fhandler.fetch(p->data<std::string>())) {
							newItem(fhandler, p->name());
						}
					}
				}
			}
		}
	}

}

void DfxQListenerNode::loadTestData() {

	auto item00 = new DfxQListenerItem(this);
	auto item01 = new DfxQListenerItem(this);
	auto item02 = new DfxQListenerItem(this);

	_items.push_back(item00);
	_items.push_back(item01);
	_items.push_back(item02);
}

std::string DfxQListenerNode::listenerPath() {
	return _listenerPath;
}

void DfxQListenerNode::setListenerPath(std::string pth) {
	_listenerPath = pth;
}

void DfxQListenerNode::clear() {
	std::vector<DfxQListenerItem*> its;
	for (auto item : _items) its.push_back(item);

	for (auto item : its) {
		removeItem(item);
		delete item;
	}
}

void DfxQListenerNode::refresh() {
	clear();
}

void DfxQListenerNode::moveFlowUP(std::string path) {
	DfxListenerHandler handler;

	if (handler.fetch(listenerPath())) {
		auto any = handler.dataObject()->rawData<fl::DataObject>()->property("paths");
		if (any != nullptr) {
		
			int counter = 0;
			bool found  = false;
			for (auto key : any->rawData<fl::DataObject>()->propertyKeys()) {
		
				if (key == path) {
					found = true;
					break;
				}
				counter++;
			}

			if (found) {
			
				auto length = any->rawData<fl::DataObject>()->propertyKeys().size();
				
				if (length > 1 && counter > 0) {
					std::swap(any->rawData<fl::DataObject>()->propertyKeys()[counter],
							  any->rawData<fl::DataObject>()->propertyKeys()[counter - 1]);
				}

				clear();
				addExisting();
			}
		}
	}
}

void DfxQListenerNode::moveFlowDown(std::string path) {
	DfxListenerHandler handler;

	if (handler.fetch(listenerPath())) {
		auto any = handler.dataObject()->rawData<fl::DataObject>()->property("paths");
		if (any != nullptr) {

			int counter = 0;
			bool found = false;
			for (auto key : any->rawData<fl::DataObject>()->propertyKeys()) {

				if (key == path) {
					found = true;
					break;
				}
				counter++;
			}

			if (found) {

				auto length = any->rawData<fl::DataObject>()->propertyKeys().size();

				if (length > 1 && counter < length - 1) {
					std::swap(any->rawData<fl::DataObject>()->propertyKeys()[counter],
						any->rawData<fl::DataObject>()->propertyKeys()[counter + 1]);
				}

				clear();
				addExisting();
			}
		}
	}
}