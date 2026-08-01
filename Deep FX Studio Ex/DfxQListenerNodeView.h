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
#include "DfxQStyleScrollBar.h"
#include "DfxQListenerCollection.h"


class DfxQListenerNodeView : public QGraphicsView {
	Q_OBJECT
private:

protected:
	void wheelEvent(QWheelEvent *event) override; 
	bool eventFilter(QObject *o, QEvent *e) override;

public:
	DfxQListenerNodeView();
	~DfxQListenerNodeView();

	QGraphicsItem* itemAt(const QPointF &pos);

	void deleteSelected();
};