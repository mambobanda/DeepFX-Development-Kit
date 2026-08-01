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
#include <QTextStream>
#include <QtXML/QDomDocument> 
#include <DfxGlobal.h>


class DfxXMLSceneLoader {
private:

	class ConnectionInfo {
	public:
		size_t output;
		size_t input;
		size_t source;
		size_t target;
	};

	std::vector<ConnectionInfo> connectionInfo;
private:

	void loadScene(QDomElement &e);
	void loadLayers(QDomElement &e);
	void loadNodes(DfxLayerItem *item, QDomElement &e);
	void loadChannels(QDomElement &e, DfxEntity *entity, size_t instanceID);
	void findNodeProperties(QDomElement &e, std::vector < std::pair<std::string, std::string>> &properties);
	void findConnections(QDomElement &e, std::vector<DfxXMLSceneLoader::ConnectionInfo>  &connections);
	void linkConnections(std::vector<DfxXMLSceneLoader::ConnectionInfo> connectionInfo);
public:
	DfxXMLSceneLoader();
	void load(std::string path);
};