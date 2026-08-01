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

#include "DfxQComputeCompoundOutNode.h"
#include "DfxQFluentNodeEditor.h"


DfxQComputeCompoundOutNode::DfxQComputeCompoundOutNode(QGraphicsItem *parent) :DfxQComputeBaseNode(parent) {

	_label = "Output Slots";
	_labelPointSize = 8;
	_labelItem = new QGraphicsTextItem(this);
	_labelItem->setPlainText("Output Slots");

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	_append_prop = new DfxQComputeNodeProperty(DfxQComputeNodeProperty::SlotAlignment::LEFT, this);
	_append_prop->label()->setLabel("");
	_append_prop->slot()->setRadius(8);
	_append_prop->slot()->setFillColor(QColor(122, 122, 122));
	_append_prop->setMode(DfxQComputeNodeProperty::SlotMode::APPEND_COMPOUND_OUTPUT);
	_append_prop->setOutNode(this);

	_editor = nullptr;

	_any = nullptr;

	_headerColor = QColor(21, 26, 35);

	mousePressed = false;
}

DfxQComputeCompoundOutNode::~DfxQComputeCompoundOutNode() {
	int check = 1;
}

void DfxQComputeCompoundOutNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option)
		Q_UNUSED(widget)

		painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	if (isSelected()) {
		painter->setPen(QPen(Qt::white, 2));
		painter->setBrush(QColor(26, 32, 44));
		//painter->setBrush(QColor(225, 225, 225));
	}
	else {
		painter->setPen(QPen(QColor(50, 50, 50, 100), 2));
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

void DfxQComputeCompoundOutNode::buildNode() {
	clear();

	if (_any == nullptr) return;

	if (_any->type() == fl::type::DATA_OBJECT) {

		if (_any->rawData<fl::DataObject>()->mode() == fl::DataObject::COMPOUND_NODE) {
			int slotSpacing = 10;

			_labelItem->setFont(labelFont());

			for (auto pth : _any->data<fl::DataObject>().outputReferences()) {

				auto prop = DfxSpace::getInstance()->findAbsolute(pth.first);

				if (prop == nullptr) continue;

				if (isValidProperty(prop->type())) {

					DfxQComputeNodeProperty *n_prop = nullptr;


					if (prop->role() == fl::role::OUTPUT) {
						n_prop = new DfxQComputeNodeProperty(DfxQComputeNodeProperty::SlotAlignment::LEFT, this);
						n_prop->setMode(DfxQComputeNodeProperty::SlotMode::COMPOUND_OUTPUT_PROXY);
					}

					if (n_prop != nullptr) {
						n_prop->label()->setLabel(prop->label());
						n_prop->slot()->setPropertyPath(prop->absolutePath());
						n_prop->slot()->setFillColor(DfxQComputeSlotColors::slotColor(prop->type()));
						n_prop->setOutNode(this);
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
				_append_prop->setPos((-width / 2) - 12, headerYPos + 15);

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

				if (_any->data<fl::DataObject>().hasProperty("out_xpos")) xpos = _any->data<fl::DataObject>().property("out_xpos")->data<float>();
				if (_any->data<fl::DataObject>().hasProperty("out_ypos")) ypos = _any->data<fl::DataObject>().property("out_ypos")->data<float>();

				setPos(xpos, ypos);
			}
			else {
				int width = _labelItem->boundingRect().width() * 1.6f;
				QFontMetrics fm(labelFont());
				int headerSize = fm.height() + 13;
				int headerYPos = -headerSize;
				_labelItem->setPos((-width / 2) + 2, headerYPos + 2);
				_append_prop->setPos((-width / 2) - 12, headerYPos + 15);
				QPainterPath _header;
				QPainterPath _body;

				_body.addRect(-width / 2, headerYPos - 2, width, headerSize * 2);
				_header.addRect((-width / 2) + 2, headerYPos, width - 4, headerSize);
				header = _header;
				body = _body;
				setPath(_header.united(_body));

				float xpos = 0.f;
				float ypos = 0.f;

				if (_any->data<fl::DataObject>().hasProperty("out_xpos")) xpos = _any->data<fl::DataObject>().property("out_xpos")->data<float>();
				if (_any->data<fl::DataObject>().hasProperty("out_ypos")) ypos = _any->data<fl::DataObject>().property("out_ypos")->data<float>();

				setPos(xpos, ypos);
			}
		}
	}
}

std::string DfxQComputeCompoundOutNode::label() {
	return _label;
}

int DfxQComputeCompoundOutNode::type() const {
	return Type;
}

void DfxQComputeCompoundOutNode::setLabel(std::string str) {
	_label = str;
	_labelItem->setPlainText(str.c_str());
}

QFont DfxQComputeCompoundOutNode::labelFont() {
	QFont font;
	font.setPointSize(_labelPointSize);
	font.setBold(true);

	return font;
}

int DfxQComputeCompoundOutNode::labelPointSize() {
	return _labelPointSize;
}

void DfxQComputeCompoundOutNode::setLabelPointSize(int lsz) {
	_labelPointSize = lsz;
}

QColor DfxQComputeCompoundOutNode::headerColor() {
	return _headerColor;
}

void DfxQComputeCompoundOutNode::setHeaderColor(QColor cl) {
	_headerColor = cl;
}

std::string DfxQComputeCompoundOutNode::dataObjectPath() {
	return _any->absolutePath();
}

void DfxQComputeCompoundOutNode::updateSlotConnectionPositions() {
	for (auto p : nslots) p->updateSlotConnectionPositions();
}

DfxFluentNodeEditor *DfxQComputeCompoundOutNode::editor() {
	return _editor;
}

void DfxQComputeCompoundOutNode::setEditor(DfxFluentNodeEditor *edt) {
	_editor = edt;
}

void DfxQComputeCompoundOutNode::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	DfxQComputeBaseNode::mousePressEvent(event);
}

void DfxQComputeCompoundOutNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	//updateSlotConnectionPositions();
	DfxQComputeBaseNode::mouseMoveEvent(event);
}

void DfxQComputeCompoundOutNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

	for (auto item : scene()->selectedItems()) {
		if (item->type() == DfxQNodeEnums::COMPUTE_NODE || item->type() == DfxQNodeEnums::COMPUTE_COMPOUND_NODE ||
			item->type() == DfxQNodeEnums::COMPUTE_COMPOUND_IN_NODE || item->type() == DfxQNodeEnums::COMPUTE_COMPOUND_OUT_NODE) {

			auto bnode = (DfxQComputeBaseNode*)item;
			bnode->forcePositionUpdate();
		}
	}

	DfxQComputeBaseNode::mouseReleaseEvent(event);
}

DfxQComputeNodeProperty *DfxQComputeCompoundOutNode::appendOutput(std::string name, std::string label) {
	_any->rawData<fl::DataObject>()->addOutputReference(name, label);
	buildNode();

	if (nslots.back()->slot()->propertyPath() == name) {
		return nslots.back();
	}
	else {
		return nullptr;
	}
}

void DfxQComputeCompoundOutNode::removeOutput(std::string name) {
	_any->rawData<fl::DataObject>()->removeOutputReference(name);
	buildNode();
}

void DfxQComputeCompoundOutNode::clear() {

	for (auto slt : nslots) {
		slt->setParentItem(nullptr);
		for (auto l : slt->slot()->links()) delete l;
		delete slt;
	}

	nslots.clear();
}