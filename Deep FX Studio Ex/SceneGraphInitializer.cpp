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

#include "DfxSgNodeDB.h"

#include "DfxSimulationStageCommon.h"

#include "DfxGroupHandler.h"

#include "DfxObjectHandler.h"

#include "DfxNullHandler.h"

#include "DfxOSPRayRenderHandler.h"
#include "DfxOSPRayCameraHandler.h"
#include "DfxOSPRaySunSkyLightHandler.h"

#include "DfxLxRenderHandler.h"
#include "DfxLxCameraHandler.h"
#include "DfxLxEnvironmentLightHandler.h"
#include "DfxLxPointLightHandler.h"
#include "DfxLxInfiniteLightHandler.h"
#include "DfxLxSunSkyLightHandler.h"

#include "DfxBulletStageHandler.h"
#include "DfxBulletActorHandler.h"
#include "DfxBulletConstraintPropHandler.h"
#include "DfxBulletActorNode.h"
#include "DfxBulletConstraintPropNode.h"

#include "DfxRiptideStageHandler.h"
#include "DfxRiptideStageNode.h"
#include "DfxRiptideActorNode.h"
#include "DfxRiptideActorHandler.h"
#include "DfxRiptideCachePropHandler.h"
#include "DfxRiptideCachePropNode.h"
#include "DfxRiptideEmitterPropHandler.h"
#include "DfxRiptideEmitterPropNode.h"
#include "DfxRiptideHeaterPropHandler.h"
#include "DfxRiptideHeaterPropNode.h"

#include "DfxPointCloudHandler.h"
#include "DfxGeneratePointsHandler.h"
#include "DfxPointReaderHandler.h"
#include "DfxPointCloudMesherHandler.h"
#include "DfxPointAttributeInstancerHandler.h"
#include "DfxPointSamplerHandler.h"
#include "DfxPointShapeHandler.h"
#include "DfxPointProximityKillHandler.h"
#include "DfxPointProximityScaleHandler.h"

#include "DfxEntropyStageHandler.h"
#include "DfxEntropyDomainPropHandler.h"
#include "DfxEntropyCachePropHandler.h"
#include "DfxEntropyActorHandler.h"

#include "DfxGridHandler.h"
#include "DfxLoadVDBHandler.h"
#include "DfxMeshToGridHandler.h"

#include "DfxAlembicLoaderHandler.h"

namespace scenegraph {
	void initializeNodeCallbacks() {

		///////////////////////
		// Load Callbacks
		//////////////////////

		// Stage
		DfxSqNodeDB::getInstance()->addLoadCallback(deepsg::SgStageActorCollectionNode::getUniqueName(), DfxSimulationStageCommon::loadPropActorNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(deepsg::SgStagePropCollectionNode::getUniqueName(), DfxSimulationStageCommon::loadPropActorNode);

		// Group
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxGroupNode::getUniqueName(), DfxGroupHandler::loadNode);

		// Null
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxNullNode::getUniqueName(), DfxNullHandler::loadNode);

		// Geometry
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxGeometryNode::getUniqueName(), DfxObjectHandler::loadNode);

		// OSPRay
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxOSPRayRenderNode::getUniqueName(), DfxOSPRayRenderHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxOSPRayCameraWidget::getUniqueName(), DfxOSPRayCameraHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxOSPRaySunSkyLightWidget::getUniqueName(), DfxOSPRaySunSkyLightHandler::loadNode);

		// Luxcore
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxLuxCoreRenderNode::getUniqueName(), DfxLxRenderHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxLxCameraWidget::getUniqueName(), DfxLxCameraHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxLxEnvironmentLightWidget::getUniqueName(), DfxLxEnvironmentLightHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxLxPointLightWidget::getUniqueName(), DfxLxPointLightHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxLxSunSkyLightWidget::getUniqueName(), DfxLxSunSkyLightHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxLxInfiniteLightWidget::getUniqueName(), DfxLxInfiniteLightHandler::loadNode);

		// Bullet
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxBulletStageNode::getUniqueName(), DfxBulletStageHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxBulletActorNode::getUniqueName(), DfxBulletActorHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxBulletConstraintPropNode::getUniqueName(), DfxBulletConstraintPropHandler::loadNode);

		// Riptide
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxRiptideStageNode::getUniqueName(), DfxRiptideStageHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxRiptideActorNode::getUniqueName(), DfxRiptideActorHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxRiptideCachePropNode::getUniqueName(), DfxRiptideCachePropHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxRiptideEmitterPropNode::getUniqueName(), DfxRiptideEmitterPropHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxRiptideHeaterPropNode::getUniqueName(), DfxRiptideHeaterPropHandler::loadNode);

		// Entropy
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxEntropyStageNode::getUniqueName(), DfxEntropyStageHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxEntropyDomainPropNode::getUniqueName(), DfxEntropyDomainPropHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxEntropyCachePropNode::getUniqueName(), DfxEntropyCachePropHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxEntropyActorNode::getUniqueName(), DfxEntropyActorHandler::loadNode);

		// Point Cloud
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxPointCloudNode::getUniqueName(), DfxPointCloudHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxPointAttributeInstancerTool::getUniqueName(), DfxPointAttributeInstancerHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxGeneratePointsTool::getUniqueName(), DfxGeneratePointsHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxPointReaderTool::getUniqueName(), DfxPointReaderHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxPointCloudMesherNode::getUniqueName(), DfxPointCloudMesherHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxPointSamplerTool::getUniqueName(), DfxPointSamplerHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxPointShapeTool::getUniqueName(), DfxPointShapeHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxPointProximityKillTool::getUniqueName(), DfxPointProximityKillHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxPointProximityScaleTool::getUniqueName(), DfxPointProximityScaleHandler::loadNode);

		// Grid
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxGridNode::getUniqueName(), DfxGridHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxLoadVDBTool::getUniqueName(), DfxLoadVDBHandler::loadNode);
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxMeshToGridTool::getUniqueName(), DfxMeshToGridHandler::loadNode);

		// Alembic
		DfxSqNodeDB::getInstance()->addLoadCallback(DfxAlembicLoaderTool::getUniqueName(), DfxAlembicLoaderHandler::loadNode);

		//////////////////////////////
		// Destroy/Delete Callbacks
		//////////////////////////////

		// Stage
		DfxSqNodeDB::getInstance()->addDeleteCallback(deepsg::SgStageActorCollectionNode::getUniqueName(), DfxSimulationStageCommon::propActorDeleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(deepsg::SgStagePropCollectionNode::getUniqueName(), DfxSimulationStageCommon::propActorDeleteNode);

		// Group
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxGroupNode::getUniqueName(), DfxGroupHandler::deleteNode);

		// Null
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxNullNode::getUniqueName(), DfxNullHandler::deleteNode);

		// Geometry
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxGeometryNode::getUniqueName(), DfxObjectHandler::deleteNode);
		
		// OSPRay
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxOSPRayRenderNode::getUniqueName(), DfxOSPRayRenderHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxOSPRayCameraWidget::getUniqueName(), DfxOSPRayCameraHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxOSPRaySunSkyLightWidget::getUniqueName(), DfxOSPRaySunSkyLightHandler::deleteNode);

		// Luxcore
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxLuxCoreRenderNode::getUniqueName(), DfxLxRenderHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxLxCameraWidget::getUniqueName(), DfxLxCameraHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxLxEnvironmentLightWidget::getUniqueName(), DfxLxEnvironmentLightHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxLxPointLightWidget::getUniqueName(), DfxLxPointLightHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxLxSunSkyLightWidget::getUniqueName(), DfxLxSunSkyLightHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxLxInfiniteLightWidget::getUniqueName(), DfxLxInfiniteLightHandler::deleteNode);

		// Bullet
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxBulletStageNode::getUniqueName(), DfxBulletStageHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxBulletActorNode::getUniqueName(), DfxBulletActorHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxBulletConstraintPropNode::getUniqueName(), DfxBulletConstraintPropHandler::deleteNode);

		// Riptide
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxRiptideStageNode::getUniqueName(), DfxRiptideStageHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxRiptideActorNode::getUniqueName(), DfxRiptideActorHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxRiptideCachePropNode::getUniqueName(), DfxRiptideCachePropHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxRiptideEmitterPropNode::getUniqueName(), DfxRiptideEmitterPropHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxRiptideHeaterPropNode::getUniqueName(), DfxRiptideHeaterPropHandler::deleteNode);

		// Entropy
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxEntropyStageNode::getUniqueName(), DfxEntropyStageHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxEntropyDomainPropNode::getUniqueName(), DfxEntropyDomainPropHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxEntropyCachePropNode::getUniqueName(), DfxEntropyCachePropHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxEntropyActorNode::getUniqueName(), DfxEntropyActorHandler::deleteNode);

		// Point Cloud
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxPointCloudNode::getUniqueName(), DfxPointCloudHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxPointAttributeInstancerTool::getUniqueName(), DfxPointAttributeInstancerHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxGeneratePointsTool::getUniqueName(), DfxGeneratePointsHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxPointReaderTool::getUniqueName(), DfxPointReaderHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxPointCloudMesherNode::getUniqueName(), DfxPointCloudMesherHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxPointSamplerTool::getUniqueName(), DfxPointSamplerHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxPointShapeTool::getUniqueName(), DfxPointShapeHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxPointProximityKillTool::getUniqueName(), DfxPointProximityKillHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxPointProximityScaleTool::getUniqueName(), DfxPointProximityScaleHandler::deleteNode);

		// Grid
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxGridNode::getUniqueName(), DfxGridHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxLoadVDBTool::getUniqueName(), DfxLoadVDBHandler::deleteNode);
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxMeshToGridTool::getUniqueName(), DfxMeshToGridHandler::deleteNode);

		// Alembic
		DfxSqNodeDB::getInstance()->addDeleteCallback(DfxAlembicLoaderTool::getUniqueName(), DfxAlembicLoaderHandler::deleteNode);

	}
}