#include "DfxQNode.h"
#include <iostream>

DfxQNode::DfxQNode(QGraphicsItem *parent):QGraphicsPathItem(parent) {

	title = new QGraphicsTextItem(this);
	QFont font;
	font.setBold(true);
	title->setFont(font);
	setSlotSpacing(10);

	
	setPen(QPen(Qt::darkGreen));
	setBrush(Qt::green);

	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}


void DfxQNode::addSlot(QString label, DfxQNodeSlotType t) {

	DfxQNodeSlot *slot = new DfxQNodeSlot(t, label, this);	
	slot->setParentNode(this);

	nodeSlots.push_back(slot);

	int width   = 0;
	for (auto slt : nodeSlots) if (slt->getFontWidth() > width) width = slt->getFontWidth();
	
	int count  = 0;
	int height = 0;

	for (auto slt : nodeSlots) {
		height = (count * slot->boundingRect().height()) + (count * slotSpacing);

		if(slt->getType() == DfxQNodeSlotType::INPUT_SLOT)
			slt->setPos(-width/2 - (slot->getRadius()), height);
		else
			slt->setPos(width/2 + (slot->getRadius()), height);

		count++;
	}

	int nheight = height + slot->boundingRect().height();

	QPainterPath p;
	int xx = slot->getRadius() + slot->getMargin() + 5;
	p.addRoundedRect(-width/2, -xx, width, nheight + xx, 5, 5);
	setPath(p);

	QFontMetrics fm(title->font());
	title->setPos(-width / 2, -(fm.height()*2.5f));
}

void DfxQNode::addInputSlot(QString label) {
	addSlot(label, DfxQNodeSlotType::INPUT_SLOT);
}

void DfxQNode::addOutputSlot(QString label) {
	addSlot(label, DfxQNodeSlotType::OUTPUT_SLOT);
}

void DfxQNode::setSlotSpacing(int sp) {
	slotSpacing = sp;
}

void DfxQNode::setTitle(QString txt) {
	title->setPlainText(txt);
}


void DfxQNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->setRenderHint(QPainter::Antialiasing);

	if (isSelected()) {
		painter->setPen(QPen(Qt::darkYellow));
		painter->setBrush(Qt::yellow);
	}
	else {
		painter->setPen(QPen(Qt::darkGreen));
		painter->setBrush(Qt::green);
	}

	painter->drawPath(path());
}

int DfxQNode::type() const {
	return Type;
}

QVariant DfxQNode::itemChange(GraphicsItemChange change, const QVariant &value)
{

	Q_UNUSED(change);

	/*std::cout << (int)change <<std::endl;
	for (DfxQNodeSlot *slt : nodeSlots)
		slt->itemChange(change, value);*/

	return value;
}