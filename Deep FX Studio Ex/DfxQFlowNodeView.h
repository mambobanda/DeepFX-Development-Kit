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
#include "DfxQFluentCollectionBase.h"
#include "DfxQStyleScrollBar.h"
#include "DfxQFlowCollection.h"
#include "DfxQNodeEditorCommon.h"
#include "DfxQFlowEnums.h"

class DfxQFlowNodeView : public QGraphicsView {
	Q_OBJECT
private:

	class LinkTemp {
	public:
		DfxQFlowBaseNode *source;
		DfxQFlowBaseNode *destination;
		DfxQFlowLink     *link;
	};

private:
	std::string _flowPath;
	LinkTemp linkTemp;
protected:

	void wheelEvent(QWheelEvent *event) override;
	bool eventFilter(QObject *o, QEvent *e) override;

	void notifyIfNodesAreSelected();

public:
	DfxQFlowNodeView();
	~DfxQFlowNodeView();

	QGraphicsItem* itemAt(const QPointF &pos);

	std::string flowPath();
	void setFlowPath(std::string);

	void updatePositions();

	void deleteSelected();
signals:
	void nodesSelected(std::vector<DfxQFlowBaseNode*> nodes);
};