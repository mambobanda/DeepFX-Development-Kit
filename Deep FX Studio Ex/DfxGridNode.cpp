/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include "DfxGridNode.h"
#include "DfxQStackControl.h"
#include "DfxQControlPanel.h"
#include "DfxQControlArray.h"
#include "DfxQBoolControl.h"
#include "DfxQCheckBoxControl.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQFluentPathControl.h"
#include "DfxQFilePathControl.h"
#include "DfxToolFlDispatcher.h"
#include "DfxPointCache.h"

std::string DfxGridNode::_uniqueName = "UID_GRID_NODE";

std::string DfxGridNode::uniqueName() {
	return _uniqueName;
}

std::string DfxGridNode::getUniqueName() {
	return _uniqueName;
}

void DfxGridNode::beginningEvaluatingTools() {
}

void DfxGridNode::finishedEvaluatingTools() {
	DfxGlobal::command::exec("updateScene");
}

void DfxGridNode::clear() {

}

std::vector<std::pair<QWidget*, QString>> DfxGridNode::propertyForms() {

	std::pair<QWidget*, QString> viz(vizForm(), "Visualization");
	std::pair<QWidget*, QString> render(renderForm(), "Rendering");

	return std::vector<std::pair<QWidget*, QString>> { viz, render };
}

QWidget *DfxGridNode::vizForm() {
	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto propertiesControlPanel = new DfxControlPanel();

	DfxQControlArray *propertiesControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// Type Controls
	auto voxel_pixel_size = new DfxQDecimalSpinnerControl(path + "voxel_pixel_size", "", false);
	auto transparency = new DfxQCheckBoxControl(path + "transparency", "", false);

	propertiesControlArray->addControl("Voxel Pixel Size", voxel_pixel_size);
	propertiesControlArray->addSpacer(1, 3);
	propertiesControlArray->addControl("Transparency", transparency);

	propertiesControlPanel->setTitle("Viewport");
	propertiesControlPanel->setWidget(propertiesControlArray);

	// Main layout
	main_layout->addWidget(propertiesControlPanel);

	widget->setLayout(main_layout);

	return widget;
}

QWidget *DfxGridNode::renderForm() {
	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto propertiesControlPanel = new DfxControlPanel();

	DfxQControlArray *propertiesControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// Type Controls
	auto use_render_hint = new DfxQCheckBoxControl(path + "use_render_hint", "", false);
	auto cull_out_of_bounds = new DfxQCheckBoxControl(path + "cull_out_of_bounds", "", false);

	auto material_id = new DfxQIntegerSpinnerControl(path + "material_id", "", false);

	auto voxel_size = new DfxQDecimalSpinnerControl(path + "voxel_size", "", false);
	auto width  = new DfxQIntegerSpinnerControl(path + "width", "", false);
	auto height = new DfxQIntegerSpinnerControl(path + "height", "", false);
	auto depth  = new DfxQIntegerSpinnerControl(path + "depth", "", false);
	
	propertiesControlArray->addControl(use_render_hint, "Use Render Hint");
	propertiesControlArray->addSpacer(1, 3);
	propertiesControlArray->addControl(cull_out_of_bounds, "Cull Out Of Bounds");
	propertiesControlArray->addSpacer(1, 6);
	propertiesControlArray->addControl("Material ID", material_id);
	propertiesControlArray->addSpacer(1, 6);
	propertiesControlArray->addControl("Voxel Size", voxel_size);
	propertiesControlArray->addSpacer(1, 3);
	propertiesControlArray->addControl("Width", width);
	propertiesControlArray->addSpacer(1, 3);
	propertiesControlArray->addControl("Height", height);
	propertiesControlArray->addSpacer(1, 3);
	propertiesControlArray->addControl("depth", depth);

	propertiesControlPanel->setTitle("Render Hint");
	propertiesControlPanel->setWidget(propertiesControlArray);

	// Main layout
	main_layout->addWidget(propertiesControlPanel);

	widget->setLayout(main_layout);

	return widget;
}