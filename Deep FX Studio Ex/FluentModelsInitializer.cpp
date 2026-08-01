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

#include <FlModelDB.h>
#include "FluentModelsInitializer.h"

#include "DfxGroupHandler.h"

#include "DfxNullHandler.h"

#include "DfxObjectHandler.h"

#include "DfxFlowHandler.h"

#include "DfxOSPRayRenderHandler.h"
#include "DfxOSPRayCameraHandler.h"
#include "DfxOSPRaySunSkyLightHandler.h"

#include "DfxLxRenderHandler.h"
#include "DfxLxCameraHandler.h"
#include "DfxLxEnvironmentLightHandler.h"
#include "DfxLxPointLightHandler.h"
#include "DfxLxSunSkyLightHandler.h"
#include "DfxLxInfiniteLightHandler.h"

#include "DfxLuxMaterialHandler.h"
#include "DfxLuxcoreMaterials.h"

#include "DfxOSPRayMaterialHandler.h"
#include "DfxOSPRayMaterials.h"

#include "DfxBulletStageHandler.h"
#include "DfxBulletConstraintPropHandler.h"

#include "DfxPointCloudHandler.h"
#include "DfxPointAttributeInstancerHandler.h"
#include "DfxGeneratePointsHandler.h"
#include "DfxPointReaderHandler.h"
#include "DfxPointCloudMesherHandler.h"
#include "DfxPointSamplerHandler.h"
#include "DfxPointShapeHandler.h"
#include "DfxPointProximityKillHandler.h"
#include "DfxPointProximityScaleHandler.h"

#include "DfxAlembicLoaderHandler.h"

#include "DfxRiptideStageHandler.h"
#include "DfxRiptideActorHandler.h"
#include "DfxRiptideCachePropHandler.h"
#include "DfxRiptideEmitterPropHandler.h"
#include "DfxRiptideHeaterPropHandler.h"

#include "DfxCForEachNode.h"
#include "DfxCMapNode.h"
#include "DfxCFilterNode.h"
#include "DfxCPartitionNode.h"
#include "DfxCPeekNode.h"
#include "DfxCReduceNode.h"

#include "DfxCBuildArrayNode.h"
#include "DfxCCommonFloatNodes.h"
#include "DfxCCommonVectorNodes.h"
#include "DfxCCommonStringNodes.h"
#include "DfxCFloatComparisionNodes.h"
#include "DfxCFloatRangeNode.h"
#include "DfxCConsoleOutNode.h"

#include "DfxGridHandler.h"
#include "DfxLoadVDBHandler.h"
#include "DfxMeshToGridHandler.h"

#include "DfxCSceneToSet.h"
#include "DfxCSetNode.h"
#include "DfxCSetSize.h"
#include "DfxCMoveSetNode.h"
#include "DfxCProximityNode.h"
#include "DfxCUpdateSetAttribute.h"
#include "DfxCFilterFloatAttribute.h"
#include "DfxCReadSetAttribute.h"

#include "DfxCSimplexToBullet.h"
#include "DfxCStreamToBullet.h"
#include "DfxCBulletRigidBodyAttributes.h"
#include "DfxCBulletActivationState.h"
#include "DfxCBulletAddForce.h"
#include "DfxCBulletKill.h"

#include "DfxEntropyStageHandler.h"
#include "DfxEntropyDomainPropHandler.h"
#include "DfxEntropyCachePropHandler.h"
#include "DfxEntropyActorHandler.h"

#include "DfxCompoundNodeHandler.h"

#include "DfxCVoronoiFracture.h"

#include "DfxCExplodeSetNode.h"

#include "DfxCToggleSetVisibility.h"

#include "DfxCTimeNode.h"

#include "DfxCTrackSetSize.h"
#include "DfxCOverrideSetMaterialID.h"


namespace flmodel {

	void initModels() {

		// Flow
		fl::ModelDB::getInstance()->addModel(fl::Flow::decision_model_id, new fl::Flow);
		fl::ModelDB::getInstance()->addModel(DfxFlowHandler::model_id, new DfxFlowHandler);

		// Group
		fl::ModelDB::getInstance()->addModel(DfxGroupHandler::model_id, new DfxGroupHandler);

		// Null
		fl::ModelDB::getInstance()->addModel(DfxNullHandler::model_id, new DfxNullHandler);

		// Object
		fl::ModelDB::getInstance()->addModel(DfxObjectHandler::model_id, new DfxObjectHandler);

		// OSPRay
		fl::ModelDB::getInstance()->addModel(DfxOSPRayRenderHandler::model_id, new DfxOSPRayRenderHandler);
		fl::ModelDB::getInstance()->addModel(DfxOSPRayCameraHandler::model_id, new DfxOSPRayCameraHandler);
		fl::ModelDB::getInstance()->addModel(DfxOSPRaySunSkyLightHandler::model_id, new DfxOSPRaySunSkyLightHandler);

		// Luxcore
		fl::ModelDB::getInstance()->addModel(DfxLxRenderHandler::model_id, new DfxLxRenderHandler);
		fl::ModelDB::getInstance()->addModel(DfxLxCameraHandler::model_id, new DfxLxCameraHandler);
		fl::ModelDB::getInstance()->addModel(DfxLxEnvironmentLightHandler::model_id, new DfxLxEnvironmentLightHandler);
		fl::ModelDB::getInstance()->addModel(DfxLxPointLightHandler::model_id, new DfxLxPointLightHandler);
		fl::ModelDB::getInstance()->addModel(DfxLxSunSkyLightHandler::model_id, new DfxLxSunSkyLightHandler);
		fl::ModelDB::getInstance()->addModel(DfxLxInfiniteLightHandler::model_id, new DfxLxInfiniteLightHandler);

		// OSPRay Materials
		fl::ModelDB::getInstance()->addModel(DfxOSPRayMaterialHandler::model_id, new DfxOSPRayMaterialHandler);

		fl::ModelDB::getInstance()->addModel(ospmat::principledID, new ospmat::PrincipledModel);

		fl::ModelDB::getInstance()->addModel(ospmat::imageTextureID, new ospmat::ImageTextureModel);

		// Luxcore Materials
		fl::ModelDB::getInstance()->addModel(DfxLuxMaterialHandler::model_id, new DfxLuxMaterialHandler);

		fl::ModelDB::getInstance()->addModel(luxmat::nullID, new luxmat::NullModel);
		fl::ModelDB::getInstance()->addModel(luxmat::matteID, new luxmat::MatteModel);
		fl::ModelDB::getInstance()->addModel(luxmat::matteTranslucentID, new luxmat::MatteTranslucentModel);
		fl::ModelDB::getInstance()->addModel(luxmat::emissionID, new luxmat::EmissionModel);
		fl::ModelDB::getInstance()->addModel(luxmat::disneyID, new luxmat::DisneyModel);
		fl::ModelDB::getInstance()->addModel(luxmat::mixID, new luxmat::MixModel);
		fl::ModelDB::getInstance()->addModel(luxmat::glassID, new luxmat::GlassModel);
		fl::ModelDB::getInstance()->addModel(luxmat::archGlassID, new luxmat::ArchGlassModel);
		fl::ModelDB::getInstance()->addModel(luxmat::roughGlassID, new luxmat::RoughGlassModel);
		fl::ModelDB::getInstance()->addModel(luxmat::carPaintID, new luxmat::CarPaintModel);
		fl::ModelDB::getInstance()->addModel(luxmat::clothID, new luxmat::ClothModel);
		fl::ModelDB::getInstance()->addModel(luxmat::glossyID, new luxmat::GlossyModel);
		fl::ModelDB::getInstance()->addModel(luxmat::glossyTranslucentID, new luxmat::GlossyTranslucentModel);
		fl::ModelDB::getInstance()->addModel(luxmat::metalID, new luxmat::MetalModel);
		fl::ModelDB::getInstance()->addModel(luxmat::mirrorID, new luxmat::MirrorModel);
		fl::ModelDB::getInstance()->addModel(luxmat::velvetID, new luxmat::VelvetModel);
		fl::ModelDB::getInstance()->addModel(luxmat::roughMatteID, new luxmat::RoughMatteModel);

		fl::ModelDB::getInstance()->addModel(luxmat::imagemapID, new luxmat::ImagemapModel);
		fl::ModelDB::getInstance()->addModel(luxmat::normalmapID, new luxmat::NormalmapModel);
		fl::ModelDB::getInstance()->addModel(luxmat::checkerboard2dID, new luxmat::CheckerboardModel);
		fl::ModelDB::getInstance()->addModel(luxmat::fbmID, new luxmat::FbmModel);
		fl::ModelDB::getInstance()->addModel(luxmat::fresnelColorID, new luxmat::FresnelColorModel);
		fl::ModelDB::getInstance()->addModel(luxmat::fresnelSopraID, new luxmat::FresnelSopraModel);
		fl::ModelDB::getInstance()->addModel(luxmat::blendID, new luxmat::BlendModel);
		fl::ModelDB::getInstance()->addModel(luxmat::cloudID, new luxmat::CloudModel);
		fl::ModelDB::getInstance()->addModel(luxmat::distortedNoiseID, new luxmat::DistortedNoiseModel);
		fl::ModelDB::getInstance()->addModel(luxmat::magicID, new luxmat::MagicModel);
		fl::ModelDB::getInstance()->addModel(luxmat::marbleID, new luxmat::MarbleModel);
		fl::ModelDB::getInstance()->addModel(luxmat::musgraveID, new luxmat::MusgraveModel);
		fl::ModelDB::getInstance()->addModel(luxmat::noiseID, new luxmat::NoiseModel);
		fl::ModelDB::getInstance()->addModel(luxmat::stucciID, new luxmat::StucciModel);
		fl::ModelDB::getInstance()->addModel(luxmat::woodID, new luxmat::WoodModel);
		fl::ModelDB::getInstance()->addModel(luxmat::voronoiID, new luxmat::VoronoiModel);

		fl::ModelDB::getInstance()->addModel(luxmat::mixTexID, new luxmat::MixTexModel);
		fl::ModelDB::getInstance()->addModel(luxmat::clampID, new luxmat::ClampModel);
		fl::ModelDB::getInstance()->addModel(luxmat::hsvID, new luxmat::HsvModel);

		// Bullet 
		fl::ModelDB::getInstance()->addModel(DfxBulletStageHandler::model_id, new DfxBulletStageHandler);
		fl::ModelDB::getInstance()->addModel(DfxBulletActorHandler::model_id, new DfxBulletActorHandler);
		fl::ModelDB::getInstance()->addModel(DfxBulletConstraintPropHandler::model_id, new DfxBulletConstraintPropHandler);

		fl::ModelDB::getInstance()->addModel(compute_node::simplexToBullet_model_id, new compute_node::SimplexToBulletNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::streamToBullet_model_id, new compute_node::StreamToBulletNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::bulletRigidBodyAttributes_model_id, new compute_node::BulletRigidBodyAttributesNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::bulletAddForce_model_id, new compute_node::BulletAddForceNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::bulletActivationState_model_id, new compute_node::BulletActivationStateNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::bulletKill_model_id, new compute_node::BulletKillNodeHandler);

		// Riptide
		fl::ModelDB::getInstance()->addModel(DfxRiptideStageHandler::model_id, new DfxRiptideStageHandler);
		fl::ModelDB::getInstance()->addModel(DfxRiptideActorHandler::model_id, new DfxRiptideActorHandler);
		fl::ModelDB::getInstance()->addModel(DfxRiptideCachePropHandler::model_id, new DfxRiptideCachePropHandler);
		fl::ModelDB::getInstance()->addModel(DfxRiptideEmitterPropHandler::model_id, new DfxRiptideEmitterPropHandler);
		fl::ModelDB::getInstance()->addModel(DfxRiptideHeaterPropHandler::model_id, new DfxRiptideHeaterPropHandler);

		// Entropy 
		fl::ModelDB::getInstance()->addModel(DfxEntropyStageHandler::model_id, new DfxEntropyStageHandler);
		fl::ModelDB::getInstance()->addModel(DfxEntropyDomainPropHandler::model_id, new DfxEntropyDomainPropHandler);
		fl::ModelDB::getInstance()->addModel(DfxEntropyCachePropHandler::model_id, new DfxEntropyCachePropHandler);
		fl::ModelDB::getInstance()->addModel(DfxEntropyActorHandler::model_id, new DfxEntropyActorHandler);

		// Point clouds
		fl::ModelDB::getInstance()->addModel(DfxPointCloudHandler::model_id, new DfxPointCloudHandler);
		fl::ModelDB::getInstance()->addModel(DfxPointAttributeInstancerHandler::model_id, new DfxPointAttributeInstancerHandler);
		fl::ModelDB::getInstance()->addModel(DfxGeneratePointsHandler::model_id, new DfxGeneratePointsHandler);
		fl::ModelDB::getInstance()->addModel(DfxPointReaderHandler::model_id, new DfxPointReaderHandler);
		fl::ModelDB::getInstance()->addModel(DfxPointCloudMesherHandler::model_id, new DfxPointCloudMesherHandler);
		fl::ModelDB::getInstance()->addModel(DfxPointSamplerHandler::model_id, new DfxPointSamplerHandler);
		fl::ModelDB::getInstance()->addModel(DfxPointShapeHandler::model_id, new DfxPointShapeHandler);
		fl::ModelDB::getInstance()->addModel(DfxPointProximityKillHandler::model_id, new DfxPointProximityKillHandler);
		fl::ModelDB::getInstance()->addModel(DfxPointProximityScaleHandler::model_id, new DfxPointProximityScaleHandler);

		// Grid
		fl::ModelDB::getInstance()->addModel(DfxGridHandler::model_id, new DfxGridHandler);
		fl::ModelDB::getInstance()->addModel(DfxLoadVDBHandler::model_id, new DfxLoadVDBHandler);
		fl::ModelDB::getInstance()->addModel(DfxMeshToGridHandler::model_id, new DfxMeshToGridHandler);

		// Alembic
		fl::ModelDB::getInstance()->addModel(DfxAlembicLoaderHandler::model_id, new DfxAlembicLoaderHandler);

		// Compute Nodes
		fl::ModelDB::getInstance()->addModel(DfxCompoundNodeHandler::model_id, new DfxCompoundNodeHandler);

		fl::ModelDB::getInstance()->addModel(compute_node::forEach_model_id, new compute_node::ForEachNodeHandler);

		fl::ModelDB::getInstance()->addModel(compute_node::buildArray_model_id, new compute_node::BuildArrayNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::toString_model_id, new compute_node::ToStringNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::floatRange_model_id, new compute_node::FloatRangeNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::consoleOut_model_id, new compute_node::ConsoleOutNodeHandler);

		fl::ModelDB::getInstance()->addModel(compute_node::floatValue_model_id, new compute_node::FloatValueNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::arithmeticFloat_model_id, new compute_node::ArithmeticFloatNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::mathFunctionFloat_model_id, new compute_node::MathFunctionsNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::piFloat_model_id, new compute_node::PiFloatNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::floatLessThan_model_id, new compute_node::FloatLessThanNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::floatEqualTo_model_id, new compute_node::FloatEqualToNodeHandler);

		fl::ModelDB::getInstance()->addModel(compute_node::vectorValue_model_id, new compute_node::VectorValueNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::vectorLength_model_id, new compute_node::VectorLengthNodeHandler);

		// Sets
		fl::ModelDB::getInstance()->addModel(compute_node::sceneToSet_model_id, new compute_node::SceneToSetNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::set_model_id, new compute_node::SetNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::updateSetAttribute_model_id, new compute_node::UpdateSetAttributeNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::toggleSetVisibilty_model_id, new compute_node::ToggleSetVisibiltyNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::proximity_model_id, new compute_node::ProximityNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::moveSet_model_id, new compute_node::MoveSetNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::setSize_model_id, new compute_node::SetSizeNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::readAttribute_model_id, new compute_node::ReadAttributeNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::filterFloatAttribute_model_id, new compute_node::FilterFloatAttributeNodeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::floatHasValue_model_id, new compute_node::FloatHasValueNodeHandler);

		fl::ModelDB::getInstance()->addModel(compute_node::trackSetSize_model_id, new compute_node::TrackSetSizeHandler);
		fl::ModelDB::getInstance()->addModel(compute_node::overrideSetMatID_model_id, new compute_node::OverrideSetMatIDHandler);

		// Fracture
		fl::ModelDB::getInstance()->addModel(compute_node::voronoiFracture_model_id, new compute_node::VoronoiFractureNodeHandler);

		// Transforms
		fl::ModelDB::getInstance()->addModel(compute_node::explodeSet_model_id, new compute_node::ExplodeSetNodeHandler);

		// Time
		fl::ModelDB::getInstance()->addModel(compute_node::time_model_id, new compute_node::TimeNodeHandler);

	}
}