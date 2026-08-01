/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include "DfxVDBGridChannel.h"
#include <openvdb/tools/GridOperators.h>


DfxVDBGridChannel::DfxVDBGridChannel(): 
	floatAccessor(_tempFloatTree),
	vec3fAccessor(_tempVecF3Tree),
	intAccessor(_tempInt32Tree),
	vec3iAccessor(_tempVecI3Tree) {

}

DfxVDBGridChannel::~DfxVDBGridChannel() {
	_ptr.reset();
}

void DfxVDBGridChannel::setVDBGrid(openvdb::GridBase::Ptr &grd) {
	_ptr = grd;
	_voxelCount = _ptr->activeVoxelCount();
}

openvdb::GridBase::Ptr &DfxVDBGridChannel::getVDBGrid() {
	return _ptr;
}

unsigned int DfxVDBGridChannel::valueType() {

	if (_ptr->type() == openvdb::Int32Grid::gridType()) {
		return INT;
	}
	else if (_ptr->type() == openvdb::Vec3IGrid::gridType()) {
		return INT_VEC3;
	}
	else if (_ptr->type() == openvdb::FloatGrid::gridType()) {
		return FLOAT;
	}
	else if (_ptr->type() == openvdb::Vec3fGrid::gridType()) {
		return FLOAT_VEC3;
	}

	return 0;
}

std::string DfxVDBGridChannel::name() {
	return _ptr->getName();
}

void *DfxVDBGridChannel::channelData() {
	return &_ptr;
}

size_t DfxVDBGridChannel::voxelCount() {
	return _voxelCount;
}

bool DfxVDBGridChannel::hasValue(int i, int j, int k) {
	if (_ptr->type() == openvdb::Int32Grid::gridType()) {
		openvdb::Coord xcoord(i, j, k);
		return (intAccessor.isVoxel(xcoord) && intAccessor.isValueOn(xcoord));
	}
	else if (_ptr->type() == openvdb::Vec3IGrid::gridType()) {
		openvdb::Coord xcoord(i, j, k);
		return (vec3iAccessor.isVoxel(xcoord) && vec3iAccessor.isValueOn(xcoord));
	}
	else if (_ptr->type() == openvdb::FloatGrid::gridType()) {
		openvdb::Coord xcoord(i, j, k);
		return (floatAccessor.isVoxel(xcoord) && floatAccessor.isValueOn(xcoord));
	}
	else if (_ptr->type() == openvdb::Vec3fGrid::gridType()) {
		openvdb::Coord xcoord(i, j, k);
		return (vec3fAccessor.isVoxel(xcoord) && vec3fAccessor.isValueOn(xcoord));
	}

	return false;
}

int DfxVDBGridChannel::intValue(int i, int j, int k) {

	if (_ptr->type() == openvdb::Int32Grid::gridType()) {
		return intAccessor.getValue(openvdb::Coord(i,j,k));
	}

	return 0;
}

glm::ivec3 DfxVDBGridChannel::int3Value(int i, int j, int k) {

	if (_ptr->type() == openvdb::Vec3IGrid::gridType()) {
		auto vec = vec3iAccessor.getValue(openvdb::Coord(i, j, k));
		return glm::ivec3(vec.x(), vec.y(), vec.z());
	}

	return glm::ivec3(0);
}

std::pair<float, float> DfxVDBGridChannel::floatMinMax() {

	float minValue = 99999999999999;
	float maxValue = -99999999999999;
	auto floatGrid = openvdb::gridPtrCast<openvdb::FloatGrid>(_ptr);
	for (openvdb::FloatGrid::ValueOnCIter iter = floatGrid->cbeginValueOn(); iter.test(); ++iter) {
		float value = *iter;
		minValue = std::min(minValue, value);
		maxValue = std::max(maxValue, value);
	}

	return std::pair<float, float>(minValue, maxValue);
}

float DfxVDBGridChannel::floatValue(int i, int j, int k) {

	if (_ptr->type() == openvdb::FloatGrid::gridType()) {
		return floatAccessor.getValue(openvdb::Coord(i, j, k));
	}

	return 0;
}

glm::vec3 DfxVDBGridChannel::float3Value(int i, int j, int k) {

	if (_ptr->type() == openvdb::Vec3fGrid::gridType()) {
		auto vec = vec3fAccessor.getValue(openvdb::Coord(i, j, k));
		return glm::vec3(vec.x(), vec.y(), vec.z());
	}

	return glm::vec3(0);
}

glm::ivec3 DfxVDBGridChannel::dimension() {

	int xcoordmax = -std::numeric_limits<int>::max();
	int ycoordmax = -std::numeric_limits<int>::max();
	int zcoordmax = -std::numeric_limits<int>::max();

	if (_ptr->type() == openvdb::Int32Grid::gridType()) {
		auto intGrid = openvdb::gridPtrCast<openvdb::Int32Grid>(_ptr);

		for (auto iter = intGrid->cbeginValueOn(); iter.test(); ++iter) {
			auto coord = iter.getCoord();
			xcoordmax = std::max(xcoordmax, coord.x());
			ycoordmax = std::max(ycoordmax, coord.y());
			zcoordmax = std::max(zcoordmax, coord.z());
		}
	}
	else if (_ptr->type() == openvdb::Vec3IGrid::gridType()) {
		auto vec3iGrid = openvdb::gridPtrCast<openvdb::Vec3IGrid>(_ptr);

		for (auto iter = vec3iGrid->cbeginValueOn(); iter.test(); ++iter) {
			auto coord = iter.getCoord();
			xcoordmax = std::max(xcoordmax, coord.x());
			ycoordmax = std::max(ycoordmax, coord.y());
			zcoordmax = std::max(zcoordmax, coord.z());
		}
	}
	else if (_ptr->type() == openvdb::FloatGrid::gridType()) {
		auto floatGrid = openvdb::gridPtrCast<openvdb::FloatGrid>(_ptr);
	
		for (auto iter = floatGrid->cbeginValueOn(); iter.test(); ++iter) {
			auto coord = iter.getCoord();
			xcoordmax = std::max(xcoordmax, coord.x());
			ycoordmax = std::max(ycoordmax, coord.y());
			zcoordmax = std::max(zcoordmax, coord.z());
		}
	}
	else if (_ptr->type() == openvdb::Vec3fGrid::gridType()) {
		auto vec3Grid = openvdb::gridPtrCast<openvdb::Vec3fGrid>(_ptr);
		
		for (auto iter = vec3Grid->cbeginValueOn(); iter.test(); ++iter) {
			auto coord = iter.getCoord();
			xcoordmax = std::max(xcoordmax, coord.x());
			ycoordmax = std::max(ycoordmax, coord.y());
			zcoordmax = std::max(zcoordmax, coord.z());
		}
	}
	
	return glm::ivec3(xcoordmax + 1, ycoordmax + 1, zcoordmax + 1);
}

float DfxVDBGridChannel::voxelSize() {
	auto vsize = _ptr->transform().voxelSize();
	return std::max(vsize.x(), std::max(vsize.y(), vsize.z()));
}

openvdb::GridBase::Ptr &DfxVDBGridChannel::gridBasePtr() {
	return _ptr;
}

int DfxVDBGridChannel::index3d(int i, int j, int k, int size_i, int size_j, int size_k) {
	size_t index = i + j * size_i + size_i * size_j * k;

	if (size_i == 0 || size_j == 0 || size_k == 0) throw std::invalid_argument("Unknown Dimension");

	if (index >= (size_i * size_j * size_k))  std::out_of_range("Base Array Out Of Range");

	return index;
}

float *DfxVDBGridChannel::flattenGridFloat(int arraySize, int xdim, int ydim, int zdim, SgGridChannel::FloatFilter filter) {

	auto _arr = new float[arraySize];
	
	for (int i = 0; i < arraySize; i++) _arr[i] = 0.f;

	auto floatGrid = openvdb::gridPtrCast<openvdb::FloatGrid>(_ptr);

	for (openvdb::FloatGrid::ValueOnIter iter = floatGrid->beginValueOn(); iter.test(); ++iter) {
		auto coord = iter.getCoord();
		auto idx = index3d(coord.x(), coord.y(), coord.z(), xdim, ydim, zdim);

		if(idx >= 0 && idx < arraySize) _arr[idx] = filter(iter.getValue());
	}

	return _arr;
}

void DfxVDBGridChannel::allCoordinatesFloat(float voxelSize, std::vector<float> &coord) {
	auto floatGrid = openvdb::gridPtrCast<openvdb::FloatGrid>(_ptr);
	auto vcount = floatGrid->baseTree().activeVoxelCount();

	openvdb::math::Transform::Ptr transform = floatGrid->transformPtr();
	openvdb::FloatGrid::Accessor accessor = floatGrid->getAccessor();
	openvdb::math::ISGradient<openvdb::math::CD_2ND> gradientOp;

	if (vcount) {
		coord.resize(vcount * 3);

		int count = 0;
		for (openvdb::FloatGrid::ValueOnIter iter = floatGrid->beginValueOn(); iter.test(); ++iter) {
			auto index = count * 3;

			auto xcoord = floatGrid->transform().indexToWorld(iter.getCoord());
		
			coord[index] = voxelSize * float(xcoord.x());
			coord[index + 1] = voxelSize * float(xcoord.y());
			coord[index + 2] = voxelSize * float(xcoord.z());

			count++;
		}
	}
}

void DfxVDBGridChannel::allCoordinatesAndFloatValues(float voxelSize, std::vector<float> &coord, std::vector<float> &values, FloatFilter filter) {
	auto floatGrid = openvdb::gridPtrCast<openvdb::FloatGrid>(_ptr);
	auto vcount = floatGrid->baseTree().activeVoxelCount();

	if (vcount) {
		coord.resize(vcount * 3);
		values.resize(vcount);

		int count = 0;
		for (openvdb::FloatGrid::ValueOnIter iter = floatGrid->beginValueOn(); iter.test(); ++iter) {
			auto index = count * 3;

			auto xcoord = iter.getCoord();
			auto value  = iter.getValue();

			coord[index] = voxelSize * float(xcoord.x());
			coord[index + 1]  = voxelSize * float(xcoord.y());
			coord[index + 2]  = voxelSize * float(xcoord.z());

			values[count] = filter(value);

			count++;
		}
	}
}

void DfxVDBGridChannel::allCoordinatesGradientsAndFloatValues(float voxelSize, std::vector<float> &coord, std::vector<float> &values, std::vector<float> &gradients, FloatFilter filter) {
	auto floatGrid = openvdb::gridPtrCast<openvdb::FloatGrid>(_ptr);
	auto vcount = floatGrid->baseTree().activeVoxelCount();

	openvdb::math::Transform::Ptr transform = floatGrid->transformPtr();
	openvdb::FloatGrid::Accessor accessor = floatGrid->getAccessor();
	openvdb::math::ISGradient<openvdb::math::CD_2ND> gradientOp;

	if (vcount) {
		coord.resize(vcount * 3);
		gradients.resize(vcount * 3);
		values.resize(vcount);

		int count = 0;
		for (openvdb::FloatGrid::ValueOnIter iter = floatGrid->beginValueOn(); iter.test(); ++iter) {
			auto index = count * 3;

			auto xcoord = floatGrid->transform().indexToWorld(iter.getCoord());
			auto value = iter.getValue();
			auto grad = gradientOp.result<openvdb::FloatGrid::Accessor>(accessor, iter.getCoord());

			gradients[index] = grad.x();
			gradients[index + 1] = grad.y();
			gradients[index + 2] = grad.z();

			coord[index] = voxelSize * float(xcoord.x());
			coord[index + 1] = voxelSize * float(xcoord.y());
			coord[index + 2] = voxelSize * float(xcoord.z());

			values[count] = filter(value);		

			count++;
		}
	}
}


void DfxVDBGridChannel::initAccessor() {

	if (_ptr->type() == openvdb::Int32Grid::gridType()) {
		auto intGrid = openvdb::gridPtrCast<openvdb::Int32Grid>(_ptr);
		intAccessor = intGrid->getAccessor();
	}
	else if (_ptr->type() == openvdb::Vec3IGrid::gridType()) {
		auto vec3iGrid = openvdb::gridPtrCast<openvdb::Vec3IGrid>(_ptr);
		vec3iAccessor = vec3iGrid->getAccessor();
	}
	else if (_ptr->type() == openvdb::FloatGrid::gridType()) {
		auto floatGrid = openvdb::gridPtrCast<openvdb::FloatGrid>(_ptr);
		floatAccessor = floatGrid->getAccessor();
	}
	else if (_ptr->type() == openvdb::Vec3fGrid::gridType()) {
		auto vec3Grid = openvdb::gridPtrCast<openvdb::Vec3fGrid>(_ptr);
		vec3fAccessor = vec3Grid->getAccessor();
	}
	
}