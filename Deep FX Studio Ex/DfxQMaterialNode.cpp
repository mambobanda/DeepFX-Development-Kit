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

#include <QGraphicsScene>
#include "DfxQMaterialNode.h"



DfxQMaterialNode::DfxQMaterialNode(QGraphicsItem *parent):DfxQMaterialBaseNode(parent) {
	_label = "Default Node";
	_labelPointSize = 8;
	_labelItem = new QGraphicsTextItem(this);

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	_headerColor = QColor(21, 26, 35);

	mousePressed = false;
}

DfxQMaterialNode::~DfxQMaterialNode() {

}

void DfxQMaterialNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	if (isSelected()) {
		painter->setPen(QPen(Qt::white,2));
		painter->setBrush(QColor(26, 32, 44));
		//painter->setBrush(QColor(225, 225, 225));
	}
	else {
		painter->setPen(QPen(QColor(50,50,50,100), 2));
		painter->setBrush(QColor(26, 32, 44));
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

void DfxQMaterialNode::buildNode(fl::Any *any) {
	clear();

	if (any->type() == fl::type::DATA_OBJECT) {
		int slotSpacing = 10;

		_labelItem->setPlainText(any->label().c_str());
		_labelItem->setFont(labelFont());

		for (auto prop : any->data<fl::DataObject>().propertiesOriginalOrder()) {
			if (prop->type() == fl::type::FLOAT ||
				prop->type() == fl::type::INT   ||
				prop->type() == fl::type::UNSIGNED_INT ||
				prop->type() == fl::type::FLOAT ||
				prop->type() == fl::type::STD_STRING ) {

				DfxQMaterialNodeProperty *n_prop = nullptr;

				if (prop->role() == fl::role::INPUT) {
					n_prop = new DfxQMaterialNodeProperty(DfxQMaterialNodeProperty::SlotAlignment::LEFT, this);
				}
				else if (prop->role() == fl::role::OUTPUT) {
					n_prop = new DfxQMaterialNodeProperty(DfxQMaterialNodeProperty::SlotAlignment::RIGHT, this);
				}

				if (n_prop != nullptr) {
					n_prop->label()->setLabel(prop->label());
					n_prop->slot()->setPropertyPath(prop->absolutePath());
					setSlotColor(n_prop);
					nslots.push_back(n_prop);
				}
			}
		}

		if (!nslots.empty()) {

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

				if (slt->slotAlignment() == DfxQMaterialNodeProperty::SlotAlignment::LEFT)
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


			float xpos = 0.f;
			float ypos = 0.f;

			if (any->data<fl::DataObject>().hasProperty("xpos")) xpos = any->data<fl::DataObject>().property("xpos")->data<float>();
			if (any->data<fl::DataObject>().hasProperty("ypos")) ypos = any->data<fl::DataObject>().property("ypos")->data<float>();
		
			setPos(xpos, ypos);
		}
	}
}

std::string DfxQMaterialNode::label() {
	return _label;
}

void DfxQMaterialNode::setLabel(std::string str) {
	_label = str;
	_labelItem->setPlainText(str.c_str());
}

QFont DfxQMaterialNode::labelFont() {
	QFont font;
	font.setPointSize(_labelPointSize);
	font.setBold(true);

	return font;
}

int DfxQMaterialNode::labelPointSize() {
	return _labelPointSize;
}

void DfxQMaterialNode::setLabelPointSize(int lsz) {
	_labelPointSize = lsz;
}

QColor DfxQMaterialNode::headerColor() {
	return _headerColor;
}

void DfxQMaterialNode::setHeaderColor(QColor cl) {
	_headerColor = cl;
}

void DfxQMaterialNode::updateSlotConnectionPositions() {
	for (auto p : nslots) p->updateSlotConnectionPositions();
}

void DfxQMaterialNode::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	DfxQMaterialBaseNode::mousePressEvent(event);
}

void DfxQMaterialNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	updateSlotConnectionPositions();
	DfxQMaterialBaseNode::mouseMoveEvent(event);
}

void DfxQMaterialNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

	for (auto item : scene()->selectedItems()) {
		if (item->type() == DfxQMaterialNodeEnums::BASE_NODE) {

			auto bnode = (DfxQMaterialBaseNode*)item;
			bnode->forcePositionUpdate();
		}
	}

	DfxQMaterialBaseNode::mouseReleaseEvent(event);
}

void DfxQMaterialNode::setSlotColor(DfxQMaterialNodeProperty *prop) {

	auto space = DfxSpace::getInstance();

	auto any = space->findAbsolute(prop->slot()->propertyPath());

	if (any != nullptr) {
		if (any->type() == fl::type::STD_STRING) {

			if (any->name() == "color_out" ||
				any->name().find("_color") != std::string::npos ||
				any->name() == "bumpmap") {
				prop->slot()->setFillColor(QColor("yellow"));
			}
			else if (any->name() == "material_out" ||
				any->name() == "mat_00" ||
				any->name() == "mat_01" ||
				any->name() == "material_output" ||
				any->name() == "volume_output" ||
				any->name() == "displacement_output" ) {
				prop->slot()->setFillColor(QColor("green"));
			}
		}
		else if (any->type() == fl::type::FLOAT) {
			prop->slot()->setFillColor(QColor("blue"));
		}
		else if (any->type() == fl::type::INT || any->type() == fl::type::UNSIGNED_INT) {
			prop->slot()->setFillColor(QColor("purple"));
		}
	}
}

void DfxQMaterialNode::clear() {

}