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
#include <QObject>
#include <SgWidgetNode.h>
#include <SgTransformNode.h>
#include <DfxDeepSimplexGeoAdapter.h>
#include <SgRootNode.h>
#include <QItemSelectionModel>
#include "DfxSceneSelectionModel.h"
#include "DfxQDeepSGTreeModel.h"


class DfxQSceneManager : public QObject {
	Q_OBJECT
private:
	static DfxQSceneManager *_instance;
	DfxQDeepSGTreeModel *_model;
	QItemSelectionModel *_selectionModel;

	bool ignoreSceneSelectionModelSignal;
	bool ignoreModelSelectionModelSignal;

	DfxQSceneManager();
private:
	void removeAll(deepsg::SgNode *node);
public:
	
	virtual ~DfxQSceneManager();

	static DfxQSceneManager *getInstance();

	void moveNode(deepsg::SgNode *node, deepsg::SgNode *to);
	void removeNode(deepsg::SgNode *node);
	void toggleVisibilty(deepsg::SgNode *node);
	void toggleRenderable(deepsg::SgNode *node);
	void removeAll();

	DfxQDeepSGTreeModel *model();
	QItemSelectionModel *selectionModel();

signals:
	void sceneUpdated();

public slots:
	void modelSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
	void sceneSelectionChanged();
};