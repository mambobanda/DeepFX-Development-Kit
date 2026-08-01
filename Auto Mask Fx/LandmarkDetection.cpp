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

#include "LandmarkDetector.h"

LandmarkDetector::LandmarkDetector() {

}

void LandmarkDetector::init() {

	facemark = FacemarkLBF::create();
	facemark->loadModel("lbfmodel.yaml");
}

std::vector<vector<Point2f>> LandmarkDetector::generateFromImage(cv::Mat &image) {

	std::vector<vector<Point2f> > landmarks;
	CascadeClassifier faceDetector;

	if (faceDetector.load(("haarcascade_frontalface_alt2.xml"))) {

		if (!facemark->empty()) {

			vector<Rect> faces;
			Mat gray;

			if (!image.empty()) {

				cvtColor(image, gray, COLOR_BGR2GRAY);
				faceDetector.detectMultiScale(gray, faces);

				std::cout << "Number of Faces Found: " << faces.size() << std::endl;

				if (faces.size() <= 0) return landmarks;

				Rect maxRect;
				float area = 0.f;

				for (auto r : faces) {
					float ar = r.width * r.height;
					if (ar > area) {
						area = ar;
						maxRect = r;
					}
				}

				faces.clear();
				faces.push_back(maxRect);

				bool success = facemark->fit(image, faces, landmarks);
				
				if (!success) {
					std::cout << "Face Landmark fitting failed.\n";
				}

				//if (success) {
				//	if (success)
				//	{
				//		// If successful, render the landmarks on the face
				//		for (int i = 0; i < landmarks.size(); i++)
				//		{
				//			drawLandmarks(image, landmarks[i]);
				//			imshow("Facial Landmark Detection", image);
				//		}
				//	}
				//	else {
				//		std::cout << "Face fitting failed.\n";
				//	}

				//	
				//}
			}
			else {
				std::cout << "Landmark detector failed to load image.\n";
			}
		}
		else {
			std::cout << "Failed to load landmark classifier.\n";
		}

	}
	else {
		std::cout << "Failed to load face detector classifier.\n";
	}

	

	return landmarks;
}

std::vector<vector<Point2f>> LandmarkDetector::generateFromImage(std::string filename) {
	Mat image = imread(filename);
	return generateFromImage(image);
}

void LandmarkDetector::drawPolyline(Mat &im, const vector<Point2f> &landmarks, const int start, const int end, bool isClosed) {
	
	vector <Point> points;
	for (int i = start; i <= end; i++) {
		points.push_back(cv::Point(landmarks[i].x, landmarks[i].y));
	}
	 
	polylines(im, points, isClosed, COLOR, 2, 16);
}

void LandmarkDetector::drawLandmarks(Mat &im, vector<Point2f> &landmarks) {

	if (landmarks.size() == 68) {
		drawPolyline(im, landmarks, 0, 16);           // Jaw line
		drawPolyline(im, landmarks, 17, 21);          // Left eyebrow
		drawPolyline(im, landmarks, 22, 26);          // Right eyebrow
		drawPolyline(im, landmarks, 27, 30);          // Nose bridge
		drawPolyline(im, landmarks, 30, 35, true);    // Lower nose
		drawPolyline(im, landmarks, 36, 41, true);    // Left eye
		drawPolyline(im, landmarks, 42, 47, true);    // Right Eye
		drawPolyline(im, landmarks, 48, 59, true);    // Outer lip
		drawPolyline(im, landmarks, 60, 67, true);    // Inner lip
	}
	else
	{ 
		for (int i = 0; i < landmarks.size(); i++)
		{
			circle(im, landmarks[i], 3, COLOR, FILLED);
		}
	}
}