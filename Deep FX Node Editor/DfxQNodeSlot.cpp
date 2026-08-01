#include "DfxQNodeSlot.h"

DfxQNodeSlot::DfxQNodeSlot(DfxQNodeSlotType type, QString lb, QGraphicsItem *parent):QGraphicsPathItem(parent) {

	connection_id_gen = 0;

	label = new QGraphicsTextItem(lb, this);

	_type = type;

	setRadiusMargin(5, 2);

	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

void DfxQNodeSlot::setRadiusMargin(int r, int m) {

	radius_ = r;
	margin_ = m;

	if(_type == DfxQNodeSlotType::OUTPUT_SLOT)
		label->setPos(-radius_ - margin_ - label->boundingRect().width(), -label->boundingRect().height() / 2);
	else
		label->setPos(radius_ + margin_, -label->boundingRect().height() / 2);

	QPainterPath p;
	p.addEllipse(-radius_, -radius_, 2 * radius_, 2 * radius_);
	setPath(p);
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
	nd->setID(++connection_id_gen);

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

void DfxQNodeSlot::setParentNode(DfxQNode* pnd) {
	parentNode = pnd;
}

DfxQNode* DfxQNodeSlot::getParentNode() const {
	return parentNode;
}

bool DfxQNodeSlot::isConnected(DfxQNodeSlot *other) {
	foreach(DfxQNodeConnection *conn, m_connections)
		if (conn->slot1() == other || conn->slot2() == other)
			return true;

	return false;
}

bool DfxQNodeSlot::isOutput() {
	return _type == DfxQNodeSlotType::OUTPUT_SLOT ? true : false;
}

bool DfxQNodeSlot::isInput() {
	return _type == DfxQNodeSlotType::INPUT_SLOT ? true : false;
}

int DfxQNodeSlot::type() const {
	return Type;
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

void DfxQNodeSlot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->setRenderHint(QPainter::Antialiasing);
	
	painter->setPen(QPen(Qt::darkRed));
	painter->setBrush(Qt::red);

	painter->drawPath(path());
}