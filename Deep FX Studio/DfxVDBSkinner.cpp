/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2016 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */

#include "DfxVDBSkinner.h"
#include <glm\geometric.hpp>
#include <assimp\Exporter.hpp>
#include <assimp\scene.h>
#include <assimp/postprocess.h> 


#ifdef USE_OPENVDB_LIB

#define _HAS_STD_BYTE 0
#undef foreach
#include <openvdb/tools/ParticlesToLevelSet.h>
#include <openvdb/tools/VolumeToMesh.h>
#include <openvdb/tools/LevelSetFilter.h>

namespace vdb{
	
	int Filter::getType(){
		return type;
	}

	string Filter::getName(){
		return name;
	}

	DilateFilter::DilateFilter(){
		name = "Dilate";
		type = DILATE_FILTER;
		offset = -0.02;
		iterations = 1;
	}

	ErodeFilter::ErodeFilter(){
		name = "Erode";
		type = ERODE_FILTER;
		offset = 0.015;
		iterations = 1;
	}
	
	MeanFilter::MeanFilter(){
		name = "Mean";
		type = MEAN_FILTER;
		width = 1;
		iterations = 1;
	}
	
	MedianFilter::MedianFilter(){
		name = "Median";
		type = MEDIAN_FILTER;
		width = 1;
		iterations = 1;
	}

	GaussianFilter::GaussianFilter(){
		name = "Gaussian";
		type = GAUSSIAN_FILTER;
		width = 1;
		iterations = 1;
	}
	
	LaplacianFilter::LaplacianFilter(){
		name = "Laplacian";
		type = LAPLACIAN_FILTER;
		iterations = 1;
	}
	
	MeanCurvatureFilter::MeanCurvatureFilter(){
		name = "Mean Curvature";
		type = MEAN_CURVATURE_FILTER;
		iterations = 1;
	}
}

typedef struct {
	int a[3];
	int b[3];
}SplitQuad;


SplitQuad quadToTriangles(Mesh *mesh, int counter) {
	SplitQuad sq;

	glm::vec3 vec_a = mesh->verts[mesh->indices[counter]];
	glm::vec3 vec_b = mesh->verts[mesh->indices[counter + 1]];
	glm::vec3 vec_c = mesh->verts[mesh->indices[counter + 2]];
	glm::vec3 vec_d = mesh->verts[mesh->indices[counter + 3]];

	float dist1 = glm::distance(vec_a, vec_c);
	float dist2 = glm::distance(vec_b, vec_d);

	if (dist1 > dist2) {
		sq.a[0] = mesh->indices[counter];
		sq.a[1] = mesh->indices[counter + 1];
		sq.a[2] = mesh->indices[counter + 3];

		sq.b[0] = mesh->indices[counter + 1];
		sq.b[1] = mesh->indices[counter + 2];
		sq.b[2] = mesh->indices[counter + 3];
	}
	else {
		sq.a[0] = mesh->indices[counter];
		sq.a[1] = mesh->indices[counter + 1];
		sq.a[2] = mesh->indices[counter + 2];

		sq.b[0] = mesh->indices[counter];
		sq.b[1] = mesh->indices[counter + 2];
		sq.b[2] = mesh->indices[counter + 3];
	}

	return sq;
}

OpenvdbParticleSkinner::OpenvdbParticleSkinner(){
	defaultSettings();
}

void OpenvdbParticleSkinner::defaultSettings(){
	halfWidth  = 1.5f;
	gridRadius = GRID_RADIUS_R;
	particleRadius = 0.045;
}


void OpenvdbParticleSkinner::skinParticles(glm::vec3 *particles, int particleCount, Mesh *mesh, vector<vdb::Filter*> filters){
	VDBInitialize(); 

	openvdb::Real voxelSize = gridRadius;
	
	vector<Quad> return_quads;

	OpenvdbParticleList particleList;
	particleList.addParticles(particles, particleCount, particleRadius);

	openvdb::FloatGrid::Ptr grid = openvdb::createLevelSet<openvdb::FloatGrid>(voxelSize, halfWidth);

	openvdb::tools::ParticlesToLevelSet<openvdb::FloatGrid> raster(*grid);

	raster.setGrainSize(1);
    raster.rasterizeSpheres(particleList);	
	raster.finalize();

	openvdb::tools::LevelSetFilter<openvdb::FloatGrid> filter(*grid);
	filter.setGrainSize(1);

	for(vdb::Filter *flt: filters){

		//Dilate Filter
		if(flt->getType() == DILATE_FILTER){
			vdb::DilateFilter *df = (vdb::DilateFilter*)flt;

			filter.offset(df->offset);
			
			filter.dilate(df->iterations);
		}

		//Erode Filter
		if(flt->getType() == ERODE_FILTER){
			vdb::ErodeFilter *ef = (vdb::ErodeFilter*)flt;

			filter.offset(ef->offset);
			
			filter.erode(ef->iterations);
		}

		//Mean Filter
		if(flt->getType() == MEAN_FILTER){
			vdb::MeanFilter *mf = (vdb::MeanFilter*)flt;
			
			for(int i=0; i < mf->iterations; i++){
				filter.mean(mf->width);
			}
		}

		//Meadian Filter
		if(flt->getType() == MEDIAN_FILTER){
			vdb::MedianFilter *mf = (vdb::MedianFilter*)flt;

			for(int i=0; i < mf->iterations; i++){
				filter.median(mf->width);
			}
		}

		//Gaussian Filter
		if(flt->getType() == GAUSSIAN_FILTER){
			vdb::GaussianFilter *mf = (vdb::GaussianFilter*)flt;

			for(int i=0; i < mf->iterations; i++){
				filter.gaussian(mf->width);
			}
		}

		//Laplacian Filter
		if(flt->getType() == GAUSSIAN_FILTER){
			vdb::LaplacianFilter *lf = (vdb::LaplacianFilter*)flt;

			for(int i=0; i < lf->iterations; i++){
				filter.laplacian();
			}
		}

		//Mean Curvatue Filter
		if(flt->getType() == MEAN_CURVATURE_FILTER){
			vdb::MeanCurvatureFilter *mf = (vdb::MeanCurvatureFilter*)flt;

			for(int i=0; i < mf->iterations; i++){
				filter.meanCurvature();
			}
		}
	}

	std::vector<openvdb::Vec3s> points;
    std::vector<openvdb::Vec3I> triangles;
	std::vector<openvdb::Vec4I> quads;

	openvdb::tools::volumeToMesh(*grid, points, quads);
	// openvdb::tools::volumeToMesh(*grid, points, triangles, quads, 0.f, 0.2f);

	mesh->indices = new unsigned int[quads.size()*4];
	mesh->verts   = new glm::vec3[points.size()];
	mesh->normals = new glm::vec3[points.size()];
	mesh->nbFaces = quads.size();
	mesh->nbVerts = points.size();

	int vindex = 0;
	int iindex = 0;

	for(int j=0; j < mesh->nbVerts  ; j++){
		mesh->verts[j] = glm::vec3(points[j].x(),
							 points[j].y(),
							 points[j].z());
	}

	for(int j=0; j < mesh->nbFaces; j++){
		mesh->indices[iindex] = quads[j].x();
		mesh->indices[iindex + 1] = quads[j].y();
		mesh->indices[iindex + 2] = quads[j].z();
		mesh->indices[iindex + 3] = quads[j].w();

		iindex += 4;
	}

	// create gradient op
	openvdb::math::Transform::Ptr transform = grid->transformPtr();
	openvdb::FloatGrid::Accessor accessor = grid->getAccessor();
	openvdb::math::Gradient<openvdb::math::UniformScaleMap, openvdb::math::CD_2ND> gradientOp;
	openvdb::math::UniformScaleMap::Ptr gridMap = transform->map<openvdb::math::UniformScaleMap>();
	openvdb::Coord ijk;
	openvdb::Vec3f grad;
	openvdb::Vec3d indexPos;

	for(int i=0; i < mesh->nbVerts;  i++ ){
		//ijk = transform->worldToIndexCellCentered(points[v]);
		ijk = transform->worldToIndexNodeCentered(points[i]);
		grad = gradientOp.result<openvdb::FloatGrid::Accessor>(*gridMap, accessor, ijk);
		mesh->normals[i].x = grad[0];
		mesh->normals[i].y = grad[1];
		mesh->normals[i].z = grad[2];
	}

}

int OpenvdbParticleSkinner::getGridRadius(){
	return gridRadius;
}

float OpenvdbParticleSkinner::getParticleRadius(){
	return particleRadius;
}

float OpenvdbParticleSkinner::getHalfWidth(){
	return halfWidth;
}

void OpenvdbParticleSkinner::setGridRadius(float gr){
	gridRadius = gr;
}

void OpenvdbParticleSkinner::setParticleRadius(float pr){
	particleRadius = pr;
}

void OpenvdbParticleSkinner::setHalfWidth(float hw){
	halfWidth = hw;
}

#endif