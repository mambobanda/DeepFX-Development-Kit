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
#include <SgPointCloudNode.h>
#include <DfxBounds.h>
#include "DfxInstancerBase.h"
#include "DfxQStackControl.h"
#include "DfxQControlPanel.h"
#include "DfxQControlArray.h"
#include "DfxQBoolControl.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQFluentPathControl.h"
#include "DfxToolFlDispatcher.h"
#include "DfxInstanceManager.h"
#include "DfxQCheckBoxControl.h"

class DfxPointAttributeInstancerTool : public DfxToolNode, public DfxInstancerBase {
private:

	QWidget *form();
	fl::Dispatcher *_dispatcher;
	ds::PointCloud *_pointCloud;

	ds::Channel *position;
	ds::Channel *positionOverride;

	ds::Channel *rotationOverride;
	ds::Channel *scaleOverride;

	ds::Channel *useTransformMatrixOverride;

	ds::Channel *hasRotationOverride;
	ds::Channel *hasTransformOverride;
	ds::Channel *hasScaleOverride;

	ds::Channel *instanceShapePath;
	ds::Channel *transformOverride;

	ds::Channel *_hidden;

	std::vector<deepsg::SgNode*> _shapeNodes;
	std::vector<DfxBoundsMinMax> _shapeAABBs;

	static std::string _uniqueName;

	bool useBasePosition;
	unsigned int _proxyType;
	bool _enable;
	unsigned int _limit;

	std::vector<std::pair<std::string, unsigned int>> renderModeEnums;

protected:
	virtual void onSetReferencePath(std::string path) override;

public:
	struct RenderMode { enum { AABB = 0, SHAPES = 1 }; };

public:

	DfxPointAttributeInstancerTool();
	virtual ~DfxPointAttributeInstancerTool();

	std::string uniqueName() override;
	static std::string getUniqueName();

	// Tool Overrides
	bool isReady() override;
	bool evaluateOnUpdate() override;
	bool evaluateOnNewFrame() override;
	bool evaluateOnValueChanged() override;
	void evaluate(SgNode *parent) override;
	void create(SgNode *parent) override;
	void destroy() override;

	std::vector<std::pair<QWidget*, QString>> propertyForms() override;

	// Instancing Overrides
	void begin() override;
	void end() override;

	size_t instanceCount() override;

	bool useTransformMatrix(size_t index) override;

	glm::vec3 instancePosition(size_t index) override;
	glm::vec3 instanceRotation(size_t index) override;
	glm::vec3 instanceScale(size_t index)  override;

	bool isEnabled() override;
	bool instanceHasRotation(size_t index) override;
	bool instanceHasScale(size_t index)  override;
	bool instanceHasTransform(size_t index) override;
	bool instanceHasShape(size_t index) override;

	unsigned int limit();

	glm::mat4 instanceTransform(size_t index) override;
	glm::mat4 finalTransform(size_t index) override;

	bool hidden(size_t index) override;

	unsigned int instanceProxyType() override;
	deepsg::SgNode *instanceShape(size_t index) override;
	DfxBoundsMinMax instanceShapeAABB(size_t index) override;
};