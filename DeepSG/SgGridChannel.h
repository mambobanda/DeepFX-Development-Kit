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

#include <string>
#include <vector>
#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace deepsg {

	class SgGridChannel {
	public:

		virtual ~SgGridChannel();

		using FloatFilter = std::function<float(float)>;

		enum { NILL = 0, FLOAT = 1, FLOAT_VEC2 = 2, FLOAT_VEC3 = 3, FLOAT_VEC4 = 4, INT = 5, INT_VEC2 = 6, INT_VEC3 = 7, INT_VEC4 = 8 };

		virtual unsigned int valueType() = 0;

		virtual std::string name() = 0;
		
		virtual void *channelData() = 0;

		virtual size_t voxelCount() = 0;

		virtual bool hasValue(int i, int j, int k) = 0;

		virtual int intValue(int i, int j, int k) = 0;
		virtual glm::ivec3 int3Value(int i, int j, int k) = 0;

		virtual std::pair<float, float> floatMinMax() = 0;
		virtual float floatValue(int i, int j, int k) = 0;
		virtual glm::vec3 float3Value(int i, int j, int k) = 0;
		virtual float *flattenGridFloat(int arraySize, int xdim, int ydim, int zdim, FloatFilter filter) = 0;
		
		virtual void allCoordinatesFloat(float voxelSize, std::vector<float> &coord) = 0;
		virtual void allCoordinatesAndFloatValues(float voxelSize, std::vector<float> &coord, std::vector<float> &values, FloatFilter filter) = 0;
		virtual void allCoordinatesGradientsAndFloatValues(float voxelSize, std::vector<float> &coord, std::vector<float> &values, std::vector<float> &gradients, FloatFilter filter) = 0;
		
		virtual glm::ivec3 dimension() = 0;

		virtual float voxelSize() = 0;


	};
}