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

#include <openvdb/tools/ParticlesToLevelSet.h>
#include <openvdb/tools/VolumeToMesh.h>
#include <openvdb/tools/LevelSetFilter.h>
#include "DfxVDBSkinner.h"


namespace vdb {

	int Filter::getType() {
		return type;
	}

	string Filter::getName() {
		return name;
	}

	DilateFilter::DilateFilter() {
		name = "Dilate";
		type = DILATE_FILTER;
		offset = -0.02;
		iterations = 1;
	}

	ErodeFilter::ErodeFilter() {
		name = "Erode";
		type = ERODE_FILTER;
		offset = 0.015;
		iterations = 1;
	}

	MeanFilter::MeanFilter() {
		name = "Mean";
		type = MEAN_FILTER;
		width = 1;
		iterations = 1;
	}

	MedianFilter::MedianFilter() {
		name = "Median";
		type = MEDIAN_FILTER;
		width = 1;
		iterations = 1;
	}

	GaussianFilter::GaussianFilter() {
		name = "Gaussian";
		type = GAUSSIAN_FILTER;
		width = 1;
		iterations = 1;
	}

	LaplacianFilter::LaplacianFilter() {
		name = "Laplacian";
		type = LAPLACIAN_FILTER;
		iterations = 1;
	}

	MeanCurvatureFilter::MeanCurvatureFilter() {
		name = "Mean Curvature";
		type = MEAN_CURVATURE_FILTER;
		iterations = 1;
	}
}

class OpenvdbParticleList {
private:
	struct OpenvdbParticle {
		openvdb::Vec3R p, v;
		openvdb::Real  r;
	};

	openvdb::Real   mRadiusScale;
	openvdb::Real   mVelocityScale;
	OpenvdbParticle **particles;
	int nbParticles;

public:
	typedef openvdb::Vec3R  PosType;

	OpenvdbParticleList();
	~OpenvdbParticleList();

	size_t  size()          const;

	void getPos(size_t n, openvdb::Vec3R& xyz) const;

	void getPosRad(size_t n, openvdb::Vec3R& xyz, openvdb::Real& rad) const;

	void getPosRadVel(size_t n, openvdb::Vec3R& xyz, openvdb::Real& rad, openvdb::Vec3R& vel) const;

	void getAtt(size_t n, openvdb::Index32& att) const;

	void addParticles(glm::vec3 *particles, int particleCount, float radius);

	void add(openvdb::Vec3R pos, openvdb::Vec3R vel, openvdb::Real radius);
};

OpenvdbParticleList::OpenvdbParticleList() {
	mRadiusScale = 1.0f;
	mVelocityScale = 1.0f;
	particles = NULL;
}

OpenvdbParticleList::~OpenvdbParticleList() {
	if (particles != NULL) {
		for (int i = 0; i < nbParticles; i++) {
			delete particles[i];
		}

		delete[] particles;
	}
}

void add(openvdb::Vec3R pos, openvdb::Vec3R vel, openvdb::Real radius) {

}

void OpenvdbParticleList::addParticles(glm::vec3 *particles, int particleCount, float radius) {

	this->nbParticles = particleCount;
	this->particles = new OpenvdbParticle*[particleCount];

	for (int i = 0; i < particleCount; i++) {

		OpenvdbParticle *pa = new OpenvdbParticle();
		pa->p = openvdb::Vec3R((double)particles[i].x, (double)particles[i].y, (double)particles[i].z);
		pa->v = openvdb::Vec3R((double)0.0, (double)0.0, (double)0.0);
		pa->r = radius;

		this->particles[i] = pa;
	}
}

size_t OpenvdbParticleList::size() const { return nbParticles; }

void OpenvdbParticleList::getPos(size_t n, openvdb::Vec3R&pos) const { pos = particles[n]->p; }

void OpenvdbParticleList::getPosRad(size_t n, openvdb::Vec3R& pos, openvdb::Real& rad) const {
	pos = particles[n]->p;
	rad = mRadiusScale * particles[n]->r;
}

void OpenvdbParticleList::getPosRadVel(size_t n, openvdb::Vec3R& pos, openvdb::Real& rad, openvdb::Vec3R& vel) const {
	pos = particles[n]->p;
	rad = mRadiusScale * particles[n]->r;
	vel = mVelocityScale * particles[n]->v;
}


void OpenvdbParticleList::getAtt(size_t n, openvdb::Index32& att) const { att = openvdb::Index32(n); }

OpenvdbParticleSkinner::OpenvdbParticleSkinner() {
	defaultSettings();
}

void OpenvdbParticleSkinner::defaultSettings() {
	halfWidth = 1.5f;
	gridRadius = GRID_RADIUS_R;
	particleRadius = 0.045;
}

//void OpenvdbParticleSkinner::addFilter(vdb::Filter *flt){
//	filters.push_back(flt);
//}
//
//vdb::Filter *OpenvdbParticleSkinner::getFilter(int index){
//	return filters[index];
//}
//
//void OpenvdbParticleSkinner::removeFilter(int index){
//	vector<vdb::Filter*>::iterator it = filters.begin();
//	
//	int counter = 0;
//	for(;it != filters.end(); it++){
//		if(counter == index){
//			delete (*it);
//			filters.erase(it);
//		}
//	}
//}
//
//int OpenvdbParticleSkinner::getFilterCount(){
//	return filters.size();
//}
//
//void OpenvdbParticleSkinner::clearFilters(){
//	for(vdb::Filter *flt: filters){
//		delete flt;
//	}
//
//	filters.clear();
//}

void OpenvdbParticleSkinner::skinParticles(glm::vec3 *particles, int particleCount, Dfx3dMeshData *mesh, vector<vdb::Filter*> filters) {
	VDBInitialize();

	openvdb::Real voxelSize = GridSizes::size(gridRadius);

	//vector<Quad> return_quads;

	OpenvdbParticleList particleList;
	particleList.addParticles(particles, particleCount, particleRadius);

	openvdb::FloatGrid::Ptr grid = openvdb::createLevelSet<openvdb::FloatGrid>(voxelSize, halfWidth);

	openvdb::tools::ParticlesToLevelSet<openvdb::FloatGrid> raster(*grid);

	raster.setGrainSize(1);
	raster.rasterizeSpheres(particleList);

	openvdb::tools::LevelSetFilter<openvdb::FloatGrid> filter(*grid);
	filter.setGrainSize(1);

	for (vdb::Filter *flt : filters) {

		//Dilate Filter
		if (flt->getType() == DILATE_FILTER) {
			vdb::DilateFilter *df = (vdb::DilateFilter*)flt;

			filter.offset(df->offset);

			filter.dilate(df->iterations);
		}

		//Erode Filter
		if (flt->getType() == ERODE_FILTER) {
			vdb::ErodeFilter *ef = (vdb::ErodeFilter*)flt;

			filter.offset(ef->offset);

			filter.erode(ef->iterations);
		}

		//Mean Filter
		if (flt->getType() == MEAN_FILTER) {
			vdb::MeanFilter *mf = (vdb::MeanFilter*)flt;

			for (int i = 0; i < mf->iterations; i++) {
				filter.mean(mf->width);
			}
		}

		//Meadian Filter
		if (flt->getType() == MEDIAN_FILTER) {
			vdb::MedianFilter *mf = (vdb::MedianFilter*)flt;

			for (int i = 0; i < mf->iterations; i++) {
				filter.mean(mf->width);
			}
		}

		//Gaussian Filter
		if (flt->getType() == GAUSSIAN_FILTER) {
			vdb::GaussianFilter *mf = (vdb::GaussianFilter*)flt;

			for (int i = 0; i < mf->iterations; i++) {
				filter.gaussian(mf->width);
			}
		}

		//Laplacian Filter
		if (flt->getType() == GAUSSIAN_FILTER) {
			vdb::LaplacianFilter *lf = (vdb::LaplacianFilter*)flt;

			for (int i = 0; i < lf->iterations; i++) {
				filter.laplacian();
			}
		}

		//Mean Curvatue Filter
		if (flt->getType() == MEAN_CURVATURE_FILTER) {
			vdb::MeanCurvatureFilter *mf = (vdb::MeanCurvatureFilter*)flt;

			for (int i = 0; i < mf->iterations; i++) {
				filter.meanCurvature();
			}
		}
	}

	std::vector<openvdb::Vec3s> points;
	std::vector<openvdb::Vec3I> triangles;
	std::vector<openvdb::Vec4I> quads;

	openvdb::tools::volumeToMesh(*grid, points, quads);
	
	mesh->resizeVertexBuffer(points.size());
	mesh->resizeIndexBuffer(quads.size() * 4 * 6);

	int vindex = 0;
	int iindex = 0;

	for (int j = 0; j < mesh->getVertexCount(); j++) {
		glm::vec3 v(points[j].x(), points[j].y(), points[j].z());
		mesh->setVertex(j, v);
	}

	for (int j = 0; j < quads.size(); j++) {

		mesh->setIndex(iindex, quads[j].x());
		mesh->setIndex(iindex + 1, quads[j].y());
		mesh->setIndex(iindex + 2, quads[j].z());

		mesh->setIndex(iindex + 3, quads[j].x());
		mesh->setIndex(iindex + 4, quads[j].z());
		mesh->setIndex(iindex + 6, quads[j].w());

		iindex += 6;
	}

	// create gradient op
	openvdb::math::Transform::Ptr transform = grid->transformPtr();
	openvdb::FloatGrid::Accessor accessor = grid->getAccessor();
	openvdb::math::Gradient<openvdb::math::UniformScaleMap, openvdb::math::CD_2ND> gradientOp;
	openvdb::math::UniformScaleMap::Ptr gridMap = transform->map<openvdb::math::UniformScaleMap>();
	openvdb::Coord ijk;
	openvdb::Vec3f grad;
	openvdb::Vec3d indexPos;

	for (int i = 0; i < mesh->getVertexCount(); i++) {
		//ijk = transform->worldToIndexCellCentered(points[v]);
		ijk = transform->worldToIndexNodeCentered(points[i]);
		grad = gradientOp.result<openvdb::FloatGrid::Accessor>(*gridMap, accessor, ijk);

		glm::vec3 n = glm::vec3(grad[0], grad[0], grad[0]);
		mesh->setNormal(i, n);
		mesh->setColor(i, glm::vec3(0.69f, 0.69f, 0.69f));
	}
}

int OpenvdbParticleSkinner::getGridRadius() {
	return gridRadius;
}

float OpenvdbParticleSkinner::getParticleRadius() {
	return particleRadius;
}

float OpenvdbParticleSkinner::getHalfWidth() {
	return halfWidth;
}

void OpenvdbParticleSkinner::setGridRadius(int gr) {
	gridRadius = gr;
}

void OpenvdbParticleSkinner::setParticleRadius(float pr) {
	particleRadius = pr;
}

void OpenvdbParticleSkinner::setHalfWidth(float hw) {
	halfWidth = hw;
}