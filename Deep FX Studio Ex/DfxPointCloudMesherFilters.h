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

#include <string>

using namespace std;

namespace DfxPointCloudMesherFilters {
	
	enum { 
		DILATE_FILTER = 0, ERODE_FILTER = 1, MEAN_FILTER = 2, MEDIAN_FILTER = 3, 
		GAUSSIAN_FILTER = 5, LAPLACIAN_FILTER = 6, MEAN_CURVATURE_FILTER = 7 
	};

	class Filter {
	protected:
		string name;
		int type;
	public:
		string getName();
		int getType();
	};

	class DilateFilter : public Filter {
	public:
		DilateFilter();
		float offset;
		int iterations;
	};

	class ErodeFilter : public Filter {
	public:
		ErodeFilter();
		float offset;
		int iterations;
	};

	class MeanFilter : public Filter {
	public:
		MeanFilter();
		int width;
		int iterations;
	};

	class MedianFilter : public Filter {
	public:
		MedianFilter();
		int width;
		int iterations;
	};

	class GaussianFilter : public Filter {
	public:
		GaussianFilter();
		int width;
		int iterations;
	};

	class LaplacianFilter : public Filter {
	public:
		LaplacianFilter();
		int iterations;
	};

	class MeanCurvatureFilter : public Filter {
	public:
		MeanCurvatureFilter();
		int iterations;
	};
}