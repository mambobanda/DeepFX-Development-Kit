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


#include "ModellingCommands.h"
#include "DfxLuxMaterialHandler.h"
#include "DfxOSPRayMaterialHandler.h"

namespace command {

	// Delete Vetrtices
	void DeleteVertices::execute() {

		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedVertexIndices().empty()) {

			if (DfxWidgetManager::getInstance()->isWidgetActive() &&
				!DfxWidgetManager::getInstance()->isNull()) {

				auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

				if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {
					auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
					auto mesh = adapter->getMesh()->resource<ds::Mesh>();
					
					std::vector<ds::Vert*> verts;
					for (auto i : DfxMeshSelectionModel::getInstance()->selectedVertexIndices()) {
						verts.push_back(mesh->vertices()[i]);
					}
					
					for (auto v : verts) mesh->removeVertex(v);

					mesh->reindexVertices();
					mesh->reindexEdges();
					mesh->reindexFaces();

					DfxMeshSelectionModel::getInstance()->selectedVertexIndices().clear();
					
					DfxUpdater::getInstance()->sceneUpdated();
				}
			}
		}
	}

	void DeleteVertices::execute(DfxCmdArguments) {
		execute();
	}

	void *DeleteVertices::create() {
		return new DeleteVertices;
	}

	// Delete Edges
	void DeleteEdges::execute() {

		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().empty()) {

			if (DfxWidgetManager::getInstance()->isWidgetActive() &&
				!DfxWidgetManager::getInstance()->isNull()) {

				auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

				if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {
					auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
					auto mesh = adapter->getMesh()->resource<ds::Mesh>();

					std::vector<ds::Edge*> edges;
					for (auto i : DfxMeshSelectionModel::getInstance()->selectedEdgeIndices()) {
						edges.push_back(mesh->edges()[i]);
					}

					for (auto v : edges) mesh->removeEdge(v);

					mesh->reindexVertices();
					mesh->reindexEdges();
					mesh->reindexFaces();

					DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().clear();

					DfxUpdater::getInstance()->sceneUpdated();
				}
			}
		}
	}

	void DeleteEdges::execute(DfxCmdArguments) {
		execute();
	}

	void *DeleteEdges::create() {
		return new DeleteEdges;
	}

	// Delete Faces
	void DeleteFaces::execute() {

		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedFaceIndices().empty()) {

			if (DfxWidgetManager::getInstance()->isWidgetActive() &&
				!DfxWidgetManager::getInstance()->isNull()) {

				auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

				if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {
					auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
					auto mesh = adapter->getMesh()->resource<ds::Mesh>();

					std::vector<ds::Face*> faces;
					for (auto i : DfxMeshSelectionModel::getInstance()->selectedFaceIndices()) {
						faces.push_back(mesh->faces()[i]);
					}

					for (auto v : faces) mesh->removeFace(v);

					mesh->reindexVertices();
					mesh->reindexEdges();
					mesh->reindexFaces();

					DfxMeshSelectionModel::getInstance()->selectedFaceIndices().clear();

					DfxUpdater::getInstance()->sceneUpdated();
				}
			}
		}
	}

	void DeleteFaces::execute(DfxCmdArguments) {
		execute();
	}

	void *DeleteFaces::create() {
		return new DeleteFaces;
	}

	// Extrude Face / Faces
	void ExtrudeFaces::execute() {

		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedFaceIndices().empty()) {

			if (DfxWidgetManager::getInstance()->isWidgetActive() &&
				!DfxWidgetManager::getInstance()->isNull()) {

				auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

				if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {
					auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
					auto mesh = adapter->getMesh()->resource<ds::Mesh>();
					ds::MeshOperator mop;

					mop.setMesh(mesh);
					auto newFaces = mop.extrudeFaces(DfxMeshSelectionModel::getInstance()->selectedFaceIndices());
					DfxMeshSelectionModel::getInstance()->selectedFaceIndices().clear();

					if (!newFaces.empty()) {
						for (auto f : newFaces) {
							DfxMeshSelectionModel::getInstance()->selectedFaceIndices().push_back(f->getIndex());
						}
						mop.calcSmoothNormals("smooth_normals");
						mop.calcFaceNormals("face_normals");
						DfxGlobal::console::println("Face/Faces extruded...");		
					}
					else {
						DfxGlobal::console::println("Failed to extrude Face/Faces...");
					}	

					DfxUpdater::getInstance()->sceneUpdated();
				}
			}
		}
	}

	void ExtrudeFaces::execute(DfxCmdArguments) {
		execute();
	}

	void *ExtrudeFaces::create() {
		return new ExtrudeFaces;
	}

	// ~
	void TriangulateFaces::execute() {

		if (DfxMeshSelectionModel::getInstance()->nodeReference() && !DfxMeshSelectionModel::getInstance()->selectedFaceIndices().empty()) {

			if (DfxWidgetManager::getInstance()->isWidgetActive() &&
				!DfxWidgetManager::getInstance()->isNull()) {

				auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

				if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {
					auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
					auto mesh = adapter->getMesh()->resource<ds::Mesh>();
					ds::MeshOperator mop;

					mop.setMesh(mesh);
					auto newFaces = mop.triangulateFace(DfxMeshSelectionModel::getInstance()->selectedFaceIndices());

					if (!newFaces.empty()) {
						DfxMeshSelectionModel::getInstance()->selectedFaceIndices().clear();

						for (auto f : newFaces) {
							DfxMeshSelectionModel::getInstance()->selectedFaceIndices().push_back(f->getIndex());
						}

						mop.calcSmoothNormals("smooth_normals");
						mop.calcFaceNormals("face_normals");
						DfxGlobal::console::println("Face/Faces triangulated...");
					}
					else {
						DfxGlobal::console::println("Failed Triangulate Face/Faces...");
					}

					DfxUpdater::getInstance()->sceneUpdated();
				}
			}
		}
	}

	void TriangulateFaces::execute(DfxCmdArguments) {
		execute();
	}

	void *TriangulateFaces::create() {
		return new TriangulateFaces;
	}

	// Bevel Edges
	void BevelEdge::execute() {

		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().empty()) {

			if (DfxWidgetManager::getInstance()->isWidgetActive() &&
				!DfxWidgetManager::getInstance()->isNull()) {

				auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

				if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {
					auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
					auto mesh = adapter->getMesh()->resource<ds::Mesh>();
					ds::MeshOperator mop;

					mop.setMesh(mesh);
					auto newEdges = mop.bevelEdges02(DfxMeshSelectionModel::getInstance()->selectedEdgeIndices(), 0.1f);

					DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().clear();

					if (!newEdges.empty()) {
						for (auto f : newEdges) {
							DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().push_back(f->getIndex());
						}
						mop.calcSmoothNormals("smooth_normals");
						mop.calcFaceNormals("face_normals");
						DfxGlobal::console::println("Edges Beveled...");
					}
					else {
						DfxGlobal::console::println("Failed to bevel edges...");
					}

					DfxUpdater::getInstance()->sceneUpdated();
				}
			}
		}
	}

	void BevelEdge::execute(DfxCmdArguments) {
		execute();
	}

	void *BevelEdge::create() {
		return new BevelEdge;
	}

	// Create Face Set
	void CreateFaceSet::execute() {
		if (DfxMeshSelectionModel::getInstance()->nodeReference() && !DfxMeshSelectionModel::getInstance()->selectedFaceIndices().empty()) {

			auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

			if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {
				auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
				auto mesh = adapter->getMesh()->resource<ds::Mesh>();

				bool ok;
				QString text = QInputDialog::getText(nullptr, "New Set", "Set Name:", QLineEdit::Normal, "default", &ok);

				if (ok && !text.isEmpty()) {
						
					std::string setName = std::string("set:") + text.toStdString();

					if (!mesh->hasProperty(setName, ds::PropertyType::FACE)) {
						mesh->addProperty(setName, ds::PropertyType::FACE, ds::type::INT);
					}

					mesh->fillProperty<int>(setName, 0, ds::PropertyType::FACE);

					for (size_t inx = 0; inx < mesh->faces().size(); inx++) mesh->faces()[inx]->setProperty<int>(setName, 0);

					for (auto inx : DfxMeshSelectionModel::getInstance()->selectedFaceIndices()) {
						mesh->faces()[inx]->setProperty<int>(setName, 1);
					}

					DfxUpdater::getInstance()->sceneUpdated();
				}
			}
			
		}
	}

	void CreateFaceSet::execute(DfxCmdArguments) {
		execute();
	}

	void *CreateFaceSet::create() {
		return new CreateFaceSet;
	}
	
	// Tag Surface
	void TagSurface::execute() {
		if (DfxMeshSelectionModel::getInstance()->nodeReference() && !DfxMeshSelectionModel::getInstance()->selectedFaceIndices().empty()) {

			auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();
			

			if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {
				auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
				auto mesh = adapter->getMesh()->resource<ds::Mesh>();

				bool ok;
				QString text = QInputDialog::getText(nullptr, "Surface Name", "Set Name:", QLineEdit::Normal, "default_surface", &ok);

				if (ok && !text.isEmpty()) {

					auto refPath = geonode->referencePath();
					unsigned int surf_id = DfxSurfaceDictionary::getInstance()->maxID(refPath) + 1;
					DfxSurfaceDictionary::getInstance()->insert(refPath, surf_id, text.toStdString());

					for (auto inx : DfxMeshSelectionModel::getInstance()->selectedFaceIndices()) {
						mesh->faces()[inx]->setSurfID(surf_id);
					}
				}
			}
			else if(geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DYNA_MESH){
				auto adapter = (DfxDynaMeshGeoAdapter*)geonode->getGeometryAdapter();
				auto mesh = adapter->getMesh();

				bool ok;
				QString text = QInputDialog::getText(nullptr, "Surface Name", "Set Name:", QLineEdit::Normal, "default_surface", &ok);

				if (ok && !text.isEmpty()) {

					auto refPath = geonode->referencePath();
					unsigned int surf_id = DfxSurfaceDictionary::getInstance()->maxID(refPath) + 1;
					DfxSurfaceDictionary::getInstance()->insert(refPath, surf_id, text.toStdString());

					for (auto inx : DfxMeshSelectionModel::getInstance()->selectedFaceIndices()) {
						mesh->faceIDs()[inx] = surf_id;
					}
				}
			}
		}
	}

	void TagSurface::execute(DfxCmdArguments args) {
		execute();
	}

	void *TagSurface::create() {
		return new TagSurface;
	}

	// Link Materials To Surfaces
	void LinkMaterialToSurfaces::execute() {}

	void LinkMaterialToSurfaces::execute(DfxCmdArguments args) {
		if (!args.empty()) {
			if (DfxMeshSelectionModel::getInstance()->nodeReference() && !DfxMeshSelectionModel::getInstance()->selectedFaceIndices().empty()) {

				auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();
		
				auto adapter = geonode->getGeometryAdapter();

				std::set<unsigned int> ids;
				for (auto inx : DfxMeshSelectionModel::getInstance()->selectedFaceIndices()) {
					ids.insert(adapter->getPolygonSurfaceID(inx));
				}

				for (auto sid : ids) {
					DfxMaterialLinker::getInstance()->updateObjectLink(geonode->referencePath(), sid, args[1]);
				}

				DfxUpdater::getInstance()->materialUpdated();
			}
		}
	}

	void *LinkMaterialToSurfaces::create() {
		return new LinkMaterialToSurfaces;
	}

	// Print Surfaces
	void PrintSurfaces::execute() {
		if (!DfxSceneSelectionModel::getInstance()->selected().empty()) {

			for (auto node : DfxSceneSelectionModel::getInstance()->selected()) {

				if (node->getType() == deepsg::SgNodeType::GEOMETRY) {
					auto geonode = (deepsg::SgGeoNode*)node;

					if (geonode->getGeometryAdapter()->type() == DfxGeoAdapterType::DEEP_SIMPLEX) {
						auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
						auto mesh = adapter->getMesh()->resource<ds::Mesh>();

						std::unordered_map<std::string, int> surfaces;

						for (auto f : mesh->faces()) {
							if (!f->isPropertyNull("surface")) {
								surfaces[f->getProperty<std::string>("surface")] = 0;
							}
						}

						if (!surfaces.empty()) {
							for (auto srf : surfaces) {
								DfxGlobal::console::println("Surface: " + srf.first);
							}
						}
						else {
							DfxGlobal::console::println("Object has not been assigned any surfaces.");
						}
					}
				}
			}
		}
	}

	void PrintSurfaces::execute(DfxCmdArguments) {
		execute();
	}

	void *PrintSurfaces::create() {
		return new PrintSurfaces;
	}

	// New Material
	void NewMaterial::execute() {
		QStringList materialTypes;
		materialTypes << "LuxCoreRender" << "OSPRay";

		QString text = QInputDialog::getItem(nullptr, "Material Type", "Type:", materialTypes, 0, false);

		if (text == QString("LuxCoreRender")) {
			DfxMaterialManager::getInstance()->newMaterial([]() {
				DfxLuxMaterialHandler handler;
				handler.create();
			});
		}
		else if (text == QString("OSPRay")) {
			DfxMaterialManager::getInstance()->newMaterial([]() {
				DfxOSPRayMaterialHandler handler;
				handler.create();
			});
		}
		else {
			DfxGlobal::console::println("Unknown Material Type");
		}
		
	}

	void NewMaterial::execute(DfxCmdArguments) {
		execute();
	}

	void *NewMaterial::create() {
		return new NewMaterial;
	}
}