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

#include <QGraphicsPathItem>
#include "DfxQMaterialNodeEnums.h"
#include "DfxQMaterialNodeProperty.h"


class DfxQMaterialBaseNode : public QGraphicsPathItem {
protected:
	static size_t ID_GEN;

	std::vector<DfxQMaterialNodeProperty*> nslots;
	size_t _nid;

protected:
	std::string _dataObjectPath;

public:
	enum { Type = DfxQMaterialNodeEnums::BASE_NODE };

	DfxQMaterialBaseNode(QGraphicsItem *parent = nullptr);
	~DfxQMaterialBaseNode();

	int type() const;
	size_t nodeID();

	std::string dataObjectPath();
	void setDataObjectPath(std::string);

	void forcePositionUpdate();

	std::vector <DfxQMaterialNodeProperty*> properties();
};
