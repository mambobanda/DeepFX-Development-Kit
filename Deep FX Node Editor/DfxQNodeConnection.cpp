#include "DfxQNodeConnection.h"

DfxQNodeConnection::DfxQNodeConnection(QGraphicsItem *parent) :QGraphicsPathItem(parent) {

	setPen(QPen(Qt::black, 2));
	setBrush(Qt::NoBrush);
	setZValue(-1);

	slot_1 = nullptr;
	slot_2 = nullptr;
}

DfxQNodeConnection::~DfxQNodeConnection() {

	if (slot_1 != nullptr)
		slot_1->removeConnection(getID());
	if (slot_2 != nullptr)
		slot_2->removeConnection(getID());
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

void DfxQNodeConnection::updatePosFromPorts()
{
	pos1 = slot_1->scenePos();
	pos2 = slot_2->scenePos();
}

void DfxQNodeConnection::updatePath()
{
	QPainterPath p;

	//QPointF pos1(m_port1->scenePos());
	//QPointF pos2(m_port2->scenePos());

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

void DfxQNodeConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->setRenderHint(QPainter::Antialiasing);

	painter->drawPath(path());
}