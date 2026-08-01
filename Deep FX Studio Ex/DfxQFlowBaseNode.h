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

#include <QGraphicsPathItem>
#include <libavoid/libavoid.h>
#include "DfxQFlowEnums.h"
#include "DfxQFlowLink.h"

class DfxQFlowBaseNode : public QGraphicsPathItem {
private:
	static size_t UID_GEN;
	size_t _uid;
protected:

	std::vector<DfxQFlowLink*> _links;

	Avoid::Router   *_router;
	Avoid::ShapeRef *_shapeRef;

	std::string _flowPath;

	bool initialPaint;
private:
	int _fid;

protected:
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
public:

	DfxQFlowBaseNode(Avoid::Router *rt, QGraphicsItem *parent = nullptr);
	virtual ~DfxQFlowBaseNode();

	size_t uid();

	int fid();
	void setFid(int);

	virtual int type() const = 0;
	virtual QPointF center() const = 0;
	virtual void removeFromFlow()  = 0;

	std::string flowPath();
	void setFlowPath(std::string);
	DfxQFlowLink *findLinkWithSrc(DfxQFlowBaseNode*, uint srcState);
	void addLink(DfxQFlowLink*);
	void removeLink(DfxQFlowLink*);

	void removeAndUnbindAllLinks();

	Avoid::ShapeRef *avoidShape();

	void updateLinkPositions();

	virtual QPointF normalConnectorPos();
	virtual QPointF yesConnectorPos();
	virtual QPointF noConnectorPos();
	virtual QPointF inputConnectorPos();
};