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
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include <DfxGlobal.h>
#include <SgGeoAdapterInterface.h>
#include <glm/gtx/string_cast.hpp>
#include "DfxTimelineManager.h"
#include "DfxAlembicIO.h"


namespace DfxAlembicIO {

	void accumulateXform(glm::dmat4 &xform, Alembic::AbcGeom::IXform &t_xform, int sample, int offset) {	
		auto schema = t_xform.getSchema();
		auto value = t_xform.getSchema().getValue().getMatrix();

		sample += offset;
		if ((sample + 1) > schema.getNumSamples()) {
			auto diff = (sample - schema.getNumSamples()) - 1;
			sample = diff + offset;
		}

		if (sample > schema.getNumSamples()) {
			xform *= glm::make_mat4(value.getValue());
		}
		else {
			Alembic::Abc::ISampleSelector selector((Alembic::Abc::index_t)sample); // Select data
			Alembic::AbcGeom::XformSample xform_sample;

			schema.get(xform_sample, selector);

			xform *= glm::make_mat4(xform_sample.getMatrix().getValue());
		}		

	}

	bool loadPolyMesh(Alembic::AbcGeom::IPolyMesh &pmesh, std::string objPath, unsigned int surfid_offset, DfxDynaMesh *mesh, int frame, 
		float sample, float scale, int offset, glm::dmat4 xform, bool loop, bool applyXform) {
		
		// printf("xform: %s\n", glm::to_string(xform).c_str());

		auto schema = pmesh.getSchema(); // Get Polymesh schema
		bool load = true;

		auto fframe = DfxTimelineManager::getInstance()->firstFrame();

		if (schema.getNumSamples() <= 0) return false;
	
		if (loop) {
			if ((sample + 1) > schema.getNumSamples()) {
				int factor = int(sample / schema.getNumSamples());
				int diff = sample - (schema.getNumSamples() * factor);
				sample = diff;
			}
		}

		sample += offset; 
		if ((sample + 1) > schema.getNumSamples()) {
			auto diff = (sample - schema.getNumSamples()) - 1;
			sample = diff + offset;
		}

		if (sample > schema.getNumSamples()) load = false;

		if (load) {

			std::vector<std::string> facesetNames;
			schema.getFaceSetNames(facesetNames);

			Alembic::Abc::ISampleSelector selector((Alembic::Abc::index_t)sample); // Select data
			Alembic::AbcGeom::IPolyMeshSchema::Sample mesh_samp;
			Alembic::AbcGeom::IN3fGeomParam::Sample norm_samp;
			Alembic::AbcGeom::IV2fGeomParam::Sample uv_samp;
			schema.get(mesh_samp, selector); // Retrieve data at frame

			bool hasNormals = false;
			bool hasUVs = false;

			if (schema.getNormalsParam().isIndexed()) {
				hasNormals = true;
			}

			if (schema.getUVsParam().isIndexed()) {
				hasUVs = true;
				uv_samp = schema.getUVsParam().getIndexedValue(selector);
			}

			auto surfid_counter = surfid_offset;
			std::unordered_map<size_t, size_t> faceSurfIDMap;
			if (!objPath.empty()) {
				if (!facesetNames.empty()) {
					DfxSurfaceDictionary::getInstance()->create(objPath);

					for (auto fname : facesetNames) {
						Alembic::AbcGeom::IFaceSetSchema::Sample fset;
						fset = schema.getFaceSet(fname).getSchema().getValue(selector);

						auto surfid = surfid_counter++;
						DfxSurfaceDictionary::getInstance()->insert(objPath, surfid, fname);

						for (int i = 0; i < fset.getFaces()->size(); i++) {
							faceSurfIDMap[(*fset.getFaces())[i]] = surfid;
						}
					}
				}
			}

			// Fetch Vertices, Normals and UVS
			int p_count = mesh_samp.getPositions()->size();
			if (p_count > 0) {
				float *positions = new float[p_count * 3];
				float *normals = new float[p_count * 3];

				for (int i = 0; i < p_count; i++) {

					int counter = i * 3;
					int counter_uv = i * 2;
				
					auto alembic_p = mesh_samp.getPositions()->get()[i];
					glm::dvec3 p = glm::dvec3(alembic_p.x, alembic_p.y, alembic_p.z) ;
					
					if(applyXform) p = glm::dvec3(xform * glm::dvec4(p, 1.0));
					
					p *= double(scale);

					positions[counter] = p.x;
					positions[counter + 1] = p.y;
					positions[counter + 2] = p.z;

					normals[counter] = 0.f;
					normals[counter + 1] = 0.f;
					normals[counter + 2] = 0.f;

				}

				mesh->setVertices(positions);
				mesh->setSmoothNormals(normals);
				mesh->setVertexCount(p_count);

				std::vector<unsigned int> indices;
				std::vector<glm::vec2> uvs;
				std::vector<unsigned int> t_fid;
				std::vector<std::pair<unsigned int, std::pair<std::vector<std::pair<size_t, glm::vec2>>, int32_t>>> ngons;
				int f_count = mesh_samp.getFaceCounts()->size();
				int p_counter = 0;
				for (long int i = 0; i < f_count; i++) {
					auto degrees = mesh_samp.getFaceCounts()->get()[i];

					if (degrees == 3) {
					
						indices.push_back(mesh_samp.getFaceIndices()->get()[p_counter + 2]);
						indices.push_back(mesh_samp.getFaceIndices()->get()[p_counter + 1]);
						indices.push_back(mesh_samp.getFaceIndices()->get()[p_counter]);
		
						if (hasUVs && uv_samp.getIndices()->size() == mesh_samp.getFaceIndices()->size()) {
							
							auto index00 = (*uv_samp.getIndices())[p_counter + 2];
							auto index01 = (*uv_samp.getIndices())[p_counter + 1];
							auto index02 = (*uv_samp.getIndices())[p_counter + 0];
							
							auto uv00 = (*uv_samp.getVals())[index00];
							auto uv01 = (*uv_samp.getVals())[index01];
							auto uv02 = (*uv_samp.getVals())[index02];

							uvs.push_back(glm::vec2(uv00.x, -uv00.y));
							uvs.push_back(glm::vec2(uv01.x, -uv01.y));
							uvs.push_back(glm::vec2(uv02.x, -uv02.y));

							/*printf("\nu00: %f, v00: %f \n", uv00.x, (uv00.y));
							printf("u01: %f, v01: %f \n", uv01.x, (uv01.y));
							printf("u02: %f, v02: %f \n", uv02.x, (uv02.y));*/
						}
						else {
							uvs.push_back(glm::vec2(0.f));
							uvs.push_back(glm::vec2(0.f));
							uvs.push_back(glm::vec2(0.f));
						}
						
						t_fid.push_back(i);
					}
					else if (degrees > 3) {
						std::vector<std::pair<size_t, glm::vec2>> t_indices;

						for (int j = degrees - 1; j >= 0; j--) {
							auto _index = mesh_samp.getFaceIndices()->get()[p_counter + j];
							glm::vec2 tuv;

							if (hasUVs && uv_samp.getIndices()->size() == mesh_samp.getFaceIndices()->size()) {
								auto index = (*uv_samp.getIndices())[p_counter + j];
								auto uv = (*uv_samp.getVals())[index];
								tuv = glm::vec2(uv.x, -uv.y);
							}
							else {
								tuv =  glm::vec2(0.f);
							}

							std::pair<size_t, glm::vec2> tp(_index, tuv);
							t_indices.push_back(tp);
						}

						std::pair<std::vector<std::pair<size_t, glm::vec2>>, int32_t> p(t_indices, degrees);

						ngons.push_back(std::pair<unsigned int, std::pair<std::vector<std::pair<size_t, glm::vec2>>, int32_t>>(i,p));
					}

					p_counter += degrees;
				}

				if (!ngons.empty()) {
					std::vector<std::vector<size_t>> t_indices;
					std::vector<std::vector<glm::vec2>> t_new_uvs;
					std::vector<std::vector<unsigned int>> t_new_fids;
					t_indices.resize(ngons.size());
					t_new_uvs.resize(ngons.size());
					t_new_fids.resize(ngons.size());

					#pragma omp parallel default(shared)
					{
						#pragma omp for schedule(static)
						for (long int i = 0; i < ngons.size(); i++) {
							std::unordered_map<int32_t, glm::vec2> uv_map;
							auto p = ngons[i];
							std::vector<size_t> _inds;

							for (auto val: p.second.first) {
								uv_map[val.first] = val.second;
							}

							for (auto inx : p.second.first) _inds.push_back(inx.first);

							auto t_triangles = deepsg::SgGeoAdapterInterface::polygonToTriangles(positions, p.second.second, _inds, p.first);

							for (auto t : t_triangles) {
								t_indices[i].push_back(t.a);
								t_indices[i].push_back(t.b);
								t_indices[i].push_back(t.c);

								t_new_uvs[i].push_back(uv_map[t.a]);
								t_new_uvs[i].push_back(uv_map[t.b]);
								t_new_uvs[i].push_back(uv_map[t.c]);

								t_new_fids[i].push_back(p.first);
							}
						}
					}

					for (auto t_i : t_indices) {
						for (auto i : t_i) indices.push_back(i);
					}

					for (auto t_uvi : t_new_uvs) {
						for (auto i : t_uvi) uvs.push_back(i);
					}

					for (auto t_fiid: t_new_fids) {
						for (auto i : t_fiid) t_fid.push_back(i);
					}
				}

				int tri_count = indices.size() / 3;
				if (tri_count > 0) {
					unsigned int *_indices = new unsigned int[indices.size()];
					float *_uvs = new float[uvs.size() * 2];
					unsigned int *_fids = new unsigned int[tri_count];
					float *_fnormals = new float[tri_count * 3];

					for (int i = 0; i < indices.size(); i++) {
						auto offset = i * 2;

						_indices[i] = indices[i];

						_uvs[offset] = uvs[i].x;
						_uvs[offset + 1] = uvs[i].y;

					}

					for (int i = 0; i < t_fid.size(); i++) {
						auto offsetNormal = i * 3;

						_fids[i] = faceSurfIDMap[t_fid[i]];

						_fnormals[offsetNormal] = 0.f;
						_fnormals[offsetNormal + 1] = 0.f;
						_fnormals[offsetNormal + 2] = 0.f;
					}
					
					mesh->setIndices(_indices);
					mesh->setUvs(_uvs);
					mesh->setFaceIDs(_fids);
					mesh->setFaceNormals(_fnormals);
					mesh->setTriangleCount(tri_count);
				}

				mesh->buildEdgeList();
				mesh->calcNormals();

				return true;
			}
		}

		return false;
	}

	void shapePathList(std::string path, std::vector<std::string> &shape_paths, Alembic::AbcGeom::IObject *obj) {

		if (obj == nullptr) {
			Alembic::Abc::IArchive archive; // Alembic Archive
			Alembic::AbcCoreFactory::IFactory factory;
			factory.setPolicy(Alembic::Abc::ErrorHandler::kQuietNoopPolicy);
			Alembic::AbcCoreFactory::IFactory::CoreType coreType; // Core Engine Type
			archive = factory.getArchive(std::string(path), coreType);

			if (coreType != Alembic::AbcCoreFactory::IFactory::CoreType::kUnknown) {
				Alembic::AbcGeom::IObject top = archive.getTop(); // Get the root object

				shapePathList("", shape_paths, &top);
			}
		}
		else {

			// Check if Object is polymesh
			if (Alembic::AbcGeom::IPolyMesh::matches(obj->getHeader())) {
				shape_paths.push_back(obj->getFullName());
			}

			for (int i = 0; i < obj->getNumChildren(); i++) {
				auto n = obj->getChild(i);
				shapePathList("", shape_paths, &n);
			}

		}
	}

	bool findFirstPolyMesh(Alembic::AbcGeom::IPolyMesh &pmesh, Alembic::AbcGeom::IObject &obj, glm::dmat4 &xform, float sample, int offset) {

		// Check if Object is polymesh
		if (Alembic::AbcGeom::IPolyMesh::matches(obj.getHeader())) {		
			pmesh = Alembic::AbcGeom::IPolyMesh(obj); // Return polymesh
			return true;
		}

		if (Alembic::AbcGeom::IXform::matches(obj.getHeader())) {

			auto t_xform = Alembic::AbcGeom::IXform(obj);	
			accumulateXform(xform, t_xform, sample, offset);
		
		}

		// Continue searching for polymesh down hiearchy
		for (int i = 0; i < obj.getNumChildren(); i++) {
			auto n = obj.getChild(i);
			return findFirstPolyMesh(pmesh, n, xform, sample, offset);
		}

		return false;
	}

	bool loadFirstPolyMesh(std::string path, std::string objPath, DfxDynaMesh *mesh, float sample, float scale, int offset, bool loop, bool applyXform) {

		Alembic::Abc::IArchive archive; // Alembic Archive
		Alembic::AbcCoreFactory::IFactory factory;
		factory.setPolicy(Alembic::Abc::ErrorHandler::kQuietNoopPolicy);
		Alembic::AbcCoreFactory::IFactory::CoreType coreType; // Core Engine Type
		archive = factory.getArchive(std::string(path), coreType);

		// If archive opened sucessfully continue
		if (coreType != Alembic::AbcCoreFactory::IFactory::CoreType::kUnknown) {
			Alembic::AbcGeom::IObject top = archive.getTop(); // Get the root object
			Alembic::AbcGeom::IPolyMesh pmesh; // Polymesh container

			glm::dmat4 xform(1.f);

			// To Find a polymesh object
			if (findFirstPolyMesh(pmesh, top, xform, sample, offset)) {
				return loadPolyMesh(pmesh, objPath, DfxSurfaceDictionary::ID_LIMIT, mesh, 0, sample, scale, offset, xform, loop, applyXform); // Retrieve data from polymesh object
			}
		}

		return false;
	}

	bool findLoadPolyMesh(std::string path, std::string name, Alembic::AbcGeom::IPolyMesh &pmesh, Alembic::AbcGeom::IObject *obj, glm::dmat4 &xform, bool applyXform,
		std::string objPath, DfxDynaMesh *mesh, float sample, float scale, int offset, bool loop) {

		if (obj == nullptr) {
			Alembic::Abc::IArchive archive; // Alembic Archive
			Alembic::AbcCoreFactory::IFactory factory;
			factory.setPolicy(Alembic::Abc::ErrorHandler::kQuietNoopPolicy);
			Alembic::AbcCoreFactory::IFactory::CoreType coreType; // Core Engine Type
			archive = factory.getArchive(std::string(path), coreType);

			if (coreType != Alembic::AbcCoreFactory::IFactory::CoreType::kUnknown) {
				Alembic::AbcGeom::IObject top = archive.getTop(); // Get the root object
				Alembic::AbcGeom::IPolyMesh _pmesh;

				glm::dmat4 xform(1.f);
				if (findLoadPolyMesh("", name, _pmesh, &top, xform)) {
					return loadPolyMesh(_pmesh, objPath, DfxSurfaceDictionary::ID_LIMIT, mesh, 0, sample, scale, offset, xform, loop, applyXform);
				}
			}
		}
		else {

			// Check if Object is polymesh
			if (Alembic::AbcGeom::IPolyMesh::matches(obj->getHeader())) {
				auto nname = obj->getFullName();
				if (obj->getFullName() == name) {
					pmesh = Alembic::AbcGeom::IPolyMesh(*obj);
					return true;
				}
			}

			if (Alembic::AbcGeom::IXform::matches(obj->getHeader())) {
				auto t_xform = Alembic::AbcGeom::IXform(*obj);
				accumulateXform(xform, t_xform, sample, offset);
			}

			for (int i = 0; i < obj->getNumChildren(); i++) {
				auto n = obj->getChild(i);
				if (findLoadPolyMesh("", name, pmesh, &n, xform)) {
					return true;
				}
			}

		}

		return false;
	}
}