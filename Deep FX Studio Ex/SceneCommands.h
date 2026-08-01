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

#include <generator/PlaneMesh.hpp>
#include <generator/BoxMesh.hpp>
#include <generator/SphereMesh.hpp>
#include <generator/TorusKnotMesh.hpp>
#include <generator/TorusMesh.hpp>
#include <generator/SpringMesh.hpp>
#include <generator/BoxMesh.hpp>
#include <generator/TeapotMesh.hpp>
#include <generator/CylinderMesh.hpp>
#include <generator/TubeMesh.hpp>
#include <generator/CapsuleMesh.hpp>

#include <stack>
#include <unordered_map>
#include <DsTools.h>
#include <DfxAbstractCommand.h>
#include <DfxGlobal.h>
#include "DfxPointProximityKillHandler.h"
#include "DfxPointProximityScaleHandler.h"
#include "DfxWidgetHandler.h"
#include "DfxUpdater.h"
#include "DfxWidgetManager.h"
#include "DfxTranslateWidget.h"
#include "DfxScaleWidget.h"
#include "DfxRotateWidget.h"
#include "DfxLxCameraHandler.h"
#include "DfxLxPointLightHandler.h"
#include "DfxLxEnvironmentLightHandler.h"
#include "DfxLxSunSkyLightHandler.h"
#include "DfxOSPRayRenderHandler.h"
#include "DfxOSPRayCameraHandler.h"
#include "DfxOSPRaySunSkyLightHandler.h"
#include "DfxPointCloudHandler.h"
#include "DfxGeneratePointsHandler.h"
#include "DfxPointSamplerHandler.h"
#include "DfxPointAttributeInstancerHandler.h"
#include "DfxBulletStageHandler.h"
#include "DfxAlembicLoaderHandler.h"
#include "DfxBulletConstraintPropHandler.h"
#include "DfxLxRenderHandler.h"
#include "DfxSurfaceDictionary.h"
#include "DfxResourceManager.h"
#include "DfxSurfaceDictionary.h"
#include "DfxQPushButtonStyles.h"
#include "DfxGroupHandler.h"
#include "DfxNullHandler.h"
#include "DfxPointCloudMesherHandler.h"
#include "DfxPointReaderHandler.h"
#include "DfxLxInfiniteLightHandler.h"
#include "IndexedTriangleMesh.h"
#include "DfxPointShapeHandler.h"
#include "DfxGridHandler.h"
#include "DfxLoadVDBHandler.h"
#include "DfxMeshToGridHandler.h"
#include "DfxEntropyStageHandler.h"
#include "DfxEntropyDomainPropHandler.h"
#include "DfxEntropyCachePropHandler.h"

#include "DfxUpdater.h"

namespace command {

	void addPrimitive(std::vector<generator::MeshVertex> &vertArray, std::vector<IndexedMeshArray> &faceArray, std::string primitiveName);

	/// Translate Tool
	class TranslateTool : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Scale Tool
	class ScaleTool : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Rotate Tool
	class RotateTool : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Object Mode
	class ObjectMode : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Vertex Mode
	class VertexMode : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Edge Mode
	class EdgeMode : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Face Mode
	class FaceMode : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Delete
	class Delete : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Group
	class AddGroup : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Null
	class AddNull : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	// Add Empty Primitive
	class AddEmptyPrimitive : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	// Add Plane Primitive
	class AddPlanePrimitive : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	// Add Cube Primitive
	class AddCubePrimitive : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	// Add Sphere Primitive
	class AddSpherePrimitive : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	// Add Torus Knot Primitive
	class AddTorusKnotPrimitive : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	// Add Torus Primitive
	class AddTorusPrimitive : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	// Add Tube Primitive
	class AddTubePrimitive : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	// Add Spring Primitive
	class AddSpringPrimitive : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	// Add Teapot Primitive
	class AddTeapotPrimitive : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	// Add Cylinder Primitive
	class AddCylinderPrimitive : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	// Add Capsule Primitive
	class AddCapsulePrimitive : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add OSPRay Render to scene
	class AddOSPRayRender : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add OSPRay Camera to scene
	class AddOSPRayCamera : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add OSPRay Sun Sky Light to scene
	class AddOSPRaySunSkyLight : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Lux Render Node
	class AddLuxcoreRender : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Lux Camera to scene
	class AddCamera : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Lux Point Light to scene
	class AddPointLight : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Lux Environment Light scene
	class AddEnvironmentLight : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Lux Sun Sky Light to scene
	class AddSunSkyLight : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Lux Infinite Light to scene
	class AddInfiniteLight : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Bullet Simulation Stage
	class AddBulletStage : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Riptide Simulation Stage
	class AddRiptideStage : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Duplicate
	class Duplicate : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Point Cloud
	class AddPointCloud : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Point Cloud Mesher
	class AddPointCloudMesher : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Point Generator
	class AddPointGenerator : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Point Sampler
	class AddPointSampler : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Point Attribute Instancer
	class AddPointAttributeInstancer : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Point Shape Tool
	class AddPointShapeTool : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Point Proximity Kill Tool
	class AddPointProximityKillTool : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Point Proximity Scale Tool
	class AddPointProximityScaleTool : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Alembic Loader
	class AddAlembicLoader : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Bullet Contstraint Node
	class AddBulletConstraintNode : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Riptide Cache Node
	class AddRiptideCacheNode : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Deep Rising Emitter Node
	class AddRiptideEmitterNode : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Deep Rising Heater Node
	class AddRiptideHeaterNode : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Point Reader
	class AddPointReader : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Grid Node
	class AddGrid : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Load VDB Node
	class AddLoadVDBNode : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Mesh To Grid Node
	class AddMeshToGridNode : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Entropy Stage Node
	class AddEntropyStageNode : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Entropy Domain Node
	class AddEntropyDomainNode : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	/// Add Entropy Cache Node
	class AddEntropyCacheNode : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};
}
