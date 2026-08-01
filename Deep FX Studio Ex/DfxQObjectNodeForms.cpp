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


#include "DfxQObjectNodeForms.h"

namespace form {
	QWidget *objectRenderForm(std::vector<deepsg::SgNode*> nodes) {
		QWidget *widget = new QWidget;

		QVBoxLayout *main_layout = new QVBoxLayout;

		auto basicControlPanel = new DfxControlPanel();

		DfxQControlArray *lensControlArray = new DfxQControlArray();

		std::string path = nodes[0]->referencePath() + "/";

		auto smooth_shading = new DfxQBoolControl(path + "smooth_shading", "", false);

		DfxQControlArray *basicControlArray = new DfxQControlArray();

		basicControlArray->addControl("Smooth Shading", smooth_shading);
	
		basicControlPanel->setTitle("Shading");
		basicControlPanel->setWidget(basicControlArray);

		main_layout->addWidget(basicControlPanel);

		widget->setLayout(main_layout);

		return widget;
	}
}