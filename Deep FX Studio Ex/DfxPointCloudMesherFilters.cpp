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

#include "DfxPointCloudMesherFilters.h"

namespace DfxPointCloudMesherFilters {

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