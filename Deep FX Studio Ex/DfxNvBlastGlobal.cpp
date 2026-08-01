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

#ifdef NDEBUG
#undef _DEBUG
#endif 
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
			instance->init();
		}

		return instance;	
	}

	FractureTool DfxNvBlastGlobal::newFractureTool(deepsg::SgGeoAdapterInterface* mesh, glm::mat4 trans) {
		using namespace Nv::Blast;

		Nv::Blast::FractureTool* fTool = NvBlastExtAuthoringCreateFractureTool();

		size_t numIndices = mesh->getTriangleIndexListSize();

		NvcVec3 *pos = new NvcVec3[numIndices];
		NvcVec3 *norm = new NvcVec3[numIndices];
		NvcVec2 *uvs = new NvcVec2[numIndices];
		uint32_t *indices = mesh->getTriangleIndexList();
		uint32_t *uniform_indices = new uint32_t[numIndices];
		uint32_t *materialIDS = new uint32_t[(numIndices / 3)];
		
		std::unique_ptr<unsigned int> sourceFaceIndices(mesh->getTriangleSourceFaceIndexList());

		for (size_t i = 0; i < (numIndices / 3); i++) {
			int counter = i * 3;

			auto uvMap = mesh->getPolygonUVMap(sourceFaceIndices.get()[i]);

			auto v1 = glm::vec3(trans * glm::vec4(mesh->getVertex(indices[counter]), 1.f));
			auto v2 = glm::vec3(trans * glm::vec4(mesh->getVertex(indices[counter + 1]), 1.f));
			auto v3 = glm::vec3(trans * glm::vec4(mesh->getVertex(indices[counter + 2]), 1.f));

			auto n1 = mesh->getSmoothNormal(indices[counter]);
			auto n2 = mesh->getSmoothNormal(indices[counter + 1]);
			auto n3 = mesh->getSmoothNormal(indices[counter + 2]);

			auto auv = uvMap[indices[counter]];
			auto buv = uvMap[indices[counter + 1]];
			auto cuv = uvMap[indices[counter + 2]];
		
			pos[counter].x = v1.x;
			pos[counter].y = v1.y;
			pos[counter].z = v1.z;

			pos[counter + 1].x = v2.x;
			pos[counter + 1].y = v2.y;
			pos[counter + 1].z = v2.z;

			pos[counter + 2].x = v3.x;
			pos[counter + 2].y = v3.y;
			pos[counter + 2].z = v3.z;
			
			norm[counter].x = n1.x;
			norm[counter].y = n1.y;
			norm[counter].z = n1.z;

			norm[counter + 1].x = n2.x;
			norm[counter + 1].y = n2.y;
			norm[counter + 1].z = n2.z;

			norm[counter + 2].x = n3.x;
			norm[counter + 2].y = n3.y;
			norm[counter + 2].z = n3.z;

			uvs[counter].x = auv.x;
			uvs[counter].y = auv.y;

			uvs[counter + 1].x = buv.x;
			uvs[counter + 1].y = buv.y;

			uvs[counter + 2].x = cuv.x;
			uvs[counter + 2].y = cuv.y;

			uniform_indices[counter] = counter;
			uniform_indices[counter + 1] = counter + 1;
			uniform_indices[counter + 2] = counter + 2;

			materialIDS[i] = mesh->getPolygonSurfaceID(sourceFaceIndices.get()[i]);
		}

		Nv::Blast::Mesh* nv_mesh = NvBlastExtAuthoringCreateMesh(pos, norm, uvs, numIndices, uniform_indices, numIndices);
		nv_mesh->setMaterialId((const int32_t*)materialIDS);
		MeshCleaner* clr = NvBlastExtAuthoringCreateMeshCleaner();
		Nv::Blast::Mesh* nmesh;
		nmesh = clr->cleanMesh(nv_mesh);
		clr->release();
		nv_mesh->release();
		nv_mesh = nmesh;

		fTool->setSourceMesh(nv_mesh);

		delete[] pos;
		delete[] norm;
		delete[] uvs;
		delete[] indices;
		delete[] uniform_indices;
		delete[] materialIDS;

		FractureTool ft;
		ft.ftool = fTool;
		ft.mesh = nv_mesh;
		
		return ft;
	}

	void DfxNvBlastGlobal::extractFracturePieces(Nv::Blast::FractureTool *ftool, std::vector<Mesh*> &arr) {

		if (ftool->getChunkCount() > 0) {

			for (size_t i = 1; i < ftool->getChunkCount(); i++) {

				Nv::Blast::Triangle*  triangleBuffer = nullptr;

				uint32_t tcount = ftool->getBaseMesh(i, triangleBuffer);

				IndexedBlastMesh ibm;
				ibm.buildIndexedMesh(triangleBuffer, tcount);

				if (ibm.getVertexArray().size() > 0) {
					auto *mesh = new Mesh;

					mesh->nbVerts = ibm.getVertexArray().size();
					mesh->nbFaces = ibm.getIndexArray().size() / 3;
					mesh->verts = new glm::vec3[ibm.getVertexArray().size()];
					mesh->normals = new glm::vec3[ibm.getVertexArray().size()];
					mesh->indices = new unsigned int[ibm.getIndexArray().size()];

					for (size_t j = 0; j < ibm.getVertexArray().size(); j++) {

						NvcVec3 p = ibm.getVertexArray()[j].p;
						NvcVec3 n = ibm.getVertexArray()[j].n;
						// NvcVec2 uv = ibm.getVertexArray()[j].uv[0];

						mesh->verts[j] = glm::vec3(p.x, p.y ,p.z);
						mesh->normals[j] = glm::vec3(n.x, n.y, n.z);
					}

					for (size_t j = 0; j < ibm.getIndexArray().size(); j++) {
						mesh->indices[j] = ibm.getIndexArray()[j];
					}

					arr.push_back(mesh);
				}

				delete[] triangleBuffer;
			}
		}
	}

	void DfxNvBlastGlobal::extractFracturePieces2(Nv::Blast::FractureTool *ftool, std::vector<Mesh*> &arr) {

		if (ftool->getChunkCount() > 0) {

			for (size_t i = 1; i < ftool->getChunkCount(); i++) {

				Nv::Blast::Triangle*  triangleBuffer = nullptr;

				uint32_t tcount = ftool->getBaseMesh(i, triangleBuffer);
			
				if (tcount > 0) {
					auto *mesh = new Mesh;

					mesh->nbVerts = tcount * 3;
					mesh->nbFaces = tcount;
					mesh->verts = new glm::vec3[mesh->nbVerts];
					mesh->normals = new glm::vec3[mesh->nbVerts];
					mesh->uvs = new glm::vec2[mesh->nbVerts];
					mesh->indices = new unsigned int[tcount * 3];
					mesh->materialID = new int[tcount];

					for (size_t j = 0; j < tcount; j++) {
						size_t offset = j * 3;

						mesh->verts[offset] = glm::vec3(triangleBuffer[j].a.p.x, triangleBuffer[j].a.p.y, triangleBuffer[j].a.p.z);
						mesh->verts[offset + 1] = glm::vec3(triangleBuffer[j].b.p.x, triangleBuffer[j].b.p.y, triangleBuffer[j].b.p.z);
						mesh->verts[offset + 2] = glm::vec3(triangleBuffer[j].c.p.x, triangleBuffer[j].c.p.y, triangleBuffer[j].c.p.z);

						mesh->normals[offset] = glm::vec3(triangleBuffer[j].a.n.x, triangleBuffer[j].a.n.y, triangleBuffer[j].a.n.z);
						mesh->normals[offset + 1] = glm::vec3(triangleBuffer[j].b.n.x, triangleBuffer[j].b.n.y, triangleBuffer[j].b.n.z);
						mesh->normals[offset + 2] = glm::vec3(triangleBuffer[j].c.n.x, triangleBuffer[j].c.n.y, triangleBuffer[j].c.n.z);

						mesh->uvs[offset] = glm::vec2(triangleBuffer[j].a.uv[0].x, triangleBuffer[j].a.uv[0].y);
						mesh->uvs[offset + 1] = glm::vec2(triangleBuffer[j].b.uv[0].x, triangleBuffer[j].b.uv[0].y);
						mesh->uvs[offset + 2] = glm::vec2(triangleBuffer[j].c.uv[0].x, triangleBuffer[j].c.uv[0].y);

						mesh->indices[offset] = offset;
						mesh->indices[offset + 1] = offset + 1;
						mesh->indices[offset + 2] = offset + 2;

						mesh->materialID[j] = triangleBuffer[j].materialId;
					}

					arr.push_back(mesh);
				}

				delete[] triangleBuffer;
			}
		}
	}

	std::vector<Mesh*> DfxNvBlastGlobal::voronoiFracture(deepsg::SgGeoAdapterInterface *mesh, VoronoiOptions opts, glm::mat4 trans,
		std::vector<glm::vec3> samples, int sites_per_cluster, float cluster_radius, bool use_clustering, int materialID) {

		Nv::Blast::VoronoiSitesGenerator* voronoiSitesGenerator = nullptr;
		std::vector<Mesh*> fracture_pieces;
		const NvcVec3* sites = nullptr;
		uint32_t sitesCount = 0;
		/*if (!initSuccess)
		{
			DfxGlobal::console::println("Failed to initialize PhysX"); 
			return fracture_pieces;
		}*/

		FractureTool ft = DfxNvBlastGlobal::newFractureTool(mesh, trans);
		ft.ftool->setInteriorMaterialId(materialID);

		if (samples.size() <= 0) {
			SimpleRandomGenerator rng;

			if (use_clustering) {
				auto r_size = std::max(sites_per_cluster, opts.cell_count);
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

				voronoiSitesGenerator->clusteredSitesGeneration(opts.cell_count, sites_per_cluster, cluster_radius);
				sitesCount = voronoiSitesGenerator->getVoronoiSites(sites);
			}
			else
			{
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
			
		}
		else {
			auto _sites = new NvcVec3[samples.size()];

			size_t counter = 0;
			for (auto v : samples) {
				_sites[counter] = NvcVec3{ v.x, v.y, v.z };
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
		extractFracturePieces2(ft.ftool, fracture_pieces);

		ft.ftool->release();

		if (samples.size() <= 0) {
			voronoiSitesGenerator->release();
		}
		else {
			delete[] sites;
		}

		return fracture_pieces;
	}

	std::vector<Mesh*> DfxNvBlastGlobal::cutoutFracture(deepsg::SgGeoAdapterInterface* mesh, CutoutOptions opts, glm::mat4 trans) {

		std::vector<Mesh*> fracture_pieces;
		physx::PxVec3 pnormal(opts.plane_normal.x, opts.plane_normal.y, opts.plane_normal.z);
		NvcVec3 ppoint{ opts.plane_point.x, opts.plane_point.y, opts.plane_point.z };

		FractureTool ft = DfxNvBlastGlobal::newFractureTool(mesh, trans);

		Nv::Blast::CutoutConfiguration cutoutConfig;
		physx::PxVec3 axis = pnormal;
		if (axis.isZero())
		{
			axis = physx::PxVec3(0.f, 0.f, 1.f);
		}
		axis.normalize();
		float d = axis.dot(physx::PxVec3(0.f, 0.f, 1.f));
		
		if (d < (1e-6f - 1.0f)) {
			cutoutConfig.transform.q = NvcQuat{ physx::PxPi, 1.f, 0.f, 0.f };
		}
		else if (d < 1.f) {
			float s = physx::PxSqrt((1 + d) * 2);
			float invs = 1 / s;
			auto c = axis.cross(physx::PxVec3(0.f, 0.f, 1.f));
			cutoutConfig.transform.q = NvcQuat{ c.x * invs, c.y * invs, c.z * invs, s * 0.5f };
			// cutoutConfig.transform.q.normalize();
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

	std::vector<Mesh*> DfxNvBlastGlobal::sliceFracture(deepsg::SgGeoAdapterInterface* mesh, SliceOptions opts, glm::mat4 trans) {

		std::vector<Mesh*> fracture_pieces;

		FractureTool ft = DfxNvBlastGlobal::newFractureTool(mesh, trans);

		Nv::Blast::SlicingConfiguration slConfig;
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

	std::vector<Mesh*> DfxNvBlastGlobal::planeFracture(deepsg::SgGeoAdapterInterface* mesh, CutOptions opts, glm::mat4 trans) {

		std::vector<Mesh*> fracture_pieces;

		Nv::Blast::NoiseConfiguration noise;
		noise.amplitude = opts.amplitude;
		noise.frequency = opts.frequency;
		noise.octaveNumber = opts.octaves;
		noise.samplingInterval = NvcVec3 { opts.segments.x, opts.segments.y, opts.segments.z };
		NvcVec3 pnormal { opts.plane_normal.x, opts.plane_normal.y, opts.plane_normal.z };
		NvcVec3 ppoint { opts.plane_point.x, opts.plane_point.y, opts.plane_point.z };

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