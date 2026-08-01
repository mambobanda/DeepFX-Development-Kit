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

#undef Q_FOREACH
#include <openvdb/tools/ParticlesToLevelSet.h>
#include <openvdb/tools/VolumeToMesh.h>
#include <openvdb/tools/LevelSetFilter.h>
#include <openvdb/tools/Interpolation.h>
#define foreach Q_FOREACH

#include "DfxPointCloudMesher.h"
#include "DfxOpenVDBPointList.h"


DfxPointCloudMesher::DfxPointCloudMesher() {
	defaultSettings();
}

DfxPointCloudMesher::~DfxPointCloudMesher() {

}

void DfxPointCloudMesher::defaultSettings() {
	halfWidth = 3.f;
	particleRadius = 0.02;
	group = -1;
}

void DfxPointCloudMesher::setGroup(int grp) {
	group = grp;
}

int DfxPointCloudMesher::getGroup() {
	return group;
}

Mesh *DfxPointCloudMesher::meshParticles(DfxPointCloudNode *pcloud, float gridSize, float particleRadius, std::vector<DfxPointCloudMesherFilters::Filter*> filters) {

	openvdb::Real voxelSize = gridSize;

	vector<Quad> return_quads;

	int *groupIds = nullptr;
	auto posChannel = pcloud->geo()->channel("@position");
	auto gidChannel = pcloud->geo()->channel("@group_id");

	if (gidChannel != nullptr) {
		groupIds = gidChannel->at<int>();
	}

	size_t pCount = pcloud->geo()->size();

	if (pCount <= 0) return nullptr;

	DfxOpenVDBPointList particleList;
	auto p_rc = particleList.addParticles(posChannel->at<glm::vec3>(), nullptr, nullptr, groupIds, pCount, particleRadius * 2.f, group);

	if (p_rc > 0) {

		openvdb::FloatGrid::Ptr gridBase = openvdb::createLevelSet<openvdb::FloatGrid>(voxelSize, halfWidth);
		openvdb::FloatGrid::Ptr gridColor = openvdb::createLevelSet<openvdb::FloatGrid>(voxelSize, halfWidth);

		openvdb::tools::ParticlesToLevelSet<openvdb::FloatGrid, openvdb::Vec3f> rasterBase(*gridBase);
		openvdb::tools::ParticlesToLevelSet<openvdb::FloatGrid, openvdb::Vec3f> rasterColor(*gridColor);

		rasterBase.setGrainSize(1);
		rasterBase.rasterizeSpheres(particleList);
		rasterBase.finalize();
		auto attributeGridBase = rasterBase.attributeGrid();

		particleList.setUseColorAttr(true);

		rasterColor.setGrainSize(1);
		rasterColor.rasterizeSpheres(particleList);
		rasterColor.finalize();
		auto attributeGridColor = rasterColor.attributeGrid();

		/*
		openvdb::tools::LevelSetFilter<openvdb::FloatGrid> filterColor(*gridColor);
		filterColor.setGrainSize(1);
		filterColor.gaussian(1);
		*/

		openvdb::tools::LevelSetFilter<openvdb::FloatGrid> filter(*gridBase);
		filter.setGrainSize(1);

		for (DfxPointCloudMesherFilters::Filter *flt : filters) {

			// Dilate Filter
			if (flt->getType() == DfxPointCloudMesherFilters::DILATE_FILTER) {
				DfxPointCloudMesherFilters::DilateFilter *df = (DfxPointCloudMesherFilters::DilateFilter*)flt;

				filter.offset(df->offset);

				filter.dilate(df->iterations);
			}

			// Erode Filter
			if (flt->getType() == DfxPointCloudMesherFilters::ERODE_FILTER) {
				DfxPointCloudMesherFilters::ErodeFilter *ef = (DfxPointCloudMesherFilters::ErodeFilter*)flt;

				filter.offset(ef->offset);

				filter.erode(ef->iterations);
			}

			// Mean Filter
			if (flt->getType() == DfxPointCloudMesherFilters::MEAN_FILTER) {
				DfxPointCloudMesherFilters::MeanFilter *mf = (DfxPointCloudMesherFilters::MeanFilter*)flt;

				for (int i = 0; i < mf->iterations; i++) {
					filter.mean(mf->width);
				}
			}

			// Meadian Filter
			if (flt->getType() == DfxPointCloudMesherFilters::MEDIAN_FILTER) {
				DfxPointCloudMesherFilters::MedianFilter *mf = (DfxPointCloudMesherFilters::MedianFilter*)flt;

				for (int i = 0; i < mf->iterations; i++) {
					filter.median(mf->width);
				}
			}

			// Gaussian Filter
			if (flt->getType() == DfxPointCloudMesherFilters::GAUSSIAN_FILTER) {
				DfxPointCloudMesherFilters::GaussianFilter *mf = (DfxPointCloudMesherFilters::GaussianFilter*)flt;

				for (int i = 0; i < mf->iterations; i++) {
					filter.gaussian(mf->width);
				}
			}

			// Laplacian Filter
			if (flt->getType() == DfxPointCloudMesherFilters::LAPLACIAN_FILTER) {
				DfxPointCloudMesherFilters::LaplacianFilter *lf = (DfxPointCloudMesherFilters::LaplacianFilter*)flt;

				for (int i = 0; i < lf->iterations; i++) {
					filter.laplacian();
				}
			}

			// Mean Curvatue Filter
			if (flt->getType() == DfxPointCloudMesherFilters::MEAN_CURVATURE_FILTER) {
				DfxPointCloudMesherFilters::MeanCurvatureFilter *mf = (DfxPointCloudMesherFilters::MeanCurvatureFilter*)flt;

				for (int i = 0; i < mf->iterations; i++) {
					filter.meanCurvature();
				}
			}
		}

		std::vector<openvdb::Vec3s> points;
		std::vector<openvdb::Vec3I> triangles;
		std::vector<openvdb::Vec4I> quads;

		openvdb::tools::volumeToMesh(*gridBase, points, quads);

		auto mesh = new Mesh;
		mesh->indices = new unsigned int[quads.size() * 4];
		mesh->verts = new glm::vec3[points.size()];
		mesh->normals = new glm::vec3[points.size()];
		mesh->nbFaces = quads.size();
		mesh->nbVerts = points.size();

		int vindex = 0;
		int iindex = 0;

		for (int j = 0; j < mesh->nbVerts; j++) {
			mesh->verts[j] = glm::vec3(points[j].x(),
				points[j].y(),
				points[j].z());

		}

		for (int j = 0; j < mesh->nbFaces; j++) {
			mesh->indices[iindex] = quads[j].x();
			mesh->indices[iindex + 1] = quads[j].y();
			mesh->indices[iindex + 2] = quads[j].z();
			mesh->indices[iindex + 3] = quads[j].w();

			iindex += 4;
		}

		// create gradient op
		openvdb::math::Transform::Ptr transform = gridBase->transformPtr();
		openvdb::FloatGrid::Accessor accessor = gridBase->getAccessor();
		openvdb::math::Gradient<openvdb::math::UniformScaleMap, openvdb::math::CD_2ND> gradientOp;
		openvdb::math::UniformScaleMap::Ptr gridMap = transform->map<openvdb::math::UniformScaleMap>();
		openvdb::Coord ijk;
		openvdb::Vec3f grad;
		openvdb::Vec3d indexPos;

		for (int i = 0; i < mesh->nbVerts; i++) {
			//ijk = transform->worldToIndexCellCentered(points[v]);
			ijk = transform->worldToIndexNodeCentered(points[i]);
			grad = gradientOp.result<openvdb::FloatGrid::Accessor>(*gridMap, accessor, ijk);
			mesh->normals[i].x = grad[0];
			mesh->normals[i].y = grad[1];
			mesh->normals[i].z = grad[2];
		}

		//Sample Velocities from attribute grid
		/*if (attributeGridBase) {
			openvdb::tools::GridSampler<openvdb::Vec3fTree, openvdb::tools::BoxSampler> sampler(*attributeGridBase);
			openvdb::Vec3f vel;

			mesh->velocites = new glm::vec3[points.size()];

			for (int i = 0; i < mesh->nbVerts; i++) {
				vel = sampler.wsSample(points[i]);
				mesh->velocites[i].x = vel.x();
				mesh->velocites[i].y = vel.y();
				mesh->velocites[i].z = vel.z();
			}
		}*/

		//Sample Colors from attribute grid
		/*if (attributeGridColor) {
			openvdb::tools::GridSampler<openvdb::Vec3fTree, openvdb::tools::BoxSampler> sampler(*attributeGridColor);
			openvdb::Vec3f col;

			mesh->colors = new glm::vec3[points.size()];

			for (int i = 0; i < mesh->nbVerts; i++) {
				col = sampler.wsSample(points[i]);
				mesh->colors[i].x = col.x();
				mesh->colors[i].y = col.y();
				mesh->colors[i].z = col.z();
			}
		}*/
		return mesh;
	}

	return nullptr;
}

float DfxPointCloudMesher::getParticleRadius() {
	return particleRadius;
}

float DfxPointCloudMesher::getHalfWidth() {
	return halfWidth;
}

void DfxPointCloudMesher::setParticleRadius(float pr) {
	particleRadius = pr;
}

void DfxPointCloudMesher::setHalfWidth(float hw) {
	halfWidth = hw;
}