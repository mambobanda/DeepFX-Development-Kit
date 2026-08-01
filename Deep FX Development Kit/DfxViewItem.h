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

#include <QOpenGLFunctions_2_1>
#include <glm\geometric.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\vec3.hpp>
#include <unordered_map>
#include "DfxColorCounter.h"
#include "DfxColorID.h"
#include "DfxBounds.h"
#include "DfxEntityDB.h"
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxEntity.h"
#include "DfxRotation.h"


class DfxViewItem : public DfxEntity, protected QOpenGLFunctions_2_1 {
private:
	static DfxColorCounter color_id_counter;
	static std::unordered_map<std::string, DfxViewItem*> color_map;
private:
	bool selected;
	bool hidden;
protected:
	DfxColorID colorID;
public:
	static const DfxPropertyID __meta_type__	= 1000;
	static const DfxPropertyID item_id			= 1001;
	static const DfxPropertyID item_type		= 1002;
	static const DfxPropertyID position			= 1003;
	static const DfxPropertyID rotation			= 1004;
	static const DfxPropertyID scale			= 1005;
	static const DfxPropertyID rotationOrder	= 1006;
	
public:

	DfxViewItem(size_t itemType, DfxGlobalEntityName uniquename);
	virtual ~DfxViewItem();

	virtual void compute() override;

	virtual void onDataChanged(DfxEntityDataInstance *instance) override;

	virtual void draw() = 0;

	virtual void drawSelection();

	virtual DfxBounds bounds() = 0;

	virtual void calculateBounds() = 0;

	virtual bool selectable() = 0;

	virtual bool preprocessViewItem();

	virtual void preprocess();

	virtual size_t getItemType();

	virtual bool isRenderable();

	bool isSelected();

	void setSelected(bool);

	bool isHidden();

	void hide(bool);

	void setPosition(glm::vec3 pos);
	glm::vec3 getPosition();

	void setRotation(glm::vec3 rot);
	glm::vec3 getRotation();

	unsigned int getRotationOrder();
	void setRotationOrder(unsigned int);

	void setScale(glm::vec3 scl);
	glm::vec3 getScale();

	static void initialize();

	glm::mat4 transformationMatrix();

	DfxColorID getColorID();

	static DfxViewItem *findViewItemByColorID(DfxColorID cid);
};