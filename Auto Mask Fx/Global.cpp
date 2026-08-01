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

#include <QFileDialog>
#include <QProgressDialog>
#include "LandmarkDetector.h"
#include "Global.h"

Global *Global::instance = nullptr;

Global::Global() {
	_image = nullptr;
}

Global *Global::getInstance() {

	if (instance == nullptr) {
		instance = new Global;
	}

	return instance;
}

void Global::loadImage() {
	
	QString fileName
		= QFileDialog::getOpenFileName(nullptr, QString("Load Object"), "c:/", QString("Files (*.png)"));

	if (fileName.length() > 0) {

		_imagePath = fileName.toStdString();
		if (_image == nullptr) {
			_image = new QImage();
		}

		if (_image->load(fileName)) {
			emit new_image();
		}
		else {
			std::cout << "Failed to load image..\n";
		}
	}
}

void Global::findLandmarks() {
	LandmarkDetector lmd;
	auto landmarks = lmd.generateFromImage(_imagePath);

	cout << "Landmark Count: " << landmarks.size() << std::endl;
}

void Global::fitModel() {

	LandmarkDetector lmd;
	ModelFittingEOS mfeos;
	eos::core::LandmarkCollection<Eigen::Vector2f> lCollection;

	Mat image = imread(_imagePath);

	if (image.empty()) {
		std::cout << "Model Fitting failed to load Image.\n";
		return;
	}

	lmd.init();
	auto landmarks = lmd.generateFromImage(_imagePath);

	if (landmarks.size() > 0) {


		if (landmarks[0].size() > 0) {

			int cid = 1;
			for (auto &ld : landmarks[0]) {

				eos::core::Landmark<Eigen::Vector2f> lm;
				lm.name = std::to_string(cid);
				lm.coordinates[0] = ld.x;
				lm.coordinates[1] = ld.y;
				lCollection.emplace_back(lm);

				cid++;
			}
		}
		else {
			std::cout << "No landmarks aborting.\n";
			return;
		}
	}else{
		std::cout << "No landmarks group aborting.\n";
		return;
	}

	mfeos.init();
	auto outimg = mfeos.fitModelToImage(image, lCollection);
	cv::imwrite( "out_img.png", outimg );
	imshow("Fitting Image", outimg);
}

void Global::fitModelObj() {
	
	QSettings settings;
	QString srcPath = settings.value("automask/obj/src", QDir::homePath()).toString();
	QString destPath = settings.value("automask/obj/dest", QDir::homePath()).toString();


	VideoCapture video;
	LandmarkDetector lmd;
	ModelFittingEOS mfeos;

	QDialog dialog;

	QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
	QPushButton  *ok = new QPushButton("Generate");
	QPushButton  *cancel = new QPushButton("Cancel");

	QBoxLayout *vlayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);

	QBoxLayout *sourceLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	QBoxLayout *destinationLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	QBoxLayout *scaleLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);

	QLabel *srcName = new QLabel("Source:");
	QLineEdit *src = new QLineEdit();
	QPushButton *srcBtn = new QPushButton("->");

	src->setText(srcPath);

	sourceLayout->addWidget(srcName);
	sourceLayout->addWidget(src);
	sourceLayout->addWidget(srcBtn);

	QLabel *destName = new QLabel("Dest Path:");
	QLineEdit *dest = new QLineEdit();
	QPushButton *destBtn = new QPushButton("->");

	dest->setText(destPath);

	destinationLayout->addWidget(destName);
	destinationLayout->addWidget(dest);
	destinationLayout->addWidget(destBtn);

	QLabel *scaleFactor = new QLabel("Scale Factor:");
	QDoubleSpinBox *scale = new QDoubleSpinBox();
	scale->setValue(1.f);
	scale->setMinimum(0.f);

	scaleLayout->addWidget(scaleFactor);
	scaleLayout->addWidget(scale);

	QButtonGroup *radioGroup = new QButtonGroup();
	QRadioButton *normalTransform = new QRadioButton("Offest Transform");
	QRadioButton *noOffsetTransform = new QRadioButton("No Offest Transform");
	QRadioButton *noTransform = new QRadioButton("No transform");

	radioGroup->addButton(normalTransform);
	radioGroup->addButton(noOffsetTransform);
	radioGroup->addButton(noTransform);

	radioGroup->setId(normalTransform, 1);
	radioGroup->setId(noOffsetTransform, 2);
	radioGroup->setId(noTransform, 3);

	normalTransform->setChecked(true);

	vlayout->addLayout(sourceLayout);
	vlayout->addLayout(destinationLayout);
	vlayout->addLayout(scaleLayout);
	vlayout->addWidget(normalTransform);
	vlayout->addWidget(noOffsetTransform);
	vlayout->addWidget(noTransform);
	vlayout->addWidget(buttonBox);
	

	buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);
	buttonBox->addButton(cancel, QDialogButtonBox::RejectRole);

	dialog.setWindowTitle("Generate Object Sequence");
	dialog.setMinimumWidth(400);

	dialog.setLayout(vlayout);

	currentSrc = src;
	currentDest = dest;
	currentScale = scale;
	currentButtonGroup = radioGroup;

	QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
	QObject::connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);
	QObject::connect(srcBtn, &QPushButton::clicked, this, &Global::getVideoPath);
	QObject::connect(destBtn, &QPushButton::clicked, this, &Global::getDestinationPath);

	int rc = dialog.exec();

	if (rc) {

		auto fileName = src->text();
		auto destPath = dest->text();

		if (destPath.length() <= 0) {
			std::cout << "Invalid destination path...\n";
			return;
		}
		
		settings.setValue("automask/obj/dest", destPath);

		if (fileName.length() > 0) {
			settings.setValue("automask/obj/src", fileName);
			video.open(fileName.toStdString());

			if (!video.isOpened()) {
				std::cout << "Failed to load video file...\n";
				return;
			}
		}
		else {
			std::cout << "Invalid video path...\n";
			return;
		}

		int frame_count = video.get(CAP_PROP_FRAME_COUNT);

		if (frame_count <= 0) {
			std::cout << "0 frames found in video.\n";
			return;
		}
		else {
			std::cout << "Frame Count: " + std::to_string(frame_count) << std::endl;
		}

		Mat frame;
		int counter = 1;

		QProgressDialog progress("Fitting Video...", "Abort", 0, frame_count, nullptr);
		progress.setWindowModality(Qt::WindowModal);

		lmd.init();
		mfeos.init();
		while (video.read(frame)) {
			progress.setValue(counter - 1);

			eos::core::LandmarkCollection<Eigen::Vector2f> lCollection;
			auto landmarks = lmd.generateFromImage(frame);

			if (landmarks.size() > 0) {
				if (landmarks[0].size() > 0) {

					int cid = 1;
					for (auto &ld : landmarks[0]) {

						eos::core::Landmark<Eigen::Vector2f> lm;
						lm.name = std::to_string(cid);
						lm.coordinates[0] = ld.x;
						lm.coordinates[1] = ld.y;
						lCollection.emplace_back(lm);

						cid++;
					}
				}
				else {
					std::cout << "No landmarks found: Frame " << counter << "\n";
					continue;
				}
			}
			else {
				std::cout << "No landmarks group found: Frame " << counter << "\n";
				continue;
			}

			TransformType _ttype;

			if (radioGroup->checkedId() == 1) {
				_ttype = TransformType::OFFSET;
			}
			else if (radioGroup->checkedId() == 2) {
				_ttype = TransformType::NO_OFFSET;
			}
			else {
				_ttype = TransformType::NO_TRANSFORM;
			}


			std::stringstream ss;
			std::string postfix = std::string( 3 - std::to_string(counter).length(), '0') + std::to_string(counter);
			ss << destPath.toStdString() << "/out_" << postfix << ".obj";
			mfeos.fitModelToObj(frame, lCollection, ss.str(), _ttype, scale->value());
			
			counter++;
			if (progress.wasCanceled()) break;
		}

		progress.setValue(frame_count);
	}

}

void Global::fitModelVideo() {

	VideoCapture video;
	LandmarkDetector lmd;
	ModelFittingEOS mfeos;

	QDialog dialog;

	QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
	QPushButton  *ok = new QPushButton("Generate");
	QPushButton  *cancel = new QPushButton("Cancel");

	QBoxLayout *vlayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);

	QBoxLayout *sourceLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	QBoxLayout *destinationLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);

	QLabel *srcName = new QLabel("Source:");
	QLineEdit *src = new QLineEdit();
	QPushButton *srcBtn = new QPushButton("->");

	sourceLayout->addWidget(srcName);
	sourceLayout->addWidget(src);
	sourceLayout->addWidget(srcBtn);

	QLabel *destName = new QLabel("Dest Path:");
	QLineEdit *dest = new QLineEdit();
	QPushButton *destBtn = new QPushButton("->");

	destinationLayout->addWidget(destName);
	destinationLayout->addWidget(dest);
	destinationLayout->addWidget(destBtn);

	vlayout->addLayout(sourceLayout);
	vlayout->addLayout(destinationLayout);
	vlayout->addWidget(buttonBox);

	buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);
	buttonBox->addButton(cancel, QDialogButtonBox::RejectRole);

	dialog.setWindowTitle("Generate Image Sequence");
	dialog.setMinimumWidth(400);

	dialog.setLayout(vlayout);

	currentSrc = src;
	currentDest = dest;

	QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
	QObject::connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);
	QObject::connect(srcBtn, &QPushButton::clicked, this, &Global::getVideoPath);
	QObject::connect(destBtn, &QPushButton::clicked, this, &Global::getDestinationPath);

	int rc = dialog.exec();

	if (rc) {

		auto fileName = src->text();
		auto destPath = dest->text();

		if (destPath.length() <= 0) {
			std::cout << "Invalid destination path...\n";
			return;
		}

		if (fileName.length() > 0) {

			video.open(fileName.toStdString());

			if (!video.isOpened()) {
				std::cout << "Failed to load video file...\n";
				return;
			}
		}
		else {
			std::cout << "Invalid video path...\n";
			return;
		}

		int frame_count = video.get(CAP_PROP_FRAME_COUNT);

		if (frame_count <= 0) {
			std::cout << "0 frames found in video.\n";
			return;
		}
		else {
			std::cout << "Frame Count: " + std::to_string(frame_count) << std::endl;
		}

		Mat frame;
		int counter = 1;

		QProgressDialog progress("Fitting Video...", "Abort", 0, frame_count, nullptr);
		progress.setWindowModality(Qt::WindowModal);

		lmd.init();
		mfeos.init();
		while (video.read(frame)) {
			progress.setValue(counter - 1);

			eos::core::LandmarkCollection<Eigen::Vector2f> lCollection;
			auto landmarks = lmd.generateFromImage(frame);

			if (landmarks.size() > 0) {
				if (landmarks[0].size() > 0) {

					int cid = 1;
					for (auto &ld : landmarks[0]) {

						eos::core::Landmark<Eigen::Vector2f> lm;
						lm.name = std::to_string(cid);
						lm.coordinates[0] = ld.x;
						lm.coordinates[1] = ld.y;
						lCollection.emplace_back(lm);

						cid++;
					}
				}
				else {
					std::cout << "No landmarks found: Frame " << counter << "\n";
					continue;
				}
			}
			else {
				std::cout << "No landmarks group found: Frame " << counter << "\n";
				continue;
			}

			std::stringstream ss;
			std::string postfix = std::string(4 - std::to_string(counter).length(), '0') + std::to_string(counter);
			ss << destPath.toStdString() << "/out_img_" << postfix << ".png";
			auto outimg = mfeos.fitModelToImage(frame, lCollection);
			cv::imwrite(ss.str(), outimg);

			counter++;
			if (progress.wasCanceled()) break;
		}
		progress.setValue(frame_count);
	}
}

QImage *Global::getImage() {
	return _image;
}

void Global::getVideoPath() {

	auto pt = currentSrc->text();
	QString fileName
		= QFileDialog::getOpenFileName(nullptr, QString("Load Object"), pt, QString("Files (*.mp4)"));

	currentSrc->setText(fileName);
}

void Global::getDestinationPath() {

	auto pt = currentDest->text();
	QString fileName
		= QFileDialog::getExistingDirectory(nullptr, QString("Load Object"), pt);

	currentDest->setText(fileName);
}