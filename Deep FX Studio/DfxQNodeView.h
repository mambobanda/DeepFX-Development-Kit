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

#include <QtWidgets\QGraphicsView>
#include <QtWidgets\QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QEvent>
#include <QPoint>
#include <QLine>
#include <QMenu>
#include <QAction>
#include <QTransform>
#include <string>
#include <QCursor>
#include <QScrollbar>
#include <DfxGlobal.h>
#include <DfxLayerManager.h>
#include <QGraphicsScene>
#include <unordered_map>
#include "DfxGlobal.h"
#include "DfxQNodeCommon.h"
#include "DfxQNodeConnection.h"
#include "DfxLayer.h"
#include "DfxNodeViewCopyPaste.h"
#include "DfxQHierarchicalLayoutAlpha.h"

class DfxQNodeView : public QGraphicsView {
	Q_OBJECT
private:
	QGraphicsScene *scene;
	DfxQNodeConnection *conn;
	DfxQNodeConnection *disConn;
	QPointF disConnPos;
	DfxQBaseNode *propertyPanelPtr;
	DfxNodeViewCopyPaste copyPaste;

	bool rightMouseButtonDown;
	int rightMouseButtonStartX;
	int rightMouseButtonStartY;

	DfxQNodeSlot *selectedSlot;
protected:
	void wheelEvent(QWheelEvent *event) override;
	void moveToTop(DfxQBaseNode *);
public:

	DfxQNodeView(QWidget *parent = Q_NULLPTR);

	bool eventFilter(QObject *o, QEvent *e); 
	
	QGraphicsItem* DfxQNodeView::itemAt(const QPointF &pos);

	QGraphicsScene *getScene();

	void fitAllNodes();
	void uniformLayout(bool everything = false);

	// Edit Action
	QAction *copySelectedAction;
	QAction *cutSelectedAction;
	QAction *renameNodeAction;
	QAction *pasteSelectedAction;
	QAction *deleteSelectedAction;
	QAction *selectAllAction;
	QAction *exportNodeAction;

	//Prototype Actions
	QAction *createRangedLoopAction;

	// Slot Action
	QAction *liftSlotAction;
	QAction *dropSlotAction;

public slots:

	void renameSelected();
	void copySelected();
	void cutSelected();
	void pasteSelected();
	void deleteSelected();
	void selectAll();
	void updateViewableLayer(DfxLayerItem *);
	void setSceneToNull();
	void exportSelectedNodes();
	void previousLayer();
	void homeLayer();

	void liftSlot();
	void dropSlot();

	//Prototype
	void createRangedLoop();

	// Graph UI Functions
	std::vector<DfxEntity*> getNodesSelectedInGraph();
	
};