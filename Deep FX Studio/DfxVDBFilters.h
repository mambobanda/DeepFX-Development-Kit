/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2017 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */

#pragma once

#include <string>

using namespace std;

#define DILATE_FILTER 0
#define ERODE_FILTER 1
#define MEAN_FILTER 2
#define MEDIAN_FILTER 3
#define GAUSSIAN_FILTER 5
#define LAPLACIAN_FILTER 6
#define MEAN_CURVATURE_FILTER 7

namespace vdb{
	class Filter{
	protected:
		string name;
		int type;
	public:
		string getName();
		int getType();
	};

	class DilateFilter: public Filter{
	public:
		DilateFilter();
		float offset;
		int iterations;
	};

	class ErodeFilter: public Filter{
	public:
		ErodeFilter();
		float offset;
		int iterations;
	};

	class MeanFilter: public Filter{
	public:
		MeanFilter();
		int width;
		int iterations;
	};

	class MedianFilter: public Filter{
	public:
		MedianFilter();
		int width;
		int iterations;
	};

	class GaussianFilter: public Filter{
	public:
		GaussianFilter();
		int width;
		int iterations;
	};

	class LaplacianFilter: public Filter{
	public:
		LaplacianFilter();
		int iterations;
	};

	class MeanCurvatureFilter: public Filter{
	public:
		MeanCurvatureFilter();
		int iterations;
	};
}