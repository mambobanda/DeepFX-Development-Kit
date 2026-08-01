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

#include <QtSvg/QGraphicsSvgItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <vector>
#include <QFont>
#include <QLabel>
#include <QLineEdit>
#include "DfxQEnumControl.h"
#include "DfxQDecimal3SpinnerControl.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQDirectoryPathControl.h"
#include "DfxQFilePathControl.h"
#include "DfxEntity.h"
#include "DfxQNodeCommon.h"
#include "DfxQNodeSlot.h"
#include "DfxQStringControl.h"
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxQNodePropertiesPanel.h"
#include "DfxQNodeType.h"
#include "DfxEnvelopeMap.h"


class DfxQBaseNode : public QGraphicsPathItem {
protected:
	std::vector<DfxQNodeSlot*> nodeSlots;

	QGraphicsProxyWidget *propertiesForm;

	DfxEntity *entity;
	DfxQNodeType nodeType;
	void *referenceLayer;

	float prefferedWidth;
	float prefferedHeight;

protected:
	bool slotIsArray(DfxEntity *ent, size_t property);

public:
	enum { Type = QGraphicsItem::UserType + 1 };

	DfxQBaseNode(DfxEntity *entity, DfxQNodeType type, QGraphicsItem *parent = nullptr);

	virtual ~DfxQBaseNode();

	int type() const;

	size_t ID();

	DfxEntity *getEntity();

	DfxQNodeType getNodeType();

	void DfxQBaseNode::removeConnections();

	void *getLayerReference();

	void setLayerReference(void *);

	void cleanEntity();

	std::vector<DfxQNodeSlot*> getAllSlots();

	DfxQNodeSlot *findSlot(size_t);

	QGraphicsProxyWidget *buildPropertiesForm();

	void destroyPropertiesForm();

	bool hasPropertiesForm();

	QColor workOutSlotColor(DfxEntity &, size_t id);

	bool hasSlot(size_t );

	virtual void rebuild();

	void setPrefferedWidth(float);
	float getPrefferedWidth();

	void setPrefferedHeight(float );
	float getPrefferedHeight();
};