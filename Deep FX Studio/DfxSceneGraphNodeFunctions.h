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

#include <QFileDialog>
#include <DfxCMeshVoxelizer.h>
#include <DfxPointItem.h>
#include <DfxCPointXForm.h>
#include <DfxMapUniformDisplacement.h>
#include <DfxMapUniformDisplacementProxy.h>
#include <DfxObjectItemInfo.h>
#include <DfxCSPHLiquid.h>
#include <DfxCSPHRigid.h>
#include <DfxCSPHParticleCache.h>
#include <DfxCDivergenceFreeSPHSolver.h>
#include <DfxCBulletSolver.h>
#include <DfxCBulletRigidObject.h>
#include <DfxCBulletRigidArray.h>
#include <DfxPointGrid.h>
#include <DfxObjectItemCopy.h>
#include <DfxSpotterItem.h>
#include <DfxObjectViewer.h>
#include "DfxStudioCommon.h"
#include "DfxMathNodes.h"
#include "DfxGlobal.h"
#include "DfxTime.h"
#include "DfxTimeNodes.h"
#include "DfxSceneGraphSerializer.h"


class DfxSceneGraphNodeFunctions {
public:
	// IO
	static void saveGraph();
	static void loadGraph();

	// Math Nodes
	static void addSCountNode();
	static void addSUniformArrayNode();
	static void addSValueNode();
	static void addSAddNode();
	static void addSSubtractNode();
	static void addSMultiplyNode();
	static void addSDivideNode();

	static void addVCountNode();
	static void addVUniformArrayNode();
	static void addVValueNode();
	static void addVAddNode();

	// Point Nodes
	static void addMeshVoxelizerNode();
	static void addPointViewNode();
	static void addPointXFormNode();
	static void addPointGridNode();

	// Time Nodes
	static void addTimeInfoNode();

	// Item Nodes
	static void addSpotterNode();
	static void addCopyObjectNode();
	static void addObjectProxyViewerNode();
	static void addObjectInfoNode();
	static void addMapUniformDisplacementNode();
	static void addMapUniformDisplacementProxyNode();

	// Simulation Nodes
	static void addDFSPHNode();
	static void addSphRigidNode();
	static void addSphLiquidNode();
	static void addSphParticleCacheNode();

	static void addBulletSolver();
	static void addBulletRigidObject();
	static void addBulletRigidArray();
	
};