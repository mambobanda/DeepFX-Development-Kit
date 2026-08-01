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

#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/face.hpp>

using namespace std;
using namespace cv;
using namespace cv::face;

#define COLOR Scalar(255, 200,0)

class LandmarkDetector {
private:
	Ptr<Facemark> facemark;
private:

	void drawPolyline(Mat &im, const vector<Point2f> &landmarks, const int start, const int end, bool isClosed = false);
	void drawLandmarks(Mat &im, vector<Point2f> &landmarks);

public:
	LandmarkDetector();

	void init();
	std::vector<vector<Point2f>> generateFromImage(std::string filename);
	std::vector<vector<Point2f>> generateFromImage(cv::Mat &image);
};