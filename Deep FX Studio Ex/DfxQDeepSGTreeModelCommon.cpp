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

#include "DfxQDeepSGTreeModelCommon.h"
#include "DfxObjectHandler.h"
#include "DfxPointCloudHandler.h"
#include "DfxLxCameraHandler.h"
#include "DfxLxRenderHandler.h"
#include "DfxGroupHandler.h"
#include "DfxGridHandler.h"
#include "DfxNullHandler.h"

QVariant DfxQDeepSGTreeModelCommon::data(const QModelIndex &index, int role) {

	if (!index.isValid())
		return QVariant();

	int ico_size = 18;

	if (role == Qt::DisplayRole) {

		deepsg::SgNode *item = static_cast<deepsg::SgNode*>(index.internalPointer());

		QString label = "N/A";

		if (index.column() == 0) {

			if (item->getType() == deepsg::SgNodeType::ROOT) {
				label = "Scene";
			}
			else if (item->getType() == deepsg::SgNodeType::GROUP) {
				DfxGroupHandler handler;
				bool rc = handler.fetch(item->referencePath());

				if (rc) {
					label = handler.dataObject()->label().c_str();
				}
				else {
					label = (std::string("Group 000 ")).c_str();
				}
			}
			else if (item->getType() == deepsg::SgNodeType::RENDER_DELEGATE) {

				auto any = DfxSpace::getInstance()->findAbsolute(item->referencePath());

				if (any) {
					label = any->label().c_str();
				}
				else {
					label = (std::string("Render Delegate ") + std::to_string(item->getID())).c_str();
				}

			}
			else if (item->getType() == deepsg::SgNodeType::NILL) {
				DfxNullHandler handler;
				bool rc = handler.fetch(item->referencePath());

				if (rc) {
					label = handler.dataObject()->label().c_str();
				}
				else {
					label = (std::string("Null 000 ")).c_str();
				}
			}
			else if (item->getType() == deepsg::SgNodeType::GEOMETRY) {
				DfxObjectHandler handler;
				bool rc = handler.fetch(item->referencePath());

				if (rc) {
					label = handler.dataObject()->label().c_str();
				}
				else {
					label = (std::string("Mesh Data ") + std::to_string(item->getID())).c_str();
				}
			}
			else if (item->getType() == deepsg::SgNodeType::POINT_CLOUD) {
				DfxPointCloudHandler handler;
				bool rc = handler.fetch(item->referencePath());

				if (rc) {
					label = handler.dataObject()->label().c_str();
				}
				else {
					label = (std::string("Point Cloud ") + std::to_string(item->getID())).c_str();
				}
			}
			else if (item->getType() == deepsg::SgNodeType::GRID) {
				DfxGridHandler handler;
				bool rc = handler.fetch(item->referencePath());

				if (rc) {
					label = handler.dataObject()->label().c_str();
				}
				else {
					label = (std::string("Grid") + std::to_string(item->getID())).c_str();
				}
			}
			else if (item->getType() == deepsg::SgNodeType::TOOL) {

				auto any = DfxSpace::getInstance()->findAbsolute(item->referencePath());

				if (any) {
					label = any->label().c_str();
				}
				else {
					label = (std::string("Tool ") + std::to_string(item->getID())).c_str();
				}
			}
			else if (item->getType() == deepsg::SgNodeType::SHAPE) {
				auto any = DfxSpace::getInstance()->findAbsolute(item->referencePath());

				if (any != nullptr) {
					label = any->label().c_str();
				}
				else {
					label = ((deepsg::SgShapeNode*)item)->name().c_str();
				}
			}
			else if (item->getType() == deepsg::SgNodeType::RENDER) {
				auto any = DfxSpace::getInstance()->findAbsolute(item->referencePath());

				if (any != nullptr) {
					label = any->label().c_str();
				}
				else {
					label = (std::string("Render Node ") + std::to_string(item->getID())).c_str();
				}
			}
			else if (item->getType() == deepsg::SgNodeType::STAGE) {
				auto any = DfxSpace::getInstance()->findAbsolute(item->referencePath());

				if (any != nullptr) {
					label = any->label().c_str();
				}
				else {
					label = (std::string("Simulation Stage") + std::to_string(item->getID())).c_str();
				}
			}
			else if (item->getType() == deepsg::SgNodeType::STAGE_ACTOR_COLLECTION) {
				label = "Actors";
			}
			else if (item->getType() == deepsg::SgNodeType::STAGE_ACTOR) {
				auto any = DfxSpace::getInstance()->findAbsolute(item->referencePath());

				if (any != nullptr) {
					label = any->label().c_str();
				}
				else {
					label = (std::string("Stage Actor") + std::to_string(item->getID())).c_str();
				}
			}
			else if (item->getType() == deepsg::SgNodeType::STAGE_PROP_COLLECTION) {
				label = "Props";
			}
			else if (item->getType() == deepsg::SgNodeType::STAGE_PROP) {
				auto any = DfxSpace::getInstance()->findAbsolute(item->referencePath());

				if (any != nullptr) {
					label = any->label().c_str();
				}
				else {
					label = (std::string("Stage Prop") + std::to_string(item->getID())).c_str();
				}
			}

			return QVariant(label);
		}
	}

	if (role == Qt::DecorationRole) {

		deepsg::SgNode *item = static_cast<deepsg::SgNode*>(index.internalPointer());

		if (index.column() == 0) {

			if (item->getType() == deepsg::SgNodeType::ROOT) {
				return QPixmap(":/icon/graph_icon").scaled(ico_size, ico_size);
			}
			else if (item->getType() == deepsg::SgNodeType::GROUP) {
				return QPixmap(":/icon/group_icon").scaled(ico_size, ico_size);
			}
			else if (item->getType() == deepsg::SgNodeType::NILL) {
				return QPixmap(":/icon/null_icon").scaled(ico_size, ico_size);
			}
			else if (item->getType() == deepsg::SgNodeType::GEOMETRY) {
				return QPixmap(":/icon/object_icon").scaled(ico_size, ico_size);
			}
			else if (item->getType() == deepsg::SgNodeType::POINT_CLOUD) {
				return QPixmap(":/icon/point_cloud_icon").scaled(ico_size, ico_size);
			}
			else if (item->getType() == deepsg::SgNodeType::GRID) {
				return QPixmap(":/icon/grid_icon").scaled(ico_size, ico_size);
			}
			else if (item->getType() == deepsg::SgNodeType::TOOL) {
				return QPixmap(":/icon/tool_node_icon").scaled(ico_size, ico_size);
			}
			else if (item->getType() == deepsg::SgNodeType::RENDER) {
				return QPixmap(":/icon/render_node_icon").scaled(ico_size, ico_size);
			}
			else if (item->getType() == deepsg::SgNodeType::SHAPE) {

				auto shapeNode = (deepsg::SgShapeNode*)item;

				if (shapeNode->shapeType() == DfxItemWidgetType::UNDEFINED) {
					return QPixmap(":/icon/undefined_shape_icon").scaled(ico_size, ico_size);
				}
				else if (shapeNode->shapeType() == DfxItemWidgetType::POINT_LIGHT ||
					shapeNode->shapeType() == DfxItemWidgetType::ENVIRONMENT_LIGHT ||
					shapeNode->shapeType() == DfxItemWidgetType::SUN_SKY_LIGHT ||
					shapeNode->shapeType() == DfxItemWidgetType::SPOT_LIGHT ||
					shapeNode->shapeType() == DfxItemWidgetType::AREA_LIGHT) {

					return QPixmap(":/icon/light_icon").scaled(ico_size, ico_size);
				}
				else if (shapeNode->shapeType() == DfxItemWidgetType::CAMERA) {
					return QPixmap(":/icon/camera_icon").scaled(ico_size, ico_size);
				}

			}
			else if (item->getType() == deepsg::SgNodeType::STAGE) {
				return QPixmap(":/icon/sim_stage_icon").scaled(ico_size, ico_size);
			}
			else if (item->getType() == deepsg::SgNodeType::STAGE_ACTOR_COLLECTION) {
				return QPixmap(":/icon/sim_actors_icon").scaled(ico_size, ico_size);
			}
			else if (item->getType() == deepsg::SgNodeType::STAGE_ACTOR) {
				return QPixmap(":/icon/sim_actor_icon").scaled(ico_size, ico_size);
			}
			else if (item->getType() == deepsg::SgNodeType::STAGE_PROP_COLLECTION) {
				return QPixmap(":/icon/sim_props_icon").scaled(ico_size, ico_size);
			}
			else if (item->getType() == deepsg::SgNodeType::STAGE_PROP) {
				return QPixmap(":/icon/sim_prop_icon").scaled(ico_size, ico_size);
			}
		}
		else if (index.column() == 1) {

			if (item->isVisible() && item->getType() != deepsg::SgNodeType::ROOT) {
				return QPixmap(":/layers_tb/layer_visibility_icon").scaled(ico_size, ico_size);
			}
			else if(item->getType() != deepsg::SgNodeType::ROOT){
				return QPixmap(":/layers_tb/layer_visibility_inactive_icon").scaled(ico_size, ico_size);
			}
		}
		else if (index.column() == 2) {

			if (item->isRenderable() && item->getType() != deepsg::SgNodeType::ROOT) {
				return QPixmap(":/icon/node_renderable_icon").scaled(ico_size, ico_size);
			}
			else if (item->getType() != deepsg::SgNodeType::ROOT) {
				return QPixmap(":/icon/node_renderable_inactive_icon").scaled(ico_size, ico_size);
			}
		}
	}

	if(index.column() == 1 && role == Qt::TextAlignmentRole) {
		return Qt::AlignCenter;
	}

	return QVariant();
}

deepsg::SgNode *DfxQDeepSGTreeModelCommon::chilAtIndex(int row, const QModelIndex &parent){
	deepsg::SgNode *parentItem = nullptr;

	if (!parent.isValid())
		parentItem = deepsg::SgRootNode::getInstance()->root();
	else
		parentItem = static_cast<deepsg::SgNode*>(parent.internalPointer());

	deepsg::SgNode *childItem = parentItem->childAt(row);

	if (childItem != nullptr) {
		return childItem;
	}
	else
		return nullptr;
}

deepsg::SgNode *DfxQDeepSGTreeModelCommon::childParent(const QModelIndex &index){

	if (!index.isValid())
		return nullptr;

	deepsg::SgNode *childItem = static_cast<deepsg::SgNode*>(index.internalPointer());
	deepsg::SgNode *parentItem = childItem->parent();

	if (parentItem == nullptr || parentItem->getID() == deepsg::SgRootNode::getInstance()->root()->getID())
		return nullptr;

	return parentItem;
}

int DfxQDeepSGTreeModelCommon::rowCount(const QModelIndex &parent) {

	deepsg::SgNode  *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = deepsg::SgRootNode::getInstance()->root();
	else
		parentItem = static_cast<deepsg::SgNode*>(parent.internalPointer());

	return parentItem->children().size();
}

int DfxQDeepSGTreeModelCommon::columnCount(const QModelIndex &parent){
	return 1;
}