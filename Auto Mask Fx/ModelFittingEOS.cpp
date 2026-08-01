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

#include <eos/core/Image.hpp>
#include <eos/core/image/opencv_interop.hpp>
#include <eos/render/draw_utils.hpp>
#include <eos/render/texture_extraction.hpp>
#include <eos/cpp17/optional.hpp>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string>


#include "ModelFittingEOS.h"
using namespace eos;
namespace po = boost::program_options;
namespace fs = boost::filesystem;
using eos::core::Landmark;
using eos::core::LandmarkCollection;
using cv::Mat;
using std::cout;
using std::endl;
using std::string;
using std::vector;


ModelFittingEOS::ModelFittingEOS() {

}

void ModelFittingEOS::init() {

	std::string modelfile("share/sfm_shape_3448.bin");
	std::string mappingsfile("share/ibug_to_sfm.txt");
	std::string contourfile("share/sfm_model_contours.json");
	std::string edgetopologyfile("share/sfm_3448_edge_topology.json");
	std::string blendshapesfile("share/expression_blendshapes_3448.bin");

	try
	{
		morphable_model = eos::morphablemodel::load_model(modelfile);
	}
	catch (const std::runtime_error& e)
	{
		cout << "Error loading the Morphable Model: " << e.what() << endl;
		return;
	}

	try
	{
		landmark_mapper = eos::core::LandmarkMapper(mappingsfile);
	}
	catch (const std::exception& e)
	{
		cout << "Error loading the landmark mappings: " << e.what() << endl;
		return;
	}

	// The expression blendshapes:
	blendshapes = eos::morphablemodel::load_blendshapes(blendshapesfile);

	// These two are used to fit the front-facing contour to the ibug contour landmarks:
	model_contour = contourfile.empty() ? fitting::ModelContour() : fitting::ModelContour::load(contourfile);

	ibug_contour = fitting::ContourLandmarks::load(mappingsfile);

	// The edge topology is used to speed up computation of the occluding face contour fitting:
	edge_topology = morphablemodel::load_edge_topology(edgetopologyfile);

}

cv::Mat ModelFittingEOS::fitModelToImage(Mat &image, LandmarkCollection<Eigen::Vector2f> &landmarks) {

	std::string outputbasename("out");

	eos::morphablemodel::MorphableModel morphable_model_with_expressions(
		morphable_model.get_shape_model(), blendshapes, morphable_model.get_color_model(), cpp17::nullopt,
		morphable_model.get_texture_coordinates());



	// Draw the loaded landmarks:
	Mat outimg = image.clone();
	for (auto&& lm : landmarks)
	{
		cv::rectangle(outimg, cv::Point2f(lm.coordinates[0] - 2.0f, lm.coordinates[1] - 2.0f),
			cv::Point2f(lm.coordinates[0] + 2.0f, lm.coordinates[1] + 2.0f), { 255, 0, 0 });
	}

	// Fit the model, get back a mesh and the pose:
	core::Mesh mesh;
	fitting::RenderingParameters rendering_params;
	std::tie(mesh, rendering_params) = fitting::fit_shape_and_pose(
		morphable_model_with_expressions, landmarks, landmark_mapper, image.cols, image.rows, edge_topology,
		ibug_contour, model_contour, 5, cpp17::nullopt, 30.0f);

	// The 3D head pose can be recovered as follows:
	float yaw_angle = glm::degrees(glm::yaw(rendering_params.get_rotation()));
	// and similarly for pitch and roll.

	// Extract the texture from the image using given mesh and camera parameters:
	const Eigen::Matrix<float, 3, 4> affine_from_ortho =
		fitting::get_3x4_affine_camera_matrix(rendering_params, image.cols, image.rows);
	const core::Image4u isomap =
		render::extract_texture(mesh, affine_from_ortho, core::from_mat(image), true);

	// Draw the fitted mesh as wireframe, and save the image:
	render::draw_wireframe(outimg, mesh, rendering_params.get_modelview(), rendering_params.get_projection(),
		fitting::get_opencv_viewport(image.cols, image.rows));
	/*fs::path outputfile = outputbasename + ".png";
	cv::imwrite(outputfile.string(), outimg);*/

	// Save the mesh as textured obj:
	/*outputfile.replace_extension(".obj");
	core::write_textured_obj(mesh, outputfile.string());*/

	// And save the isomap:
	/*outputfile.replace_extension(".isomap.png");
	cv::imwrite(outputfile.string(), core::to_mat(isomap));*/

	return outimg;
}

void ModelFittingEOS::fitModelToObj(cv::Mat &image, eos::core::LandmarkCollection<Eigen::Vector2f> &landmarks, std::string outpath, TransformType ttype, float scale) {

	std::string outputbasename("out");

	eos::morphablemodel::MorphableModel morphable_model_with_expressions(
		morphable_model.get_shape_model(), blendshapes, morphable_model.get_color_model(), cpp17::nullopt,
		morphable_model.get_texture_coordinates());

	// Draw the loaded landmarks:
	Mat outimg = image.clone();
	for (auto&& lm : landmarks)
	{
		cv::rectangle(outimg, cv::Point2f(lm.coordinates[0] - 2.0f, lm.coordinates[1] - 2.0f),
			cv::Point2f(lm.coordinates[0] + 2.0f, lm.coordinates[1] + 2.0f), { 255, 0, 0 });
	}

	// Fit the model, get back a mesh and the pose:
	core::Mesh mesh;
	fitting::RenderingParameters rendering_params;
	std::tie(mesh, rendering_params) = fitting::fit_shape_and_pose(
		morphable_model_with_expressions, landmarks, landmark_mapper, image.cols, image.rows, edge_topology,
		ibug_contour, model_contour, 5, cpp17::nullopt, 30.0f);

	// The 3D head pose can be recovered as follows:
	float yaw_angle = glm::degrees(glm::yaw(rendering_params.get_rotation()));
	// and similarly for pitch and roll.

	// Extract the texture from the image using given mesh and camera parameters:
	const Eigen::Matrix<float, 3, 4> affine_from_ortho =
		fitting::get_3x4_affine_camera_matrix(rendering_params, image.cols, image.rows);
	const core::Image4u isomap =
		render::extract_texture(mesh, affine_from_ortho, core::from_mat(image), true);


	for (auto &vert : mesh.vertices) {

		glm::vec3 pt(vert.x(), vert.y(), vert.z());
		pt *= scale;

		vert = Eigen::Vector3f(pt.x, pt.y, pt.z);
	}

	if (ttype == TransformType::NO_TRANSFORM) {
		core::write_obj(mesh, outpath);
	}
	else if(ttype == TransformType::NO_OFFSET){
		for (auto &vert : mesh.vertices) {

			auto modelview = glm::toMat4(rendering_params.get_rotation());
			glm::vec3 pt_trans(vert.x(), vert.y(), vert.z());
			glm::vec3 pt = glm::vec3(modelview * glm::vec4(pt_trans, 1.f));

			vert = Eigen::Vector3f(pt.x, pt.y, pt.z);
		}

		core::write_obj(mesh, outpath);
	}
	else {
		
		for (auto &vert : mesh.vertices) {

			auto modelview = rendering_params.get_modelview();
			glm::vec3 pt_trans(vert.x(), vert.y(), vert.z());
			glm::vec3 pt = glm::vec3(modelview * glm::vec4(pt_trans, 1.f));

			vert = Eigen::Vector3f(pt.x, pt.y, pt.z);
		}

		core::write_obj(mesh, outpath);
	}
}
