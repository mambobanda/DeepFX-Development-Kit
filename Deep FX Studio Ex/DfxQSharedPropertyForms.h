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

#include <QWidget>
#include <vector>
#include <SgNode.h>
#include <SgNodeType.h>
#include <QBoxLayout>
#include <QToolbox.h>
#include <QLineEdit>
#include <QTabWidget>
#include <QFormlayout>
#include <QScrollArea>
#include <QSpacerItem>
#include <QGroupBox>
#include <QToolBox>
#include <QTabBar>
#include <QComboBox>
#include "DfxObjectHandler.h"
#include "DfxBaseForm.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQControlArray.h"
#include "DfxQControlPanel.h"
#include "DfxItemWidgetType.h"


//static const std::string SgRotationTypeStr[]{
//		"XYZ",
//		"XZY",
//		"YXZ",
//		"YZX",
//		"ZXY",
//		"ZYX",
//		"QUATERNION"
//};
namespace form {
	struct RotationOrder { enum { XYZ = 0, XZY = 1, YXZ = 2, YZX = 3, ZXY = 4, ZYX = 5, QUATERNION = 6 }; };
	QWidget *xformWidget(std::vector<deepsg::SgNode*> nodes);
}