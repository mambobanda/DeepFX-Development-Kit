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

#pragma once

#include <string>
#include <vector>

class DfxToolMapNode {
public:
	enum class Type{ ROOT, TOOL_DOMAIN, SECTION, GROUP, ITEM, MENU, MENU_ITEM };
private:
	DfxToolMapNode::Type type;
	std::vector<DfxToolMapNode*> children;
	DfxToolMapNode *parent;

	std::string name;
	std::string command;
	std::string resource;
	bool active;

public:

	std::string getName();
	std::string getCommand();
	std::string getResource();
	DfxToolMapNode::Type getType();
	bool isAlive();
	
	static DfxToolMapNode *Root();
	DfxToolMapNode *end();

	DfxToolMapNode *addDomain(std::string name, std::string resource, bool active=true);
	DfxToolMapNode *addSection(std::string name, std::string resource, bool active = true);
	DfxToolMapNode *addGroup(std::string name, std::string resource, bool active = true);
	DfxToolMapNode *addMenu(std::string name, std::string resource, bool active = true);
	DfxToolMapNode *addItem(std::string name, std::string resource, std::string command, bool active = true);
	DfxToolMapNode *addMenuItem(std::string name, std::string resource, std::string command, bool active = true);

	std::vector<DfxToolMapNode*> getChildren();
};