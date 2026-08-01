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

#include "DfxToolMapNode.h"


std::string DfxToolMapNode::getName() {
	return name;
}

std::string DfxToolMapNode::getCommand() {
	return command;
}

std::string DfxToolMapNode::getResource() {
	return resource;
}

bool DfxToolMapNode::isAlive() {
	return active;
}

DfxToolMapNode::Type DfxToolMapNode::getType() {
	return type;
}

DfxToolMapNode *DfxToolMapNode::Root() {
	DfxToolMapNode *root = new DfxToolMapNode;
	root->parent = nullptr;
	root->name = "ROOT";
	root->type = DfxToolMapNode::Type::ROOT;

	return root;

}
DfxToolMapNode *DfxToolMapNode::end() {
	return parent;
}

DfxToolMapNode *DfxToolMapNode::addDomain(std::string name, std::string resource, bool active) {
	DfxToolMapNode *domain = new DfxToolMapNode;
	domain->parent = this;
	domain->type = DfxToolMapNode::Type::TOOL_DOMAIN;
	domain->name = name;
	domain->resource = resource;
	domain->active = active;

	children.push_back(domain);

	return domain;
}

DfxToolMapNode *DfxToolMapNode::addSection(std::string name, std::string resource, bool active) {
	DfxToolMapNode *section = new DfxToolMapNode;
	section->parent = this;
	section->type = DfxToolMapNode::Type::SECTION;
	section->name = name;
	section->resource = resource;
	section->active = active;

	children.push_back(section);

	return section;
}

DfxToolMapNode *DfxToolMapNode::addGroup(std::string name, std::string resource, bool active) {
	DfxToolMapNode *group = new DfxToolMapNode;
	group->parent = this;
	group->type = DfxToolMapNode::Type::GROUP;
	group->name = name;
	group->resource = resource;
	group->active = active;

	children.push_back(group);

	return group;
}

DfxToolMapNode *DfxToolMapNode::addMenu(std::string name, std::string resource, bool active) {
	DfxToolMapNode *menu = new DfxToolMapNode;
	menu->parent = this;
	menu->type = DfxToolMapNode::Type::MENU;
	menu->name = name;
	menu->resource = resource;
	menu->active = active;

	children.push_back(menu);

	return menu;
}

DfxToolMapNode *DfxToolMapNode::addItem(std::string name, std::string resource, std::string command, bool active) {
	DfxToolMapNode *item = new DfxToolMapNode;
	item->parent = this;
	item->type = DfxToolMapNode::Type::ITEM;
	item->name = name;
	item->command = command;
	item->resource = resource;
	item->active = active;

	children.push_back(item);

	return this;
}

DfxToolMapNode *DfxToolMapNode::addMenuItem(std::string name, std::string resource, std::string command, bool active) {
	DfxToolMapNode *item = new DfxToolMapNode;
	item->parent = this;
	item->type = DfxToolMapNode::Type::MENU_ITEM;
	item->name = name;
	item->command = command;
	item->resource = resource;
	item->active = active;

	children.push_back(item);

	return this;
}

std::vector<DfxToolMapNode*> DfxToolMapNode::getChildren() {
	return children;
}