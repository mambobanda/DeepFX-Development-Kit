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

#include "DfxTorusPrimitiveCommand.h"


namespace DfxCommands {

	size_t TorusPrimitiveCommand::id_gen = 0; 

	void TorusPrimitiveCommand::execute() {
		using namespace generator;

		QDialog dialog;

		QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
		QPushButton  *ok = new QPushButton("Create");
		QPushButton  *cancel = new QPushButton("Cancel");

		QFormLayout *flayout = new QFormLayout();
		QSpinBox *slices = new QSpinBox();
		QSpinBox *segments = new QSpinBox();
		QDoubleSpinBox *radiusMajor = new QDoubleSpinBox();
		QDoubleSpinBox *radiusMinor = new QDoubleSpinBox();
		QDoubleSpinBox *minorStart = new QDoubleSpinBox();
		QDoubleSpinBox *minorSweep = new QDoubleSpinBox();
		QDoubleSpinBox *majorStart = new QDoubleSpinBox();
		QDoubleSpinBox *majorSweep = new QDoubleSpinBox();

		buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);
		buttonBox->addButton(cancel, QDialogButtonBox::RejectRole);

		slices->setMinimum(2);
		slices->setMaximum(1000);
		slices->setValue(16);

		segments->setMinimum(2);
		segments->setMaximum(1000);
		segments->setValue(32);

		radiusMinor->setMinimum(0.0001);
		radiusMinor->setMaximum(1000.0);
		radiusMinor->setValue(.25f);

		radiusMajor->setMinimum(0.0001);
		radiusMajor->setMaximum(1000.0);
		radiusMajor->setValue(1.f);

		minorStart->setMinimum(0.);
		minorStart->setMaximum(360.);
		minorStart->setValue(0.f);

		minorSweep->setMinimum(0.);
		minorSweep->setMaximum(360.);
		minorSweep->setValue(360.f);

		majorStart->setMinimum(0.);
		majorStart->setMaximum(360.);
		majorStart->setValue(0.f);

		majorSweep->setMinimum(0.);
		majorSweep->setMaximum(360.);
		majorSweep->setValue(360.f);

		flayout->addRow(QString("&Slices:"), slices);
		flayout->addRow(QString("&Segments:"), segments);
		flayout->addRow(QString("&Radius Minor:"), radiusMinor);
		flayout->addRow(QString("&Radius Major:"), radiusMajor);
		flayout->addRow(QString("&Minor Start:"), minorStart);
		flayout->addRow(QString("&Minor Sweep:"), minorSweep);
		flayout->addRow(QString("&Major Start:"), majorStart);
		flayout->addRow(QString("&Major Sweep:"), majorSweep);

		flayout->addWidget(buttonBox);

		dialog.setWindowTitle("Create Torus");
		dialog.setMinimumWidth(200);

		dialog.setLayout(flayout);

		QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
		QObject::connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);

		int rc = dialog.exec();

		if (rc) {

			double _min_radius = radiusMinor->value();
			double _maj_radius = radiusMajor->value();
			int _slices = slices->value();
			int _segments = segments->value();
			double _min_Start = minorStart->value();
			double _min_Sweep = glm::radians(minorSweep->value());
			double _maj_Start = majorStart->value();
			double _maj_Sweep = glm::radians(majorSweep->value());

			TorusMesh torus(_min_radius, _maj_radius, _slices, _segments, _min_Start, _min_Sweep, _maj_Start, _maj_Sweep);
			auto vertices = torus.vertices();
			auto faces = torus.triangles();

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

				oitem->setLogicalName("Torus_" + std::to_string(id_gen++));
				oitem->calculateBounds();
				oitem->initOpenGlBuffer();
				oitem->setFilerReferenced(false);
				oitem->setIsReady(true);
				DfxScene::getInstance()->addItem(oitem);

				DfxCoreApplication::updateViewports();
			}
		}
	}

	void TorusPrimitiveCommand::execute(DfxCmdArguments args) {
		execute();
	}

	void *TorusPrimitiveCommand::create() {
		return new TorusPrimitiveCommand();
	}
}