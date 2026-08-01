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

#include "DfxQNodeSlot.h"
#include "DfxQBaseNode.h"


DfxQNodeSlot::DfxQNodeSlot(DfxQNodeSlotType type, QString lb, DfxPropertyID prop, bool squareSlot, QGraphicsItem *parent)
	:QGraphicsPathItem(parent) {

	property = prop;
	lifted = false;
	_type = type;

	if (_type == DfxQNodeSlotType::OUTPUT_SLOT || _type == DfxQNodeSlotType::INPUT_SLOT) {
		label = new QGraphicsTextItem(lb, this);
		label->setDefaultTextColor(Qt::black);
	}

	squareSlotShape = squareSlot;

	if (_type == DfxQNodeSlotType::OUTPUT_SLOT || _type == DfxQNodeSlotType::INPUT_SLOT) 
		setRadiusMargin(5, 2);
	else if (_type == DfxQNodeSlotType::GROUP_OUTPUT_SLOT || _type == DfxQNodeSlotType::GROUP_INPUT_SLOT) 
		setRadiusMargin(10, 4);

	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

DfxQNodeSlot::~DfxQNodeSlot() {
	
}

void DfxQNodeSlot::setRadiusMargin(int r, int m) {

	if (_type == DfxQNodeSlotType::OUTPUT_SLOT || _type == DfxQNodeSlotType::INPUT_SLOT) {
		radius_ = r;
		margin_ = m;

		if (_type == DfxQNodeSlotType::OUTPUT_SLOT)
			label->setPos(-radius_ - margin_ - label->boundingRect().width(), -label->boundingRect().height() / 2);
		else
			label->setPos(radius_ + margin_, -label->boundingRect().height() / 2);

		QPainterPath p;

		if (squareSlotShape)
			p.addRect(-radius_, -radius_, radius_ * 2, radius_ * 2);
		else
			p.addEllipse(-radius_, -radius_, 2 * radius_, 2 * radius_);

		setPath(p);
	}
	else if (_type == DfxQNodeSlotType::GROUP_INPUT_SLOT || _type == DfxQNodeSlotType::GROUP_OUTPUT_SLOT) {
		radius_ = r;
		margin_ = m;

		if (_type == DfxQNodeSlotType::GROUP_INPUT_SLOT) {
			QPainterPath p;
			p.addEllipse(-radius_, -radius_, 2 * radius_, 2 * radius_);
			setPath(p);
		}
		else {
			QPainterPath p;
			p.addRect(-radius_ * 0.5f, -radius_ * 0.5f, 2 * radius_* 0.5f, 2 * radius_ * 0.5f);
			setPath(p);
		}
	}
}

int DfxQNodeSlot::getFontWidth() {
	QFontMetrics fm(label->font());
	return label->boundingRect().width() + radius_ + margin_;
}

int DfxQNodeSlot::getFontHeight() {
	QFontMetrics fm(label->font());
	return fm.height();
}

DfxQNodeSlotType DfxQNodeSlot::getType() {
	return _type;
}

int DfxQNodeSlot::getRadius() {
	return radius_;
}

int DfxQNodeSlot::getMargin() {
	return margin_;
}

//QVector<DfxQNodeConnection*>& DfxQNodeSlot::connections()
//{
//	return m_connections;
//}

void DfxQNodeSlot::addConnection(DfxQNodeConnection* nd) {
	m_connections.push_back(nd);
}

void DfxQNodeSlot::removeConnection(size_t _id) {

	for (int i = 0; i < m_connections.size(); i++) {

		if (m_connections[i]->getID() == _id) {
			m_connections.erase(m_connections.begin() + i);
			break;
		}
	}
}

bool DfxQNodeSlot::hasConnections() {
	if (m_connections.size() > 0)
		return true;
	else
		return false;
}

DfxQNodeConnection* DfxQNodeSlot::getConnection(size_t inx) {
	return m_connections[inx];
}

void DfxQNodeSlot::removeConnections() {

	std::vector<DfxQNodeConnection*> tempList;

	for (auto conn: m_connections) {
		tempList.push_back(conn);
	}
	
	for (auto conn : tempList) delete conn;
}

void DfxQNodeSlot::setParentNode(DfxQBaseNode* pnd) {
	parentNode = pnd;
}

DfxQBaseNode* DfxQNodeSlot::getParentNode() const {
	return parentNode;
}

DfxEntity* DfxQNodeSlot::getParentEntity() {
	return parentNode->getEntity();
}

DfxPropertyID DfxQNodeSlot::getPropertyID() {
	return property;
}

size_t DfxQNodeSlot::getPropertyType() {
	return parentNode->getEntity()->getPropertyType(property);
}

bool DfxQNodeSlot::isConnected(DfxQNodeSlot *other) {
	foreach(DfxQNodeConnection *conn, m_connections)
		if (conn->slot1() == other || conn->slot2() == other)
			return true;

	return false;
}

bool DfxQNodeSlot::isOutput() {
	return (_type == DfxQNodeSlotType::OUTPUT_SLOT || _type == DfxQNodeSlotType::GROUP_OUTPUT_SLOT) ? true : false;
}

bool DfxQNodeSlot::isInput() {
	return (_type == DfxQNodeSlotType::INPUT_SLOT || _type == DfxQNodeSlotType::GROUP_INPUT_SLOT) ? true : false;
}

int DfxQNodeSlot::type() const {
	return Type;
}

DfxQNodeSlotType DfxQNodeSlot::getSlotType() {
	return _type;
}

std::vector<DfxQNodeConnection*> &DfxQNodeSlot::getAllConnections() {
	return m_connections;
}

QVariant DfxQNodeSlot::itemChange(GraphicsItemChange change, const QVariant &value) {

	if (change == ItemScenePositionHasChanged)
	{
		foreach(DfxQNodeConnection *conn, m_connections)
		{
			conn->updatePosFromPorts();
			conn->updatePath();
		}
	}

	return value;
}

void DfxQNodeSlot::setSlotColor(QColor c) {
	slotColor = c;
}

void DfxQNodeSlot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	
	painter->setPen(QPen(Qt::white));

	if (lifted) {
		painter->setBrush(QColor(Qt::white));
	}
	else {
		painter->setBrush(slotColor);
	}

	painter->drawPath(path());
}

bool DfxQNodeSlot::isLifted() {
	return lifted;
}

void DfxQNodeSlot::setLifted(bool val) {
	lifted = val;
}