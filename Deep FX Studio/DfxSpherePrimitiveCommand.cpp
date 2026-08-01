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

#include "DfxSpherePrimitiveCommand.h"


namespace DfxCommands {

	size_t SpherePrimitiveCommand::id_gen = 0;
	
	void SpherePrimitiveCommand::execute() {
		using namespace generator;

		QDialog dialog;

		QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
		QPushButton  *ok = new QPushButton("Create");
		QPushButton  *cancel = new QPushButton("Cancel");

		QFormLayout *flayout = new QFormLayout();
		QSpinBox *slices = new QSpinBox();
		QSpinBox *segments = new QSpinBox();
		QDoubleSpinBox *radius = new QDoubleSpinBox();
		QDoubleSpinBox *sliceStart = new QDoubleSpinBox();
		QDoubleSpinBox *sliceSweep = new QDoubleSpinBox();
		QDoubleSpinBox *segmentStart = new QDoubleSpinBox();
		QDoubleSpinBox *segmentSweep = new QDoubleSpinBox;

		buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);
		buttonBox->addButton(cancel, QDialogButtonBox::RejectRole);

		slices->setMinimum(2);
		slices->setMaximum(1000);
		slices->setValue(32);

		segments->setMinimum(2);
		segments->setMaximum(1000);
		segments->setValue(16);

		radius->setMinimum(0.0001);
		radius->setMaximum(1000.0);
		radius->setValue(1.f);

		sliceStart->setMinimum(0.);
		sliceStart->setMaximum(360.);
		sliceStart->setValue(0.f);

		sliceSweep->setMinimum(0.);
		sliceSweep->setMaximum(360.);
		sliceSweep->setValue(360.f);

		segmentStart->setMinimum(0.);
		segmentStart->setMaximum(360.);
		segmentStart->setValue(0.f);

		segmentSweep->setMinimum(0.);
		segmentSweep->setMaximum(360.);
		segmentSweep->setValue(180.f);

		flayout->addRow(QString("&Slices:"), slices);
		flayout->addRow(QString("&Segments:"), segments);
		flayout->addRow(QString("&Radius:"), radius);
		flayout->addRow(QString("&Slice Start:"), sliceStart);
		flayout->addRow(QString("&Slice Sweep:"), sliceSweep);
		flayout->addRow(QString("&Segment Start:"), segmentStart);
		flayout->addRow(QString("&Segment Sweep:"), segmentSweep);
		
		flayout->addWidget(buttonBox);
		
		dialog.setWindowTitle("Create Sphere");
		dialog.setMinimumWidth(200);

		dialog.setLayout(flayout);

		QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
		QObject::connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);

		int rc = dialog.exec();

		if (rc) {

			double _radius = radius->value();
			int _slices   = slices->value();
			int _segments = segments->value();
			double _sliceStart = sliceStart->value();
			double _sliceSweep = glm::radians(sliceSweep->value());
			double _segmentStart = segmentStart->value();
			double _segmentSweep = glm::radians(segmentSweep->value());

			SphereMesh sphere(_radius, _slices, _segments, _sliceStart, _sliceSweep, _segmentStart, _segmentSweep);
			auto vertices = sphere.vertices();
			auto faces = sphere.triangles();
			
			std::vector<MeshVertex> vertArray;
			std::vector<generator::Triangle> faceArray;

			while (!vertices.done()) {
				vertArray.push_back(vertices.generate());
				vertices.next();
			}

			while (!faces.done()) {
				faceArray.push_back(faces.generate());
				faces.next();
			}


			if (vertArray.size() > 0 && faceArray.size() > 0) {
				DfxObjectItem *oitem = new DfxObjectItem(DfxObjectItem_Identifier);
				Dfx3dMeshData *mesh = oitem->getMeshData();

				mesh->resizeVertexBuffer(vertArray.size());
				mesh->resizeIndexBuffer(faceArray.size() * 3);

				for (size_t inx = 0; inx < vertArray.size(); inx++) {
					auto p = vertArray[inx].position;
					auto n = vertArray[inx].position;
					glm::vec3 c(0.69f, 0.69f, 0.69f);

					mesh->setVertex(inx, glm::vec3(p.x, p.y, p.z));
					mesh->setNormal(inx, glm::vec3(n.x, n.y, n.z));
					mesh->setColor(inx, glm::vec3(c.x, c.y, c.z));
				}

				size_t fcounter = 0;

				for (size_t inx = 0; inx < faceArray.size(); inx++) {
					auto indicies = faceArray[inx].vertices;

					mesh->setIndex(fcounter, indicies.x);
					mesh->setIndex(fcounter + 1, indicies.y);
					mesh->setIndex(fcounter + 2, indicies.z);

					fcounter += 3;
				}

				oitem->setLogicalName("Sphere_" + std::to_string(id_gen++));
				oitem->calculateBounds();
				oitem->initOpenGlBuffer();
				oitem->setFilerReferenced(false);
				oitem->setIsReady(true);
				DfxScene::getInstance()->addItem(oitem);

				DfxCoreApplication::updateViewports();
			}
		}
	}

	void SpherePrimitiveCommand::execute(DfxCmdArguments args) {
		execute();
	}

	void *SpherePrimitiveCommand::create() {
		return new SpherePrimitiveCommand();
	}
}