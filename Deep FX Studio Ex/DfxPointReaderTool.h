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
#include <regex>
#include <Partio.h>
#include "DfxTimelineManager.h"
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

namespace filesystem = std::experimental::filesystem;

class DfxPointReaderTool : public DfxToolNode {
private:

	QWidget *form();
	fl::Dispatcher *_dispatcher;
	std::vector<std::pair<std::string, unsigned int>> typeQControlenums;

	static std::string _uniqueName;
protected:
	virtual void onSetReferencePath(std::string path) override;

private:
	std::unordered_map<std::string, std::string> fileCacheMap;
	DfxPointFrame *getFileCacheFrame(int frm);
	bool loadCache();

	std::string current_path;
public:

	DfxPointReaderTool();
	virtual ~DfxPointReaderTool();

	std::string uniqueName() override;
	static std::string getUniqueName();

	bool isReady() override;
	bool evaluateOnUpdate() override;
	bool evaluateOnNewFrame() override;
	bool evaluateOnValueChanged() override;
	void evaluate(SgNode *parent) override;
	void create(SgNode *parent) override;
	void destroy() override;

	std::vector<std::string> getDirectoryFilenames(std::string dirpath);
	int getFileIncrement(const std::string s);
	void getPreviewColor(float speed, float limit, glm::vec3 primary, glm::vec3 secondary, glm::vec3 &out);

	std::vector<std::pair<QWidget*, QString>> propertyForms() override;
};