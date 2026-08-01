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

#include <QObject>
#include <QVector>
#include <DfxEntity.h>
#include <DfxViewItem.h>
#include <DfxCSimulationEntity.h>
#include <DfxQLayerModel.h>
#include <DfxScene.h>
#include <stack>
#include <random>
#include "DfxGroupNode.h"
#include "DfxLayer.h"
#include "DfxRangedLoopNode.h"
#include "DfxQRangedLoopNode.h"


class DfxLayerManager: public QObject{
	Q_OBJECT
private:
	static DfxLayerManager *instance;

	DfxLayerManager();

	bool use_maximise_scene;
	void maximiseScene(QGraphicsScene *, DfxQBaseNode *centerOn);
private:
	DfxLayerItem *root;
	DfxLayerItem *selectedLayer;
	DfxQLayerModel *model;

	unsigned int level;
public:

	virtual ~DfxLayerManager();

	static  DfxLayerManager *getInstance();

	DfxLayerItem *getRoot();

	DfxQLayerModel *getModel();

	size_t layerCount();

	DfxLayerItem *getLayerItem(size_t);

	DfxLayerItem *getSelectedLayer();

	QGraphicsScene *getScene(size_t);

	void moveLayerUp();

	void moveLayerDown();
	
	void insertNode(DfxLayerItem *item, DfxLayerItem *layer);

	DfxLayerItem *findNodeByOldID(size_t);

	DfxLayerItem *findNodeByID(size_t id);

	void addViewNode(DfxViewItem *);

	void addNode(DfxEntity *);

	void addCompositeNode(DfxEntity *);

	DfxLayerItem *addGroupNode(DfxEntity *);

	DfxLayerItem *addImportNode(DfxLayerItem *item, DfxLayerItem *parent);

	DfxLayerItem *addRangedLoop(std::vector<DfxEntity*> delegates, QRectF boundingRect);

	DfxLayerItem *addNewLayer();

	void switchSelectedLayer(DfxLayerItem *item);

	bool previousLayer();

	bool homeLayer();

	void removeLayer();

	void removeLayer(size_t );

	void removeNode(DfxQBaseNode *, bool cleanEntity=true);

	void removeimportNode();

	std::vector<DfxLayerItem*> findImportNodes(DfxQBaseNode *);

	DfxLayerItem *findLayer(DfxQBaseNode *);

	DfxLayerItem *getLayer(size_t index);

	std::vector<DfxViewItem*> findAllViewItems();

	void renameLayer(std::string str, DfxLayerItem *);

	void clear();

	void reset();

	void resetSelectedLayer();

	void setUseMaximiseScene(bool );

	void forceMaximise();
	
	void incrementLevel();

	void decrementLevel();

	void updateView();

	unsigned int getLevel();
signals:
	void layerSelected(DfxLayerItem *);
	void levelUpdated(unsigned int);
	void removingLayer();
	void removingLayerDone();
public slots:

	void updateState(const QModelIndex &index);
};