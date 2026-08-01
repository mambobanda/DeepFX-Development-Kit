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

#include <glm/vec2.hpp>
#include "DfxNvBlastGlobal.h"

namespace dfxblast {

	bool DfxNvBlastGlobal::initSuccess = false;
	DfxNvBlastGlobal *DfxNvBlastGlobal::instance = nullptr;

	DfxNvBlastGlobal::DfxNvBlastGlobal() {

	}

	DfxNvBlastGlobal::~DfxNvBlastGlobal() {

	}

	void DfxNvBlastGlobal::init() {

		using namespace Nv::Blast;

		NvBlastGlobalSetErrorCallback(&errorCallback);

		gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, NvBlastGetPxAllocatorCallback(), NvBlastGetPxErrorCallback());
		if (!gFoundation)
		{
			DfxGlobal::console::println("Can't init PhysX foundation\n");
			return;
		}

		physx::PxTolerancesScale scale;
		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, scale, true);
		if (!gPhysics)
		{
			DfxGlobal::console::println("Can't create Physics\n");
			return;
		}

		physx::PxCookingParams cookingParams(scale);
		cookingParams.buildGPUData = true;
		gCooking = PxCreateCooking(PX_PHYSICS_VERSION, gPhysics->getFoundation(), cookingParams);
		if (!gCooking)
		{
			DfxGlobal::console::println("Can't create Cooking\n");
			return;
		}

		initSuccess = true;
	}

	bool DfxNvBlastGlobal::isInitiallized() {
		return initSuccess;
	}

	physx::PxFoundation*	DfxNvBlastGlobal::foundation() const {
		return gFoundation;
	}

	physx::PxPhysics*		DfxNvBlastGlobal::physics() const {
		return gPhysics;
	}

	physx::PxCooking*		DfxNvBlastGlobal::cooking() const {
		return gCooking;
	}

	DfxNvBlastGlobal *DfxNvBlastGlobal::getInstance() {

		if (instance == nullptr) {
			instance = new DfxNvBlastGlobal();
		}

		return instance;
	
	}

	FractureTool DfxNvBlastGlobal::newFractureTool(Dfx3dMeshData* mesh, glm::mat4 trans) {
		using namespace Nv::Blast;

		Nv::Blast::FractureTool* fTool = NvBlastExtAuthoringCreateFractureTool();

		physx::PxVec3* pos = new physx::PxVec3[mesh->getVertexCount()];
		physx::PxVec3* norm = new physx::PxVec3[mesh->getVertexCount()];
		physx::PxVec2* uv = new physx::PxVec2[mesh->getVertexCount()];
		uint32_t *indicies = new uint32_t[mesh->getIndexCount()];

		for (size_t i = 0; i < mesh->getVertexCount(); i++) {
			glm::vec3  p = glm::vec3(trans * glm::vec4(mesh->getVertex(i), 1.f));
			glm::vec3  n = mesh->getNormal(i);
			glm::vec2  t = mesh->getUV(i);

			pos[i] = physx::PxVec3(p.x, p.y, p.z);
			norm[i] = physx::PxVec3(n.x, n.y, n.z);
			uv[i] = physx::PxVec2(t.x, t.y);
		}

		for (size_t i = 0; i < mesh->getIndexCount(); i++) {
			indicies[i] = mesh->getIndex(i);
		}

		Nv::Blast::Mesh* nv_mesh = NvBlastExtAuthoringCreateMesh(pos, norm, uv, mesh->getVertexCount(), indicies, mesh->getIndexCount());

		MeshCleaner* clr = NvBlastExtAuthoringCreateMeshCleaner();
		Nv::Blast::Mesh* nmesh;
		nmesh = clr->cleanMesh(nv_mesh);
		clr->release();
		nv_mesh->release();
		nv_mesh = nmesh;

		fTool->setSourceMesh(nv_mesh);

		delete[] pos;
		delete[] norm;
		delete[] uv;
		delete[] indicies;

		FractureTool ft;
		ft.ftool = fTool;
		ft.mesh = nv_mesh;
		
		return ft;
	}

	void DfxNvBlastGlobal::extractFracturePieces(Nv::Blast::FractureTool *ftool, std::vector<Dfx3dMeshData*> &arr) {
		using namespace Nv::Blast;

		if (ftool->getChunkCount() > 0) {

			for (size_t i = 1; i < ftool->getChunkCount(); i++) {

				Triangle*  triangleBuffer = nullptr;

				uint32_t tcount = ftool->getBaseMesh(i, triangleBuffer);

				IndexedBlastMesh ibm;
				ibm.buildIndexedMesh(triangleBuffer, tcount);

				if (ibm.getVertexArray().size() > 0) {
					Dfx3dMeshData *dfx_mesh = new Dfx3dMeshData();

					dfx_mesh->resizeVertexBuffer(ibm.getVertexArray().size());
					dfx_mesh->resizeIndexBuffer(ibm.getIndexArray().size());

					for (size_t j = 0; j < ibm.getVertexArray().size(); j++) {

						physx::PxVec3 p = ibm.getVertexArray()[j].p;
						physx::PxVec3 n = ibm.getVertexArray()[j].n;
						physx::PxVec2 uv = ibm.getVertexArray()[j].uv[0];

						dfx_mesh->setVertex(j, glm::vec3(p.x, p.y ,p.z));
						dfx_mesh->setNormal(j, glm::vec3(n.x, n.y, n.z));
					}

					for (size_t j = 0; j < ibm.getIndexArray().size(); j++) {

						dfx_mesh->setIndex(j, ibm.getIndexArray()[j]);
					}

					arr.push_back(dfx_mesh);
				}

				delete[] triangleBuffer;
			}
		}
	}

	std::vector<Dfx3dMeshData*> DfxNvBlastGlobal::voronoiFracture(Dfx3dMeshData* mesh, VoronoiOptions opts, glm::mat4 trans, std::vector<glm::vec3> samples) {
		using namespace Nv::Blast;

		Nv::Blast::VoronoiSitesGenerator* voronoiSitesGenerator = nullptr;
		std::vector<Dfx3dMeshData*> fracture_pieces;
		const physx::PxVec3* sites = nullptr;
		uint32_t sitesCount = 0;
		/*if (!initSuccess)
		{
			DfxGlobal::console::println("Failed to initialize PhysX"); 
			return fracture_pieces;
		}*/

		FractureTool ft = DfxNvBlastGlobal::newFractureTool(mesh, trans);
		
		if (samples.size() <= 0) {
			SimpleRandomGenerator rng;
			rng.seed(opts.rand_seed);
			voronoiSitesGenerator = NvBlastExtAuthoringCreateVoronoiSitesGenerator(ft.mesh, &rng);

			if (voronoiSitesGenerator == nullptr)
			{
				DfxGlobal::console::println("Failed to create Voronoi sites generator");
				ft.ftool->release();
				ft.mesh->release();
				voronoiSitesGenerator->release();
				return fracture_pieces;
			}

			voronoiSitesGenerator->uniformlyGenerateSitesInMesh(opts.cell_count);
			sitesCount = voronoiSitesGenerator->getVoronoiSites(sites);
		}
		else {
			auto _sites = new physx::PxVec3[samples.size()];

			size_t counter = 0;
			for (auto v : samples) {
				_sites[counter] = physx::PxVec3(v.x, v.y, v.z);
				counter++;
			}

			sites = _sites;
			sitesCount = samples.size();
		}
		
		if (ft.ftool->voronoiFracturing(0, sitesCount, sites, false) != 0)
		{
			DfxGlobal::console::println("Failed to fracture with Voronoi");
			ft.ftool->release();
			ft.mesh->release();
			
			return fracture_pieces;
		}

		ft.mesh->release();

		ft.ftool->finalizeFracturing();
		extractFracturePieces(ft.ftool, fracture_pieces);

		ft.ftool->release();

		if (samples.size() <= 0) {
			voronoiSitesGenerator->release();
		}
		else {
			delete[] sites;
		}

		return fracture_pieces;
	}

	std::vector<Dfx3dMeshData*> DfxNvBlastGlobal::cutoutFracture(Dfx3dMeshData* mesh, CutoutOptions opts, glm::mat4 trans) {
		using namespace Nv::Blast;

		std::vector<Dfx3dMeshData*> fracture_pieces;
		physx::PxVec3 pnormal(opts.plane_normal.x, opts.plane_normal.y, opts.plane_normal.z);
		physx::PxVec3 ppoint(opts.plane_point.x, opts.plane_point.y, opts.plane_point.z);

		FractureTool ft = DfxNvBlastGlobal::newFractureTool(mesh, trans);

		CutoutConfiguration cutoutConfig;
		physx::PxVec3 axis = pnormal;
		if (axis.isZero())
		{
			axis = physx::PxVec3(0.f, 0.f, 1.f);
		}
		axis.normalize();
		float d = axis.dot(physx::PxVec3(0.f, 0.f, 1.f));
		
		if (d < (1e-6f - 1.0f)) {
			cutoutConfig.transform.q = physx::PxQuat(physx::PxPi, physx::PxVec3(1.f, 0.f, 0.f));
		}
		else if (d < 1.f) {
			float s = physx::PxSqrt((1 + d) * 2);
			float invs = 1 / s;
			auto c = axis.cross(physx::PxVec3(0.f, 0.f, 1.f));
			cutoutConfig.transform.q = physx::PxQuat(c.x * invs, c.y * invs, c.z * invs, s * 0.5f);
			cutoutConfig.transform.q.normalize();
		}

		cutoutConfig.transform.p = ppoint;
		if (opts.bitmap != nullptr) {		
			cutoutConfig.cutoutSet = NvBlastExtAuthoringCreateCutoutSet();
			NvBlastExtAuthoringBuildCutoutSet(*cutoutConfig.cutoutSet, opts.bitmap, opts.map_width, opts.map_height, 0.001f, 1.f, false, true);
			
			SimpleRandomGenerator rng;
			rng.seed(0);

			if (ft.ftool->cutout(0, cutoutConfig, false, &rng) != 0) {
				DfxGlobal::console::println("Failed to fracture with cutout");
				ft.ftool->release();
				ft.mesh->release();

				return fracture_pieces;
			}

			ft.mesh->release();

			ft.ftool->finalizeFracturing();
			extractFracturePieces(ft.ftool, fracture_pieces);

			ft.ftool->release();
		}

		return fracture_pieces;
	}

	std::vector<Dfx3dMeshData*> DfxNvBlastGlobal::sliceFracture(Dfx3dMeshData* mesh, SliceOptions opts, glm::mat4 trans) {
		using namespace Nv::Blast;

		std::vector<Dfx3dMeshData*> fracture_pieces;


		FractureTool ft = DfxNvBlastGlobal::newFractureTool(mesh, trans);

		SlicingConfiguration slConfig;
		slConfig.x_slices = opts.x_slices;
		slConfig.y_slices = opts.y_slices;
		slConfig.z_slices = opts.z_slices;
		slConfig.angle_variations = opts.angleVariation;
		slConfig.offset_variations = opts.offsetVariation;

		SimpleRandomGenerator rng;
		rng.seed(0);

		if (ft.ftool->slicing(0, slConfig, false, &rng) != 0) {
			DfxGlobal::console::println("Failed to fracture with slicing");
			ft.ftool->release();
			ft.mesh->release();

			return fracture_pieces;
		}

		ft.mesh->release();

		ft.ftool->finalizeFracturing();
		extractFracturePieces(ft.ftool, fracture_pieces);

		ft.ftool->release();

		return fracture_pieces;
	}

	std::vector<Dfx3dMeshData*> DfxNvBlastGlobal::planeFracture(Dfx3dMeshData* mesh, CutOptions opts, glm::mat4 trans) {
		using namespace Nv::Blast;

		std::vector<Dfx3dMeshData*> fracture_pieces;

		NoiseConfiguration noise;
		noise.amplitude = opts.amplitude;
		noise.frequency = opts.frequency;
		noise.octaveNumber = opts.octaves;
		noise.samplingInterval = physx::PxVec3(opts.segments.x, opts.segments.y, opts.segments.z);
		physx::PxVec3 pnormal(opts.plane_normal.x, opts.plane_normal.y, opts.plane_normal.z);
		physx::PxVec3 ppoint(opts.plane_point.x, opts.plane_point.y, opts.plane_point.z);

		FractureTool ft = DfxNvBlastGlobal::newFractureTool(mesh, trans);

		SimpleRandomGenerator rng;
		rng.seed(0);

		if (ft.ftool->cut(0, pnormal, ppoint, noise, false, &rng) != 0) {
			DfxGlobal::console::println("Failed to fracture with Plane");
			ft.ftool->release();
			ft.mesh->release();

			return fracture_pieces;
		}

		ft.mesh->release();

		ft.ftool->finalizeFracturing();
		extractFracturePieces(ft.ftool, fracture_pieces);

		ft.ftool->release();

		return fracture_pieces;
	}
}