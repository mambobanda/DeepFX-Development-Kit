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

#include "VNode.h"


namespace xvault {

	namespace rapid {
		
		class rjNode: public Node {
		private:

			std::string _name;
			std::vector <Attribute*> _attributes;
			std::vector <Buffer*> _arrays;
			std::vector <Node*> _nodes;

		public:
			
			rjNode(std::string name);
			virtual ~rjNode();

			std::string name() override;
			std::vector <Attribute*> &attributes() override;
			std::vector <Buffer*> &arrays() override;
			std::vector <Node*> &nodes() override;
			void clear() override;

			void addAttribute(Attribute *attr);
			void addBuffer(Buffer *buff); 
			void addNode(Node *node);

			Attribute *findAttributeByName(std::string) override;
			Buffer *findBufferByName(std::string) override;
			Node *findNodeByName(std::string) override;
		};

	}

}