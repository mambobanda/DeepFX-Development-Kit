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

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <DfxGlobal.h>
#include <FlAnyHelpers.h>
#include "DfxQNodeEditorCommon.h"
#include "DfxQStyleScrollBar.h"
#include "DfxQComputeNodeSlot.h"
#include "DfxQComputePropertyLink.h"
#include "DfxQComputeNodeCollection.h"
#include "DfxSpaceUtil.h"
#include "DfxQComputePopupMenu.h"
#include "DfxComputeNodeDB.h"


class DfxQComputeNodeView : public QGraphicsView {
	Q_OBJECT
private:
	class LinkTemp {
	public:
		DfxQComputeNodeSlot *source;
		DfxQComputeNodeSlot *destination;
		DfxQComputePropertyLink  *link;
	};

private:
	LinkTemp linkTemp;
	DfxQComputePopupMenu *_nodePopupMenu;
	DfxFluentNodeEditor *_nodeEditor;

	DfxQComputePropertyLink *disconnectionLink;
	QPointF disconnectionLinkPos;
protected:

	void wheelEvent(QWheelEvent *event) override;
	bool eventFilter(QObject *o, QEvent *e) override;

	void notifyIfNodesAreSelected();

	void enterEvent(QEvent *event) override;
public:
	DfxQComputeNodeView(DfxFluentNodeEditor *nEditor = nullptr);
	~DfxQComputeNodeView();

	QGraphicsItem* itemAt(const QPointF &pos);

	
	void deleteSelected();
	void deleteLinks(std::vector <DfxQComputePropertyLink*>);

	void viewFitAll();
signals:
	void nodesSelected(std::vector<DfxQComputeBaseNode*> nodes);
};