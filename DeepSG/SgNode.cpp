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


#include "SgNode.h"
#include "SgTransformOverride.h"
#include "SgToolNode.h"
#include "SgNodeFilter.h"

namespace deepsg {

	SgNode::SgNode(SgNodeType nt) {
		nid = SgNodeID::getID();
		type = nt;
		_layer = 0;
		_parent = nullptr;
		independent = true;
		_protected = false;
		_moveable  = true;
		_visible    = true;
		_renderable = true;
		_referencePath = "";
	}

	SgNode::~SgNode() {
		for (auto filter : _filters) filter->setNode(nullptr);

		ignoreAll();
		unsubscribeAll();

		clearTransformOverrides();
	}

	size_t SgNode::getID() {
		return nid;
	}

	SgNodeType SgNode::getType() {
		return type;
	}

	void SgNode::setParent(SgNode *p) {
		_parent = p;
	}

	SgNode *SgNode::parent() {
		return _parent;
	}

	std::vector<SgNode*> &SgNode::children() {
		return _children;
	}

	size_t SgNode::size() {
		return _children.size();
	}

	void SgNode::addChild(SgNode *ch) {
		if (ch->parent() != nullptr) ch->parent()->removeChild(ch);
		_children.push_back(ch);
		ch->setParent(this);

		if (ch->getType() == SgNodeType::TOOL) ((SgToolNode*)ch)->create(this);
		if (ch->getType() == SgNodeType::TOOL) evaluateTools();
	}

	void SgNode::removeChild(SgNode *ch) {
		removeChild(ch->getID());
	}

	void SgNode::moveChild(size_t src, size_t dest) {
		vector_move<SgNode*>(_children, src, dest);
		evaluateTools();
	}

	SgNode *SgNode::childAt(size_t inx) {

		if (inx < _children.size()) {
			return _children[inx];
		}
		else {
			return nullptr; 
		}
	}

	bool SgNode::isIndependent() {
		return independent;
	}

	bool SgNode::isProtected() {
		return _protected;
	}

	void SgNode::setIsProtected(bool bval) {
		_protected = bval;
	}

	bool SgNode::isMovable() {
		return _moveable;
	}

	void SgNode::setIsMovable(bool bval) {
		_moveable = bval;
	}

	bool SgNode::isVisible() {
		return _visible;
	}

	void SgNode::setIsVisible(bool val) {
		_visible = val;
	}

	bool SgNode::isRenderable() {
		return _renderable;
	}

	void SgNode::setIsRenderable(bool val) {
		_renderable = val;
	}

	unsigned int SgNode::layer() {
		return _layer;
	}

	void SgNode::setLayer(unsigned int ly) {
		_layer = ly;
	}

	void SgNode::removeChild(size_t id) {

		auto itr = _children.begin();
		bool isTool = false;
		SgToolNode *tl = nullptr;
		while (itr != _children.end()) {

			if ((*itr)->getID() == id) {

				if (!(*itr)->isProtected()) {

					if ((*itr)->getType() == SgNodeType::TOOL) {
						isTool = true;
						tl = (SgToolNode*)*itr;
					}

					(*itr)->setParent(nullptr);
					_children.erase(itr);

					if (isTool) {
						((SgToolNode*)tl)->destroy();
						evaluateTools();
					}
				}

				return;
			}

			itr++;
		}
	}

	SgNode *SgNode::findChild(SgNode *ch) {
		return findChild(ch->getID());
	}

	SgNode *SgNode::findChild(size_t id) {
		auto itr = _children.begin();

		while (itr != _children.end()) {

			if ((*itr)->getID() == id) {
				return (*itr);
			}

			itr++;
		}

		return nullptr;
	}

	size_t SgNode::relativeIndex() {

		size_t index = 0;

		if (_parent != nullptr) {

			for (auto n : _parent->children()) {
				if (n->getID() == this->getID()) break;
				index++;
			}
		}

		return index;
	}

	std::string SgNode::referencePath() {
		return _referencePath;
	}

	void SgNode::onSetReferencePath(std::string path) {}

	void SgNode::setReferencePath(std::string path) {
		_referencePath = path;
		onSetReferencePath(path);
	}

	void SgNode::addTransformOverride(SgTransformOverride *overr) {
		overr->setParent(this);
		_transformOverrides.push_back(overr);
	}

	void SgNode::removeTransformOverride(SgTransformOverride *overr) {
		auto iter = _transformOverrides.begin();

		while (iter != _transformOverrides.end()) {

			if ((*iter)->id() == overr->id()) {
				(*iter)->setParent(nullptr);
				_transformOverrides.erase(iter);
				return;
			}

			iter++;
		}
	}

	std::vector<SgTransformOverride*> SgNode::transformOverrides() {
		return _transformOverrides;
	}

	void SgNode::clearTransformOverrides() {
		std::vector<SgTransformOverride*> temp;

		for (auto ov : _transformOverrides) temp.push_back(ov);
		for (auto ov : temp) removeTransformOverride(ov);
		_transformOverrides.clear();
	}

	void SgNode::beginningEvaluatingTools() {}

	void SgNode::evaluateTools() {
		beginningEvaluatingTools();

		for (auto child: children()) {
			if (child->getType() == SgNodeType::TOOL) {
				auto tool = (SgToolNode*)child;
				if (tool->isReady()) {
					tool->evaluate(this);
				}
			}
		}

		finishedEvaluatingTools();
	}

	void SgNode::finishedEvaluatingTools() {}

	void SgNode::addNodeFilter(SgNodeFilter *flt) {
		flt->setNode(this);
		_filters.push_back(flt);
	}

	void SgNode::removeNodeFilter(SgNodeFilter *flt) {
		auto itr = _filters.begin();

		while (itr != _filters.end()) {

			if ((*itr)->id() == flt->id()) {
				(*itr)->setNode(nullptr);
				_filters.erase(itr);
				return;
			}

			itr++;
		}
	}

	void SgNode::beginningEvaluatingNodeFilters() {}

	void SgNode::evaluateNodeFilters(SgStateInfo *info) {
		
		for (auto filter: _filters) {
			filter->begin(this, info);
			filter->filter(this, info);
			filter->end(this, info);
		}

	}

	void SgNode::finishedEvaluatingNodeFilters() {}

	void SgNode::listenToNode(SgNode *node) {
		_emitters[node->getID()] = node;
		node->_listeners[getID()] = this;
	}

	void SgNode::emitSignal(size_t signal) {
		for (auto node: _listeners) {
			node.second->processSignal(signal);
		}
	}

	void SgNode::ignoreNode(SgNode *node) {
		auto itr0 = node->_listeners.find(getID());
		auto itr1 = _emitters.find(node->getID());

		if (itr0 != node->_listeners.end()) _listeners.erase(itr0);

		if (itr1 != _emitters.end()) _emitters.erase(itr1);
	}

	void SgNode::ignoreAll() {
		std::vector<SgNode*> nodes;

		for (auto em : _emitters) nodes.push_back(em.second);

		for (auto node : nodes) ignoreNode(node);
	}

	void SgNode::unsubscribeListener(SgNode *node) {
		node->ignoreNode(this);
	}

	void SgNode::unsubscribeAll() {
		std::vector<SgNode*> nodes;

		for (auto list : _listeners) nodes.push_back(list.second);

		for (auto node : nodes) node->ignoreNode(this);

	}

	void SgNode::processSignal(size_t signal) { }
}