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

#include "DfxPlanePrimitiveCommand.h"


namespace DfxCommands {

	size_t PlanePrimitiveCommand::id_gen = 0;

	void PlanePrimitiveCommand::execute() {
		using namespace generator;

		QDialog dialog;

		QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
		QPushButton  *ok = new QPushButton("Create");
		QPushButton  *cancel = new QPushButton("Cancel");

		QFormLayout *flayout = new QFormLayout();
		QSpinBox *segmentsX = new QSpinBox();
		QSpinBox *segmentsY = new QSpinBox();
		QDoubleSpinBox *sizeX = new QDoubleSpinBox();
		QDoubleSpinBox *sizeY = new QDoubleSpinBox();
	

		buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);
		buttonBox->addButton(cancel, QDialogButtonBox::RejectRole);

		segmentsX->setMinimum(1);
		segmentsX->setMaximum(1000);
		segmentsX->setValue(1);

		segmentsY->setMinimum(1);
		segmentsY->setMaximum(1000);
		segmentsY->setValue(1);

		sizeX->setMinimum(0.0001);
		sizeX->setMaximum(1000.0);
		sizeX->setValue(5.f);

		sizeY->setMinimum(0.0001);
		sizeY->setMaximum(1000.0);
		sizeY->setValue(5.f);


		flayout->addRow(QString("&sizeU:"), sizeX);
		flayout->addRow(QString("&sizeV:"), sizeY);

		flayout->addRow(QString("&segmentsU:"), segmentsX);
		flayout->addRow(QString("&segmentsV:"), segmentsY);

		flayout->addWidget(buttonBox);

		dialog.setWindowTitle("Create Plane");
		dialog.setMinimumWidth(200);

		dialog.setLayout(flayout);

		QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
		QObject::connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);

		int rc = dialog.exec();

		if (rc) {

			glm::ivec2 seg(segmentsX->value(), segmentsY->value());

			glm::dvec2 si(sizeX->value(), sizeY->value());

			PlaneMesh box(si, seg);
			auto vertices = box.vertices();
			auto faces = box.triangles();

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

				oitem->setLogicalName("Plane_" + std::to_string(id_gen++));
				oitem->calculateBounds();
				oitem->initOpenGlBuffer();
				oitem->setFilerReferenced(false);
				oitem->setIsReady(true);
				DfxScene::getInstance()->addItem(oitem);

				DfxCoreApplication::updateViewports();
			}
		}
	}

	void PlanePrimitiveCommand::execute(DfxCmdArguments args) {
		execute();
	}

	void *PlanePrimitiveCommand::create() {
		return new PlanePrimitiveCommand();
	}
}