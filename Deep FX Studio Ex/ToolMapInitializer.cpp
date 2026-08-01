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


#include "ToolMapInitializer.h"

namespace toolmap {
	void initialize() {

		DfxToolMap::getInstance()
		->addDomain("Main", "")
			->addSection("Create / Edit", "")
				->addGroup("Mesh", "")
					->addItem("Plane", "", "addPlanePrimitive")
					->addItem("Cube", "", "addCubePrimitive")
					->addItem("Shpere", "", "addSpherePrimitive")
					->addItem("Torus Knot", "", "addTorusKnotPrimitive")
					->addItem("Torus", "", "addTorusPrimitive")
					->addItem("Empty", "", "addEmptyPrimitive")
					// ->addItem("Spring", "", "addSpringPrimitive")
					// ->addItem("Teapot", "", "addTeapotPrimitive")
					// ->addItem("Cylinder", "", "addCylinderPrimitive")
					// ->addItem("Tube", "", "addTubePrimitive")
					// ->addItem("Capsule", "", "addCapsulePrimitive")
					->end()
				->addGroup("Point Cloud", "")
					->addItem("Point Cloud", "", "addPointCloud")
					->addItem("Point Generator", "", "addPointGenerator")
					->addItem("Point Sampler", "", "addPointSampler")
					->addItem("Point Instancer", "", "addPointAttributeInstancer")
					->addItem("Point Reader", "", "addPointReader")
					->addItem("Point Mesher", "", "addPointMesher")
					->addMenu("Shapers", "")
						->addMenuItem("Shape", "", "addPointShapeTool")
						->addMenuItem("K-Proximity", "", "addPointProximityKillTool")
						->addMenuItem("S-Proximity", "", "addPointProximityScaleTool")
						->end()
					->end()
				->addGroup("Volume", "")
					->addItem("Volume", "", "addGridNode")
					->addItem("VDB Loader", "", "addVDBLoader")
					->addItem("Mesh To Volume", "", "addMeshToGridNode")
					->end()
				->addGroup("Alembic", "")
					->addItem("Alembic Loader", "", "addAlembicLoader")
					// ->addItem("Alembic Saver", "", "")
					->end()
				/*->addGroup("Edit", "")
					->addItem("Duplicate", "", "duplicate")
					->end()*/
				->addGroup("Item", "")
					->addItem("Null", "", "addNull")
					->addItem("Group", "", "addGroup")
					->end()
				->end()
			->addSection("Mesh", "")
				->addGroup("Selection", "")
					->addItem("Connected Faces", "", "selectConnectedFaces")
					->addItem("Face Loop", "", "selectFaceLoop")
					->addItem("All Faces", "", "selectAllFaces")
					/*->addItem("Invert", "", "command")
					->addItem("V Incident Edge", "", "selectVIncidentEdges")
					->addItem("V Incident Face", "", "selectVIncidentFaces")
					->addItem("E Adjacent Vertex", "", "selectEAdjacentEdges")
					->addItem("Print Edge ID", "", "printEdgeID")
					->addItem("Print Face ID", "", "printFaceID")
					->addItem("Print Vertex ID", "", "printVertexID")*/	
					->end()
				/*->addGroup("Surface", "")
					->addItem("Set Surface", "", "assignSurface")
					->end()*/
			   /*	->addGroup("Normals", "")
					->addItem("Smooth Normals", "", "")
					->addItem("Face Normals", "", "")
					->end()*/
				/*->addGroup("Face Tools", "")
					->addItem("Extrude", "", "extrudeFaces")
					->addItem("Triangulate", "", "triangulateFaces")
					->addItem("Bridge", "", "command")
					->addItem("Smooth", "", "command")
					->addItem("Face Loop", "", "command")
					->addItem("Subdivide", "", "command")
					->end()
				->addGroup("Edge Tools", "")
					->addItem("Split", "", "command")
					->addItem("Cut", "", "command")
					->addItem("Extrude", "", "command")
					->addItem("Bevel", "", "bevelEdge")
					->addItem("Slide", "", "command")
					->end()
				->addGroup("Vertex Tools", "")
					->addItem("Connect", "", "command")
					->end()*/
				->end()
			->addSection("Render", "")
				->addGroup("OSPRay", "")
					->addItem("Render Node", "", "addOSPRayRender")
					->addItem("Camera", "", "addOSPRayCamera")
					->addItem("Point Light", "", "addOSPRayPointLight")
					->addItem("HDRI Light", "", "addOSPRayHdriLight")
					->addItem("Sun Sky Light", "", "addOSPRaySunSkyLight")
					->end()
				->addGroup("Luxcore", "")
					->addItem("Render Node", "", "addLxRenderNode")
					->addItem("Camera", "", "addLxCamera")
					->addItem("Point Light", "", "addLxPointLight")
					->addItem("Environment Light", "", "addLxEnvironmentLight")
					->addItem("Sun Sky Light", "", "addLxSunSkyLight")
					->end()
					
				/*->addGroup("View", "")
					->addItem("Play Blast", "", "")
					->end()*/
				->end()
			->addSection("Simulation", "")
				->addGroup("Bullet", "")
					->addItem("Bullet Stage", "", "addBulletStage")
					->addItem("Bullet Constraint", "", "addBulletConstraint")
					/*->addItem("Add Rigid", "", "command")
					->addItem("Add Softbody", "", "command")
					->addItem("Add Force", "", "command")
					->addItem("Add Contraints", "", "command")*/
					->end()
				->addGroup("Riptide", "")
					->addItem("Riptide Stage", "", "addRiptideStage")
					->addItem("Riptide Cache", "", "addRiptideCache")
					->addItem("Riptide Emitter", "", "addRiptideEmitter")
					->addItem("Riptide Heater", "", "addRiptideHeater")
					->end()
				->addGroup("Entropy", "")
					->addItem("Entropy Stage", "", "addEntropyStage")
					->addItem("Entropy Domain", "", "addEntropyDomain")
					->addItem("Entropy Cache", "", "addEntropyCache")
					->end();
				/*->addGroup("Partinomics FX", "")
					->addItem("Pt Stage", "", "")
					->end();		*/		
	}
}