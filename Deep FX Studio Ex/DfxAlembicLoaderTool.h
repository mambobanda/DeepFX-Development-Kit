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

#include <DfxGlobal.h>
#include <DfxToolNode.h>
#include <filesystem>
#include "DfxQStackControl.h"
#include "DfxQControlPanel.h"
#include "DfxQControlArray.h"
#include "DfxQBoolControl.h"
#include "DfxQCheckBoxControl.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQStringComboBoxControl.h"
#include "DfxQFilePathControl.h"
#include "DfxQLineEditControl.h"
#include "DfxToolFlDispatcher.h"
#include "DfxAlembicIO.h"

namespace filesystem = std::experimental::filesystem;

class DfxAlembicLoaderTool : public DfxToolNode {
private:
	static std::string _uniqueName;
	QWidget *form();
	fl::Dispatcher *_dispatcher;
	//std::vector<std::pair<std::string, unsigned int>> typeQControlenums;
	deepsg::SgGeoNode *_parent;
	bool _active;
protected:
	virtual void onSetReferencePath(std::string path) override;
	
public:

	DfxAlembicLoaderTool();
	virtual ~DfxAlembicLoaderTool();

	std::string uniqueName();
	static std::string getUniqueName();

	bool isReady() override;
	bool evaluateOnUpdate() override;
	bool evaluateOnNewFrame() override;
	bool evaluateOnValueChanged() override;
	void evaluate(SgNode *parent) override;
	void create(SgNode *parent) override;
	void destroy() override;

	std::vector<std::pair<QWidget*, QString>> propertyForms() override;

};