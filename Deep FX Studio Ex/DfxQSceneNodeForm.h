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

#include "DfxShapeNode.h"
#include "DfxRenderDelegateNode.h"
#include "DfxQSharedPropertyForms.h"
#include "DfxQObjectNodeForms.h"
#include "DfxSimulationStageBase.h"
#include "DfxSimulationStageActorBase.h"
#include "DfxSimulationStagePropBase.h"
#include "DfxPointCloudNode.h"
#include "DfxGeneratePointsTool.h"
#include "DfxRenderNodeBase.h"
#include "DfxNullHandler.h"
#include "DfxGridHandler.h"


class DfxSceneNodeForm : public DfxBaseForm {
private:
	QVBoxLayout *vbox;
	QToolBox *toolbox;

	QFormLayout *_nameLabelLayout;
	QLineEdit *_name;
	QLineEdit *_label;

	QTabWidget *_tab;

private:
	QWidget *objectPropertiesWidget(std::vector<deepsg::SgNode*> nodes);
	void styleTab(QTabWidget *_tab);

public:
	~DfxSceneNodeForm();
	void create(std::vector<deepsg::SgNode*> nodes);
	void destroy() override;
};