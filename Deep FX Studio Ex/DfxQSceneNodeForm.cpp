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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxQSceneNodeForm.h"

DfxSceneNodeForm::~DfxSceneNodeForm() {
	
}

void DfxSceneNodeForm::create(std::vector<deepsg::SgNode*> nodes) {

	vbox = new QVBoxLayout;

	_name  = new QLineEdit();
	_label = new QLineEdit();
	_nameLabelLayout = new QFormLayout();
	_tab = new QTabWidget();

	styleTab(_tab);
	_tab->tabBar()->setUsesScrollButtons(true);

	// _nameLabelLayout->addRow("Name: ", _name);
	// _nameLabelLayout->addRow("Label: ", _label);

	vbox->addLayout(_nameLabelLayout);
	vbox->addWidget(_tab);

	QTabWidget *tab;
	if (nodes.size() > 0 && nodes[0]->getType() != deepsg::SgNodeType::ROOT) {

		if (nodes[0]->xFormProperties() != nullptr ) {
			auto xformTab = form::xformWidget(nodes);
			_tab->addTab(xformTab, "Transform");
		}

		if (nodes[0]->getType() == deepsg::SgNodeType::GEOMETRY) {
			auto geoNode = (DfxGeometryNode*)nodes[0];
			for (auto form : geoNode->propertyForms()) {
				_tab->addTab(form.first, form.second);
			}
		}
		if (nodes[0]->getType() == deepsg::SgNodeType::GRID) {
			auto gridNode = (DfxGridNode*)nodes[0];
			for (auto form : gridNode->propertyForms()) {
				_tab->addTab(form.first, form.second);
			}
		}
		else if (nodes[0]->getType() == deepsg::SgNodeType::NILL) {
			auto nullNode = (DfxNullNode*)nodes[0];
			for (auto form : nullNode->propertyForms()) {
				_tab->addTab(form.first, form.second);
			}
		}
		else if (nodes[0]->getType() == deepsg::SgNodeType::RENDER_DELEGATE) {
			auto nullNode = (DfxRenderDelegateNode*)nodes[0];
			for (auto form : nullNode->propertyForms()) {
				_tab->addTab(form.first, form.second);
			}
		}
		else if (nodes[0]->getType() == deepsg::SgNodeType::SHAPE) {
			auto shapeNode = (DfxShapeNode*)nodes[0];

			for (auto form : shapeNode->propertyForms()) {
				_tab->addTab(form.first, form.second);
			}
		}
		else if (nodes[0]->getType() == deepsg::SgNodeType::STAGE) {
			auto simStage = (DfxSimulationStageBase*)nodes[0];

			for (auto form : simStage->propertyForms()) {
				_tab->addTab(form.first, form.second);
			}
		}
		else if (nodes[0]->getType() == deepsg::SgNodeType::STAGE_ACTOR) {
			auto simActor = (DfxSimulationStageActorBase*)nodes[0];

			for (auto form : simActor->propertyForms()) {
				_tab->addTab(form.first, form.second);
			}
		}
		else if (nodes[0]->getType() == deepsg::SgNodeType::STAGE_PROP) {
			auto simProp = (DfxSimulationStagePropBase*)nodes[0];

			for (auto form : simProp->propertyForms()) {
				_tab->addTab(form.first, form.second);
			}
		}
		else if (nodes[0]->getType() == deepsg::SgNodeType::POINT_CLOUD) {
			auto pointCloud = (DfxPointCloudNode*)nodes[0];

			for (auto form : pointCloud->propertyForms()) {
				_tab->addTab(form.first, form.second);
			}
		}
		else if (nodes[0]->getType() == deepsg::SgNodeType::TOOL) {
			auto simProp = (DfxToolNode*)nodes[0];

			for (auto form : simProp->propertyForms()) {
				_tab->addTab(form.first, form.second);
			}
		}
		else if (nodes[0]->getType() == deepsg::SgNodeType::RENDER) {
			auto renderNode = (DfxRenderNodeBase*)nodes[0];

			for (auto form : renderNode->propertyForms()) {
				_tab->addTab(form.first, form.second);
			}
		}
			
	}

	setLayout(vbox);
}

void DfxSceneNodeForm::destroy() {

}

void DfxSceneNodeForm::styleTab(QTabWidget *tab) {
	tab->setStyleSheet(tab->styleSheet() +
		"QTabWidget::pane { border: 0; }");

	tab->setStyleSheet(tab->styleSheet() +
		"QTabBar::tab{height: 17px; }");
}

QWidget *DfxSceneNodeForm::objectPropertiesWidget(std::vector<deepsg::SgNode*> nodes) {
	/*DfxObjectHandler handler;
	auto rc = handler.fetch(nodes[0]->referencePath());

	if (rc) {
		_tab->addTab(new QWidget(), "Render");
	}*/

	return new QWidget;
}


