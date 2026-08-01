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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <openvdb/openvdb.h>
#include <SgGridChannel.h>

namespace xvdb = openvdb::v5_2abi4;

class DfxVDBGridChannel: public deepsg::SgGridChannel {
private:
	openvdb::GridBase::Ptr _ptr;

	openvdb::Int32Tree _tempInt32Tree;
	openvdb::Vec3STree _tempVecF3Tree;
	openvdb::FloatTree _tempFloatTree;
	openvdb::Vec3ITree _tempVecI3Tree;

	xvdb::tree::ValueAccessor<xvdb::FloatTree, true> floatAccessor;
	xvdb::tree::ValueAccessor<xvdb::Vec3STree, true> vec3fAccessor;

	xvdb::tree::ValueAccessor<xvdb::Int32Tree, true> intAccessor;
	xvdb::tree::ValueAccessor<xvdb::Vec3ITree, true> vec3iAccessor;

	size_t _voxelCount;

public:

	DfxVDBGridChannel();
	virtual ~DfxVDBGridChannel();

	void setVDBGrid(openvdb::GridBase::Ptr &grd);
	openvdb::GridBase::Ptr &getVDBGrid();

	size_t voxelCount() override;

	unsigned int valueType() override;

	std::string name() override;

	void *channelData() override;

	bool hasValue(int i, int j, int k) override;

	int intValue(int i, int j, int k) override;
	glm::ivec3 int3Value(int i, int j, int k) override;

	std::pair<float, float> floatMinMax() override;
	float floatValue(int i, int j, int k) override;
	glm::vec3 float3Value(int i, int j, int k) override;
	float *flattenGridFloat(int arraySize, int xdim, int ydim, int zdim, SgGridChannel::FloatFilter filter) override;

	void allCoordinatesFloat(float voxelSize, std::vector<float> &coord) override;
	void allCoordinatesAndFloatValues(float voxelSize, std::vector<float> &coord, std::vector<float> &values, FloatFilter filter) override;
	void allCoordinatesGradientsAndFloatValues(float voxelSize, std::vector<float> &coord, std::vector<float> &values, std::vector<float> &gradients, FloatFilter filter) override;

	glm::ivec3 dimension() override;

	float voxelSize() override;

	int index3d(int i, int j, int k, int size_i, int size_j, int size_k);

	openvdb::GridBase::Ptr &gridBasePtr();
	void initAccessor();
};