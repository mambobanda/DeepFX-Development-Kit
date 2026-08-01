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

#include "DfxQComputeNodeCollection.h"
#include "DfxQComputeNodeSlot.h"
#include "DfxQComputeCompoundNode.h"
#include "DfxQComputeCompoundInNode.h"
#include "DfxQComputeCompoundOutNode.h"

DfxQComputeNodeCollection::DfxQComputeNodeCollection() {
	_computeGraphPath = "";
	_editor = nullptr;
}

DfxQComputeNodeCollection::~DfxQComputeNodeCollection() {
	for (auto n : nodes) {
		removeNode(n);
	}
}

void DfxQComputeNodeCollection::drawBackground(QPainter *painter, const QRectF &rect) {

	int scale = 100000;
	int spacing = 10;

	painter->setPen(QPen(QColor(96, 96, 96, 50), 1.f));

	for (int x = -scale; x <= scale; x += spacing)
		painter->drawLine(x, -scale, x, scale);

	for (int y = -scale; y <= scale; y += spacing)
		painter->drawLine(-scale, y, scale, y);
}

std::string DfxQComputeNodeCollection::computeGraphPath() {
	return _computeGraphPath;
}

DfxFluentNodeEditor *DfxQComputeNodeCollection::editor() {
	return _editor;
}

void DfxQComputeNodeCollection::setEditor(DfxFluentNodeEditor *edt) {
	_editor = edt;
}

void DfxQComputeNodeCollection::setComputeGraphPath(std::string matPath) {
	_computeGraphPath = matPath;
}

void DfxQComputeNodeCollection::constructLinks() {
	auto space = DfxSpace::getInstance();
	std::unordered_map<std::string, DfxQComputeNodeSlot*> slotMap;

	auto allItems = items();

	for (auto item : allItems) {
		if (item->type() == DfxQNodeEnums::COMPUTE_NODE || 
			item->type() == DfxQNodeEnums::COMPUTE_COMPOUND_NODE) {
			auto bnode = (DfxQComputeBaseNode*)item;
			for (auto sl : bnode->properties()) {
				slotMap[sl->slot()->propertyPath()] = sl->slot();
			}
		}
	}

	for (auto p : slotMap) {
		auto slot = p.second;
		if (slot->mode() == DfxQComputeNodeSlot::SlotMode::INPUT) {
			if (space->hasBinding(slot->propertyPath())) {
				for (auto link : space->findLinks(slot->propertyPath())) {

					auto itrSource = slotMap.find(link.output_path);

					if (itrSource != slotMap.end()) {
						auto linkItem = new DfxQComputePropertyLink;

						linkItem->setSlot00(itrSource->second);
						linkItem->setSlot01(slot);

						linkItem->updatePosition();

						addItem(linkItem);
					}
				}
			}
		}
	}
}

void DfxQComputeNodeCollection::addNode(DfxQComputeBaseNode *node) {
	addItem(node);
	nodes.push_back(node);
}

void DfxQComputeNodeCollection::deleteNode(DfxQComputeBaseNode *) {

}

void DfxQComputeNodeCollection::removeNode(DfxQComputeBaseNode *node) {
	delete node;
}

void DfxQComputeNodeCollection::deleteAllLinks() {

	auto allItems = items();

	for (auto item : allItems) {
		if (item->type() == DfxQNodeEnums::COMPUTE_LINK) {
			removeItem(item);
			delete item;
		}
	}
}

void DfxQComputeNodeCollection::clearAll() {
	for (auto item : items()) {
		if (item->type() == DfxQNodeEnums::COMPUTE_BASE_NODE ) {

			auto _item = (DfxQFlowBaseNode*)item;
			removeItem(item);
			delete item;
		}
		else if (item->type() == DfxQNodeEnums::COMPUTE_LINK) {

		}
		else {
			removeItem(item);
		}
	}
	clear();
}

void DfxQComputeNodeCollection::addExisting(std::string str) {
	clearAll();
	_computeGraphPath = str;

	auto space = DfxSpace::getInstance();
	auto compute_graph = space->findAbsolute(_computeGraphPath);

	DfxQComputeCompoundInNode*  inputs;
	DfxQComputeCompoundOutNode* outputs;

	if (compute_graph != nullptr) {
		if (compute_graph->type() == fl::type::DATA_OBJECT) {

			if (compute_graph->rawData<fl::DataObject>()->mode() == fl::DataObject::COMPOUND_NODE) {
				inputs = new DfxQComputeCompoundInNode;
				outputs = new DfxQComputeCompoundOutNode;

				inputs->setAny(compute_graph);
				inputs->setEditor(_editor);
				inputs->buildNode();

				outputs->setAny(compute_graph);
				outputs->setEditor(_editor);
				outputs->buildNode();

				addItem(inputs);
				addItem(outputs);
			}

			for (auto p : compute_graph->rawData<fl::DataObject>()->properties()) {
				if (p->type() == fl::type::DATA_OBJECT) {
					if (p->rawData<fl::DataObject>()->mode() == fl::DataObject::NODE) {

						auto qnode = new DfxQComputeNode();
						qnode->buildNode(p);
						qnode->setDataObjectPath(p->absolutePath());

						addItem(qnode);
					}
					else if (p->rawData<fl::DataObject>()->mode() == fl::DataObject::COMPOUND_NODE) {
						auto qnode = new DfxQComputeCompoundNode();
						qnode->setAny(p);
						qnode->setCorrectIconState();
						qnode->setEditor(_editor);
						qnode->buildNode();

						addItem(qnode);
					}
				}
			}

			if (compute_graph->rawData<fl::DataObject>()->mode() == fl::DataObject::COMPOUND_NODE) {
				restoreCompoundInNodeLinks(inputs);
				restoreCompoundOutNodeLinks(outputs);
			}

			constructLinks();
		}
	}
}

std::pair<std::string, unsigned int> DfxQComputeNodeCollection::typeInfo() {
	return std::pair<std::string, unsigned int>(_computeGraphPath, QFluentViewType::NODE);
}

void DfxQComputeNodeCollection::restoreCompoundInNodeLinks(DfxQComputeCompoundInNode *innode) {

	for (auto in_slot : innode->allSlots()) {

		for (auto item : items()) {
			if (item->type() == DfxQNodeEnums::COMPUTE_NODE ||
				item->type() == DfxQNodeEnums::COMPUTE_COMPOUND_NODE) {
				for (auto chd : item->childItems()) {

					if (chd->type() == DfxQNodeEnums::COMPUTE_PROPERTY) {
						auto inner_prop = (DfxQComputeNodeProperty*)chd;

						if (in_slot->slot()->propertyPath() == inner_prop->slot()->propertyPath()) {

							auto link = new DfxQComputePropertyLink;
							link->setSlot00(in_slot->slot());
							link->setSlot01(inner_prop->slot());
							link->setLinkType(DfxQComputePropertyLink::COMPOUND_INPUT_LINK);
							link->updatePosition();

							addItem(link);

							break;
						}
					}
				}
			}
			/*else if (item->type() == DfxQNodeEnums::COMPUTE_COMPOUND_NODE) {

			}*/
		}
	}
}

void DfxQComputeNodeCollection::restoreCompoundOutNodeLinks(DfxQComputeCompoundOutNode *outnode) {

	for (auto out_slot : outnode->allSlots()) {

		for (auto item : items()) {
			if (item->type() == DfxQNodeEnums::COMPUTE_NODE ||
				item->type() == DfxQNodeEnums::COMPUTE_COMPOUND_NODE) {
				for (auto chd : item->childItems()) {

					if (chd->type() == DfxQNodeEnums::COMPUTE_PROPERTY) {
						auto inner_prop = (DfxQComputeNodeProperty*)chd;

						if (out_slot->slot()->propertyPath() == inner_prop->slot()->propertyPath()) {

							auto link = new DfxQComputePropertyLink;
							link->setSlot00(inner_prop->slot());
							link->setSlot01(out_slot->slot());
							link->setLinkType(DfxQComputePropertyLink::COMPOUND_OUTPUT_LINK);
							link->updatePosition();

							addItem(link);

							break;
						}
					}
				}
			}
			/*else if (item->type() == DfxQNodeEnums::COMPUTE_COMPOUND_NODE) {

			}*/
		}
	}
}