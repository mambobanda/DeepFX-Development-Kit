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

#include "DfxVDBParticleMesher.h"

#ifdef USE_OPENVDB_LIB 

#include "IOUtilities.h"

namespace DfxNodes {

	typedef struct {
		int a[3];
		int b[3];
	}SplitQuad;

	/*glm::vec3 vdbFloat3ToGlm(DfxVDBFloat3 fl) {
		return glm::vec3(fl.x, fl.y, fl.z);
	}
	+
	SplitQuad quadToTriangles(DfxVDBOutputMesh *mesh, int counter) {
		SplitQuad sq;

		glm::vec3 vec_a = vdbFloat3ToGlm(mesh->verts[mesh->indices[counter]]);
		glm::vec3 vec_b = vdbFloat3ToGlm(mesh->verts[mesh->indices[counter + 1]]);
		glm::vec3 vec_c = vdbFloat3ToGlm(mesh->verts[mesh->indices[counter + 2]]);
		glm::vec3 vec_d = vdbFloat3ToGlm(mesh->verts[mesh->indices[counter + 3]]);

		float dist1 = glm::distance(vec_a, vec_c);
		float dist2 = glm::distance(vec_b, vec_d);

		if (dist1 > dist2) {
			sq.a[0] = mesh->indices[counter];
			sq.a[1] = mesh->indices[counter + 1];
			sq.a[2] = mesh->indices[counter + 3];

			sq.b[0] = mesh->indices[counter + 1];
			sq.b[1] = mesh->indices[counter + 2];
			sq.b[2] = mesh->indices[counter + 3];
		}
		else {
			sq.a[0] = mesh->indices[counter];
			sq.a[1] = mesh->indices[counter + 1];
			sq.a[2] = mesh->indices[counter + 2];

			sq.b[0] = mesh->indices[counter];
			sq.b[1] = mesh->indices[counter + 2];
			sq.b[2] = mesh->indices[counter + 3];
		}

		return sq;
	}*/

	SplitQuad quadToTriangles(Mesh *mesh, int counter) {
		SplitQuad sq;

		glm::vec3 vec_a = mesh->verts[mesh->indices[counter]];
		glm::vec3 vec_b = mesh->verts[mesh->indices[counter + 1]];
		glm::vec3 vec_c = mesh->verts[mesh->indices[counter + 2]];
		glm::vec3 vec_d = mesh->verts[mesh->indices[counter + 3]];

		float dist1 = glm::distance(vec_a, vec_c);
		float dist2 = glm::distance(vec_b, vec_d);

		if (dist1 > dist2) {
			sq.a[0] = mesh->indices[counter];
			sq.a[1] = mesh->indices[counter + 1];
			sq.a[2] = mesh->indices[counter + 3];

			sq.b[0] = mesh->indices[counter + 1];
			sq.b[1] = mesh->indices[counter + 2];
			sq.b[2] = mesh->indices[counter + 3];
		}
		else {
			sq.a[0] = mesh->indices[counter];
			sq.a[1] = mesh->indices[counter + 1];
			sq.a[2] = mesh->indices[counter + 2];

			sq.b[0] = mesh->indices[counter];
			sq.b[1] = mesh->indices[counter + 2];
			sq.b[2] = mesh->indices[counter + 3];
		}

		return sq;
	}

	DfxVDBParticleMesher::DfxVDBParticleMesher(DfxGlobalEntityName uniquename): DfxEntity(uniquename) {
		setLogicalName("VDB Particle Mesher");

		((DfxTraitUID<size_t>*)getProperty(outRef))->setRootID(getInstanceID());

		setPropertyState(mesh, DfxEntityPropertyState::DATA_STORE_STATE);

		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);
		setPropertyState(inFilters, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inGridSize, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inParticleSize, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPoints, DfxEntityPropertyState::INPUT_STATE);
	

		*(float*)DfxEntity::getProperty(inGridSize) = 0.02f;
		*(float*)DfxEntity::getProperty(inParticleSize) = 0.045f;
	}

	DfxVDBParticleMesher::~DfxVDBParticleMesher() {

	}

	void DfxVDBParticleMesher::compute() {
		using namespace DfxEntityPropertyTypes;

		Dfx3dMeshData* in_mesh = (Dfx3dMeshData*)DfxEntity::getProperty(mesh);
		float in_gsize = *(float*)DfxEntity::getProperty(inGridSize);
		float in_psize = *(float*)DfxEntity::getProperty(inParticleSize);

		in_mesh->clearIndexBuffer();
		in_mesh->clearVertexBuffer();

		std::vector<vdb::Filter*> filters;

		if (propertyHasBinding(inPoints)) {

			Float3Array *parr = (Float3Array*)getProperty(inPoints);

			if (propertyHasBinding(inFilters)) {
				
				UID *uid = (UID*)getProperty(inFilters);
				auto ft_ret = findAllFilters(*uid);

				while (!ft_ret.empty()) {

					DfxEntity *top = ft_ret.front();

					if (top->UniqueName() == DfxVDBDilateNode_Identifier) {
						auto flt = new vdb::DilateFilter;

						float ofs = *(float*)top->getProperty(DfxVDBDilateNode::inOffset);
						int iter = *(float*)top->getProperty(DfxVDBDilateNode::inIterations);

						flt->offset = ofs;
						flt->iterations = iter;

						filters.push_back(flt);
					}
					else if (top->UniqueName() == DfxVDBErodeNode_Identifier) {

						auto flt = new vdb::ErodeFilter;

						float ofs = *(float*)top->getProperty(DfxVDBErodeNode::inOffset);
						int iter = *(float*)top->getProperty(DfxVDBErodeNode::inIterations);

						flt->offset = ofs;
						flt->iterations = iter;

						filters.push_back(flt);
					}
					else if (top->UniqueName() == DfxVDBMedianNode_Identifier) {
					
						auto flt = new vdb::MedianFilter;

						float wdth = *(float*)top->getProperty(DfxVDBMedianNode::inWidth);
						int iter = *(float*)top->getProperty(DfxVDBMedianNode::inIterations);

						flt->width = wdth;
						flt->iterations = iter;

						filters.push_back(flt);
					}
					else if (top->UniqueName() == DfxVDBMeanNode_Identifier) {

						auto flt = new vdb::MeanFilter;

						float wdth = *(float*)top->getProperty(DfxVDBMeanNode::inWidth);
						int iter = *(float*)top->getProperty(DfxVDBMeanNode::inIterations);

						flt->width = wdth;
						flt->iterations = iter;

						filters.push_back(flt);
					}
					else if(top->UniqueName() == DfxVDBGaussianNode_Identifier) {
						
						auto flt = new vdb::GaussianFilter;

						float wdth = *(float*)top->getProperty(DfxVDBGaussianNode::inWidth);
						int iter = *(float*)top->getProperty(DfxVDBGaussianNode::inIterations);

						flt->width = wdth;
						flt->iterations = iter;

						filters.push_back(flt);
					}

					ft_ret.pop();
				}
			}

			if (!parr->isNull() && parr->size() > 0) {

				glm::vec3 *fpoints = new glm::vec3[parr->size()];

				for (size_t inx = 0; inx < parr->size(); inx++) {

					fpoints[inx].x = (*parr)[inx].x;
					fpoints[inx].y = (*parr)[inx].y;
					fpoints[inx].z = (*parr)[inx].z;

				}

				//auto skinner = DfxPluginManager::getInstance()->getVDBSkinnerParticlesFunc();
				OpenvdbParticleSkinner skinner;
				skinner.setGridRadius(in_gsize);
				skinner.setParticleRadius(in_psize);
				skinner.setHalfWidth(3.0);

				Mesh *output = new Mesh;
				skinner.skinParticles(fpoints, parr->size(), output, filters);
				/*auto output = skinner(fpoints, parr->size(), in_gsize, in_psize, 3.0, NULL, 0, DfxPluginManager::getInstance()->getVDBMeshGlobal());*/

				if (output->nbVerts > 0) {

					in_mesh->resizeVertexBuffer(output->nbVerts);
					in_mesh->resizeIndexBuffer(output->nbFaces * 6);

					for (int inj = 0; inj < output->nbVerts; inj++) {
						glm::vec3 v(output->verts[inj].x, output->verts[inj].y, output->verts[inj].z);
						glm::vec3 n(output->normals[inj].x, output->normals[inj].y, output->normals[inj].z);
						
						in_mesh->setVertex(inj, v);
						in_mesh->setNormal(inj, n);
						in_mesh->setColor(inj, glm::vec3(0.69f, 0.69f, 0.69f));
					}

					int tri_counter = 0;
					int quad_counter = 0;
					for (int inj = 0; inj < output->nbFaces; inj++) {

						SplitQuad split = quadToTriangles(output, quad_counter);

						in_mesh->setIndex(tri_counter, split.a[2]);
						in_mesh->setIndex(tri_counter + 1, split.a[1]);
						in_mesh->setIndex(tri_counter + 2, split.a[0]);

						in_mesh->setIndex(tri_counter + 3, split.b[2]);
						in_mesh->setIndex(tri_counter + 4, split.b[1]);
						in_mesh->setIndex(tri_counter + 5, split.b[0]);

						quad_counter += 4;
						tri_counter += 6;
					}

					delete[] fpoints;
					delete[] output->verts;
					delete[] output->normals;
					delete[] output->indices;

					delete output;

					for (size_t f_inx = 0; f_inx < filters.size(); f_inx++) {
						delete filters[f_inx];
					}
				}
			}
		}

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void DfxVDBParticleMesher::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void DfxVDBParticleMesher::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxVDBParticleMesher::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxVDBParticleMesher::isExportable() {
		return true;
	}

	void *DfxVDBParticleMesher::creator(DfxGlobalEntityName uniquename) {
		return new DfxVDBParticleMesher(uniquename);
	}

	void DfxVDBParticleMesher::initialize() {

		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inFilters, "in_filters", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inGridSize, "in_grid_size", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inParticleSize, "in_particle_size", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inPoints, "in_positions", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
		DfxEntityDB::createProperty(mesh, "@mesh_data", new DfxEntityPropertyTypes::Dfx3DMeshPropertyType());
	}

	std::queue<DfxEntity*> DfxVDBParticleMesher::findAllFilters(DfxEntityPropertyTypes::UID &uid) {
		using namespace DfxEntityPropertyTypes;

		std::queue<DfxEntity*> objs;

		DfxTraitUIDTraversalActions t_actions;

		t_actions.next_id = [&objs](size_t id) {

			auto dinstance = DfxEntityDB::getDataInstance(id);

			if (dinstance != nullptr) {
				DfxEntity *ent = dinstance->getDfxEntity();

				if (ent->UniqueName() == DfxVDBDilateNode_Identifier ||
					ent->UniqueName() == DfxVDBErodeNode_Identifier ||
					ent->UniqueName() == DfxVDBMedianNode_Identifier ||
					ent->UniqueName() == DfxVDBMeanNode_Identifier ||
					ent->UniqueName() == DfxVDBGaussianNode_Identifier) {

					objs.push(ent);
				}
			}

			return 0;
		};

		t_actions.traverse_children = [](size_t id) {
			return true;
		};

		uid.traverseUIDTree(t_actions);

		return objs;
	}
}

#endif