#pragma once

#include <QtWidgets\QGraphicsView>
#include <QtWidgets\QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QEvent>
#include "DfxQNodeCommon.h"
#include "DfxQNodeConnection.h"

class DfxQNodeView : public QGraphicsView {
	Q_OBJECT
private:
	QGraphicsScene *scene;
	DfxQNodeConnection *conn;
public:

	DfxQNodeView(QWidget *parent = Q_NULLPTR);

	bool eventFilter(QObject *o, QEvent *e); 
	
	QGraphicsItem* DfxQNodeView::itemAt(const QPointF &pos);
};