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

#include "DfxQListenerMenuButton.h"
#include "DfxQListenerNode.h"

QSvgRenderer *DfxQListenerMenuButton::renderer = nullptr;

DfxQListenerMenuButton::DfxQListenerMenuButton(QGraphicsItem *parent) :QGraphicsPathItem(parent) {
	
	if (renderer == nullptr) renderer = new QSvgRenderer(QLatin1String(":/icon/listener_menu"));
	icon = new QGraphicsSvgItem(this);

	icon->setSharedRenderer(renderer);
	icon->setScale(0.08);

	menu = new QMenu("");
	addNewFlow = new QAction("Add New Flow");

	menu->addAction(addNewFlow);

	QObject::connect(addNewFlow, &QAction::triggered, [this]() {
		DfxFlowHandler handler;

		bool ok;
		QString text = QInputDialog::getText(nullptr, "New Flow Diagram", "Name:", QLineEdit::Normal, "Flow Graph 00", &ok);

		if (ok && !text.isEmpty()) {

			if (handler.create(text.toStdString())) {
				auto any = DfxSpace::getInstance()->findAbsolute(node()->listenerPath());

				if (any != nullptr) {
					auto paths = any->rawData<fl::DataObject>()->property("paths");

					if (paths != nullptr) {
						auto fany = DfxSpace::getInstance()->any("flow_path", "Flow Path");
						fany->newData<std::string>(fl::type::STD_STRING);
						fany->setData(handler.dataObject()->absolutePath());

						DfxSpace::getInstance()->move(fany, paths);

						node()->newItem(handler, fany->name());
					}
					else {
						handler.destroy();
					}
				}
				else {
					handler.destroy();
				}
			}
		}
	});
}

DfxQListenerMenuButton::~DfxQListenerMenuButton() {
	delete menu;
}

int DfxQListenerMenuButton::type() const {
	return Type;
}

void DfxQListenerMenuButton::draw() {
	QPainterPath p;

	p.addRect(-2, -2, 29, 12);
	_box = p;

	setPath(p);
}

void DfxQListenerMenuButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	draw();

	painter->setPen(QPen(Qt::white, 0));
	painter->setBrush(QColor(255, 255, 255, 0));

	painter->drawPath(_box);
}

void DfxQListenerMenuButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	//icon->setOpacity(0.5);
	if (menu->isVisible()) {
		menu->hide();
	}
	else {
		menu->exec(QCursor::pos());
	}
}

void DfxQListenerMenuButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	//icon->setOpacity(1.0);
}

DfxQListenerNode *DfxQListenerMenuButton::node() {
	return _node;
}

void DfxQListenerMenuButton::setNode(DfxQListenerNode *pnt) {
	_node = pnt;
}