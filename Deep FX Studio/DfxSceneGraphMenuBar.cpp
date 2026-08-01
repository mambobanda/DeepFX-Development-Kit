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


#include <DfxNvBlastGlobal.h>
#include <QFileDialog>
#include <DfxCMeshVoxelizer.h>
#include <DfxCExplodeView.h>
#include <DfxPointItem.h>
#include <DfxCPointXForm.h>
#include <DfxCAlembicObjectCache.h>
#include <DfxCFoamGenerator.h>
#include <DfxMapUniformDisplacement.h>
#include <DfxMapUniformDisplacementProxy.h>
#include <DfxObjectItemInfo.h>
#include <DfxCSPHLiquid.h>
#include <DfxCSPHRigid.h>
#include <DfxCSPHMapChannel.h>
#include <DfxCSPHParticleCache.h>
#include <DfxCSPHViscosity.h>
#include <DfxCSPHVorticity.h>
#include <DfxCSPHAdhesion.h>
#include <DfxCSPHBulletRigid.h>
#include <DfxCDivergenceFreeSPHSolver.h>
#include <DfxCBulletSolver.h>
#include <DfxCBulletRigidObject.h>
#include <DfxCBulletRigidArray.h>
#include <DfxCBulletPointCache.h>
#include <DfxCBulletGlueConstraint.h>
#include <DfxCBulletPointToPointConstraint.h>
#include <DfxCBulletHingeConstraint.h>
#include <DfxPointGrid.h>
#include <DfxObjectItemCopy.h>
#include <DfxSpotterItem.h>
#include <DfxCamera.h>
#include <DfxLight.h>
#include <DfxObjectViewer.h>
#include <DfxMeshXForm.h>
#include <DfxCSPHShapeEmitter.h>
#include <DfxCParticleCacheLoader.h>
#include <DfxCParticleCache.h>
#include <DfxCParticleRigid.h>
#include <DfxCKillVolume.h>
#include <DfxObjectItemMerge.h>
#include "DfxCAlembicObjectCache.h"
#include "DfxLayerManager.h"
#include "DfxStudioCommon.h"
#include "DfxMathNodes.h"
#include "DfxGlobal.h"
#include "DfxTime.h"
#include "DfxCPlane.h"
#include "DfxTimeNodes.h"
#include "DfxSceneGraphSerializer.h"
#include "DfxSceneGraphMenuBar.h"
#include "DfxSceneGraphNodeFunctions.h"
#include "DfxVDBFiltersNodes.h"
#include "DfxVDBNodes.h"
#include "DfxCPBDGrainSolver.h"
#include "DfxCPBDGrainGrains.h"
#include "DfxCPBDGrainPointRigid.h"
#include "DfxCPBDGrainMapChannel.h"
#include "DfxStandardCompositeNode.h"
#include "DfxGlobalPropertyNodes.h"
#include "DfxRadeonProRenderNode .h"

#include <DfxCUniformMeshSampler.h>
#include <DfxVoronoiFracture.h>
#include <DfxCutoutFracture.h>
#include <DfxPlaneFracture.h>
#include <DfxSliceFracture.h>
#include "DfxVDBParticleMesher.h"

DfxSceneGraphMenuBar::DfxSceneGraphMenuBar(QWidget *parent): QMenuBar(parent) {

	rootMenu = nullptr;
}

void DfxSceneGraphMenuBar::createAddMenu(QMenu *menu) {

	rootMenu = menu;

#if	ENABLE_PROTOTYPE_FEATURES
	// Global Properties
	addMenuEntry("Globals/Scalar Global", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::ScalarValueGlobal(DfxNodes::ScalarValueGlobal_Identifier));
	});
#endif

	// Scalar Math Nodes
	addMenuEntry("Math/Scalar/Count", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::sCountNode(DfxNodes::sCountNode_Identifier));
	});

	addMenuEntry("Math/Scalar/Increment", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::sIncrementNode(DfxNodes::sIncrementNode_Identifier));
	});

	addMenuEntry("Math/Scalar/Print Scalar", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::sPrintValueNode(DfxNodes::sPrintValueNode_Identifier));
	});

	addMenuEntry("Math/Scalar/Random Array", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::sRandomArrayNode(DfxNodes::sRandomArrayNode_Identifier));
	});

	addMenuEntry("Math/Scalar/Uniform Array", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::sUniformArrayNode(DfxNodes::sUniformArrayNode_Identifier));
	});

	addMenuEntry("Math/Scalar/Value", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::sValueNode(DfxNodes::sValueNode_Identifier));
	});

	addMenuEntry("Math/Scalar/Add", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::sAddNode(DfxNodes::sAddNode_Identifier));
	});

	addMenuEntry("Math/Scalar/Subtract", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::sSubtractNode(DfxNodes::sSubtractNode_Identifier));
	});

	addMenuEntry("Math/Scalar/Multiply", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::sMultiplyNode(DfxNodes::sMultiplyNode_Identifier));
	});

	addMenuEntry("Math/Scalar/Divide", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::sDivideNode(DfxNodes::sDivideNode_Identifier));
	});

	// Group Nodes
	addMenuEntry("Groups/Group", []() {
		DfxLayerManager::getInstance()->addGroupNode(nullptr);	
	});

	// Time Nodes
	addMenuEntry("Time/Time Info", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::TimeInfo(DfxNodes::TimeInfo_Identifier));
	});

	// Vector Math Nodes
	addMenuEntry("Math/Vector/Count", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::vCountNode(DfxNodes::vCountNode_Identifier));
	});

	addMenuEntry("Math/Vector/Uniform Array", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::vUniformArrayNode(DfxNodes::vUniformArrayNode_Identifier));
	});

	addMenuEntry("Math/Vector/Value", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::vValueNode(DfxNodes::vValueNode_Identifier));
	});

	addMenuEntry("Math/Vector/Add", []() {
		DfxLayerManager::getInstance()->addNode(new DfxNodes::vAddNode(DfxNodes::vAddNode_Identifier));
	});

	// Point Nodes
	addMenuEntry("Points/Point XForm", []() {
		DfxLayerManager::getInstance()->addNode(new dfxcore::PointXForm(dfxcore::PointXForm_Identifier));
	});

	addMenuEntry("Points/Point View", []() {
		DfxGlobal::command::exec("addPointView");
	});

	addMenuEntry("Points/Point Grid", []() {
		DfxLayerManager::getInstance()->addNode(new dfxcore::DfxPointGrid(dfxcore::DfxPointGrid_Identifier));
	});

	addMenuEntry("Points/Mesh Voxelizer", []() {
		DfxLayerManager::getInstance()->addNode(new  dfxcore::MeshVoxelizer(dfxcore::MeshVoxelizer_Identifier));
	});

	addMenuEntry("Points/Uniform Mesh Sampler", []() {
		DfxLayerManager::getInstance()->addNode(new  dfxcore::DfxUniformMeshSamplerNode(dfxcore::DfxUniformMeshSamplerNode_Identifier));
	});

	// Particle Nodes
	addMenuEntry("Particles/Foam Generator", []() {
		DfxLayerManager::getInstance()->addNode(new  dfxcore::FoamGenerator(dfxcore::FoamGenerator_Identifier));
	});

	addMenuEntry("Particles/Particle Rigid", []() {
		DfxLayerManager::getInstance()->addNode(new  dfxcore::ParticleRigidObject(dfxcore::ParticleRigidObject_Identifier));
	});

	addMenuEntry("Particles/Particle Cache", []() {
		DfxLayerManager::getInstance()->addViewNode(new  dfxcore::ParticleCache(dfxcore::ParticleCache_Identifier));
	});

	addMenuEntry("Particles/Particle Kill Volume", []() {
		DfxLayerManager::getInstance()->addNode(new  dfxcore::ParticleKillVolume(dfxcore::ParticleKillVolume_Identifier));
	});


	// Meshing Nodes
#ifdef USE_OPENVDB_LIB
		addMenuEntry("Point Meshing/VDB Particle Mesher", []() {
			DfxLayerManager::getInstance()->addNode(new  DfxNodes::DfxVDBParticleMesher(DfxNodes::DfxVDBParticleMesher_Identifier));
		});

		addMenuEntry("Point Meshing/VDB Dilate Filter", []() {
			DfxLayerManager::getInstance()->addNode(new  DfxNodes::DfxVDBDilateNode(DfxNodes::DfxVDBDilateNode_Identifier));
		});

		addMenuEntry("Point Meshing/VDB Erode Filter", []() {
			DfxLayerManager::getInstance()->addNode(new  DfxNodes::DfxVDBErodeNode(DfxNodes::DfxVDBErodeNode_Identifier));
		});

		addMenuEntry("Point Meshing/VDB Median Filter", []() {
			DfxLayerManager::getInstance()->addNode(new  DfxNodes::DfxVDBMedianNode(DfxNodes::DfxVDBMedianNode_Identifier));
		});

		addMenuEntry("Point Meshing/VDB Mean Filter", []() {
			DfxLayerManager::getInstance()->addNode(new  DfxNodes::DfxVDBMeanNode(DfxNodes::DfxVDBMeanNode_Identifier));
		});

		addMenuEntry("Point Meshing/VDB Gaussian Filter", []() {
			DfxLayerManager::getInstance()->addNode(new  DfxNodes::DfxVDBGaussianNode(DfxNodes::DfxVDBGaussianNode_Identifier));
		});
#endif

	// Open VDB Nodes
#ifdef USE_OPENVDB_LIB
		addMenuEntry("Open VDB/Mesh To Volume", []() {
			DfxLayerManager::getInstance()->addNode(new  DfxNodes::VDBMeshToVolume(DfxNodes::VDBMeshToVolume_Identifier));
		});

		addMenuEntry("Open VDB/Volume To Mesh", []() {
			DfxLayerManager::getInstance()->addNode(new  DfxNodes::VDBVolumeToMesh(DfxNodes::VDBVolumeToMesh_Identifier));
		});

		addMenuEntry("Open VDB/Volume To Points", []() {
			DfxLayerManager::getInstance()->addNode(new  DfxNodes::VDBVolumeToPoints(DfxNodes::VDBVolumeToPoints_Identifier));
		});

		addMenuEntry("Open VDB/Boolean", []() {
			DfxLayerManager::getInstance()->addNode(new DfxNodes::VDBBoolean(DfxNodes::VDBBoolean_Identifier));
		});

		addMenuEntry("Open VDB/Dilate", []() {
			DfxLayerManager::getInstance()->addNode(new  DfxNodes::VDBDilate(DfxNodes::VDBDilate_Identifier));
		});

		addMenuEntry("Open VDB/Filters", []() {
			DfxLayerManager::getInstance()->addNode(new  DfxNodes::VDBFilter(DfxNodes::VDBFilter_Identifier));
		});

		addMenuEntry("Open VDB/Resample", []() {
			DfxLayerManager::getInstance()->addNode(new  DfxNodes::VDBResample(DfxNodes::VDBResample_Identifier));
		});

		addMenuEntry("Open VDB/Points To Volume", []() {
			DfxLayerManager::getInstance()->addNode(new  DfxNodes::VDBPointsToVolume(DfxNodes::VDBPointsToVolume_Identifier));
		});

		addMenuEntry("Open VDB/Save VDB", []() {
			DfxLayerManager::getInstance()->addNode(new  DfxNodes::VDBToFile(DfxNodes::VDBToFile_Identifier));
		});
#endif

	// Render Nodes
#if ENABLE_PROTOTYPE_FEATURES
	addMenuEntry("Render/Radeon Pro Render", []() {
		DfxLayerManager::getInstance()->addNode(new radeon::DfxRadeonProRenderNode(radeon::DfxRadeonProRenderNode_Identifier));
	});
#endif

	// Item Nodes
#if	ENABLE_PROTOTYPE_FEATURES
	addMenuEntry("Items/Camera", []() {
		DfxLayerManager::getInstance()->addViewNode(new DfxCamera(DfxCamera_Identifier));
	});

	addMenuEntry("Items/Light", []() {
		DfxLayerManager::getInstance()->addViewNode(new DfxLight(DfxLight_Identifier));
	});
#endif

	addMenuEntry("Items/Implicit Plane", []() {
		DfxLayerManager::getInstance()->addViewNode(new dfxcore::DfxCPlane(dfxcore::DfxCPlane_Identifier));
	});

	addMenuEntry("Items/Spotter", []() {
		DfxGlobal::command::exec("addSpotter");
		DfxGlobal::command::exec("updateViewport");
	});

	addMenuEntry("Items/Copy Mesh", []() {
		DfxLayerManager::getInstance()->addNode(new DfxObjectItemCopy(DfxObjectItemCopy_Identifier));
	});

	addMenuEntry("Items/Alembic Mesh Cache", []() {
		DfxLayerManager::getInstance()->addNode(new dfxcore::DfxCAlembicObjectCache(dfxcore::DfxAlembicObjectCache_Identifier));
	});

	addMenuEntry("Items/Object Viewer", []() {
		DfxGlobal::command::exec("addObjectProxyViewer");
	});

	addMenuEntry("Items/Object Merger", []() {
		DfxLayerManager::getInstance()->addNode(new DfxMergeObjectItem(DfxMergeObjectItem_Identifier));
	});

	addMenuEntry("Items/Mesh XForm", []() {
		DfxLayerManager::getInstance()->addNode(new DfxMeshXForm(DfxMeshXForm_Identifier));
	});

	/*addMenuEntry("Items/Map Uniform Displacment(Object)", []() {
		DfxLayerManager::getInstance()->addNode(new DfxMapUniformDisplacement(DfxMapUniformDisplacement_Identifier));
	});

	addMenuEntry("Items/Map Uniform Displacment(Proxy)", []() {
		DfxLayerManager::getInstance()->addNode(new DfxMapUniformDisplacementProxy(DfxMapUniformDisplacementProxy_Identifier));
	});*/

	// SPH Nodes
	addMenuEntry("SPH/Map SPH Channel", []() {
		DfxLayerManager::getInstance()->addNode(new dfxcore::sph::SPHMapChannel(dfxcore::sph::SPHMapChannel_Identifier));
	});

	addMenuEntry("SPH/SPH Shape Emitter", []() {
		DfxLayerManager::getInstance()->addViewNode(new dfxcore::sph::SPHShapeEmitter(dfxcore::sph::SPHShapeEmitter_Identifier));
	});

	addMenuEntry("SPH/SPH Liquid", []() {
		DfxLayerManager::getInstance()->addNode(new dfxcore::sph::SPHLiquid(dfxcore::sph::SPHLiquid_Identifier));
	});

	addMenuEntry("SPH/SPH Point Rigid", []() {
		DfxLayerManager::getInstance()->addNode(new dfxcore::sph::SPHRigid(dfxcore::sph::SPHRigid_Identifier));
	});

	addMenuEntry("SPH/SPH Bullet Rigid", []() {
		DfxLayerManager::getInstance()->addViewNode(new dfxcore::sph::SPHBulletRigid(dfxcore::sph::SPHBulletRigid_Identifier));
	});

	addMenuEntry("SPH/DFSPH Solver", []() {
		DfxLayerManager::getInstance()->addNode(new dfxcore::sph::DivergenceFreeSPHSolver(dfxcore::sph::DivergenceFreeSPHSolver_Identifier));
	});

	addMenuEntry("SPH/SPH Viscosity", []() {
		DfxLayerManager::getInstance()->addNode(new dfxcore::sph::SPHViscosity(dfxcore::sph::SPHViscosity_Identifier));
	});

	addMenuEntry("SPH/SPH Vorticity", []() {
		DfxLayerManager::getInstance()->addNode(new dfxcore::sph::SPHVorticity(dfxcore::sph::SPHVorticity_Identifier));
	});

	addMenuEntry("SPH/SPH Adhesion", []() {
		DfxLayerManager::getInstance()->addNode(new dfxcore::sph::SPHAdhesion(dfxcore::sph::SPHAdhesion_Identifier));
	});

	// Grain Physics
	addMenuEntry("Grains/Map Grain Channel", []() {
		DfxLayerManager::getInstance()->addNode(new dfxcore::pbd::PBDGrainMapChannel(dfxcore::pbd::PBDGrainMapChannel_Identifier));
	});

	addMenuEntry("Grains/Grain Solver", []() {
		DfxLayerManager::getInstance()->addNode(new dfxcore::pbd::GrainSolver(dfxcore::pbd::GrainSolver_Identifier));
	});

	addMenuEntry("Grains/Grains", []() {
		DfxLayerManager::getInstance()->addNode(new dfxcore::pbd::PBDGrains(dfxcore::pbd::PBDGrains_Identifier));
	});

	addMenuEntry("Grains/Grains Point Rigid", []() {
		DfxLayerManager::getInstance()->addNode(new dfxcore::pbd::GrainPointRigid(dfxcore::pbd::GrainPointRigid_Identifier));
	});

	// Bullet Physics
	addMenuEntry("Bullet Physics/Bullet Point Cache", []() {
		DfxLayerManager::getInstance()
			->addViewNode(new dfxcore::bullet::DfxCBulletPointCache(dfxcore::bullet::DfxCBulletPointCache_Identifier));
	});
	
	addMenuEntry("Bullet Physics/Bullet Solver", []() {
		DfxLayerManager::getInstance()->addNode(new dfxcore::bullet::DfxCBulletSolver(dfxcore::bullet::DfxCBulletSolver_Identifier));
	});

	addMenuEntry("Bullet Physics/Bullet Rigid Object", []() {
		DfxLayerManager::getInstance()
			->addNode(new dfxcore::bullet::DfxCBulletRigidObject(dfxcore::bullet::DfxCBulletRigidObject_Identifier));
	});

	addMenuEntry("Bullet Physics/Constraints/Bullet Glue Constraint", []() {
		DfxLayerManager::getInstance()
			->addNode(new dfxcore::bullet::BulletGlueConstraint(dfxcore::bullet::BulletGlueConstraint_Identifier));
	});

	addMenuEntry("Bullet Physics/Constraints/Bullet Point To Point Constraint", []() {
		DfxLayerManager::getInstance()
			->addNode(new dfxcore::bullet::BulletPointToPointConstraint(dfxcore::bullet::BulletPointToPointConstraint_Identifier));
	});

	addMenuEntry("Bullet Physics/Constraints/Bullet Hinge Constraint", []() {
		DfxLayerManager::getInstance()
			->addNode(new dfxcore::bullet::BulletHingeConstraint(dfxcore::bullet::BulletHingeConstraint_Identifier));
	});

	/*
	addMenuEntry("Bullet Physics/Rigid Array", []() {
		DfxLayerManager::getInstance()
			->addNode(new dfxcore::bullet::DfxCBulletRigidArray(dfxcore::bullet::DfxCBulletRigidArray_Identifier));
	});
	*/

	// Fracture
	addMenuEntry("Fracture/Explode View", []() {
		DfxLayerManager::getInstance()
			->addViewNode(new dfxcore::DfxCExplodeView(dfxcore::DfxCExplodeView_Identifier));
	});


	if (dfxblast::DfxNvBlastGlobal::isInitiallized()) {
		addMenuEntry("Fracture/Voronoi Fracture", []() {
			DfxLayerManager::getInstance()
				->addNode(new dfxcore::DfxVoronoiFracture(dfxcore::DfxVoronoiFractureNode_Identifier));
		});
	}

	if(dfxblast::DfxNvBlastGlobal::isInitiallized()) {
		addMenuEntry("Fracture/Cutout Fracture", []() {
			DfxLayerManager::getInstance()
				->addNode(new dfxcore::DfxCutoutFracture(dfxcore::DfxCutoutFracture_Identifier));
		});
	}

	if (dfxblast::DfxNvBlastGlobal::isInitiallized()) {
		addMenuEntry("Fracture/Slice Fracture", []() {
			DfxLayerManager::getInstance()
				->addNode(new dfxcore::DfxSliceFracture(dfxcore::DfxSliceFracture_Identifier));
	});
	}

	if (dfxblast::DfxNvBlastGlobal::isInitiallized()) {
		addMenuEntry("Fracture/Plane Fracture", []() {
			DfxLayerManager::getInstance()
				->addNode(new dfxcore::DfxPlaneFracture(dfxcore::DfxPlaneFracture_Identifier));
		});
	}

#if	ENABLE_PROTOTYPE_FEATURES
	// MISC
	addMenuEntry("MISC/Composite Node", []() {
		DfxLayerManager::getInstance()
			->addCompositeNode(new DfxNodes::DfxStandardCompositeNode());
	});
#endif
}

void DfxSceneGraphMenuBar::createFileMenu(QMenu *menu) {

}

void DfxSceneGraphMenuBar::createViewMenu(QMenu *menu) {

}

void DfxSceneGraphMenuBar::addMenuEntry(std::string path, EntryAction at) {
	std::queue<std::string> queue;
	addMenuEntry(path, at, queue, nullptr, true);
}

void DfxSceneGraphMenuBar::setRootMenu(QMenu *rt) {
	rootMenu = rt;
}

void DfxSceneGraphMenuBar::addMenuEntry(std::string path, EntryAction at, std::queue<std::string> &mqueue, QAction *parent, bool root ){

	// If this is the first recursion, split the path and load it in the Queue
	if (root == true) {

		QString temp(path.c_str());
		auto list = temp.split(QChar('/'));

		for (size_t inx = 0; inx < list.size(); inx++) {
			mqueue.push(list[inx].toUtf8().toStdString());
		}

		if (mqueue.size() > 0) {
			
			addMenuEntry("", at, mqueue, nullptr);
		}

	}
	else {

		QList<QAction*> _actions; // Exisiting actions to iterate over

		// If parent is null use the toolbar actions as a starting point
		if (parent == nullptr) {
			_actions = rootMenu->actions();
		}
		else {

			// If the parent is not a menu return.
			if (parent->menu() != Q_NULLPTR) {
				_actions = parent->menu()->actions(); 
			}
			else {
				return;
			}
		}

		// Iterate over functions
		for (auto act : _actions) {

			// Check if menu/submeu already exits
			if (act->text().compare(mqueue.front().c_str(), Qt::CaseInsensitive) == 0) {
				
				// We've reached the end of the Queue, terminate, because all
				// last actions cannot be menus
				if (mqueue.size() == 1) {
					return;
				}
				else {
					mqueue.pop();
					addMenuEntry("", at, mqueue, act);
					return;
				}
			}
		}

		// If the front of the queue doesn't match any of the exisiting 
		// menus, create the whole hiearchy.
		if (mqueue.size() > 1) {

			// Create Menus and register action
			QMenu *ptr;
			while (mqueue.size() > 1) {
				QMenu *new_menu = new QMenu(mqueue.front().c_str());

				if (parent == nullptr){
					parent = rootMenu->addMenu(new_menu);
				}
				else {
					parent = parent->menu()->addMenu(new_menu);
				}

				mqueue.pop();
			}

			QAction *new_action = new QAction(mqueue.front().c_str());
			parent->menu()->addAction(new_action);
			QObject::connect(new_action, &QAction::triggered, at);
		}
		else if (mqueue.size() == 1) {
			
			// If we are on the last element, connect the 
			// action.
			if (parent != nullptr) {
				if (parent->menu() != Q_NULLPTR) {

					QAction *new_action = new QAction(mqueue.front().c_str());
					parent->menu()->addAction(new_action);
					QObject::connect(new_action, &QAction::triggered, at);
				}
			}
		}	
	}
}