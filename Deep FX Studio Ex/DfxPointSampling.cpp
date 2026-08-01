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

#include "DfxPointSampling.h"

#include "DfxNvBlastGlobal.h"

#include "SamplingTriangleMesh.h"
#include "RegularTriangleSampling.h"
#include "PoissonDiskSampling.h"
#include "DfxTriangleMeshSampling.h"

#undef Q_FOREACH
#include <openvdb/tools/MeshToVolume.h>
#include <openvdb/tools/VolumeToMesh.h>
#include <openvdb/tools/Composite.h>
#include <openvdb/tools/LevelSetFilter.h>
#include <openvdb/tools/LevelSetUtil.h>
#include <openvdb/Exceptions.h>
#include <openvdb/util/Util.h>
#define foreach Q_FOREACH

typedef struct {
	std::vector<openvdb::Vec3s> points;
	std::vector<openvdb::Vec4I> polys;

	size_t polygonCount() {
		return polys.size();
	}
}VDB_TempMesh;


std::vector<glm::vec3> DfxPointSampling::voxelSampling(deepsg::SgGeoAdapterInterface *mesh, VoxelConfig &config) {
	std::vector<glm::vec3> output;
	openvdb::FloatGrid::Ptr grid;

	openvdb::math::Transform::Ptr xform = openvdb::math::Transform::createLinearTransform(config.radius * 2.f);

	VDB_TempMesh tm;

	for (size_t i = 0; i < mesh->getVertexCount(); i++) {
		glm::vec3  p = glm::vec3(config.xform * glm::vec4(mesh->getVertex(i), 1.f));
		openvdb::Vec3s pos = xform->worldToIndex(openvdb::Vec3s(p.x, p.y, p.z));
		tm.points.push_back(pos);
	}

	int counter = 0;

	auto triIndexList = mesh->getTriangleIndexList();
	auto triIndexSize = mesh->getTriangleIndexListSize();

	if ((triIndexSize / 3) > 0) {
		for (int i = 0; i < (triIndexSize / 3); i++) {
			tm.polys.push_back(openvdb::Vec4I(
				triIndexList[counter],
				triIndexList[counter + 1],
				triIndexList[counter + 2],
				openvdb::util::INVALID_IDX));

			counter += 3;
		}
	}
	delete[] triIndexList;

	openvdb::tools::QuadAndTriangleDataAdapter<openvdb::Vec3s, openvdb::Vec4I> tmsh(tm.points, tm.polys);
	grid = openvdb::tools::meshToVolume<openvdb::FloatGrid>(tmsh, *xform, 3.0, 3.0);

	openvdb::tools::sdfToFogVolume(*grid);

	if (!grid->empty()) {
		for (auto iter = grid->cbeginValueOn(); iter; ++iter) {
			auto pos = grid->indexToWorld(iter.getCoord());
			output.push_back(glm::vec3(pos.x(), pos.y(), pos.z()));
		}
	}

	DfxGlobal::console::println("Voxel Samples: " + std::to_string(output.size()));

	return output;
}

std::vector<glm::vec3> DfxPointSampling::uniformRandomSampling(deepsg::SgGeoAdapterInterface *mesh, UniformRandomConfig &config) {

	std::vector<glm::vec3> output;
	dfxblast::DfxNvBlastGlobal::getInstance();

	size_t numIndices = mesh->getTriangleIndexListSize();

	NvcVec3* pos = new NvcVec3[mesh->getVertexCount()];
	NvcVec3* norm = new NvcVec3[mesh->getVertexCount()];
	NvcVec2* uv = new NvcVec2[mesh->getVertexCount()];
	uint32_t *indices = mesh->getTriangleIndexList();
	const NvcVec3* sites = nullptr;

	for (size_t i = 0; i < mesh->getVertexCount(); i++) {

		glm::vec3  p = glm::vec3(config.xform * glm::vec4(mesh->getVertex(i), 1.f));
		glm::vec3  n = mesh->getSmoothNormal(i);
		glm::vec2  t = glm::vec2(0.f, 0.f);

		pos[i].x = p.x;
		pos[i].y = p.y;
		pos[i].z = p.z;

		norm[i].x = n.x;
		norm[i].y = n.y;
		norm[i].z = n.z;

		uv[i].x = t.x;
		uv[i].y = t.y;

	}

	Nv::Blast::Mesh* nv_mesh = NvBlastExtAuthoringCreateMesh(pos, norm, uv, mesh->getVertexCount(), indices, numIndices);
	dfxblast::SimpleRandomGenerator rng;
	rng.seed(config.rand_seed);
	auto voronoiSitesGenerator = NvBlastExtAuthoringCreateVoronoiSitesGenerator(nv_mesh, &rng);

	if (voronoiSitesGenerator != nullptr) {

		voronoiSitesGenerator->uniformlyGenerateSitesInMesh(config.cell_count);
		auto sitesCount = voronoiSitesGenerator->getVoronoiSites(sites);

		for (size_t i = 0; i < sitesCount; i++) {
			output.push_back(glm::vec3(sites[i].x, sites[i].y, sites[i].z));
		}

	}
	else {
		DfxGlobal::console::println("Uniform Random Sampling Failed");
	}

	if (nv_mesh != nullptr) nv_mesh->release();
	if (voronoiSitesGenerator != nullptr) voronoiSitesGenerator->release();

	delete[] pos;
	delete[] norm;
	delete[] uv;
	delete[] indices;

	DfxGlobal::console::println("Uniform Random Samples: " + std::to_string(output.size()));

	return output;
}

std::vector<glm::vec3> DfxPointSampling::sphereSampling(SphereConfig &config) {

	std::vector<glm::vec3> output;
	dfxblast::DfxNvBlastGlobal::getInstance();

	dfxblast::SimpleRandomGenerator rng;
	rng.seed(config.rand_seed);
	const NvcVec3* sites = nullptr;
	auto voronoiSitesGenerator = NvBlastExtAuthoringCreateVoronoiSitesGenerator(nullptr, &rng);

	if (voronoiSitesGenerator != nullptr) {
		voronoiSitesGenerator->generateInSphere(config.count, config.radius, NvcVec3 { config.center.x, config.center.y, config.center.z });
		auto sitesCount = voronoiSitesGenerator->getVoronoiSites(sites);

		for (size_t i = 0; i < sitesCount; i++) {
			output.push_back(glm::vec3(sites[i].x, sites[i].y, sites[i].z));
		}

		if (sitesCount > 0) delete[] sites;

	}
	else {
		DfxGlobal::console::println("Sphere Sampling Failed");
		voronoiSitesGenerator->release();
	}

	DfxGlobal::console::println("Sphere Samples: " + std::to_string(output.size()));

	return output;
}

std::vector<glm::vec3> DfxPointSampling::regularSampling(deepsg::SgGeoAdapterInterface *mesh, RegularConfig &config) {

	std::vector<glm::vec3> output;
	SPH::xTriangleMesh t_mesh;

	uint32_t *indices = mesh->getTriangleIndexList();
	size_t numIndices = mesh->getTriangleIndexListSize();

	t_mesh.initMesh(mesh->getVertexCount(), numIndices / 3);

	for (size_t inx = 0; inx < mesh->getVertexCount(); inx++) {
		glm::vec3  p = glm::vec3(config.xform * glm::vec4(mesh->getVertex(inx), 1.f));
		SPH::Vector3r v(p.x, p.y, p.z);
		t_mesh.addVertex(v);
	}

	for (size_t inx = 0; inx < numIndices / 3; inx++) {
		size_t counter = inx * 3;
		unsigned int _indices[] = { indices[counter], indices[counter + 1], indices[counter + 2] };
		t_mesh.addFace(_indices);
	}

	std::vector<SPH::Vector3r> samplePoints;

	SPH::RegularTriangleSampling sampling;
	sampling.sampleMesh(t_mesh.numVertices(), t_mesh.getVertices().data(), t_mesh.numFaces(), t_mesh.getFaces().data(), config.radius, samplePoints);

	for (auto v : samplePoints) output.push_back(glm::vec3(v.x(), v.y(), v.z()));

	t_mesh.release();

	DfxGlobal::console::println("Regular Samples: " + std::to_string(output.size()));

	return output;
}

std::vector<glm::vec3> DfxPointSampling::poissonSampling(deepsg::SgGeoAdapterInterface *mesh, PoissonConfig &config) {

	std::vector<glm::vec3> output;
	SPH::xTriangleMesh t_mesh;

	uint32_t *indices = mesh->getTriangleIndexList();
	size_t numIndices = mesh->getTriangleIndexListSize();

	t_mesh.initMesh(mesh->getVertexCount(), numIndices / 3);

	for (size_t inx = 0; inx < mesh->getVertexCount(); inx++) {
		glm::vec3  p = glm::vec3(config.xform * glm::vec4(mesh->getVertex(inx), 1.f));
		SPH::Vector3r v(p.x, p.y, p.z);
		t_mesh.addVertex(v);
	}

	for (size_t inx = 0; inx < numIndices / 3; inx++) {
		size_t counter = inx * 3;
		unsigned int _indices[] = { indices[counter], indices[counter + 1], indices[counter + 2] };
		t_mesh.addFace(_indices);
	}

	std::vector<SPH::Vector3r> samplePoints;

	SPH::xPoissonDiskSampling sampling;
	sampling.sampleMesh(t_mesh.numVertices(), t_mesh.getVertices().data(), t_mesh.numFaces(), t_mesh.getFaces().data(), config.radius, 10, 1, samplePoints);

	for (auto v : samplePoints) output.push_back(glm::vec3(v.x(), v.y(), v.z()));

	t_mesh.release();
	
	DfxGlobal::console::println("Poisson Samples: " + std::to_string(output.size()));

	return output;
}

std::vector<glm::vec3> DfxPointSampling::triangleSampling(deepsg::SgGeoAdapterInterface *mesh, TriangleConfig &config) {
	std::vector<glm::vec3> output;
	
	Mesh *tMesh = new Mesh;

	uint32_t *indices = mesh->getTriangleIndexList();
	size_t numIndices = mesh->getTriangleIndexListSize();

	tMesh->nbVerts = mesh->getVertexCount();
	tMesh->nbFaces = numIndices / 3;

	tMesh->indices = new unsigned int[numIndices];
	tMesh->verts = new glm::vec3[mesh->getVertexCount()];

	for (size_t inx = 0; inx < mesh->getVertexCount(); inx++) {
		glm::vec3  p = glm::vec3(config.xform * glm::vec4(mesh->getVertex(inx), 1.f));
		tMesh->verts[inx] = p;
	}

	for (size_t inx = 0; inx < numIndices / 3; inx++) {
		size_t counter = inx * 3;

		tMesh->indices[counter] = indices[counter];
		tMesh->indices[counter + 1] = indices[counter + 1];
		tMesh->indices[counter + 2] = indices[counter + 2];
	}

	DfxTriangleMeshSampling sampler;
	sampler.sampleMesh(*tMesh, config.radius, output);

	tMesh->clear();
	delete tMesh;

	DfxGlobal::console::println("Triangle Samples: " + std::to_string(output.size()));

	return output;
}

float DfxPointSampling::particleRadiusFromResolution(float in_resolution) {

	float pdiameter = 1.f / (10.f * std::powf(in_resolution, (1.f / 3.f)));
	float pradius = 0.5f * pdiameter;
	
	return pradius;
}