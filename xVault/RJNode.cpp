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

#include "RJNode.h"
#include "VAttribute.h"
#include "VBuffer.h"

namespace xvault {

	namespace rapid {

		rjNode::rjNode(std::string name) {
			_name = name;
		}

		rjNode::~rjNode() {
			clear();
		}

		std::string rjNode::name() {
			return _name;
		}

		std::vector <Attribute*> &rjNode::attributes() {
			return _attributes;
		}

		std::vector <Buffer*> &rjNode::arrays() {
			return _arrays;
		}

		std::vector <Node*> &rjNode::nodes() {
			return _nodes;
		}

		void rjNode::clear() {
			for (auto attr: _attributes) {
				delete attr;
			}

			for (auto buff : _arrays) {
				delete buff;
			}

			for (auto nd : _nodes) {
				nd->clear();
				delete nd;
			}
		}

		void rjNode::addAttribute(Attribute *attr) {
			_attributes.push_back(attr);
		}

		void rjNode::addBuffer(Buffer *buff) {
			_arrays.push_back(buff);
		}

		void rjNode::addNode(Node *node) {
			_nodes.push_back(node);
		}

		Attribute *rjNode::findAttributeByName(std::string nme) {

			for (auto attr: attributes()) {
				if (attr->name() == nme) {
					return attr;
				}
			}

			return nullptr;
		}

		Buffer *rjNode::findBufferByName(std::string nme) {

			for (auto buff : arrays()) {
				if (buff->name() == nme) {
					return buff;
				}
			}

			return nullptr;
		}

		Node *rjNode::findNodeByName(std::string nme) {

			for (auto node : nodes()) {
				if (node->name() == nme) {
					return node;
				}
			}

			return nullptr;
		}
	}

}