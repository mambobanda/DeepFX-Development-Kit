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

#include "ComputeNodeInitializer.h"

// Math Nodes
#include "DfxCCommonFloatNodes.h"
#include "DfxCConsoleOutNode.h"
#include "DfxCCommonStringNodes.h"
#include "DfxCCommonVectorNodes.h"
#include "DfxCGetData.h"
#include "DfxCSetData.h"
#include "DfxCTimeNode.h"
#include "DfxCBuildArrayNode.h"
#include "DfxCFloatRangeNode.h"
#include "DfxCFloatComparisionNodes.h"
#include "DfxCIfThenElseNode.h"

#include "DfxCForEachNode.h"
#include "DfxCMapNode.h"
#include "DfxCFilterNode.h"
#include "DfxCPartitionNode.h"
#include "DfxCPeekNode.h"
#include "DfxCReduceNode.h"

#include "DfxCSceneToSet.h"
#include "DfxCSetNode.h"
#include "DfxCMoveSetNode.h"
#include "DfxCToggleSetVisibility.h"
#include "DfxCUpdateSetAttribute.h"
#include "DfxCProximityNode.h"
#include "DfxCFilterFloatAttribute.h"
#include "DfxCSetSize.h"
#include "DfxCReadSetAttribute.h"

#include "DfxCSimplexToBullet.h"
#include "DfxCStreamToBullet.h"
#include "DfxCBulletRigidBodyAttributes.h"
#include "DfxCBulletActivationState.h"
#include "DfxCBulletAddForce.h"
#include "DfxCBulletKill.h"

#include "DfxCVoronoiFracture.h"

#include "DfxCExplodeSetNode.h"

#include "DfxCTrackSetSize.h"
#include "DfxCOverrideSetMaterialID.h"


namespace compute {
	void initNodes() {

		// Register Data Nodes
		// compute_node::registerGetDataNode();
		// compute_node::registerSetDataNode();

		// Register General Node
		compute_node::registerConsoleOutNode();
		compute_node::registerTimeNode();

		// Register Math Nodes
		compute_node::registerCommonFloatNodes();
		compute_node::registerFloatComparisonNodes();
		compute_node::registerCommonVectorNodes();

		// Register String Nodes
		compute_node::registerCommonStringNodes();

		// Register Array Nodes
		compute_node::registerBuildaArrayNode();
		compute_node::registerFloatRangeNode();

		// Register Iteration Nodes
		compute_node::registerForEachNode();
		compute_node::registerMapNode();
		compute_node::registerFilterNode();
		compute_node::registerPartitionNode();
		compute_node::registerPeekNode();
		compute_node::registerReduceNode();

		// Register Comparison Nodes
		compute_node::registerIfThenElseNode();

		// Register Set Nodes
		compute_node::registerSceneToSetNode();
		compute_node::registerSetNode();
		compute_node::registerUpdateSetAttributeNode();
		compute_node::registerToggleSetVisibilityNode();
		compute_node::registerProximityNode();
		compute_node::registerMoveSetNode();
		compute_node::registerFilterFloatAttributeNode();
		compute_node::registerSetSizeNode();
		compute_node::registerReadAttributeNode();

		compute_node::registerTrackSetSizeNode();
		compute_node::registerOverrideSetMatIDNode();

		// Register Bullet Nodes
		compute_node::registerSimplexToBulletNode();
		compute_node::registerStreamToBulletNode();
		compute_node::registerBulletRigidBodyAttributesNode();
		compute_node::registerBulletActivationStateNode();
		compute_node::registerBulletAddForceNode();
		compute_node::registerBulletKillNode();

		// Register Fracture Nodes
		compute_node::registerVoronoiFractureNode();

		// Register Explode Nodes
		compute_node::registerExplodeSetNode();
	}
}