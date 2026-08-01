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


#include <DsVert.h>
#include <DsMeshBuilder.h>
#include <DsMeshOperator.h>
#include <DsTools.h>
#include <iostream>
#include "VH5DsMeshResource.h"
#include "VH5Attribute.h"
#include "VH5Buffer.h"

namespace xvault {

	namespace h5 {

		h5DsMeshResource::h5DsMeshResource(std::string nme) {

			_name = nme;

			auto _contentAttribute = h5Attribute::newValueString("__content__", type::STRING, "geometry");
			auto _engineAttribute = h5Attribute::newValueString("__engine__", type::STRING, "deep_simplex");
	
			addAttribute(_contentAttribute);
			addAttribute(_engineAttribute);

		}

		h5DsMeshResource::~h5DsMeshResource() {
		}

		void h5DsMeshResource::setMesh(ds::Mesh *msh) {

			msh->reindexVertices();
			msh->reindexFaces();

			size_t vcount = msh->vertices().size();
			size_t fcount = msh->faces().size();
			size_t icount = 0;

			for (auto f : msh->faces()) icount += f->verticesCount();
			
			auto vbuffer = new h5Buffer();
			vbuffer->setName("base.vertex.__$positions$__");
			vbuffer->setType(type::FLOAT);
			vbuffer->setBufferSize(vcount * 3);

			auto vnormBuffer = new h5Buffer();
			vnormBuffer->setName("base.vertex.__$vnormals$__");
			vnormBuffer->setType(type::FLOAT);
			vnormBuffer->setBufferSize(vcount * 3);

			auto dbuffer = new h5Buffer();
			dbuffer->setName("base.face.__$degrees$__");
			dbuffer->setType(type::INT);
			dbuffer->setBufferSize(fcount);

			auto fnormBuffer = new h5Buffer();
			fnormBuffer->setName("base.face.__$fnormals$__");
			fnormBuffer->setType(type::FLOAT);
			fnormBuffer->setBufferSize(fcount * 3);

			auto fsurfidBuffer = new h5Buffer();
			fsurfidBuffer->setName("base.face.__$fsurfaceids$__");
			fsurfidBuffer->setType(type::UNSIGNED_LONG_LONG);
			fsurfidBuffer->setBufferSize(fcount);

			auto ibuffer = new h5Buffer();
			ibuffer->setName("base.face.__$indices$__");
			ibuffer->setType(type::UNSIGNED_LONG_LONG);
			ibuffer->setBufferSize(icount);

			auto vuvBuffer = new h5Buffer();
			vuvBuffer->setName("base.vertex.__$vuvs$__");
			vuvBuffer->setType(type::FLOAT);
			vuvBuffer->setBufferSize(icount * 2);

			auto heap_vbuffer = new float[vcount * 3];
			auto heap_vnormalBuffer = new float[vcount * 3];
			auto heap_dbuffer = new int[fcount];
			auto heap_fnormalsBuffer = new float[fcount * 3];
			auto heap_fsurfaceIdsBuffer = new unsigned long long[fcount];
			auto heap_ibuffer = new unsigned long long[icount];
			auto heap_vuvsBuffer = new float[icount * 2];

			size_t vcounter = 0;
			size_t uvcounter = 0;
			for (auto v : msh->vertices()) {
				
				glm::vec3 pos  = v->position();
				glm::vec3 norm = v->getProperty<glm::vec3>("smooth_normals");
				glm::vec2 uv   = v->getProperty<glm::vec2>("uvs");

				heap_vbuffer[vcounter] = pos.x;
				heap_vbuffer[vcounter + 1] = pos.y;
				heap_vbuffer[vcounter + 2] = pos.z;

				heap_vnormalBuffer[vcounter] = norm.x;
				heap_vnormalBuffer[vcounter + 1] = norm.y;
				heap_vnormalBuffer[vcounter + 2] = norm.z;

				vcounter += 3;
				uvcounter += 2;
			}

			size_t dcounter = 0;
			size_t icounter = 0;
			size_t fnormCounter = 0;
			for (auto f : msh->faces()) {

				glm::vec3 norm = f->getProperty<glm::vec3>("face_normals");
				unsigned long long int surfid = f->getSurfID();

				heap_dbuffer[dcounter] = f->verticesCount();

				heap_fnormalsBuffer[fnormCounter] = norm.x;
				heap_fnormalsBuffer[fnormCounter + 1] = norm.y;
				heap_fnormalsBuffer[fnormCounter + 2] = norm.z;

				heap_fsurfaceIdsBuffer[dcounter] = surfid;

				for (int i = 0; i < f->verticesCount(); i++) {
					auto uvOffset = icounter * 2;

					heap_ibuffer[icounter] = (unsigned long long)f->vertices()[i]->getIndex();
					
					heap_vuvsBuffer[uvOffset] = f->uvs()[i].x;
					heap_vuvsBuffer[uvOffset + 1] = f->uvs()[i].y;
					
					icounter++;
				}

				dcounter++;
				fnormCounter += 3;
			}

			vbuffer->setBufferFloat(heap_vbuffer);
			vnormBuffer->setBufferFloat(heap_vnormalBuffer);
			vuvBuffer->setBufferFloat(heap_vuvsBuffer);
			dbuffer->setBufferInt(heap_dbuffer);
			fnormBuffer->setBufferFloat(heap_fnormalsBuffer);
			fsurfidBuffer->setBufferULongLong(heap_fsurfaceIdsBuffer);
			ibuffer->setBufferULongLong(heap_ibuffer);

			addBuffer(vbuffer);
			addBuffer(vnormBuffer);
			addBuffer(vuvBuffer);
			addBuffer(dbuffer);
			addBuffer(fnormBuffer);
			addBuffer(fsurfidBuffer);
			addBuffer(ibuffer);
		}

		void h5DsMeshResource::setResourceID(size_t uid) {
			auto _idAttribute = h5Attribute::newValueULongLong("__resource_id__", type::UNSIGNED_LONG_LONG, (unsigned long long)uid);
			addAttribute(_idAttribute);
		}

		void h5DsMeshResource::setName(std::string name) {
			auto _nameAttribute = h5Attribute::newValueString("__name__", type::STRING, name);
			addAttribute(_nameAttribute);
		}

		ds::Mesh *h5DsMeshResource::toDsMesh() {
			return toDsMesh(this);
		}

		ds::Mesh *h5DsMeshResource::toDsMesh(iResource *resource) {
			
			ds::MeshBuilder meshBuilder;
			ds::Mesh *mesh = nullptr;

			bool hasVNormals = false;
			bool hasUVs		 = false;
			bool hasFNormals = false;
			bool hasSurfIds  = false;

			auto content = resource->findAttribute("__content__");
			auto engine  = resource->findAttribute("__engine__");

			auto vertBuffer = resource->findBuffer("base.vertex.__$positions$__");
			auto degBuffer = resource->findBuffer("base.face.__$degrees$__");
			auto indexBuffer = resource->findBuffer("base.face.__$indices$__");

			auto vertNormalBuffer = resource->findBuffer("base.vertex.__$vnormals$__");
			auto vertuvsBuffer = resource->findBuffer("base.vertex.__$vuvs$__");
			
			auto faceNormalBuffer = resource->findBuffer("base.face.__$fnormals$__");
			auto faceSurfaceIdBuffer = resource->findBuffer("base.face.__$fsurfaceids$__");

			if (content == nullptr || engine == nullptr) return nullptr;

			if (vertBuffer == nullptr || degBuffer == nullptr || indexBuffer == nullptr) return nullptr;

			if (vertNormalBuffer != nullptr) hasVNormals = true;
			if (vertuvsBuffer	 != nullptr) hasUVs = true;
			if (faceNormalBuffer != nullptr) hasFNormals = true;
			if (faceSurfaceIdBuffer != nullptr) hasSurfIds = true;

			if (content->valueString() != "geometry") return nullptr;
			if (engine->valueString()  != "deep_simplex") return nullptr;

			mesh = dstools::newStandardMesh();
			meshBuilder.setMesh(mesh);

			std::vector<glm::vec3> verts; verts.resize(vertBuffer->bufferSize() / 3);
			std::vector<glm::vec3> vnorms; if (hasVNormals) vnorms.resize(vertBuffer->bufferSize() / 3);

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (long long int i = 0; i < vertBuffer->bufferSize() / 3; i++) {
					size_t offset = i * 3;
					size_t uv_offset = i * 2;

					glm::vec3 v(
						vertBuffer->bufferFloat()[offset],
						vertBuffer->bufferFloat()[offset + 1],
						vertBuffer->bufferFloat()[offset + 2]);

					verts[i] = v;

					if (hasVNormals) {
						glm::vec3 nr(
							vertNormalBuffer->bufferFloat()[offset],
							vertNormalBuffer->bufferFloat()[offset + 1],
							vertNormalBuffer->bufferFloat()[offset + 2]);

						vnorms[i] = nr;
					}
				}
			}

			meshBuilder.addVertices(verts);
			verts.clear();

			size_t index_count = 0;
			std::vector<glm::vec3> fnorms;
			std::vector<unsigned long long> surfids;
			for (size_t j = 0; j < degBuffer->bufferSize(); j++) {
				std::vector<size_t> indices;
				std::vector<glm::vec2> uvs;
				size_t offset = j * 3;

				for (int k = 0; k < degBuffer->bufferInt()[j]; k++) {
					auto uvOffset = index_count * 2;
					
					indices.push_back(indexBuffer->bufferULongLong()[index_count]);

					auto _u = vertuvsBuffer->bufferFloat()[uvOffset];
					auto _v = vertuvsBuffer->bufferFloat()[uvOffset + 1];
					
					uvs.push_back(glm::vec2(_u, _v));

					index_count++;
				}

				if (faceNormalBuffer) {
					glm::vec3 nr(
						faceNormalBuffer->bufferFloat()[offset],
						faceNormalBuffer->bufferFloat()[offset + 1],
						faceNormalBuffer->bufferFloat()[offset + 2]);

					fnorms.push_back(nr);
				}

				if (faceSurfaceIdBuffer) {
					unsigned long long sid = faceSurfaceIdBuffer->bufferULongLong()[j];
					surfids.push_back(sid);
				}

				auto face = meshBuilder.addFace(indices);

				for (int k = 0; k < face->verticesCount(); k++) {
					face->uvs()[k] = uvs[k];
				}
			}

			meshBuilder.getMesh()->reindexVertices();
			meshBuilder.getMesh()->reindexEdges();
			meshBuilder.getMesh()->reindexFaces();

			ds::MeshOperator mop(mesh);

			// Assign Surface Ids
			if (hasSurfIds) {
				for (auto f : mesh->faces()) {
					f->setSurfID(surfids[f->getIndex()]);
				}

				surfids.clear();
			}		

		
			// Assign or calculate vertex normals
			if (hasVNormals) {
				for (auto v : mesh->vertices()) {
					v->setProperty<glm::vec3>("smooth_normals", vnorms[v->getIndex()]);
				}

				vnorms.clear();
			}
			else {
				mop.calcSmoothNormals("smooth_normals");
			}

			// Assign or calculate face normals
			if (hasFNormals) {
				for (auto f : mesh->faces()) {
					f->setProperty<glm::vec3>("face_normals", fnorms[f->getIndex()]);
				}
			}
			else {
				mop.calcFaceNormals("face_normals");
			}

			return mesh;
		}
	}
}