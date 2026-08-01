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

#include <vector>
#include <QList.h>
#include <fstream>
#include <sstream>
#include <DfxEntityDB.h>
#include <DfxLinkedList.h>
//#include <rapidjson/writer.h>
//#include <rapidjson/stringbuffer.h>
//#include <rapidjson/document.h>
#include "DfxGlobal.h"
#include "DfxQBaseNode.h"
#include "DfxSceneGraph.h"


class ProxyConnection {
public:
	size_t out;
	size_t target;
	size_t in;
};

class ProxyNode {
public:
	size_t instance_id;
	DfxEntity *ent;
	DfxQBaseNode  *node;
	DfxLinkedList<ProxyConnection> connections;
	int x;
	int y;
};

class DfxSceneGraphSerializer {
private:
	int formatVersion;

	DfxLinkedList<ProxyNode> _proxies;
	void installProxies();

public:

	DfxSceneGraphSerializer();

	int getFormatVersion();
	void setFormatVersion(int );

	void serializeGraph( std::vector<DfxQBaseNode *> graph, const char* path);

	void deserializeGraph(std::vector<DfxQBaseNode *> graph, const char* path);

	//bool isViewItemNode(DfxEntity *)
};
