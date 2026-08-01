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

#include "SgRootNode.h"
#include <iostream>

namespace deepsg {

	std::string SgRootNode::_uniqueName = "UID_ROOT_NODE";
	SgRootNode *SgRootNode::_instance   = nullptr;

	SgRootNode::SgRootNode() : SgNode(SgNodeType::ROOT) {

	}

	SgRootNode::SgRootNode(SgRootNode *sg, SgRootNode *sgFluent) : SgNode(SgNodeType::ROOT) {
		_root = sg;
		_fluentRoot = sgFluent;
		_root->addChild(this);
	}

	SgRootNode::~SgRootNode() {

	}

	SgRootNode *SgRootNode::getInstance() {

		if (_instance == nullptr) {
			_instance = new SgRootNode(new SgRootNode(), new SgRootNode());
		}

		return _instance;
	}

	std::string SgRootNode::uniqueName() {
		return _uniqueName;
	}

	void SgRootNode::_traverseScene00(SgNode *node, SgTraversalFunction travFunc, SgStateInfo *info, unsigned int layer) {

		if (!node->isIndependent()) {
			if (layer != 0)
				if (node->layer() != layer) return;
		}

		StateInfo *_info = new StateInfo;
		_info->setParent(info->parent());
	
		auto mat = node->transform4x4();
		_info->setMatrix(info->transformMatrix() * mat);
		
		travFunc(node, _info);

		_info->setParent(node);
		for (auto child : node->children()) {
			_traverseScene00(child, travFunc, _info);
		}

		delete _info;
	}

	TraversalFlag SgRootNode::_traverseScene01(SgNode *node, SgConditionalTraversalFunction travFunc, SgStateInfo *info, unsigned int layer) {

		if (!node->isIndependent()) {
			if (layer != 0)
				if (node->layer() != layer) return TraversalFlag::CONTINUE;
		}

		StateInfo *_info = new StateInfo;
		_info->setParent(info->parent());

		auto mat = node->transform4x4();
		_info->setMatrix(info->transformMatrix() * mat);

		TraversalFlag flag00 = travFunc(node, _info);

		if (flag00 == TraversalFlag::CONTINUE) {

			_info->setParent(node);
			for (auto child : node->children()) {
				TraversalFlag flag01 = _traverseScene01(child, travFunc, _info);

				if (flag01 == TraversalFlag::END) {
					delete _info;
					return flag01;
				}
			}

			delete _info;
			return flag00;
		}
		else {
			delete _info;
			return flag00;
		}
		
	}

	SgRootNode *SgRootNode::root() {
		return _root;
	}

	SgRootNode *SgRootNode::fluentRoot() {
		return _fluentRoot;
	}

	void SgRootNode::traverseScene(SgTraversalFunction travFunc, glm::mat4 root_transform) {

		StateInfo *_info = new StateInfo;
		_info->setParent(this);
		_info->setMatrix(root_transform);

		_traverseScene00(this, travFunc, _info);
	}

	void SgRootNode::traverseSubScene(SgNode *node, SgTraversalFunction travFunc, glm::mat4 root_transform) {

		StateInfo *_info = new StateInfo;
		_info->setParent(node);
		_info->setMatrix(root_transform);

		_traverseScene00(node, travFunc, _info);
	}

	void  SgRootNode::traverseSceneConditionally(SgConditionalTraversalFunction travFunc) {
		StateInfo *_info = new StateInfo;
		_info->setParent(this);
		_info->setMatrix(glm::mat4(1.f));

		_traverseScene01(this, travFunc, _info);
	}

	void SgRootNode::traverseSubSceneConditionally(SgNode *node, SgConditionalTraversalFunction travFunc, glm::mat4 root_transform) {

		StateInfo *_info = new StateInfo;
		_info->setParent(node);
		_info->setMatrix(root_transform);

		_traverseScene01(node, travFunc, _info);
	}

	void SgRootNode::traverseFluentScene(SgTraversalFunction travFunc, glm::mat4 root_transform) {

		StateInfo *_info = new StateInfo;
		_info->setParent(_fluentRoot);
		_info->setMatrix(root_transform);

		_traverseScene00(this, travFunc, _info);
	}

	void  SgRootNode::traverseFluentSceneConditionally(SgConditionalTraversalFunction travFunc) {
		StateInfo *_info = new StateInfo;
		_info->setParent(_fluentRoot);
		_info->setMatrix(glm::mat4(1.f));

		_traverseScene01(this, travFunc, _info);
	}

	glm::mat4 SgRootNode::getTransformation4x4(SgNode *in_node) {
		glm::mat4 mat(1.0);

		deepsg::SgConditionalTraversalFunction matFunc = [&mat, in_node](deepsg::SgNode *node, deepsg::SgStateInfo *info) {

			if (in_node->getID() == node->getID()) {
				mat = info->transformMatrix();
				return TraversalFlag::END;
			}
			else {
				return TraversalFlag::CONTINUE;
			}
		};

		traverseSceneConditionally(matFunc);

		return mat;
	}

	std::vector<SgNode*> SgRootNode::findAllNodeTypes(SgNodeType type) {
		std::vector<SgNode*> nodes;

		deepsg::SgTraversalFunction travFunc = [&nodes,type](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
			if (node->getType() == type) {
				nodes.push_back(node);
			}
		};

		traverseScene(travFunc);

		return nodes;
	}
}