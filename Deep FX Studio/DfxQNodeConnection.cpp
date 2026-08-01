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

#include <DfxEntityBasicPropertyTypes.h>
#include "DfxGroupNode.h"
#include "DfxQNodeConnection.h"


size_t DfxQNodeConnection::connection_id_gen = 0;

DfxQNodeConnection::DfxQNodeConnection(QGraphicsItem *parent, DfxQNodeConnectionType _typ) :QGraphicsPathItem(parent) {

	id = connection_id_gen++;
	
	_type = _typ;

	setPen(QPen(Qt::black, 2));
	setBrush(Qt::NoBrush);
	setZValue(-1);

	slot_1 = nullptr;
	slot_2 = nullptr;

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

DfxQNodeConnection::~DfxQNodeConnection() {
	
	if (slot_1 != nullptr) {
		slot_1->removeConnection(getID());
	}

	if (slot_2 != nullptr) {
		slot_2->removeConnection(getID());
		unbind();
		slot_2 = nullptr;
		slot_1 = nullptr;
	}
}

size_t DfxQNodeConnection::getID() {
	return id;
}

void DfxQNodeConnection::setID(size_t _id) {
	this->id = _id;
}

void DfxQNodeConnection::setPos1(const QPointF &p)
{
	pos1 = p;
}

void DfxQNodeConnection::setPos2(const QPointF &p)
{
	pos2 = p;
}

void DfxQNodeConnection::setPort1(DfxQNodeSlot *p)
{
	slot_1 = p;

	slot_1->addConnection(this);
}

void DfxQNodeConnection::setPort2(DfxQNodeSlot *p)
{
	slot_2 = p;

	slot_2->addConnection(this);
}

DfxQNodeSlot *DfxQNodeConnection::setPort1() {
	return slot_1;
}

DfxQNodeSlot *DfxQNodeConnection::setPort2(){
	return slot_2;
}

void DfxQNodeConnection::updatePosFromPorts()
{
	if (slot_1 != nullptr && slot_2 != nullptr) {
		pos1 = slot_1->scenePos();
		pos2 = slot_2->scenePos();
	}
}

void DfxQNodeConnection::updatePath()
{
	QPainterPath p;

	p.moveTo(pos1);

	qreal dx = pos2.x() - pos1.x();
	qreal dy = pos2.y() - pos1.y();

	QPointF ctr1(pos1.x() + dx * 0.25, pos1.y() + dy * 0.1);
	QPointF ctr2(pos1.x() + dx * 0.75, pos1.y() + dy * 0.9);

	p.cubicTo(ctr1, ctr2, pos2);

	setPath(p);
}

DfxQNodeSlot* DfxQNodeConnection::slot1() const {
	return slot_1;
}

DfxQNodeSlot* DfxQNodeConnection::slot2() const {
	return slot_2;
}

int DfxQNodeConnection::type() const {
	return Type;
}

void DfxQNodeConnection::bind() {
	using namespace DfxEntityPropertyTypes;

	if (_type == DfxQNodeConnectionType::NORMAL) {
		DfxEntity *ent01 = slot_1->getParentEntity();
		DfxEntity *ent02 = slot_2->getParentEntity();

		DfxEntity::bind(ent01, slot_1->getPropertyID(),
			ent02, slot_2->getPropertyID());

		ent02->makeDirty();
	}
	else if (_type == DfxQNodeConnectionType::GROUP) {
		DfxEntity *ent01 = slot_1->getParentEntity();
		DfxEntity *ent02 = slot_2->getParentEntity();

		ent01->addChild(ent02);

		UID *_uid = (UID*)ent01->getProperty(slot_1->getPropertyID());
		DfxNodes::DfxGroupNode *gp = (DfxNodes::DfxGroupNode*)ent02;

		gp->addUID(_uid);

		ent02->makeDirty();
	}
}

void DfxQNodeConnection::unbind() {
	using namespace DfxEntityPropertyTypes;

	if (_type == DfxQNodeConnectionType::NORMAL) {
		DfxEntity *ent = slot_2->getParentEntity();
		DfxEntity::unbind(ent, slot_2->getPropertyID());

		ent->makeDirty();
	}
	else if (_type == DfxQNodeConnectionType::GROUP) {
		DfxEntity *ent01 = slot_1->getParentEntity();
		DfxEntity *ent02 = slot_2->getParentEntity();

		ent01->removeChild(ent02);
		
		UID *_uid = (UID*)ent01->getProperty(slot_1->getPropertyID());
		DfxNodes::DfxGroupNode *gp = (DfxNodes::DfxGroupNode*)ent02;

		gp->removeUID(_uid);

		ent02->makeDirty();
	}
}

DfxQNodeConnectionType DfxQNodeConnection::getConnectionType() {
	return _type;
}

void DfxQNodeConnection::setConnectionType(DfxQNodeConnectionType typ) {
	_type = typ;
}

void DfxQNodeConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->setRenderHint(QPainter::Antialiasing);

	if (isSelected()) {
		painter->setPen(QPen(Qt::yellow));
		setBrush(Qt::NoBrush);
	}
	else {
		setPen(QPen(Qt::black, 2));
		setBrush(Qt::NoBrush);
	}

	painter->drawPath(path());
}