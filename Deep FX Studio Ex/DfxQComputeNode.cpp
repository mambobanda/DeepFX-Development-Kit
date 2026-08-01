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

#include "DfxQComputeNode.h"
#include "DfxQComputePropertyLink.h"
#include "QGraphicsScene.h"


DfxQComputeNode::DfxQComputeNode(QGraphicsItem *parent) :DfxQComputeBaseNode(parent) {
	_label = "Default Node";
	_labelPointSize = 8;
	_labelItem = new QGraphicsTextItem(this);

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	_headerColor = QColor(21, 26, 35);

	errorIcon = new DfxQComputeErrorIcon(1.f, this);
	errorIcon->setComputePtr(this);

	mousePressed = false;
}

DfxQComputeNode::~DfxQComputeNode() {
	int check = 1;
}

void DfxQComputeNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	bool err = false;

	if (!_dataObjectPath.empty()) {
		auto any = DfxSpace::getInstance()->findAbsolute(_dataObjectPath);
		auto dobj = any->rawData<fl::DataObject>();
		
		if (dobj->hasErrorFlag()) {
			err = true;
			errorIcon->show();
		}
		else {
			errorIcon->hide();
		}
	}

	if (isSelected()) {
		painter->setPen(QPen(Qt::white, 2));

		if (err) {
			painter->setBrush(QColor(200, 0, 0));
		}
		else {
			painter->setBrush(QColor(26, 32, 44));
		}
		//painter->setBrush(QColor(225, 225, 225));
	}
	else {
		painter->setPen(QPen(QColor(50, 50, 50, 100), 2));
		
		if (err) {
			painter->setBrush(QColor(200, 0, 0));
		}
		else {
			painter->setBrush(QColor(26, 32, 44));
		}
	}

	painter->drawPath(body);

	painter->setPen(QPen(_headerColor, 2));
	painter->setBrush(_headerColor);
	painter->drawPath(header);

	/*if (standardNode) {
		setTitle(getEntity()->getLogicalName().c_str());
	}*/

	//painter->drawPath(footer);*/
}

void DfxQComputeNode::buildNode(fl::Any *any) {
	clear();

	if (any->type() == fl::type::DATA_OBJECT) {

		setAny(any);

		_labelItem->setPlainText(any->label().c_str());
		_labelItem->setFont(labelFont());

		for (auto prop : any->rawData<fl::DataObject>()->propertiesOriginalOrder()) {
			addSlot(prop, nslots.size());
		}

		if (any->rawData<fl::DataObject>()->allowDynamicInputs()) {
			auto n_prop = new DfxQComputeNodeProperty(DfxQComputeNodeProperty::SlotAlignment::LEFT, this);
			n_prop->label()->setLabel(" ");
			n_prop->slot()->setPropertyPath(any->absolutePath());
			n_prop->slot()->setShape(DfxQComputeNodeSlot::SlotShape::CIRCLE_PLUS);
			n_prop->slot()->setFillColor(QColor::fromRgb(255, 255, 255));
			nslots.push_back(n_prop);
		}

		insertSlots();

		float xpos = 0.f;
		float ypos = 0.f;

		if (any->data<fl::DataObject>().hasProperty("xpos")) xpos = any->data<fl::DataObject>().property("xpos")->data<float>();
		if (any->data<fl::DataObject>().hasProperty("ypos")) ypos = any->data<fl::DataObject>().property("ypos")->data<float>();

		setPos(xpos, ypos);
	}
}

void DfxQComputeNode::addSlot(fl::Any *prop, size_t index) {
	if (isValidProperty(prop->type())) {

		DfxQComputeNodeProperty *n_prop = nullptr;

		if (prop->role() == fl::role::INPUT) {
			n_prop = new DfxQComputeNodeProperty(DfxQComputeNodeProperty::SlotAlignment::LEFT, this);
		}
		else if (prop->role() == fl::role::OUTPUT) {
			n_prop = new DfxQComputeNodeProperty(DfxQComputeNodeProperty::SlotAlignment::RIGHT, this);
		}

		if (n_prop != nullptr) {
			n_prop->label()->setLabel(prop->label());
			n_prop->slot()->setPropertyPath(prop->absolutePath());
			n_prop->slot()->setFillColor(DfxQComputeSlotColors::slotColor(prop->type()));
			if (prop->isArray()) n_prop->slot()->setShape(DfxQComputeNodeSlot::SlotShape::SQUARE);
			nslots.insert(nslots.begin() + index, n_prop);
		}
	}
}

void DfxQComputeNode::eraseSlot(size_t index) {
	delete nslots[index];
	nslots.erase(nslots.begin() + index);
}

void DfxQComputeNode::insertSlots() {

	if (!nslots.empty()) {
		int slotSpacing = 10;

		int width = _labelItem->boundingRect().width() * 1.6f;
		for (auto slt : nslots) {
			int sz = slt->boundingRect().width();
			//int sz = (slt->boundingRect().width() + slt->slot()->radius() + slt->margin());
			if (sz > width) {
				width = sz;
			}
		}

		int count = 0;
		int height = 0;

		for (auto slt : nslots) {
			height = (count * slt->boundingRect().height()) + (count * slotSpacing);

			if (slt->slotAlignment() == DfxQComputeNodeProperty::SlotAlignment::LEFT)
				slt->setPos(-width / 2 - (slt->slot()->radius()), height);
			else
				slt->setPos(width / 2 + (slt->slot()->radius()), height);

			count++;
		}

		int nheight = height + nslots[0]->boundingRect().height();

		QPainterPath _combined;
		QPainterPath _body;
		QPainterPath _header;
		QPainterPath _footer;

		width += nslots[0]->slot()->radius() * 2;

		QFontMetrics fm(labelFont());
		int headerSize = fm.height() + 13;
		int headerYPos = -headerSize - nslots[0]->boundingRect().height() + 5;

		_labelItem->setPos((-width / 2) + 2, headerYPos + 2);
		errorIcon->setPos((-width / 2), (headerYPos * 2) + 5);

		int xx = nslots[0]->slot()->radius() + nslots[0]->margin() + 5;
		_body.addRect(-width / 2, headerYPos - 2, width, nheight + xx + headerSize);
		_combined.addRect(-(width - 15) / 2, headerYPos - 2, (width - 15), nheight + xx + headerSize);
		_header.addRect((-width / 2) + 2, headerYPos, width - 4, headerSize);
		_combined.addRect((-width / 2) + 2, headerYPos, width - 4, headerSize);
		_footer.addRoundedRect(-width / 2, nheight, width, 10, 0, 0);
		_combined.addRoundedRect(-width / 2, nheight + xx + 10, width, 10, 0, 0);

		combined = _combined;
		body = _body;
		header = _header;
		footer = _footer;

		setPath(_combined);
		
	}
}

void DfxQComputeNode::removeSlots() {
	for (auto cp : nslots) cp->setParentItem(nullptr);
}

int DfxQComputeNode::slotIndex(std::string path) {
	size_t counter = 0;
	for (auto cp: nslots) {
		if (cp->slot()->propertyPath() == path) return counter;
		counter++;
	}

	return -1;
}

void DfxQComputeNode::updateLinkPositions() {

	for (auto cp: nslots) {
		for (auto lk : cp->slot()->links()) {
			lk->updatePosition();
		}
	}
}

std::string DfxQComputeNode::label() {
	return _label;
}

int DfxQComputeNode::type() const {
	return Type;
}

void DfxQComputeNode::setLabel(std::string str) {
	_label = str;
	_labelItem->setPlainText(str.c_str());
}

QFont DfxQComputeNode::labelFont() {
	QFont font;
	font.setPointSize(_labelPointSize);
	font.setBold(true);

	return font;
}

int DfxQComputeNode::labelPointSize() {
	return _labelPointSize;
}

void DfxQComputeNode::setLabelPointSize(int lsz) {
	_labelPointSize = lsz;
}

QColor DfxQComputeNode::headerColor() {
	return _headerColor;
}

void DfxQComputeNode::setHeaderColor(QColor cl) {
	_headerColor = cl;
}

std::string DfxQComputeNode::dataObjectPath() {
	return _dataObjectPath;
}

void DfxQComputeNode::setDataObjectPath(std::string pth) {
	_dataObjectPath = pth;
}

void DfxQComputeNode::updateSlotConnectionPositions() {
	for (auto p : nslots) p->updateSlotConnectionPositions();
}

void DfxQComputeNode::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	DfxQComputeBaseNode::mousePressEvent(event);
}

void DfxQComputeNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	//updateSlotConnectionPositions();
	DfxQComputeBaseNode::mouseMoveEvent(event);
}

void DfxQComputeNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	
	for (auto item : scene()->selectedItems()) {
		if (item->type() == DfxQNodeEnums::COMPUTE_NODE || item->type() == DfxQNodeEnums::COMPUTE_COMPOUND_NODE || 
			item->type() == DfxQNodeEnums::COMPUTE_COMPOUND_IN_NODE || item->type() == DfxQNodeEnums::COMPUTE_COMPOUND_OUT_NODE) {
			
			auto bnode = (DfxQComputeBaseNode*)item;
			bnode->forcePositionUpdate();
		}
	}
	
	DfxQComputeBaseNode::mouseReleaseEvent(event);
}

void DfxQComputeNode::clear() {
	for (auto sl : nslots) delete sl;
	nslots.clear();
}

