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


#include "SelectionCommands.h"
#include "DfxUtil.h"

namespace command {

	// Select Connceted faces
	void SelectConnectedFaces::execute() {
		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedFaceIndices().empty()) {

			auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

			if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {

				ds::MeshQuery mq;
				auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
				auto mesh = adapter->getMesh()->resource<ds::Mesh>();

				ds::FaceList fl;
				for (auto i : DfxMeshSelectionModel::getInstance()->selectedFaceIndices()) {
					fl.push_back(mesh->faces()[i]);
				}

				if (!fl.empty()) {
					auto loop = mq.connectedFaces(mesh,fl);

					if (!loop.empty()) {
						DfxMeshSelectionModel::getInstance()->selectedFaceIndices().clear();
						for (auto t : loop) DfxMeshSelectionModel::getInstance()->selectedFaceIndices().push_back(t->getIndex());
					}
					else {
						DfxGlobal::console::println("Failed to select connected faces.");
					}

				}

				DfxMeshSelectionModel::getInstance()->generateSelectedFaceTriangleIndices();
				DfxUpdater::getInstance()->sceneUpdated();
			}
			
		}
	}

	void SelectConnectedFaces::execute(DfxCmdArguments) {
		execute();
	}

	void *SelectConnectedFaces::create() {
		return new SelectConnectedFaces;
	}

	// Select Faceloop
	void SelectFaceLoop::execute() {

		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedFaceIndices().empty()) {
		
			auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

			if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {

				ds::MeshQuery mq;
				auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
				auto mesh = adapter->getMesh()->resource<ds::Mesh>();

				ds::FaceList fl;
				for (auto i : DfxMeshSelectionModel::getInstance()->selectedFaceIndices()) {
					fl.push_back(mesh->faces()[i]);
				}

				if (!fl.empty()) {
					auto loop = mq.faceLoop(fl);

					if (!loop.empty()) {
						DfxMeshSelectionModel::getInstance()->selectedFaceIndices().clear();
						for (auto t : loop) DfxMeshSelectionModel::getInstance()->selectedFaceIndices().push_back(t->getIndex());
					}
					else {
						DfxGlobal::console::println("Failed to select face loop.");
					}
					/*DfxMeshSelectionModel::getInstance()->selectedVertexIndices().clear();
					for (auto t : fl) DfxMeshSelectionModel::getInstance()->selectedFaceIndices().push_back(t->getIndex());
					DfxMode::getInstance()->setMode(DfxMode::Mode::FACE_EDIT);*/
				}

				DfxMeshSelectionModel::getInstance()->generateSelectedFaceTriangleIndices();
				DfxUpdater::getInstance()->sceneUpdated();
			}
			
		}
	}

	void SelectFaceLoop::execute(DfxCmdArguments) {
		execute();
	}

	void *SelectFaceLoop::create() {
		return new SelectFaceLoop();
	}

	// Select All Faces
	void SelectAllFaces::execute() {

		if (DfxMeshSelectionModel::getInstance()->nodeReference()) {	

			auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

			if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {

				ds::MeshQuery mq;
				auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
				auto mesh = adapter->getMesh()->resource<ds::Mesh>();

				DfxMeshSelectionModel::getInstance()->selectedFaceIndices().clear();
				for (auto f : mesh->faces()) {
					DfxMeshSelectionModel::getInstance()->selectedFaceIndices().push_back(f->getIndex());
				}

				if (!DfxMeshSelectionModel::getInstance()->selectedFaceIndices().empty()) {					
			
					DfxMode::getInstance()->setMode(DfxMode::Mode::FACE_EDIT);
				}

				DfxMeshSelectionModel::getInstance()->generateSelectedFaceTriangleIndices();
				DfxUpdater::getInstance()->sceneUpdated();
			}
			
		}
	}

	
	void SelectAllFaces::execute(DfxCmdArguments) {
		execute();
	}

	void *SelectAllFaces::create() {
		return new SelectAllFaces();
	}

	// Select Face set
	void SelectFaceSet::execute() {}

	void SelectFaceSet::execute(DfxCmdArguments args) {
		if (DfxMeshSelectionModel::getInstance()->nodeReference()) {

			auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();
			DfxMeshSelectionModel::getInstance()->selectedFaceIndices().clear();
			if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {

				ds::MeshQuery mq;
				auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
				auto mesh = adapter->getMesh()->resource<ds::Mesh>();

				std::string setName = "";
				for (int i = 1; i < args.size(); i++) {
					setName += args[i];
					if ((i + 1) != args.size()) setName += " ";
				}

				if (!args.empty()) {
					if (mesh->hasProperty(std::string("set:") + setName, ds::PropertyType::FACE)) {
						std::vector<size_t> indices;

						for (auto face : mesh->faces()) {
							if (face->getProperty<int>(std::string("set:") + setName) == 1) {
								DfxMeshSelectionModel::getInstance()->selectedFaceIndices().push_back(face->getIndex());
							}
						}

						DfxMeshSelectionModel::getInstance()->generateSelectedFaceTriangleIndices();
						DfxUpdater::getInstance()->sceneUpdated();
					}
				}

			}
			
		}
	}

	void *SelectFaceSet::create() {
		return new SelectFaceSet;
	}

	// Select Surface	
	void SelectSurface::execute() {}

	void SelectSurface::execute(DfxCmdArguments args) {

		if (DfxMeshSelectionModel::getInstance()->nodeReference()) {

			auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();
			DfxMeshSelectionModel::getInstance()->selectedFaceIndices().clear();
			if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {

				ds::MeshQuery mq;
				auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
				auto mesh = adapter->getMesh()->resource<ds::Mesh>();

				if (!args.empty()) {
					auto tag_id = std::atoi(args[1].c_str());
					
					std::vector<bool> insert(mesh->faces().size(), false);
					
					#pragma omp parallel default(shared)
					{
						#pragma omp for schedule(static)
						for (long long int inx = 0; inx < mesh->faces().size(); inx++) {
							auto face = mesh->faces()[inx];
							if (face->getSurfID() == tag_id) {
								insert[inx] = true;
							}
						}
					}

					size_t counter = 0;
					for (auto in : insert) {
						if (in) {
							DfxMeshSelectionModel::getInstance()->selectedFaceIndices().push_back(counter);
						}
						counter++;
					}
						
					DfxMeshSelectionModel::getInstance()->generateSelectedFaceTriangleIndices();
					DfxUpdater::getInstance()->sceneUpdated();
					
				}
			}
			else if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DYNA_MESH) {
				ds::MeshQuery mq;
				auto adapter = (DfxDynaMeshGeoAdapter*)geonode->getGeometryAdapter();
				auto mesh = adapter->getMesh();

				if (!args.empty()) {
					auto tag_id = std::atoi(args[1].c_str());

					std::vector<bool> insert(mesh->triangleCount(), false);

					#pragma omp parallel default(shared)
					{
						#pragma omp for schedule(static)
						for (long long int inx = 0; inx < mesh->triangleCount(); inx++) {
							if (mesh->faceIDs()[inx] == tag_id) {
								insert[inx] = true;
							}
						}
					}

					size_t counter = 0;
					for (auto in : insert) {
						if (in) {
							DfxMeshSelectionModel::getInstance()->selectedFaceIndices().push_back(counter);
						}
						counter++;
					}

					DfxMeshSelectionModel::getInstance()->generateSelectedFaceTriangleIndices();
					DfxUpdater::getInstance()->sceneUpdated();

				}
			}
		}
	}

	void *SelectSurface::create() {
		return new SelectSurface;
	}

	// ~~~
	void SelectVertexIncidentEdges::execute() {

		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedVertexIndices().empty()) {

			if (DfxWidgetManager::getInstance()->isWidgetActive() &&
				!DfxWidgetManager::getInstance()->isNull()) {

				auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

				if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {
					
					ds::MeshQuery mq;
					auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
					auto mesh = adapter->getMesh()->resource<ds::Mesh>();

					ds::EdgeList el;
					for (auto i : DfxMeshSelectionModel::getInstance()->selectedVertexIndices()) {
						auto temp_el = mq.vertexIncidentEdges(mesh->vertices()[i]);

						for(auto t: temp_el) el.push_back(t);
					}

					if (!el.empty()) {
						DfxMeshSelectionModel::getInstance()->selectedVertexIndices().clear();
						for (auto t : el) DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().push_back(t->getIndex());
						DfxMode::getInstance()->setMode(DfxMode::Mode::EDGE_EDIT);
					}

					DfxUpdater::getInstance()->sceneUpdated();
				}
			}
		}
	}

	void SelectVertexIncidentEdges::execute(DfxCmdArguments) {
		execute();
	}

	void *SelectVertexIncidentEdges::create() {
		return new SelectVertexIncidentEdges();
	}

	// ~~~
	void SelectEdgeAdjacentVertices::execute() {

		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().empty()) {

			if (DfxWidgetManager::getInstance()->isWidgetActive() &&
				!DfxWidgetManager::getInstance()->isNull()) {

				auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

				if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {

					ds::MeshQuery mq;
					auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
					auto mesh = adapter->getMesh()->resource<ds::Mesh>();

					ds::VertexList vl;
					for (auto i : DfxMeshSelectionModel::getInstance()->selectedEdgeIndices()) {
						auto temp_vl = mq.edgeAdjacentVertices(mesh->edges()[i]);

						for (auto t : temp_vl) vl.push_back(t);
					}

					if (!vl.empty()) {
						DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().clear();
						for (auto t : vl) DfxMeshSelectionModel::getInstance()->selectedVertexIndices().push_back(t->getIndex());
						DfxMode::getInstance()->setMode(DfxMode::Mode::VERTEX_EDIT);
					}

					DfxUpdater::getInstance()->sceneUpdated();
				}
			}
		}
	}

	void SelectEdgeAdjacentVertices::execute(DfxCmdArguments) {
		execute();
	}

	void *SelectEdgeAdjacentVertices::create() {
		return new SelectEdgeAdjacentVertices();
	}

	// ~~~
	void SelectVertexIncidentFaces::execute() {

		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedVertexIndices().empty()) {

			if (DfxWidgetManager::getInstance()->isWidgetActive() &&
				!DfxWidgetManager::getInstance()->isNull()) {

				auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

				if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {

					ds::MeshQuery mq;
					auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
					auto mesh = adapter->getMesh()->resource<ds::Mesh>();

					ds::FaceList fl;
					for (auto i : DfxMeshSelectionModel::getInstance()->selectedVertexIndices()) {
						auto temp_vl = mq.vertexIncidentFaces(mesh->vertices()[i]);

						for (auto t : temp_vl) fl.push_back(t);
					}

					if (!fl.empty()) {
						DfxMeshSelectionModel::getInstance()->selectedVertexIndices().clear();
						for (auto t : fl) DfxMeshSelectionModel::getInstance()->selectedFaceIndices().push_back(t->getIndex());
						DfxMode::getInstance()->setMode(DfxMode::Mode::FACE_EDIT);
					}

					DfxUpdater::getInstance()->sceneUpdated();
				}
			}
		}
	}

	void SelectVertexIncidentFaces::execute(DfxCmdArguments) {
		execute();
	}

	void *SelectVertexIncidentFaces::create() {
		return new SelectVertexIncidentFaces();
	}


	// ~~~
	void PrintFaceID::execute() {

		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedFaceIndices().empty()) {

			if (DfxWidgetManager::getInstance()->isWidgetActive() &&
				!DfxWidgetManager::getInstance()->isNull()) {

				auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

				if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {

					ds::MeshQuery mq;
					auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
					auto mesh = adapter->getMesh()->resource<ds::Mesh>();

					ds::FaceList fl;
					for (auto i : DfxMeshSelectionModel::getInstance()->selectedFaceIndices()) {
						DfxGlobal::console::println(std::string("Face ID: ") + std::to_string(mesh->vertices()[i]->getUID()));
					}

					DfxUpdater::getInstance()->sceneUpdated();
				}
			}
		}
	}

	void PrintFaceID::execute(DfxCmdArguments) {
		execute();
	}

	void *PrintFaceID::create() {
		return new PrintFaceID();
	}

	// ~~~
	void PrintEdgeID::execute() {

		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().empty()) {

			if (DfxWidgetManager::getInstance()->isWidgetActive() &&
				!DfxWidgetManager::getInstance()->isNull()) {

				auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

				if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {

					ds::MeshQuery mq;
					auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
					auto mesh = adapter->getMesh()->resource<ds::Mesh>();

					ds::FaceList fl;
					for (auto i : DfxMeshSelectionModel::getInstance()->selectedEdgeIndices()) {
						DfxGlobal::console::println(std::string("Edge ID: ") + std::to_string(mesh->edges()[i]->getUID()));
					}

					DfxUpdater::getInstance()->sceneUpdated();
				}
			}
		}
	}

	void PrintEdgeID::execute(DfxCmdArguments) {
		execute();
	}

	void *PrintEdgeID::create() {
		return new PrintEdgeID();
	}

	// ~~~
	void PrintVertexID::execute() {

		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedVertexIndices().empty()) {

			if (DfxWidgetManager::getInstance()->isWidgetActive() &&
				!DfxWidgetManager::getInstance()->isNull()) {

				auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

				if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {

					ds::MeshQuery mq;
					auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
					auto mesh = adapter->getMesh()->resource<ds::Mesh>();

					ds::FaceList fl;
					for (auto i : DfxMeshSelectionModel::getInstance()->selectedVertexIndices()) {
						DfxGlobal::console::println(std::string("Vertex ID: ") + std::to_string(mesh->vertices()[i]->getUID()));
					}

					DfxUpdater::getInstance()->sceneUpdated();
				}
			}
		}
	}

	void PrintVertexID::execute(DfxCmdArguments) {
		execute();
	}

	void *PrintVertexID::create() {
		return new PrintVertexID();
	}
}