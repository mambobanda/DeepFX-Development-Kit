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

#include <RJDocument.h>
#include <RJNode.h>
#include <VH5Attribute.h>
#include <VH5DocumentResource.h>
#include <VH5File.h>
#include <QMessageBox> 
#include <QSettings>
#include <QFileDialog>
#include "Resource.h"
#include "ResourceManager.h"

class ProjectManager: public QObject {
	Q_OBJECT
private:

	static ProjectManager *_instance;

	xvault::Document *resourcesToVDocument(xvault::Node *node, Resource *resource, unsigned int uid);
	xvault::Document *buildToVDocument();
	
	void loadResources(xvault::Node *resource_node,  Resource*resource);

	ProjectManager();
public:
	
	virtual ~ProjectManager();

	static ProjectManager *getInstance();

	void save();
	void load();

signals:
	void projectChanged();
};