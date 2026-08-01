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

#include <DfxPlugin.h>
#include "DfxSpaceReset.h"
#include "ApplicationCommands.h"
#include "SceneCommands.h"
#include "ModellingCommands.h"
#include "SelectionCommands.h"
#include "AnimationCommands.h"

namespace command {
	void initCommands() {

		/// Application Commands
		registerDfxCommand("exitApplication", command::ExitApplication::create);
		registerDfxCommand("undo", command::Undo::create);
		registerDfxCommand("redo", command::Redo::create);
		registerDfxCommand("echoSpace", command::EchoSpace::create);
		registerDfxCommand("saveProject", command::SaveProject::create);
		registerDfxCommand("saveProjectAs", command::SaveProjectAs::create);
		registerDfxCommand("loadProject", command::LoadProject::create);
		registerDfxCommand("openHotkeyEditor", command::OpenShorcutsEditor::create);
		registerDfxCommand("sceneViewContextMenu", command::SceneViewContextMenu::create);
		registerDfxCommand("updateScene", command::UpdateScene::create);
		registerDfxCommand("updateSceneAndKeys", command::UpdateSceneAndKeys::create);
		registerDfxCommand("clearScene", command::ClearScene02::create);
		registerDfxCommand("openObj", command::OpenObj::create);
		registerDfxCommand("luxTestRender", command::LuxTestRender::create);
		// registerDfxCommand("saveLayout", command::SaveLayout::create);
		// registerDfxCommand("loadLayout", command::LoadLayout::create);

		// Scene Commands	
		registerDfxCommand("addGroup", command::AddGroup::create);

		registerDfxCommand("addNull", command::AddNull::create);

		registerDfxCommand("addBulletStage", command::AddBulletStage::create);
		registerDfxCommand("addBulletConstraint", command::AddBulletConstraintNode::create);

		registerDfxCommand("addRiptideStage", command::AddRiptideStage::create);
		registerDfxCommand("addRiptideCache", command::AddRiptideCacheNode::create);
		registerDfxCommand("addRiptideEmitter", command::AddRiptideEmitterNode::create);
		registerDfxCommand("addRiptideHeater", command::AddRiptideHeaterNode::create);

		registerDfxCommand("addEntropyStage", command::AddEntropyStageNode::create);
		registerDfxCommand("addEntropyDomain", command::AddEntropyDomainNode::create);
		registerDfxCommand("addEntropyCache", command::AddEntropyCacheNode::create);

		registerDfxCommand("addLxRenderNode", command::AddLuxcoreRender::create);
		registerDfxCommand("addLxCamera", command::AddCamera::create);
		registerDfxCommand("addLxPointLight", command::AddPointLight::create);
		registerDfxCommand("addLxEnvironmentLight", command::AddEnvironmentLight::create);
		registerDfxCommand("addLxSunSkyLight", command::AddSunSkyLight::create);
		registerDfxCommand("addLxInfiniteLight", command::AddInfiniteLight::create);

		registerDfxCommand("addOSPRayRender", command::AddOSPRayRender::create);
		registerDfxCommand("addOSPRayCamera", command::AddOSPRayCamera::create);
		registerDfxCommand("addOSPRaySunSkyLight", command::AddOSPRaySunSkyLight::create);

		registerDfxCommand("addPointCloud", command::AddPointCloud::create);
		registerDfxCommand("addPointGenerator", command::AddPointGenerator::create);
		registerDfxCommand("addPointSampler", command::AddPointSampler::create);
		registerDfxCommand("addPointAttributeInstancer", command::AddPointAttributeInstancer::create);
		registerDfxCommand("addPointMesher", command::AddPointCloudMesher::create);
		registerDfxCommand("addPointReader", command::AddPointReader::create);
		registerDfxCommand("addPointShapeTool", command::AddPointShapeTool::create);
		registerDfxCommand("addPointProximityKillTool", command::AddPointProximityKillTool::create);
		registerDfxCommand("addPointProximityScaleTool", command::AddPointProximityScaleTool::create);

		registerDfxCommand("addGridNode", command::AddGrid::create);
		registerDfxCommand("addVDBLoader", command::AddLoadVDBNode::create);
		registerDfxCommand("addMeshToGridNode", command::AddMeshToGridNode::create);

		registerDfxCommand("addAlembicLoader", command::AddAlembicLoader::create);

		registerDfxCommand("addEmptyPrimitive", command::AddEmptyPrimitive::create);
		registerDfxCommand("addCubePrimitive", command::AddCubePrimitive::create);
		registerDfxCommand("addPlanePrimitive", command::AddPlanePrimitive::create);
		registerDfxCommand("addSpherePrimitive", command::AddSpherePrimitive::create);
		registerDfxCommand("addTorusKnotPrimitive", command::AddTorusKnotPrimitive::create);
		registerDfxCommand("addTorusPrimitive", command::AddTorusPrimitive::create);
		registerDfxCommand("addTubePrimitive", command::AddTubePrimitive::create);
		registerDfxCommand("addSpringPrimitive", command::AddSpringPrimitive::create);
		registerDfxCommand("addTeapotPrimitive", command::AddTeapotPrimitive::create);
		registerDfxCommand("addCylinderPrimitive", command::AddCylinderPrimitive::create);
		registerDfxCommand("addCapsulePrimitive", command::AddCapsulePrimitive::create);

		registerDfxCommand("duplicate", command::Duplicate::create);
		registerDfxCommand("delete", command::Delete::create);
		registerDfxCommand("translateTool", command::TranslateTool::create);
		registerDfxCommand("scaleTool", command::ScaleTool::create);
		registerDfxCommand("rotateTool", command::RotateTool::create);
		registerDfxCommand("objectMode", command::ObjectMode::create);
		registerDfxCommand("vertexMode", command::VertexMode::create);
		registerDfxCommand("edgeMode", command::EdgeMode::create);
		registerDfxCommand("faceMode", command::FaceMode::create);

		/// Modelling Commands
		registerDfxCommand("deleteVertices", command::DeleteVertices::create);
		registerDfxCommand("deleteEdges", command::DeleteEdges::create);
		registerDfxCommand("deleteFaces", command::DeleteFaces::create);
		registerDfxCommand("extrudeFaces", command::ExtrudeFaces::create);
		registerDfxCommand("triangulateFaces", command::TriangulateFaces::create);
		registerDfxCommand("bevelEdge", command::BevelEdge::create);
		registerDfxCommand("createFaceSet", command::CreateFaceSet::create);
		registerDfxCommand("tagSurface", command::TagSurface::create);
		registerDfxCommand("linkMatToSurfs", command::LinkMaterialToSurfaces::create);
		registerDfxCommand("printSurfaces", command::PrintSurfaces::create);
		registerDfxCommand("newMaterial", command::NewMaterial::create);

		/// Selection Commands
		registerDfxCommand("selectConnectedFaces", command::SelectConnectedFaces::create);
		registerDfxCommand("selectFaceLoop", command::SelectFaceLoop::create);
		registerDfxCommand("selectAllFaces", command::SelectAllFaces::create);
		registerDfxCommand("selectFaceSet", command::SelectFaceSet::create);
		registerDfxCommand("selectSurface", command::SelectSurface::create);
		registerDfxCommand("selectVIncidentEdges", command::SelectVertexIncidentEdges::create);
		registerDfxCommand("selectVIncidentFaces", command::SelectVertexIncidentFaces::create);
		registerDfxCommand("selectEAdjacentEdges", command::SelectEdgeAdjacentVertices::create);
		registerDfxCommand("printEdgeID", command::PrintEdgeID::create);
		registerDfxCommand("printFaceID", command::PrintFaceID::create);
		registerDfxCommand("printVertexID", command::PrintVertexID::create);

		/// Animation Commands
		registerDfxCommand("createKey", command::CreateKey::create);
		registerDfxCommand("goToFrame", command::GoToFrame::create);

		/// Reset Commands
		registerDfxCommand("resetAll", command::Reset::create);

	}
}