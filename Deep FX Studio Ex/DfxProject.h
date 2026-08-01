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

#include <DfxGlobal.h>
#include <fstream>
#include <QJsonDocument> 
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <string>
#include <experimental\filesystem>
#include <QDateTime>
#include <SgNode.h>
#include <SgRootNode.h>
#include <SgStageActorCollectionNode.h>
#include <SgStagePropCollectionNode.h>
#include "DfxIOUtil.h"
#include "DfxProjectState.h"
#include "DfxProjectHeader.h"
#include "DfxViewLayoutManager.h"
#include "DfxUtil.h"


namespace filesystem = std::experimental::filesystem;

class DfxProject: public QObject{
	Q_OBJECT
private:
	static DfxProject *_instance;
	DfxProjectState *_state;

	DfxProject();

private:
	static std::vector<deepsg::SgStageNode*> simulationStageList;

	std::string _projectPath;
private:

	void loadEverythingElse00(xvault::File *file);

	void saveGeometryResources(xvault::File *file);
	void loadGeometryResources(std::string, xvault::iResource *resource, void *userdata);

	void loadSpaceBindings(xvault::File *file);

	void loadSurfaceDictionary(xvault::File *file);

	void loadMaterialLinks(xvault::File *file);

	void loadCacheTracks(xvault::File *file);

	void loadFluentsFlows(xvault::File *file);
	fl::Flow *loadFluentsFlows(xvault::Node *node, bool isDocument);

	void mapAttributeToAny(xvault::Buffer *buffer, xvault::Attribute *attribute, fl::Any *any, unsigned int type);
	void loadSpace(xvault::Node *node, std::string parentPath, fl::Any *parentAny);
	void loadGraph(xvault::Node *node, deepsg::SgNode *sgnode);

public:
	
	virtual ~DfxProject();

	static DfxProject *getInstance();

	bool save(std::string path); /// create project and/or save scene to disk
	bool updateProject(); /// save scene to disk
	bool newProject(std::string path); /// create and set new project

	bool isProjectOpen(); /// check if project is already open

	bool load(std::string path);

	void setProjectPath(std::string);

	std::string projectPath();

signals:
	void finishedLoadingNewProject();
	void projectPathChanged();
};
