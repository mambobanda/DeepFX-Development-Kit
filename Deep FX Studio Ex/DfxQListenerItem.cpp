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

#include "DfxQListenerItem.h"
#include "DfxQFluentNodeEditor.h"

size_t DfxQListenerItem::ID_GEN = 0;

DfxQListenerItem::DfxQListenerItem(QGraphicsItem *parent):QGraphicsPathItem(parent) {

	id = ID_GEN++;

	_label = new QGraphicsTextItem("< Flow Name Here >", this);
	_editButton = new DfxQListenerItemEditButton(1.f, this);
	_deleteButton = new DfxQListenerItemDeleteButton(1.f, this);
	_orderDown = new DfxQListenerItemOrderButton(0, this);
	_orderUp = new DfxQListenerItemOrderButton(1, this);

	_label->setFont(labelFont());
	_label->setDefaultTextColor(QColor(Qt::black));

	_width = calcWidth();
	_height = 34;

	_parent = nullptr;
	_editor = nullptr; 

	menu = new QMenu;
	rename = new QAction("Rename Flow Diagram");

	menu->addAction(rename);

	_editButton->setItemPtr(this);

	_flowPath = "";

	QObject::connect(rename, &QAction::triggered, [this]() {
		DfxFlowHandler handler;

		bool ok;
		QString text = QInputDialog::getText(nullptr, "Rename Flow Diagram", "Name:", QLineEdit::Normal, labelText(), &ok);

		if (ok && !text.isEmpty()) {
			if (handler.fetch(_flowPath)) {
				setLabelText(text);
				handler.dataObject()->setLabel(text.toStdString());
			}
		}
	});
}

DfxQListenerItem::~DfxQListenerItem() {

}

void DfxQListenerItem::setEditorPtr(DfxFluentNodeEditor *ptr) {
	_editor = ptr;
	_editButton->setEditorPtr(ptr);
}

void DfxQListenerItem::setParent(DfxQListenerNode *prt) {
	_parent = prt;
	_deleteButton->setNode(prt);

	QObject::connect(_orderUp, &DfxQListenerItemOrderButton::buttonClicked, _parent, &DfxQListenerNode::moveFlowUP);
	QObject::connect(_orderDown, &DfxQListenerItemOrderButton::buttonClicked, _parent, &DfxQListenerNode::moveFlowDown);
}

size_t DfxQListenerItem::uid() {
	return id;
}

std::string DfxQListenerItem::flowPath() {
	return _flowPath;
}

void DfxQListenerItem::setFlowPath(std::string fPath) {
	_flowPath = fPath;
	_deleteButton->setFlowPath(fPath);
}

std::string DfxQListenerItem::flowName() {
	return _flowName;
}

void DfxQListenerItem::setFlowName(std::string pth) {
	_flowName = pth;
	_orderUp->setFlowPath(pth);
	_orderDown->setFlowPath(pth);
}

int DfxQListenerItem::type() const {
	return  Type;
}

float DfxQListenerItem::itemWidth() {
	return _width;
}

void DfxQListenerItem::setItemWidth(float wth) {
	_width = wth;
}

float DfxQListenerItem::itemHeight() {
	return _height;
}

void DfxQListenerItem::setItemHeight(float ht) {
	_height = ht;
}

QString DfxQListenerItem::labelText() {
	return _label->toPlainText();
}

void DfxQListenerItem::setLabelText(QString str) {
	_label->setPlainText(str);
}

float DfxQListenerItem::calcWidth() {

	float lw = _label->boundingRect().width();
	return 12 + lw + 60;
}

QFont DfxQListenerItem::labelFont() {

	QFont font;
	font.setPointSize(9);

	return font;
}

void DfxQListenerItem::layoutUi() {

	_orderDown->setPos(0, 18);
	_label->setPos(12, 3);

	_deleteButton->setPos(_width - 30, 2);
	_editButton->setPos(_width - 60, 3);
	
}

void DfxQListenerItem::draw() {
	QPainterPath p;

	float lw = _label->boundingRect().width();
	p.addRect(-3, -3, _width, 34);
	_container = p;

	setPath(p);
}


void DfxQListenerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	layoutUi();

	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	draw();

	painter->setPen(QPen(QColor(190, 190, 190), 1));
	painter->setBrush(QColor(255, 255, 255));

	painter->drawPath(_container);
}

void DfxQListenerItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	if (event->button() == Qt::MouseButton::RightButton) {
		menu->exec(QCursor::pos());
	}
}

void DfxQListenerItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	/*if (event->button() == Qt::MouseButton::LeftButton) {
		
	}*/
}

DfxQListenerItemOrderButton *DfxQListenerItem::orderUp() {
	return _orderUp;
}

DfxQListenerItemOrderButton *DfxQListenerItem::orderDown() {
	return _orderDown;
}