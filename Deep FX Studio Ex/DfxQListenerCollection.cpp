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

#include "DfxQListenerCollection.h"


DfxQListenerCollection::DfxQListenerCollection() {
	// addExisting("");
}

DfxQListenerCollection::~DfxQListenerCollection() {

}

void DfxQListenerCollection::drawBackground(QPainter *painter, const QRectF &rect) {
	int scale = 100000;
	int spacing = 10;

	painter->setPen(QPen(QColor(96, 96, 96, 50), 1.f));

	for (int x = -scale; x <= scale; x += spacing)
		painter->drawLine(x, -scale, x, scale);

	for (int y = -scale; y <= scale; y += spacing)
		painter->drawLine(-scale, y, scale, y);
}

DfxQListenerNode *DfxQListenerCollection::addListener(DfxListenerHandler &handler) {

	auto node = new DfxQListenerNode();
	node->setListenerPath(handler.dataObject()->absolutePath());
	node->setLabelText(handler.dataObject()->label().c_str());
	node->refresh();
	node->setEditorPtr(_editor);
	node->addExisting();

	addItem(node);

	return node;
}

void DfxQListenerCollection::clearAll() {

	for (auto item : items()) {
		if (item->type() == DfxQNodeEnums::LISTENER_NODE) {
			auto _item = (DfxQListenerNode*)item;
			removeItem(item);
			delete _item;
		}
	}

	clear();
}

void DfxQListenerCollection::addExisting(std::string ) {

	auto handlers = DfxListenerHandler::fetchAllListenerHandlers();

	for (auto &handler: handlers) {
		addListener(handler);
	}
}

void DfxQListenerCollection::setEditorPtr(DfxFluentNodeEditor *ptr) {
	_editor = ptr;
}


void DfxQListenerCollection::addListener(DfxQListenerNode *) {

}

void DfxQListenerCollection::deleteListener(DfxQListenerNode *) {

}

void DfxQListenerCollection::removeListener(DfxQListenerNode *) {

}

std::pair<std::string, unsigned int> DfxQListenerCollection::typeInfo() {
	return std::pair<std::string, unsigned int>("", QFluentViewType::LISTENER);
}