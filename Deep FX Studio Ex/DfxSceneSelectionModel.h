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
#include <vector>
#include <SgNode.h>
#include "DfxQPropertiesHub.h"

class DfxSceneSelectionModel: public QObject {
	Q_OBJECT
private:
	static DfxSceneSelectionModel *_instance;
private:
	std::vector<deepsg::SgNode*> _selected;

	DfxSceneSelectionModel();

	void notifyPropertiesHub();
public:

	static DfxSceneSelectionModel *getInstance();

	void addSelection(deepsg::SgNode *node);
	void removeSelection(deepsg::SgNode *node);
	void addRemoveSelection(deepsg::SgNode *node);
	bool isSelected(deepsg::SgNode *node);
	void clearSelection();

	std::vector<deepsg::SgNode*> &selected();

signals:
	void selectionChanged();
};