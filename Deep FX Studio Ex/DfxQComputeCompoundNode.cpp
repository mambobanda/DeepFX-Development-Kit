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

#include "DfxQComputeCompoundNode.h"
#include "DfxQFluentNodeEditor.h"


QSvgRenderer *DfxQComputeCompoundNode::editIconRenderer = nullptr;

DfxQComputeCompoundNode::DfxQComputeCompoundNode(QGraphicsItem *parent) :DfxQComputeBaseNode(parent) {
	_label = "Compound Node";
	_labelPointSize = 8;
	_labelItem = new QGraphicsTextItem(this);

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	if (editIconRenderer == nullptr) editIconRenderer = new QSvgRenderer(QLatin1String(":/icon/compound_edit_icon"));


	editIcon = new QGraphicsSvgItem(this);
	editIcon->setSharedRenderer(editIconRenderer);
	editIcon->setScale(0.035);

	functionalIcon = new DfxQComputeCompoundFunctionalIcon(this);
	functionalIcon->setComputeNodePtr(this);

	_editor = nullptr;

	_any = nullptr;

	_headerColor = QColor(21, 26, 35);

	mousePressed = false;
}

DfxQComputeCompoundNode::~DfxQComputeCompoundNode() {
	int check = 1;
}

void DfxQComputeCompoundNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
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

void DfxQComputeCompoundNode::buildNode() {
	clear();

	if (_any == nullptr) return;

	if (_any->type() == fl::type::DATA_OBJECT) {

		if (_any->rawData<fl::DataObject>()->mode() == fl::DataObject::COMPOUND_NODE) {
			int slotSpacing = 10;

			_labelItem->setPlainText(_any->label().c_str());
			_labelItem->setFont(labelFont());

			std::vector<std::pair<std::string, std::string>> ouputInput;
			ouputInput.reserve(_any->data<fl::DataObject>().outputReferences().size() + _any->data<fl::DataObject>().inputReferences().size()); // preallocate memory
			ouputInput.insert(ouputInput.end(), _any->data<fl::DataObject>().outputReferences().begin(), _any->data<fl::DataObject>().outputReferences().end());
			ouputInput.insert(ouputInput.end(), _any->data<fl::DataObject>().inputReferences().begin(), _any->data<fl::DataObject>().inputReferences().end());

			auto compoundPathAny = _any->rawData<fl::DataObject>()->property("path");
			auto temp_prop = new DfxQComputeNodeProperty(DfxQComputeNodeProperty::SlotAlignment::RIGHT, this);
			temp_prop->label()->setLabel(compoundPathAny->label());
			temp_prop->slot()->setPropertyPath(compoundPathAny->absolutePath());
			temp_prop->slot()->setFillColor(DfxQComputeSlotColors::slotColor(compoundPathAny->type()));
			nslots.push_back(temp_prop);

			for (auto pth : ouputInput) {

				auto prop = DfxSpace::getInstance()->findAbsolute(pth.first);

				if (prop == nullptr) continue;

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
				editIcon->setPos((-width / 2), (headerYPos * 2) + 13);
				functionalIcon->setPos((-width / 2) + 25, (headerYPos * 2) + 13);


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

				if (_any->data<fl::DataObject>().hasProperty("xpos")) xpos = _any->data<fl::DataObject>().property("xpos")->data<float>();
				if (_any->data<fl::DataObject>().hasProperty("ypos")) ypos = _any->data<fl::DataObject>().property("ypos")->data<float>();

				setPos(xpos, ypos);
			}
			else {
				int width = _labelItem->boundingRect().width() * 1.6f;
				QFontMetrics fm(labelFont());
				int headerSize = fm.height() + 13;
				int headerYPos = -headerSize;
				_labelItem->setPos((-width / 2) + 2, headerYPos + 2);
				editIcon->setPos((-width / 2), headerYPos * 2);
				functionalIcon->setPos((-width / 2) + 25, (headerYPos * 2));

				QPainterPath _header;
				QPainterPath _body;

				_body.addRect(-width / 2, headerYPos - 2, width, headerSize * 2);
				_header.addRect((-width / 2) + 2, headerYPos, width - 4, headerSize);
				header = _header;
				body = _body;
				setPath(_header.united(_body));

				float xpos = 0.f;
				float ypos = 0.f;

				if (_any->data<fl::DataObject>().hasProperty("xpos")) xpos = _any->data<fl::DataObject>().property("xpos")->data<float>();
				if (_any->data<fl::DataObject>().hasProperty("ypos")) ypos = _any->data<fl::DataObject>().property("ypos")->data<float>();

				setPos(xpos, ypos);
			}
		}
	}
}

std::string DfxQComputeCompoundNode::label() {
	return _label;
}

int DfxQComputeCompoundNode::type() const {
	return Type;
}

void DfxQComputeCompoundNode::setLabel(std::string str) {
	_label = str;
	_labelItem->setPlainText(str.c_str());
}

QFont DfxQComputeCompoundNode::labelFont() {
	QFont font;
	font.setPointSize(_labelPointSize);
	font.setBold(true);

	return font;
}

int DfxQComputeCompoundNode::labelPointSize() {
	return _labelPointSize;
}

void DfxQComputeCompoundNode::setLabelPointSize(int lsz) {
	_labelPointSize = lsz;
}

QColor DfxQComputeCompoundNode::headerColor() {
	return _headerColor;
}

void DfxQComputeCompoundNode::setHeaderColor(QColor cl) {
	_headerColor = cl;
}

std::string DfxQComputeCompoundNode::dataObjectPath() {
	return _any->absolutePath();
}

void DfxQComputeCompoundNode::updateSlotConnectionPositions() {
	for (auto p : nslots) p->updateSlotConnectionPositions();
}

DfxFluentNodeEditor *DfxQComputeCompoundNode::editor() {
	return _editor;
}

void DfxQComputeCompoundNode::setEditor(DfxFluentNodeEditor *edt) {
	_editor = edt;
}

void DfxQComputeCompoundNode::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	DfxQComputeBaseNode::mousePressEvent(event);
}

void DfxQComputeCompoundNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	//updateSlotConnectionPositions();
	DfxQComputeBaseNode::mouseMoveEvent(event);
}

void DfxQComputeCompoundNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

	for (auto item : scene()->selectedItems()) {
		if (item->type() == DfxQNodeEnums::COMPUTE_NODE || item->type() == DfxQNodeEnums::COMPUTE_COMPOUND_NODE ||
			item->type() == DfxQNodeEnums::COMPUTE_COMPOUND_IN_NODE || item->type() == DfxQNodeEnums::COMPUTE_COMPOUND_OUT_NODE) {

			auto bnode = (DfxQComputeBaseNode*)item;
			bnode->forcePositionUpdate();
		}
	}
	
	DfxQComputeBaseNode::mouseReleaseEvent(event);
}

void DfxQComputeCompoundNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {

	DfxQFluentEditorCrumb crumb;
	crumb.type = QFluentViewType::COMPOUND;
	crumb.path = _any->absolutePath();
	_editor->pushView(crumb);

	DfxQComputeBaseNode::mouseDoubleClickEvent(event);
}
void  DfxQComputeCompoundNode::setFunctionalOffIcon() {
	
}

void DfxQComputeCompoundNode::setFunctionalOnIcon() {
	functionalIcon->setOnIcon();
}

void DfxQComputeCompoundNode::setCorrectIconState() {
	if (any()->rawData<fl::DataObject>()->isFunctional()) {
		functionalIcon->setOnIcon();
	}
	else {
		functionalIcon->setOffIcon();
	}
}

void DfxQComputeCompoundNode::clear() {

}