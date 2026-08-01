/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include <QObject>
#include <unordered_map>
#include <DfxGlobal.h>
#include <FlCommon.h>
#include "DfxGraphEditorModel.h"
#include "DfxGraphEditorChannel.h"
#include "DfxQColorList.h"

class DfxGraphEditorManager: public QObject {
	Q_OBJECT
private:
	static DfxGraphEditorManager *_instance;

	DfxGraphEditorChannel *_rootChannel;
	DfxGraphEditorChannel *_baseChannel;

	std::unordered_map<std::string, DfxGraphEditorChannel*> _channelMap;

	DfxGraphEditorModel *_model;

	DfxQColorList colors;

	DfxGraphEditorManager();

private:
	void addChannel(std::string path);

public:

	virtual ~DfxGraphEditorManager();

	static DfxGraphEditorManager *getInstance();

	DfxGraphEditorChannel *rootChannel();

	void addChannel(fl::Any *any, DfxGraphEditorChannel *parent);
	void removeChannel(std::string path);

	DfxGraphEditorModel *model();

	void notifyChannelUpdated();
	void notifyNewChannelCreated();

signals:
	void modelUpdated();
	void channelUpdated();
	void newChannelCreated();

public slots:
	void selectionChanged();
};