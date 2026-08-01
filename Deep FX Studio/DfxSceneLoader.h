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

#include <stack>
#include <DfxGlobal.h>
#include <DfxLayerManager.h>
#include <stdexcept>
#include <vector>
#include <DfxEntityDB.h>
#include <DfxEntityUtil.h>
#include "DfxSceneFormatReader.h"

enum DfxSceneLoaderState { NULL_STATE=0xff, SCENE_STATE, NODE_STATE, LAYER_STATE, CONNECTION_STATE};

class DfxSceneLoader: public dfxsceneformat::DfxSceneFormatStateListener {
private:
	class ValueInfo {
	public:
		std::string objectTypename;
		std::string objectName;
		size_t type;
		size_t size;

		ValueInfo(std::string, std::string, size_t, size_t);
	};

	class ConnectionInfo {
	public:
		size_t output;
		size_t input;
		size_t source;
		size_t target;
	};

	class ImportNodeInfo {
	public:
		size_t referenceID;
		DfxLayerItem *layer;
		std::vector<ConnectionInfo> connectionList;
		int x;
		int y;
	};

private:
	int depth;
	bool array;

	std::vector<ConnectionInfo> connectionList;
	std::vector<ImportNodeInfo> importNodeList;

	std::stack<size_t> state_stack;
	std::stack<DfxLayerItem*> item_stack;
	std::stack<ValueInfo> value_info_stack;
	std::stack<ImportNodeInfo> import_info_stack;
	std::stack<ConnectionInfo> connection_info_stack;
	std::stack<std::vector<std::pair<std::string, std::string>>> node_property_stack;

	std::ifstream *file;

	bool fileValid;
private:

	void sceneProperties(ValueInfo, std::string);
	void layerProperties(ValueInfo, std::string);
	void nodeProperties(ValueInfo, std::string);
	void connectionProperties(ValueInfo, std::string);

	void linkConnections();
	void setupImportNodes();

	DfxLayerItem *newNodeItem();
public:
	DfxSceneLoader();

	virtual void setFStream(std::ifstream *);
	virtual void newObject(std::string objectTypename, std::string objectName);
	virtual void newValue(std::string objectTypename, std::string objectName, size_t type, size_t size);
	virtual void setValue(std::string val);
	virtual void endValue();
	virtual void endArray();
	virtual void endObject();
	virtual void error(dfxsceneformat::STATE st, std::string tok);

};