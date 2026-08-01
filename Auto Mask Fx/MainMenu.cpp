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

#include "Global.h"
#include "MainMenu.h"

MainMenu::MainMenu() {

	file = new QMenu("File");
	loadImageAction = new QAction("Load Image");
	findFaceLandmarks = new QAction("Find Landmarks");
	fitModel = new QAction("Fit Model");
	fitModelObj = new QAction("Fit Model (Video to OBJ Sequence)");
	fitModelMp4 = new QAction("Fit Model (Video to PNG Sequence)"); 

	file->addAction(loadImageAction);
	// file->addAction(findFaceLandmarks);
	file->addAction(fitModel);
	file->addAction(fitModelObj);
	file->addAction(fitModelMp4);

	addMenu(file);

	QObject::connect( loadImageAction, &QAction::triggered, Global::getInstance(), &Global::loadImage );
	// QObject::connect( findFaceLandmarks, &QAction::triggered, Global::getInstance(), &Global::findLandmarks);
	QObject::connect( fitModel, &QAction::triggered, Global::getInstance(), &Global::fitModel);
	QObject::connect( fitModelObj, &QAction::triggered, Global::getInstance(), &Global::fitModelObj);
	QObject::connect( fitModelMp4, &QAction::triggered, Global::getInstance(), &Global::fitModelVideo);
}