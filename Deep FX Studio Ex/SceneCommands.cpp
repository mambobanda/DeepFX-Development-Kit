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

#include <QMessageBox>
#include <DsMeshOperator.h>
#include <FlDirectedAcyclicGraph.h>
#include <generator/BoxMesh.hpp>
#include "SceneCommands.h"
#include "DfxBulletStageNode.h"
#include "DfxMeshSelectionModel.h"
#include "DfxSgNodeDB.h"
#include "DfxObjectHandler.h"
#include "DfxRiptideStageHandler.h"
#include "DfxRiptideStageNode.h"
#include "DfxRiptideCachePropHandler.h"
#include "DfxRiptideEmitterPropHandler.h"
#include "DfxRiptideHeaterPropHandler.h"
#include "IndexedTriangleMesh.h"
#include "DfxUtil.h"

namespace command {
	
	// Add Primitive 
	void addPrimitive(std::vector<IndexedTriangleMesh::Triangle> &faceArray, std::string primitiveName) {

		IndexedTriangleMesh ibm;
		ibm.buildIndexedMesh(&faceArray[0], faceArray.size());

		if (ibm.getVertexArray().size() > 0) {

			auto mesh = dstools::newStandardMesh();
			ds::MeshBuilder meshBuilder;
			meshBuilder.setMesh(mesh);

			std::vector<glm::vec3> m_verts;
			for (size_t inx = 0; inx < ibm.getVertexArray().size(); inx++) {
				auto p = ibm.getVertexArray()[inx];
				m_verts.push_back(p);
			}

			meshBuilder.addVertices(m_verts);
			meshBuilder.getMesh()->reindexVertices();

			size_t index_count = 0;
			std::vector<ds::Face*> faces;
			for (int j = 0; j < ibm.getIndexArray().size() / 3; j++) {
				size_t offset = j * 3;

				std::vector<size_t> indices;
				indices.push_back((size_t)ibm.getIndexArray()[offset]);
				indices.push_back((size_t)ibm.getIndexArray()[offset + 1]);
				indices.push_back((size_t)ibm.getIndexArray()[offset + 2]);

				auto face = meshBuilder.addFace(indices);
				faces.push_back(face);
			}

			ds::MeshOperator mop(mesh);
			mop.calcSmoothNormals("smooth_normals");
			mop.calcFaceNormals("face_normals");

			std::string _name = primitiveName;

			auto mshResource = new DfxResource(resource::mode::RESOURCE, resource::type::GEOMETRY, "");
			mshResource->setResource(mesh);
			mshResource->setFormat(resource::format::DS_MESH);
			mshResource->setReleaseCallback(DfxObjectHandler::releaseDeepSimplexResource);
			mshResource->setName(_name);

			DfxResourceManager::getInstance()->geo()->addChild(mshResource);

			DfxObjectHandler obj_handler;
			bool rc = obj_handler.create(mshResource->newPointer(), "", _name);

			if (!rc) {
				DfxGlobal::console::println("Failed To Create Primitive: Object Handler failed");
				delete mesh;
			}
			else {
				std::string name = "default";
				auto refPath = obj_handler.dataObject()->absolutePath();
				unsigned int surf_id = DfxSurfaceDictionary::getInstance()->maxID(refPath) + 1;
				DfxSurfaceDictionary::getInstance()->insert(refPath, surf_id, name);

				for (auto f : faces) f->setSurfID(surf_id);

				// auto attributes = obj_handler.createAttributes();
				auto gnode = obj_handler.createGeoNode();

				if (gnode != nullptr)
					DfxQSceneManager::getInstance()->moveNode(obj_handler.geoNode(), deepsg::SgRootNode::getInstance());
				else
					DfxGlobal::console::println("Failed To Create Primitive: Object Handler failed to create node.");
			}

			DfxUpdater::getInstance()->sceneUpdated();

		}

	}

	// Switch to translate tool
	void TranslateTool::execute() {
		if (DfxMode::getInstance()->mode() == DfxMode::Mode::OBJECT_EDIT) {
			DfxWidgetHandler handler;
			handler.create();

			DfxTranslateWidget *twidget = new DfxTranslateWidget;
			deepsg::SgWidgetNode *twidgetNode = handler.widgetNode();
			twidgetNode->setWidget(twidget);

			DfxWidgetManager::getInstance()->clear();
			DfxWidgetManager::getInstance()->setWidget(twidgetNode);

			DfxUpdater::getInstance()->sceneUpdated();
		}
	}

	void TranslateTool::execute(DfxCmdArguments) {
		execute();
	}

	void *TranslateTool::create() {
		return new TranslateTool;
	}

	// Switch to scale tool
	void ScaleTool::execute() {

		if (DfxMode::getInstance()->mode() == DfxMode::Mode::OBJECT_EDIT) {
			DfxWidgetHandler handler;
			handler.create();

			DfxScaleWidget *swidget = new DfxScaleWidget;
			deepsg::SgWidgetNode *swidgetNode = handler.widgetNode();
			swidgetNode->setWidget(swidget);

			DfxWidgetManager::getInstance()->clear();
			DfxWidgetManager::getInstance()->setWidget(swidgetNode);

			DfxUpdater::getInstance()->sceneUpdated();
		}
	}

	void ScaleTool::execute(DfxCmdArguments) {
		execute();
	}

	void *ScaleTool::create() {
		return new ScaleTool;
	}

	// Switch to rotate tool
	void RotateTool::execute() {
		if (DfxMode::getInstance()->mode() == DfxMode::Mode::OBJECT_EDIT) {
			DfxWidgetHandler handler;
			handler.create();

			DfxRotateWidget *rwidget = new DfxRotateWidget;
			deepsg::SgWidgetNode *rwidgetNode = handler.widgetNode();
			rwidgetNode->setWidget(rwidget);

			DfxWidgetManager::getInstance()->clear();
			DfxWidgetManager::getInstance()->setWidget(rwidgetNode);

			DfxUpdater::getInstance()->sceneUpdated();
		}
	}

	void RotateTool::execute(DfxCmdArguments) {
		execute();
	}

	void *RotateTool::create() {
		return new RotateTool;
	}

	// Switch to object mode
	void ObjectMode::execute() {
		
		DfxMode::getInstance()->setMode(DfxMode::Mode::OBJECT_EDIT);
		
		if (!DfxWidgetManager::getInstance()->isWidgetActive()) {
			TranslateTool tool;
			tool.execute();
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void ObjectMode::execute(DfxCmdArguments) {
		execute();
	}

	void *ObjectMode::create() {
		return new ObjectMode;
	}

	// Switch to vertex mode
	void VertexMode::execute() {
		DfxWidgetManager::getInstance()->clear();
		
		if (DfxMode::getInstance()->mode() != DfxMode::Mode::VERTEX_EDIT) {
			if (deepsg::SgRootNode::getInstance()->findAllNodeTypes(deepsg::SgNodeType::GEOMETRY).empty()) {
				DfxMode::getInstance()->setMode(DfxMode::Mode::OBJECT_EDIT);
			}
			else {
				DfxMode::getInstance()->setMode(DfxMode::Mode::VERTEX_EDIT);
			}

			DfxMeshSelectionModel::getInstance()->selectedVertexIndices().clear();
			DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().clear();
			DfxMeshSelectionModel::getInstance()->selectedFaceIndices().clear();

			DfxUpdater::getInstance()->sceneUpdated();
		}

	}

	void VertexMode::execute(DfxCmdArguments) {
		execute();
	}

	void *VertexMode::create() {
		return new VertexMode;
	}

	// Switch to edge mode
	void EdgeMode::execute() {
		DfxWidgetManager::getInstance()->clear();
		
		if (DfxMode::getInstance()->mode() != DfxMode::Mode::EDGE_EDIT) {
			if (deepsg::SgRootNode::getInstance()->findAllNodeTypes(deepsg::SgNodeType::GEOMETRY).empty()) {
				DfxMode::getInstance()->setMode(DfxMode::Mode::OBJECT_EDIT);
			}
			else {
				DfxMode::getInstance()->setMode(DfxMode::Mode::EDGE_EDIT);
			}

			DfxMeshSelectionModel::getInstance()->selectedVertexIndices().clear();
			DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().clear();
			DfxMeshSelectionModel::getInstance()->selectedFaceIndices().clear();

			DfxUpdater::getInstance()->sceneUpdated();
		}

	}

	void EdgeMode::execute(DfxCmdArguments) {
		execute();
	}

	void *EdgeMode::create() {
		return new EdgeMode;
	}

	// Switch to face mode
	void FaceMode::execute() {
		DfxWidgetManager::getInstance()->clear();
		if (DfxMode::getInstance()->mode() != DfxMode::Mode::FACE_EDIT) {

			if (deepsg::SgRootNode::getInstance()->findAllNodeTypes(deepsg::SgNodeType::GEOMETRY).empty()) {
				DfxMode::getInstance()->setMode(DfxMode::Mode::OBJECT_EDIT);
			}
			else {
				DfxMode::getInstance()->setMode(DfxMode::Mode::FACE_EDIT);
			}

			DfxMeshSelectionModel::getInstance()->selectedVertexIndices().clear();
			DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().clear();
			DfxMeshSelectionModel::getInstance()->selectedFaceIndices().clear();

			DfxUpdater::getInstance()->sceneUpdated();
		}
	
	}

	void FaceMode::execute(DfxCmdArguments) {
		execute();
	}

	void *FaceMode::create() {
		return new FaceMode;
	}

	//Delete 
	void Delete::execute() {

		if (DfxMode::getInstance()->mode() == DfxMode::Mode::OBJECT_EDIT) {
			if (!DfxSceneSelectionModel::getInstance()->selected().empty()) {

				QMessageBox msgBox;
				msgBox.setStyleSheet("QLabel{min-width: 300px;}");
				msgBox.setText("Delete from scene.");
				msgBox.setInformativeText("Do you want to delete selected items?");
				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
				msgBox.setDefaultButton(QMessageBox::No);
				
				int ret = msgBox.exec();

				if (ret == QMessageBox::Yes) {
					
					util::deleteSgNodes(DfxSceneSelectionModel::getInstance()->selected());

					DfxSceneSelectionModel::getInstance()->selected().clear();
					DfxUpdater::getInstance()->sceneUpdated();
				}
			}
		}
		else if (DfxMode::getInstance()->mode() == DfxMode::Mode::VERTEX_EDIT) {
			DfxGlobal::command::exec("deleteVertices");
		}
		else if (DfxMode::getInstance()->mode() == DfxMode::Mode::EDGE_EDIT) {
			DfxGlobal::command::exec("deleteEdges");
		}
		else if (DfxMode::getInstance()->mode() == DfxMode::Mode::FACE_EDIT) {
			DfxGlobal::command::exec("deleteFaces");
		}
	}

	void Delete::execute(DfxCmdArguments) {
		execute();
	}

	void *Delete::create() {
		return new Delete;
	}

	// Add Group
	void AddGroup::execute() {

		DfxGroupHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Group: Group Handler failed");
		}
		else {
			auto gNode = handler.groupNode();

			if (gNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(gNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Add Group: Group handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddGroup::execute(DfxCmdArguments) {
		execute();
	}

	void *AddGroup::create() {
		return new AddGroup;
	}

	// Add Null
	void AddNull::execute() {

		DfxNullHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Null: Null Handler failed");
		}
		else {
			auto gNode = handler.nullNode();

			if (gNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(gNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Add Null: Null handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddNull::execute(DfxCmdArguments) {
		execute();
	}

	void *AddNull::create() {
		return new AddNull;
	}

	// Add Empty Primitive
	void AddEmptyPrimitive::execute() {		
			
		auto gnode = util::createEmptyPrimitive();

		DfxQSceneManager::getInstance()->moveNode(gnode, deepsg::SgRootNode::getInstance());

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddEmptyPrimitive::execute(DfxCmdArguments) {
		execute();
	}

	void *AddEmptyPrimitive::create() {
		return new AddEmptyPrimitive;
	}

	// Add Plane Primitive 
	void AddPlanePrimitive::execute() {
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

		ok->setStyleSheet(ok->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());
		cancel->setStyleSheet(cancel->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());

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
			std::vector<IndexedTriangleMesh::Triangle> faceArray;

			auto xform = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
			while (!vertices.done()) {
				auto vec = vertices.generate();
				auto rvec = glm::vec3(xform * glm::vec4(vec.position, 1.f));
				vec.position = rvec;
				vertArray.push_back(vec);
				vertices.next();
			}

			while (!faces.done()) {
				auto triangle = faces.generate();
				IndexedTriangleMesh::Triangle _triangle;

				_triangle.a = vertArray[triangle.vertices.x].position;
				_triangle.b = vertArray[triangle.vertices.y].position;
				_triangle.c = vertArray[triangle.vertices.z].position;

				faceArray.push_back(_triangle);
				faces.next();
			}

			addPrimitive(faceArray, "Plane Primitive");
		}
	}

	void AddPlanePrimitive::execute(DfxCmdArguments) {
		execute();
	}

	void *AddPlanePrimitive::create() {
		return new AddPlanePrimitive;
	}

	//Add Cube Primitive 
	void AddCubePrimitive::execute() {
		using namespace generator;

		QDialog dialog;

		QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
		QPushButton  *ok = new QPushButton("Create");
		QPushButton  *cancel = new QPushButton("Cancel");

		QFormLayout *flayout = new QFormLayout();
		QSpinBox *segmentsX = new QSpinBox();
		QSpinBox *segmentsY = new QSpinBox();
		QSpinBox *segmentsZ = new QSpinBox();
		QDoubleSpinBox *sizeX = new QDoubleSpinBox();
		QDoubleSpinBox *sizeY = new QDoubleSpinBox();
		QDoubleSpinBox *sizeZ = new QDoubleSpinBox();

		ok->setStyleSheet(ok->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());
		cancel->setStyleSheet(cancel->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());

		buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);
		buttonBox->addButton(cancel, QDialogButtonBox::RejectRole);

		segmentsX->setMinimum(1);
		segmentsX->setMaximum(1000);
		segmentsX->setValue(1);

		segmentsY->setMinimum(1);
		segmentsY->setMaximum(1000);
		segmentsY->setValue(1);

		segmentsZ->setMinimum(1);
		segmentsZ->setMaximum(1000);
		segmentsZ->setValue(1);

		sizeX->setMinimum(0.0001);
		sizeX->setMaximum(1000.0);
		sizeX->setValue(.5f);

		sizeY->setMinimum(0.0001);
		sizeY->setMaximum(1000.0);
		sizeY->setValue(.5f);

		sizeZ->setMinimum(0.0001);
		sizeZ->setMaximum(1000.0);
		sizeZ->setValue(.5f);


		flayout->addRow(QString("&sizeX:"), sizeX);
		flayout->addRow(QString("&sizeY:"), sizeY);
		flayout->addRow(QString("&sizeZ:"), sizeZ);

		flayout->addRow(QString("&segmentsX:"), segmentsX);
		flayout->addRow(QString("&segmentsY:"), segmentsY);
		flayout->addRow(QString("&segmentsZ:"), segmentsZ);

		flayout->addWidget(buttonBox);

		dialog.setWindowTitle("Create Box");
		dialog.setMinimumWidth(200);

		dialog.setLayout(flayout);

		QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
		QObject::connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);

		int rc = dialog.exec();

		if (rc) {

			glm::ivec3 seg(segmentsX->value(), segmentsY->value(), segmentsZ->value());

			glm::dvec3 si(sizeX->value(), sizeY->value(), sizeZ->value());

			BoxMesh box(si, seg);
			auto vertices = box.vertices();
			auto faces = box.triangles();

			std::vector<MeshVertex> vertArray;
			std::vector<IndexedTriangleMesh::Triangle> faceArray;

			while (!vertices.done()) {
				vertArray.push_back(vertices.generate());
				vertices.next();
			}

			while (!faces.done()) {
				auto triangle = faces.generate();
				IndexedTriangleMesh::Triangle _triangle;

				_triangle.a = vertArray[triangle.vertices.x].position;
				_triangle.b = vertArray[triangle.vertices.y].position;
				_triangle.c = vertArray[triangle.vertices.z].position;

				faceArray.push_back(_triangle);
				faces.next();
			}

			addPrimitive(faceArray, "Cube Primitive");
		}
	}

	void AddCubePrimitive::execute(DfxCmdArguments) {
		execute();
	}

	void *AddCubePrimitive::create() {
		return new AddCubePrimitive;
	}

	//Add Sphere Primitive 
	void AddSpherePrimitive::execute() {
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

		ok->setStyleSheet(ok->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());
		cancel->setStyleSheet(cancel->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());

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
			int _slices = slices->value();
			int _segments = segments->value();
			double _sliceStart = sliceStart->value();
			double _sliceSweep = glm::radians(sliceSweep->value());
			double _segmentStart = segmentStart->value();
			double _segmentSweep = glm::radians(segmentSweep->value());

			SphereMesh sphere(_radius, _slices, _segments, _sliceStart, _sliceSweep, _segmentStart, _segmentSweep);
			auto vertices = sphere.vertices();
			auto faces = sphere.triangles();

			std::vector<MeshVertex> vertArray;
			std::vector<IndexedTriangleMesh::Triangle> faceArray;

			while (!vertices.done()) {
				vertArray.push_back(vertices.generate());
				vertices.next();
			}

			while (!faces.done()) {
				auto triangle = faces.generate();
				IndexedTriangleMesh::Triangle _triangle;

				_triangle.a = vertArray[triangle.vertices.x].position;
				_triangle.b = vertArray[triangle.vertices.y].position;
				_triangle.c = vertArray[triangle.vertices.z].position;

				faceArray.push_back(_triangle);
				faces.next();
			}

			addPrimitive(faceArray, "Sphere Primitive");
		}
	}

	void AddSpherePrimitive::execute(DfxCmdArguments) {
		execute();
	}

	void *AddSpherePrimitive::create() {
		return new AddSpherePrimitive;
	}

	//Add Torus Knot Primitive 
	void AddTorusKnotPrimitive::execute() {
		using namespace generator;
		
		QDialog dialog;

		QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
		QPushButton  *ok = new QPushButton("Create");
		QPushButton  *cancel = new QPushButton("Cancel");

		QFormLayout *flayout = new QFormLayout();
		QSpinBox *p = new QSpinBox();
		QSpinBox *q = new QSpinBox();
		QSpinBox *slices = new QSpinBox();
		QSpinBox *segments = new QSpinBox();

		ok->setStyleSheet(ok->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());
		cancel->setStyleSheet(cancel->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());

		buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);
		buttonBox->addButton(cancel, QDialogButtonBox::RejectRole);

		//flayout->addRow(QString("&P:"), p);
		//flayout->addRow(QString("&Q:"), q);
		flayout->addRow(QString("&Slices:"), slices);
		flayout->addRow(QString("&Segments:"), segments);

		
		p->setMinimum(1);
		p->setMaximum(10000);
		p->setValue(2);

		q->setMinimum(1);
		q->setMaximum(10000);
		q->setValue(3);
	
		slices->setMinimum(3);
		slices->setMaximum(10000);
		slices->setValue(8);
		
		segments->setMinimum(1);
		segments->setMaximum(10000);
		segments->setValue(36);

		flayout->addWidget(buttonBox);

		dialog.setWindowTitle("Create Torus Knot");
		dialog.setMinimumWidth(200);

		dialog.setLayout(flayout);

		QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
		QObject::connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);

		int rc = dialog.exec();

		if (rc) {
			int _p = p->value();
			int _q = q->value();
			int _slices = slices->value();
			int _segments = segments->value();

			TorusKnotMesh torusKnot(_p, _q, _slices, _segments);
			auto vertices = torusKnot.vertices();
			auto faces = torusKnot.triangles();

			std::vector<MeshVertex> vertArray;
			std::vector<IndexedTriangleMesh::Triangle> faceArray;

			while (!vertices.done()) {
				vertArray.push_back(vertices.generate());
				vertices.next();
			}

			while (!faces.done()) {
				auto triangle = faces.generate();
				IndexedTriangleMesh::Triangle _triangle;

				_triangle.a = vertArray[triangle.vertices.x].position;
				_triangle.b = vertArray[triangle.vertices.y].position;
				_triangle.c = vertArray[triangle.vertices.z].position;

				faceArray.push_back(_triangle);
				faces.next();
			}

			addPrimitive(faceArray, "Torus Knot Primitive");
		}
	}

	void AddTorusKnotPrimitive::execute(DfxCmdArguments) {
		execute();
	}

	void *AddTorusKnotPrimitive::create() {
		return new AddTorusKnotPrimitive;
	}

	//Add Torus Primitive 
	void AddTorusPrimitive::execute() {
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

		ok->setStyleSheet(ok->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());
		cancel->setStyleSheet(cancel->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());

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
			std::vector<IndexedTriangleMesh::Triangle> faceArray;

			while (!vertices.done()) {
				vertArray.push_back(vertices.generate());
				vertices.next();
			}

			while (!faces.done()) {
				auto triangle = faces.generate();
				IndexedTriangleMesh::Triangle _triangle;

				_triangle.a = vertArray[triangle.vertices.x].position;
				_triangle.b = vertArray[triangle.vertices.y].position;
				_triangle.c = vertArray[triangle.vertices.z].position;

				faceArray.push_back(_triangle);
				faces.next();
			}

			addPrimitive(faceArray, "Torus Primitive");
		}
	}

	void AddTorusPrimitive::execute(DfxCmdArguments) {
		execute();
	}

	void *AddTorusPrimitive::create() {
		return new AddTorusPrimitive;
	}

	//Add Tube Primitive 
	void AddTubePrimitive::execute() {
		using namespace generator;

		QDialog dialog;

		QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
		QPushButton  *ok = new QPushButton("Create");
		QPushButton  *cancel = new QPushButton("Cancel");

		/*double radius = 1.0,
			double innerRadius = 0.75,
			double size = 1.0,
			int slices = 32,
			int segments = 8,
			double start = 0.0,
			double sweep = gml::radians(360.0)*/
		QFormLayout *flayout = new QFormLayout();
		auto *radius = new QDoubleSpinBox();
		auto *innerRadius = new QDoubleSpinBox();
		auto *size   = new QDoubleSpinBox();
		auto *slices = new QSpinBox();
		auto *segments = new QSpinBox();
		auto *start = new QDoubleSpinBox();
		auto *sweep = new QDoubleSpinBox();

		ok->setStyleSheet(ok->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());
		cancel->setStyleSheet(cancel->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());

		buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);
		buttonBox->addButton(cancel, QDialogButtonBox::RejectRole);

		flayout->addRow(QString("&Radius:"), radius);
		flayout->addRow(QString("&Inner Radius:"), innerRadius);
		flayout->addRow(QString("&Size:"), size);
		flayout->addRow(QString("&Slices:"), slices);
		flayout->addRow(QString("&Segments:"), segments);
		flayout->addRow(QString("&Start:"), start);
		flayout->addRow(QString("&Sweep:"), sweep);

	
		radius->setMinimum(1.0);
		radius->setMaximum(10000.0);
		radius->setValue(1.0);
		
		innerRadius->setMinimum(1.0);
		innerRadius->setMaximum(10000.0);
		innerRadius->setValue(0.75);
	
		size->setMinimum(1.0);
		size->setMaximum(10000.0);
		size->setValue(1.0);
		
		slices->setMinimum(3);
		slices->setMaximum(10000);
		slices->setValue(32);

		segments->setMinimum(1);
		segments->setMaximum(10000);
		segments->setValue(8);
		
		start->setMinimum(0.0);
		start->setMaximum(10000.0);
		start->setValue(0.0);

		sweep->setMinimum(1.0);
		sweep->setMaximum(10000.0);
		sweep->setValue(360.0);

		flayout->addWidget(buttonBox);

		dialog.setWindowTitle("Create Tube");
		dialog.setMinimumWidth(200);

		dialog.setLayout(flayout);

		QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
		QObject::connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);

		int rc = dialog.exec();

		if (rc) {
			auto _radius = radius->value();
			auto _innerRadius = innerRadius->value();
			auto _size = size->value();
			auto _slices = slices->value();
			auto _segments = segments->value();
			auto _start = start->value();
			auto _sweep = sweep->value();

			TubeMesh tube(_radius, _innerRadius, _size, _slices, _segments, glm::radians(_start), glm::radians(_sweep));
			auto vertices = tube.vertices();
			auto faces = tube.triangles();

			std::vector<MeshVertex> vertArray;
			std::vector<IndexedTriangleMesh::Triangle> faceArray;

			while (!vertices.done()) {
				vertArray.push_back(vertices.generate());
				vertices.next();
			}

			while (!faces.done()) {
				auto triangle = faces.generate();
				IndexedTriangleMesh::Triangle _triangle;

				_triangle.a = vertArray[triangle.vertices.x].position;
				_triangle.b = vertArray[triangle.vertices.y].position;
				_triangle.c = vertArray[triangle.vertices.z].position;

				faceArray.push_back(_triangle);
				faces.next();
			}

			addPrimitive(faceArray, "Tube Primitive");
		}
	}

	void AddTubePrimitive::execute(DfxCmdArguments) {
		execute();
	}

	void *AddTubePrimitive::create() {
		return new AddTubePrimitive;
	}

	//Add Spring Primitive 
	void AddSpringPrimitive::execute() {
	}

	void AddSpringPrimitive::execute(DfxCmdArguments) {
		execute();
	}

	void *AddSpringPrimitive::create() {
		return new AddSpringPrimitive;
	}

	//Add Teapot Primitive 
	void AddTeapotPrimitive::execute() {
	}

	void AddTeapotPrimitive::execute(DfxCmdArguments) {
		execute();
	}

	void *AddTeapotPrimitive::create() {
		return new AddTeapotPrimitive;
	}

	//Add Cylinder Primitive 
	void AddCylinderPrimitive::execute() {
	}

	void AddCylinderPrimitive::execute(DfxCmdArguments) {
		execute();
	}

	void *AddCylinderPrimitive::create() {
		return new AddCylinderPrimitive;
	}

	//Add Capsule Primitive 
	void AddCapsulePrimitive::execute() {
	}

	void AddCapsulePrimitive::execute(DfxCmdArguments) {
		execute();
	}

	void *AddCapsulePrimitive::create() {
		return new AddCapsulePrimitive;
	}

	// Add OSPRay Render Node
	void AddOSPRayRender::execute() {
		DfxOSPRayRenderHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create OSPRay Render Node: Handler failed");
		}
		else {
			auto renderNode = handler.createNode();

			if (renderNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(renderNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Add OSPRay Render handler: couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddOSPRayRender::execute(DfxCmdArguments) {
		execute();
	}

	void *AddOSPRayRender::create() {
		return new AddOSPRayRender;
	}

	// Add OSPRay Camera
	void AddOSPRayCamera::execute() {
		DfxOSPRayCameraHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create OSPRay Camera: Camera Handler failed");
		}
		else {
			auto shapeNode = handler.createShapeNode();

			if (shapeNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(shapeNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Add OSPRay Camera: Camera handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddOSPRayCamera::execute(DfxCmdArguments) {
		execute();
	}

	void *AddOSPRayCamera::create() {
		return new AddOSPRayCamera;
	}

	// Add OSPRay Sun Sky Light
	void AddOSPRaySunSkyLight::execute() {
		DfxOSPRaySunSkyLightHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create OSPRay Sun Sky Light: Sun Sky Light Handler failed");
		}
		else {
			auto shapeNode = handler.createShapeNode();

			if (shapeNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(shapeNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Add OSPRay Sun Sky Light: Sun Sky Light handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddOSPRaySunSkyLight::execute(DfxCmdArguments) {
		execute();
	}

	void *AddOSPRaySunSkyLight::create() {
		return new AddOSPRaySunSkyLight;
	}

	// Add Luxcore Render Node
	void AddLuxcoreRender::execute() {
		DfxLxRenderHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Luxcore Render Node: Handler failed");
		}
		else {
			auto renderNode = handler.createNode();

			if (renderNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(renderNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Add Luxcore Render handler: couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddLuxcoreRender::execute(DfxCmdArguments) {
		execute();
	}

	void *AddLuxcoreRender::create() {
		return new AddLuxcoreRender;
	}

	// Add Lux Camera
	void AddCamera::execute() {
		DfxLxCameraHandler handler;
		
		bool rc = handler.create();
		
		if (!rc) {
			DfxGlobal::console::println("Failed To Create Camera: Camera Handler failed");
		}
		else {
			auto shapeNode = handler.createShapeNode();

			if (shapeNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(shapeNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Add Camera: Camera handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddCamera::execute(DfxCmdArguments) {
		execute();
	}

	void *AddCamera::create() {
		return new AddCamera;
	}

	// Add Lux Point Light
	void AddPointLight::execute() {
		DfxLxPointLightHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Point Light: Point Light Handler failed");
		}
		else {
			auto shapeNode = handler.createShapeNode();

			if (shapeNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(shapeNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Add Point Light: Point Light handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddPointLight::execute(DfxCmdArguments) {
		execute();
	}

	void *AddPointLight::create() {
		return new AddPointLight;
	}

	// Add Lux Enviroment Light
	void AddEnvironmentLight::execute() {
		DfxLxEnvironmentLightHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Environment Light: Environment Light Handler failed");
		}
		else {
			auto shapeNode = handler.createShapeNode();

			if (shapeNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(shapeNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Add Environment Light: Envionment Light handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddEnvironmentLight::execute(DfxCmdArguments) {
		execute();
	}

	void *AddEnvironmentLight::create() {
		return new AddEnvironmentLight;
	}

	// Add Enviroment Light
	void AddSunSkyLight::execute() {
		DfxLxSunSkyLightHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Sun Sky Light: Sun Sky Light Handler failed");
		}
		else {
			auto shapeNode = handler.createShapeNode();

			if (shapeNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(shapeNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Add Sun Sky Light: Sun Sky Light handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddSunSkyLight::execute(DfxCmdArguments) {
		execute();
	}

	void *AddSunSkyLight::create() {
		return new AddSunSkyLight;
	}

	// Add Infinite Light
	void AddInfiniteLight::execute() {
		DfxLxInfiniteLightHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Infinite Light: Infinite Light Handler failed");
		}
		else {
			auto shapeNode = handler.createShapeNode();

			if (shapeNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(shapeNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Add Infinite Light: Infinite Light handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddInfiniteLight::execute(DfxCmdArguments) {
		execute();
	}

	void *AddInfiniteLight::create() {
		return new AddInfiniteLight;
	}

	// Add Bullet Stage
	void AddBulletStage::execute() {
		DfxBulletStageHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Bullet Simulation Stage: Bullet Stage Handler failed");
		}
		else {
			
			auto stageNode = handler.createStageNode();

			if (stageNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(stageNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create Bullet Simulation Stage: Bullet Stage handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddBulletStage::execute(DfxCmdArguments) {
		execute();
	}

	void *AddBulletStage::create() {
		return new AddBulletStage;
	}

	// Duplicate
	void Duplicate::execute() {

		if (!DfxSceneSelectionModel::getInstance()->selected().empty()) {

			for (auto node : DfxSceneSelectionModel::getInstance()->selected()) {
				if (node->getType() == deepsg::SgNodeType::GEOMETRY) {

					auto geoNode = (deepsg::SgGeoNode*)node;

					DfxObjectHandler obj_handler;
					DfxObjectHandler obj_handler_source;

					if (obj_handler_source.fetch(geoNode->referencePath())) {

						auto mesh = new ds::Mesh;
						mesh->copyMesh(obj_handler_source.dsMeshData()->resource<ds::Mesh>());

						auto mshResource = new DfxResource(resource::mode::RESOURCE, resource::type::GEOMETRY, "");
						mshResource->setResource(mesh);
						mshResource->setFormat(resource::format::DS_MESH);
						mshResource->setName(obj_handler_source.dataObject()->label() + "_copy");

						bool rc = obj_handler.create(mshResource->newPointer(), "**", obj_handler_source.dataObject()->label() + "_copy");

						if (!rc) {
							DfxGlobal::console::println("Failed To Duplicate Object: Object Handler failed");
							delete mesh;
						}
						else {
							auto attributes = obj_handler.createAttributes();
							auto gnode = obj_handler.createGeoNode();

							if (gnode != nullptr)
								DfxQSceneManager::getInstance()->moveNode(obj_handler.geoNode(), deepsg::SgRootNode::getInstance());
							else
								DfxGlobal::console::println("Failed To Open Object: Object Handler failed to create node.");
						}

						DfxUpdater::getInstance()->sceneUpdated();
					}
				}
			}
		}
	}

	void Duplicate::execute(DfxCmdArguments) {
		execute();
	}

	void *Duplicate::create() {
		return new Duplicate;
	}

	// Add Point Cloud
	void AddPointCloud::execute() {
		DfxPointCloudHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Point Cloud: Point Cloud Handler failed");
		}
		else {
			auto pcloudNode = handler.createPointCloudNode();

			if (pcloudNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(pcloudNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create Point Cloud Node: Point Cloud handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddPointCloud::execute(DfxCmdArguments) {
		execute();
	}

	void *AddPointCloud::create() {
		return new AddPointCloud;
	}

	// Add Point Cloud Mesher
	void AddPointCloudMesher::execute() {
		DfxPointCloudMesherHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Point Cloud Mesher: Point Cloud Mesher Handler failed");
		}
		else {
			auto pcloudNode = handler.mesherNode();

			if (pcloudNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(pcloudNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create Point Cloud Mesher Node: Point Cloud Mesher handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddPointCloudMesher::execute(DfxCmdArguments) {
		execute();
	}

	void *AddPointCloudMesher::create() {
		return new AddPointCloudMesher;
	}

	// Add Point Generator
	void AddPointGenerator::execute() {
		DfxGeneratePointsHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Point Generator: Point Generator Handler failed");
		}
		else {
			auto tcloudNode = handler.createToolNode();

			if (tcloudNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(tcloudNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create Point Generator Node: Point Cloud handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddPointGenerator::execute(DfxCmdArguments) {
		execute();
	}

	void *AddPointGenerator::create() {
		return new AddPointGenerator;
	}

	// Add Point Sampler
	void AddPointSampler::execute() {
		DfxPointSamplerHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Point Sampler: Point Sampler Handler failed");
		}
		else {
			auto tcloudNode = handler.createToolNode();

			if (tcloudNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(tcloudNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create Point Sampler Node: Point sampler handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddPointSampler::execute(DfxCmdArguments) {
		execute();
	}

	void *AddPointSampler::create() {
		return new AddPointSampler;
	}

	// Add Point Attribute Instancer
	void AddPointAttributeInstancer::execute() {
		DfxPointAttributeInstancerHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Point Attribute Instancer: Point Attribute Instancer Handler failed");
		}
		else {
			auto tcloudNode = handler.createToolNode();

			if (tcloudNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(tcloudNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create Point Attribute Instancer: Point Attribute Instancer Handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddPointAttributeInstancer::execute(DfxCmdArguments args) {
		execute();
	}

	void *AddPointAttributeInstancer::create() {
		return new AddPointAttributeInstancer;
	}

	// Add Point Shape Tool
	void AddPointShapeTool::execute() {
		DfxPointShapeHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Point Shape Tool: Point Shape Handler failed");
		}
		else {
			auto tpointShapeNode = handler.createToolNode();

			if (tpointShapeNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(tpointShapeNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create Point Shape Tool: Point Shape Handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddPointShapeTool::execute(DfxCmdArguments args) {
		execute();
	}

	void *AddPointShapeTool::create() {
		return new AddPointShapeTool;
	}

	// Add Point Proximity Kill Handler
	void AddPointProximityKillTool::execute() {
		DfxPointProximityKillHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Point Proximity Kill Tool: Point Proximity Kill Handler failed");
		}
		else {
			auto tpointProximityKillNode = handler.createToolNode();

			if (tpointProximityKillNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(tpointProximityKillNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create Point Proximity Kill Tool:  Point Proximity Kill Handler failed create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddPointProximityKillTool::execute(DfxCmdArguments args) {
		execute();
	}

	void *AddPointProximityKillTool::create() {
		return new AddPointProximityKillTool;
	}

	// Add Point Proximity Scale Handler
	void AddPointProximityScaleTool::execute() {
		DfxPointProximityScaleHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Point Proximity Scale Tool: Point Proximity Scale Handler failed");
		}
		else {
			auto tpointProximityScaleNode = handler.createToolNode();

			if (tpointProximityScaleNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(tpointProximityScaleNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create Point Proximity Scale Tool:  Point Proximity Scale Handler failed create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddPointProximityScaleTool::execute(DfxCmdArguments args) {
		execute();
	}

	void *AddPointProximityScaleTool::create() {
		return new AddPointProximityScaleTool;
	}

	// Add Alembic Loader
	void AddAlembicLoader::execute() {

		DfxAlembicLoaderHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To create Alembic Loader: Alembic Loader Handler failed");
		}
		else {
			auto tcloudNode = handler.createToolNode();

			if (tcloudNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(tcloudNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create Alembic Loader: Alembic Loader Handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddAlembicLoader::execute(DfxCmdArguments args) {
		execute();
	}

	void *AddAlembicLoader::create() {
		return new AddAlembicLoader;
	}

	// Add Bullet Constraint Node Loader
	void AddBulletConstraintNode::execute() {

		DfxSceneGraphSelectionDialog diag;
		diag.setMode(DfxSceneGraphSelectionDialog::NODE);

		if (diag.exec()) {
			auto node = diag.node();
			
			if (node != nullptr) {

				DfxSimulationStageBase *stage = nullptr;

				if (node->getType() == deepsg::SgNodeType::STAGE_PROP_COLLECTION) {

					auto propCollectionNode = (deepsg::SgStagePropCollectionNode*)node;
					stage = (DfxSimulationStageBase*)propCollectionNode->stage();

				}
				else if (node->getType() == deepsg::SgNodeType::STAGE) {

					stage = (DfxSimulationStageBase*)node;

				}

				if (stage != nullptr) {
					if (stage->stageType() == DfxStageType::BULLET_STAGE) {

						DfxBulletConstraintPropHandler handler;

						if (handler.create()) {
							auto pnode = handler.createPropNode(stage);

							if (pnode != nullptr) {

								fl::Any *any = new fl::Any;
								any->newData<deepsg::SgStagePropNode>(DfxFluentTypes::SG_STAGE_PROP_NODE, (deepsg::SgStagePropNode*)pnode);

								stage->addProp(any);

								any->setExplicitDelete(false);
								delete any;

							}
							else {
								DfxGlobal::console::println("Failed to add bullet constraint handle node.");
							}
						}
						else {
							DfxGlobal::console::println("Failed to add bullet constraint node handle.");
						}
					}
					else {
						DfxGlobal::console::println("Failed to add bullet constraint node.");
					}
				}
			}
			else {
				DfxGlobal::console::println("Failed to add bullet constraint node, invalid node");
			}
		}
	}

	void AddBulletConstraintNode::execute(DfxCmdArguments args) {
		execute();
	}

	void *AddBulletConstraintNode::create() {
		return new AddBulletConstraintNode;
	}

	// Add Deep Rising Stage
	void AddRiptideStage::execute() {
		DfxRiptideStageHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Riptide Simulation Stage: Deep Rising Stage Handler failed");
		}
		else {

			auto stageNode = handler.createStageNode();

			if (stageNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(stageNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create Riptide Simulation Stage: Deep Rising Stage Handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddRiptideStage::execute(DfxCmdArguments) {
		execute();
	}

	void *AddRiptideStage::create() {
		return new AddRiptideStage;
	}

	// Add Deep Rising Cache Node
	void AddRiptideCacheNode::execute() {

		DfxSceneGraphSelectionDialog diag;
		diag.setMode(DfxSceneGraphSelectionDialog::NODE);

		if (diag.exec()) {
			auto node = diag.node();

			if (node != nullptr) {

				DfxSimulationStageBase *stage = nullptr;

				if (node->getType() == deepsg::SgNodeType::STAGE_PROP_COLLECTION) {

					auto propCollectionNode = (deepsg::SgStagePropCollectionNode*)node;
					stage = (DfxSimulationStageBase*)propCollectionNode->stage();

				}
				else if (node->getType() == deepsg::SgNodeType::STAGE) {

					stage = (DfxSimulationStageBase*)node;

				}
			
				if (stage != nullptr) {
					if (stage->stageType() == DfxStageType::RIPTIDE_STAGE) {
						DfxRiptideCachePropHandler handler;

						if (handler.create()) {
							auto pnode = handler.createCacheNode();

							if (pnode != nullptr) {

								fl::Any *any = new fl::Any;
								any->newData<deepsg::SgStagePropNode>(DfxFluentTypes::SG_STAGE_PROP_NODE, (deepsg::SgStagePropNode*)pnode);

								stage->addProp(any);

								any->setExplicitDelete(false);
								delete any;

							}
							else {
								DfxGlobal::console::println("Failed to add Riptide Cache handle node.");
							}
						}
						else {
							DfxGlobal::console::println("Failed to add Riptide Cache handle.");
						}
					}
				}
				else {
					DfxGlobal::console::println("Failed to add Riptide Cache node.");
				}
			}
			else {
				DfxGlobal::console::println("Failed to add Riptide Cache node, invalid node");
			}
		}
	}

	void AddRiptideCacheNode::execute(DfxCmdArguments) {
		execute();
	}

	void *AddRiptideCacheNode::create() {
		return new AddRiptideCacheNode;
	}

	// Add Deep Rising Emitter Node
	void AddRiptideEmitterNode::execute() {

		DfxSceneGraphSelectionDialog diag;
		diag.setMode(DfxSceneGraphSelectionDialog::NODE);

		if (diag.exec()) {
			auto node = diag.node();

			if (node != nullptr) {

				DfxSimulationStageBase *stage = nullptr;

				if (node->getType() == deepsg::SgNodeType::STAGE_PROP_COLLECTION) {

					auto propCollectionNode = (deepsg::SgStagePropCollectionNode*)node;
					stage = (DfxSimulationStageBase*)propCollectionNode->stage();

				}
				else if (node->getType() == deepsg::SgNodeType::STAGE) {

					stage = (DfxSimulationStageBase*)node;

				}

				if (stage != nullptr) {
					if (stage->stageType() == DfxStageType::RIPTIDE_STAGE) {
						DfxRiptideEmitterPropHandler handler;

						if (handler.create()) {
							auto pnode = handler.createEmitterNode();

							if (pnode != nullptr) {

								fl::Any *any = new fl::Any;
								any->newData<deepsg::SgStagePropNode>(DfxFluentTypes::SG_STAGE_PROP_NODE, (deepsg::SgStagePropNode*)pnode);

								stage->addProp(any);

								any->setExplicitDelete(false);
								delete any;

								DfxUpdater::getInstance()->sceneUpdated();
							}
							else {
								DfxGlobal::console::println("Failed to add Riptide Emitter handle node.");
							}
						}
						else {
							DfxGlobal::console::println("Failed to add AddRiptideEmitterNode Emitter handle.");
						}
					}
				}
				else {
					DfxGlobal::console::println("Failed to add AddRiptideEmitterNode Emitter node.");
				}
			}
			else {
				DfxGlobal::console::println("Failed to add Riptide Emitter node, invalid node");
			}
		}
	}

	void AddRiptideEmitterNode::execute(DfxCmdArguments) {
		execute();
	}

	void *AddRiptideEmitterNode::create() {
		return new AddRiptideEmitterNode;
	}

	// Add Deep Rising Cache Node
	void AddRiptideHeaterNode::execute() {

		DfxSceneGraphSelectionDialog diag;
		diag.setMode(DfxSceneGraphSelectionDialog::NODE);

		if (diag.exec()) {
			auto node = diag.node();

			if (node != nullptr) {

				DfxSimulationStageBase *stage = nullptr;

				if (node->getType() == deepsg::SgNodeType::STAGE_PROP_COLLECTION) {

					auto propCollectionNode = (deepsg::SgStagePropCollectionNode*)node;
					stage = (DfxSimulationStageBase*)propCollectionNode->stage();

				}
				else if (node->getType() == deepsg::SgNodeType::STAGE) {

					stage = (DfxSimulationStageBase*)node;

				}

				if (stage != nullptr) {
					if (stage->stageType() == DfxStageType::RIPTIDE_STAGE) {
						DfxRiptideHeaterPropHandler handler;

						if (handler.create()) {
							auto pnode = handler.createHeaterNode();

							if (pnode != nullptr) {

								fl::Any *any = new fl::Any;
								any->newData<deepsg::SgStagePropNode>(DfxFluentTypes::SG_STAGE_PROP_NODE, (deepsg::SgStagePropNode*)pnode);

								stage->addProp(any);

								any->setExplicitDelete(false);
								delete any;

								DfxUpdater::getInstance()->sceneUpdated();
							}
							else {
								DfxGlobal::console::println("Failed to add Riptide Heater handle node.");
							}
						}
						else {
							DfxGlobal::console::println("Failed to add Riptide Heater handle.");
						}
					}
				}
				else {
					DfxGlobal::console::println("Failed to add Riptide Heater node.");
				}
			}
			else {
				DfxGlobal::console::println("Failed to add Riptide Heater node, invalid node");
			}
		}
	}

	void AddRiptideHeaterNode::execute(DfxCmdArguments) {
		execute();
	}

	void *AddRiptideHeaterNode::create() {
		return new AddRiptideHeaterNode;
	}

	// Add Point Reader
	void AddPointReader::execute() {
		DfxPointReaderHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Point Reader: Point Reader Handler failed");
		}
		else {

			auto toolNode = handler.createToolNode();

			if (toolNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(toolNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create Point Reader: Point Reader Handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddPointReader::execute(DfxCmdArguments) {
		execute();
	}

	void *AddPointReader::create() {
		return new AddPointReader;
	}

	// Add Grid
	void AddGrid::execute() {
		DfxGridHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Grid: Grid Handler failed");
		}
		else {
			auto gridNode = handler.createGridNode();

			if (gridNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(gridNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create Grid Node: Grid handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddGrid::execute(DfxCmdArguments) {
		execute();
	}

	void *AddGrid::create() {
		return new AddGrid;
	}

	// Add Load VDB Node
	void AddLoadVDBNode::execute() {
		DfxLoadVDBHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create VDD Loader Tool: VDB Loader Handler failed");
		}
		else {
			auto toolNode = handler.createToolNode();

			if (toolNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(toolNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create VDB Loader Tool: VDB Loader Handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddLoadVDBNode::execute(DfxCmdArguments) {
		execute();
	}

	void *AddLoadVDBNode::create() {
		return new AddLoadVDBNode;
	}

	// Add Mesh To Grid Node
	void AddMeshToGridNode::execute() {
		DfxMeshToGridHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Mesh To Grid Tool: Mesh To Grid Handler failed.");
		}
		else {
			auto toolNode = handler.createToolNode();

			if (toolNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(toolNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create Mesh To Grid Tool: Mesh To Grid Handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddMeshToGridNode::execute(DfxCmdArguments) {
		execute();
	}

	void *AddMeshToGridNode::create() {
		return new AddMeshToGridNode;
	}

	// Add Entropy Node
	void AddEntropyStageNode::execute() {
		DfxEntropyStageHandler handler;

		bool rc = handler.create();

		if (!rc) {
			DfxGlobal::console::println("Failed To Create Entropy Stage: Add Entropy Stage failed.");
		}
		else {
			auto stageNode = handler.createStageNode();

			if (stageNode != nullptr) {
				DfxQSceneManager::getInstance()->moveNode(stageNode, deepsg::SgRootNode::getInstance());
			}
			else
				DfxGlobal::console::println("Failed To Create Entropy Stage Node: Entropy Stage Handler couldn't create node.");
		}

		DfxUpdater::getInstance()->sceneUpdated();
	}

	void AddEntropyStageNode::execute(DfxCmdArguments) {
		execute();
	}

	void *AddEntropyStageNode::create() {
		return new AddEntropyStageNode;
	}

	// Add Entropy Domain Node
	void AddEntropyDomainNode::execute() {

		DfxSceneGraphSelectionDialog diag;
		diag.setMode(DfxSceneGraphSelectionDialog::NODE);

		if (diag.exec()) {
			auto node = diag.node();

			if (node != nullptr) {

				DfxSimulationStageBase *stage = nullptr;

				if (node->getType() == deepsg::SgNodeType::STAGE_PROP_COLLECTION) {

					auto propCollectionNode = (deepsg::SgStagePropCollectionNode*)node;
					stage = (DfxSimulationStageBase*)propCollectionNode->stage();

				}
				else if (node->getType() == deepsg::SgNodeType::STAGE) {

					stage = (DfxSimulationStageBase*)node;

				}

				if (stage != nullptr) {
					if (stage->stageType() == DfxStageType::ENTROPY_STAGE) {
						DfxEntropyDomainPropHandler handler;

						if (handler.create()) {
							auto pnode = handler.createDomainNode();

							if (pnode != nullptr) {

								fl::Any *any = new fl::Any;
								any->newData<deepsg::SgStagePropNode>(DfxFluentTypes::SG_STAGE_PROP_NODE, (deepsg::SgStagePropNode*)pnode);

								stage->addProp(any);

								any->setExplicitDelete(false);
								delete any;

								DfxUpdater::getInstance()->sceneUpdated();
							}
							else {
								DfxGlobal::console::println("Failed to add Entropy Domain handle node.");
							}
						}
						else {
							DfxGlobal::console::println("Failed to add Entropy Domain handle.");
						}
					}
				}
				else {
					DfxGlobal::console::println("Failed to add Entropy Domain node.");
				}
			}
			else {
				DfxGlobal::console::println("Failed to add Entropy Domain node, invalid node");
			}
		}
	}

	void AddEntropyDomainNode::execute(DfxCmdArguments) {
		execute();
	}

	void *AddEntropyDomainNode::create() {
		return new AddEntropyDomainNode;
	}

	// Add Entropy Cache Node
	void AddEntropyCacheNode::execute() {

		DfxSceneGraphSelectionDialog diag;
		diag.setMode(DfxSceneGraphSelectionDialog::NODE);

		if (diag.exec()) {
			auto node = diag.node();

			if (node != nullptr) {

				DfxSimulationStageBase *stage = nullptr;

				if (node->getType() == deepsg::SgNodeType::STAGE_PROP_COLLECTION) {

					auto propCollectionNode = (deepsg::SgStagePropCollectionNode*)node;
					stage = (DfxSimulationStageBase*)propCollectionNode->stage();

				}
				else if (node->getType() == deepsg::SgNodeType::STAGE) {

					stage = (DfxSimulationStageBase*)node;

				}

				if (stage != nullptr) {
					if (stage->stageType() == DfxStageType::ENTROPY_STAGE) {
						DfxEntropyCachePropHandler handler;

						if (handler.create()) {
							auto pnode = handler.createCacheNode();

							if (pnode != nullptr) {

								fl::Any *any = new fl::Any;
								any->newData<deepsg::SgStagePropNode>(DfxFluentTypes::SG_STAGE_PROP_NODE, (deepsg::SgStagePropNode*)pnode);

								stage->addProp(any);

								any->setExplicitDelete(false);
								delete any;

								DfxUpdater::getInstance()->sceneUpdated();
							}
							else {
								DfxGlobal::console::println("Failed to add Entropy Cache handle node.");
							}
						}
						else {
							DfxGlobal::console::println("Failed to add Entropy Cache handle.");
						}
					}
				}
				else {
					DfxGlobal::console::println("Failed to add Entropy Cache node.");
				}
			}
			else {
				DfxGlobal::console::println("Failed to add Entropy Cache node, invalid node");
			}
		}
	}

	void AddEntropyCacheNode::execute(DfxCmdArguments) {
		execute();
	}

	void *AddEntropyCacheNode::create() {
		return new AddEntropyCacheNode;
	}
}