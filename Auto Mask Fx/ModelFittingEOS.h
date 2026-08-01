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

#include <eos/core/Landmark.hpp>
#include <eos/core/LandmarkMapper.hpp>
#include <eos/core/read_pts_landmarks.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eos/morphablemodel/MorphableModel.hpp>
#include <eos/fitting/fitting.hpp>
#include <eos/morphablemodel/Blendshape.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

enum class TransformType { OFFSET, NO_OFFSET, NO_TRANSFORM };

class ModelFittingEOS {
private:
	eos::morphablemodel::MorphableModel morphable_model;
	eos::core::LandmarkMapper landmark_mapper;
	std::vector<eos::morphablemodel::Blendshape> blendshapes;
	eos::morphablemodel::EdgeTopology edge_topology;
	eos::fitting::ModelContour model_contour;
	eos::fitting::ContourLandmarks ibug_contour;
public:
	ModelFittingEOS();

	void init();
	cv::Mat fitModelToImage(cv::Mat &image, eos::core::LandmarkCollection<Eigen::Vector2f> &landmarks);
	void fitModelToObj(cv::Mat &image, eos::core::LandmarkCollection<Eigen::Vector2f> &landmarks, std::string outpath, TransformType ttype, float scaleFactor);
};