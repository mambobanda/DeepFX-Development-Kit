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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <SgGeoAdapterInterface.h>

#undef Q_FOREACH
#include <openvdb/openvdb.h>
#define foreach Q_FOREACH


class DfxPointSampling {
public:

	class VoxelConfig {
	public:
		float radius;
		glm::mat4 xform;
	};
	
	class UniformRandomConfig {
	public:
		int rand_seed; 
		int cell_count;
		glm::mat4 xform;
	};

	class SphereConfig {
	public:
		int rand_seed;
		int count;
		float radius;
		glm::vec3 center;
	};

	class RegularConfig {
	public:
		float radius;
		glm::mat4 xform;
	};
		
	class PoissonConfig {
	public:
		float radius;
		glm::mat4 xform;
	};

	class TriangleConfig {
	public:
		float radius;
		glm::mat4 xform;
	};

public:

	static float particleRadiusFromResolution(float in_resolution);

	static std::vector<glm::vec3> voxelSampling(deepsg::SgGeoAdapterInterface *mesh, VoxelConfig &config);
	static std::vector<glm::vec3> uniformRandomSampling(deepsg::SgGeoAdapterInterface *mesh, UniformRandomConfig &config);
	static std::vector<glm::vec3> sphereSampling(SphereConfig &config);
	static std::vector<glm::vec3> regularSampling(deepsg::SgGeoAdapterInterface *mesh, RegularConfig &config);
	static std::vector<glm::vec3> poissonSampling(deepsg::SgGeoAdapterInterface *mesh, PoissonConfig &config);
	static std::vector<glm::vec3> triangleSampling(deepsg::SgGeoAdapterInterface *mesh, TriangleConfig &config);
};