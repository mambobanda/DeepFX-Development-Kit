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

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <DfxGlobal.h>
#include "DfxQStyleScrollBar.h"
#include "DfxQMaterialNodeSlot.h"
#include "DfxQMaterialPropertyLink.h"
#include "DfxQMaterialNodeCollection.h"
#include "DfxUpdater.h"


class DfxQMaterialNodeView : public QGraphicsView {
	Q_OBJECT
private:
	class LinkTemp {
	public:
		DfxQMaterialNodeSlot *source;
		DfxQMaterialNodeSlot *destination;
		DfxQMaterialPropertyLink  *link;
	};

private:
	LinkTemp linkTemp;
	DfxQMaterialPropertyLink  *disconnectionLink;
	QPointF disconnectionLinkPos;
protected:

	void wheelEvent(QWheelEvent *event) override;
	bool eventFilter(QObject *o, QEvent *e) override;

	void notifyIfNodesAreSelected();
public:

	DfxQMaterialNodeView();
	~DfxQMaterialNodeView();

	QGraphicsItem *itemAt(const QPointF &pos);

	void viewFitAll();

	void deleteLinks(std::vector<DfxQMaterialPropertyLink*> links);
	void deleteSelected();
signals:
	void nodesSelected(std::vector<DfxQMaterialBaseNode*> nodes);
};