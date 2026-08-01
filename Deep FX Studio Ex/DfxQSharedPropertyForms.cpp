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

#include "DfxQSharedPropertyForms.h"

namespace form {

	QWidget *xformWidget(std::vector<deepsg::SgNode*> nodes) {

		std::vector<std::pair<std::string, unsigned int>> xformQControlenums;

		xformQControlenums.push_back(std::pair<std::string, unsigned int>("XYZ", RotationOrder::XYZ));
		xformQControlenums.push_back(std::pair<std::string, unsigned int>("XZY", RotationOrder::XZY));
		xformQControlenums.push_back(std::pair<std::string, unsigned int>("YXZ", RotationOrder::YXZ));
		xformQControlenums.push_back(std::pair<std::string, unsigned int>("YZX", RotationOrder::YZX));
		xformQControlenums.push_back(std::pair<std::string, unsigned int>("ZXY", RotationOrder::ZXY));
		xformQControlenums.push_back(std::pair<std::string, unsigned int>("ZYX", RotationOrder::ZYX));

		QVBoxLayout *layout = new QVBoxLayout;
		QWidget *widget = new QWidget;

		auto translateControPanel = new DfxControlPanel();
		auto rotateControPanel = new DfxControlPanel();
		auto scaleControPanel = new DfxControlPanel();

		layout->setContentsMargins(0, 15, 0, 0);
		layout->setAlignment(Qt::AlignTop);

		std::string xf_path = nodes[0]->referencePath() + "/xform/";

		DfxQControlArray *control00 = new DfxQControlArray();
		control00->addControl("Position X", new DfxQDecimalSpinnerControl(xf_path + "x_pos", "", false));
		control00->addControl("Y", new DfxQDecimalSpinnerControl(xf_path + "y_pos", "", false));
		control00->addControl("Z", new DfxQDecimalSpinnerControl(xf_path + "z_pos", "", false));

		DfxQControlArray *control01 = new DfxQControlArray();
		control01->addControl("Rotate X", new DfxQDecimalSpinnerControl(xf_path + "x_rot", "", false));
		control01->addControl("Y", new DfxQDecimalSpinnerControl(xf_path + "y_rot", "", false));
		control01->addControl("Z", new DfxQDecimalSpinnerControl(xf_path + "z_rot", "", false));

		DfxQControlArray *control02 = new DfxQControlArray();
		control02->addControl("Scale X", new DfxQDecimalSpinnerControl(xf_path + "x_scl", "", false));
		control02->addControl("Y", new DfxQDecimalSpinnerControl(xf_path + "y_scl", "", false));
		control02->addControl("Z", new DfxQDecimalSpinnerControl(xf_path + "z_scl", "", false));

		DfxQComboBoxControl *rotationOrder = new DfxQComboBoxControl(xf_path + "rotation_type", "", xformQControlenums, false);

		control02->addSpacer(0, 10);

		control02->addControl("", rotationOrder);

		translateControPanel->setTitle("Position");
		translateControPanel->setWidget(control00);

		rotateControPanel->setTitle("Rotate");
		rotateControPanel->setWidget(control01);

		scaleControPanel->setTitle("Scale");
		scaleControPanel->setWidget(control02);

		layout->addWidget(translateControPanel);
		layout->addWidget(rotateControPanel);
		layout->addWidget(scaleControPanel);

		widget->setLayout(layout);

		return widget;
	}
}