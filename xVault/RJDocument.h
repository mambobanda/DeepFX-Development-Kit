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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <rapidjson/document.h>
#include <rapidjson/error/error.h> 
#include <rapidjson/error/en.h>

#include "VNode.h"
#include "VDocument.h"
#include "RJNode.h"

namespace xvault {
	namespace rapid {

		class rjDocument: public Document {
		private:
			Node *_root;
			std::string toString(Node *node, int level);

		private:
			void fromJson(rapidjson::Value::ConstMemberIterator &obj, Node *parent);

		public:
			rjDocument();
			virtual ~rjDocument();

			Node *root() override;
			std::string toString() override;
			bool fromString(std::string doc, std::string &errorMsg);

			void addAttribute(Attribute *attr);
			void addBuffer(Buffer *buffer);
			void addNode(Node *node);
		};

	}
}