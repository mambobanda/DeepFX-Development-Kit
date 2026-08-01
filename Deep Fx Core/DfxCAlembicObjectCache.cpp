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

#include "DfxCAlembicObjectCache.h"


namespace dfxcore {

	DfxCAlembicObjectCache::DfxCAlembicObjectCache(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
		setLogicalName("Alembic Object Cache");

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inSequence, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inIndex, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPrefix, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inDirPath, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inFPS, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inStartFrame, DfxEntityPropertyState::INPUT_STATE);

		((DfxEnum*)getProperty(inSequence))->add("Yes");
		((DfxEnum*)getProperty(inSequence))->add("No");
		((DfxEnum*)getProperty(inSequence))->setCurrentIndex(1);

		*(float*)getProperty(inIndex) = 0.0f;

		*(float*)getProperty(inFPS) = 24.f;
		*(float*)getProperty(inStartFrame) = 0.f;

		*(std::string*)getProperty(inDirPath) = "c:/";
		*(std::string*)getProperty(inPrefix) = "default";

		archive = nullptr;
		timeSamplePtr = nullptr;

	}

	DfxCAlembicObjectCache::~DfxCAlembicObjectCache() {

	}

	void DfxCAlembicObjectCache::compute() {
		bool sequence = ((DfxEnum*)getProperty(inSequence))->getCurrentIndex() == 0 ? true : false;

		if (sequence) {
			cacheSequence();
		}
		else {
			if (archive != nullptr) {
				cache();
			}
		}

		cleanAllProperties();
		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void DfxCAlembicObjectCache::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void  DfxCAlembicObjectCache::init() {
		using namespace  Alembic::AbcGeom;

		bool sequence = ((DfxEnum*)getProperty(inSequence))->getCurrentIndex() == 0 ? true : false;
		std::string dir = *(std::string*)getProperty(inDirPath);
		std::string pre = *(std::string*)getProperty(inPrefix);

		float fps = *(float*)getProperty(inFPS);
		float sframe = *(float*)getProperty(inStartFrame);

		if (fps <= 0) fps = 1.f;

		fps = 1.f / fps;

		*(float*)getProperty(inStartFrame) = 0.f;

		if (!sequence) {

			std::string directory = dir;
			std::string fname = pre;
			std::string ftype = ".abc";
			std::stringstream fullpath;

			if (fname.length() == 0)
				fname = "no_name";

			if (ftype.length() == 0)
				ftype = ".abc";

			fullpath << directory << "/" << fname << ftype;
			
			delete archive;

			try {

				archive = new Alembic::AbcGeom::OArchive(Alembic::AbcCoreOgawa::WriteArchive(), fullpath.str());
				timeSamplePtr = new Alembic::AbcGeom::TimeSamplingPtr(new Alembic::AbcGeom::TimeSampling( fps, sframe * fps));

				xform = new Alembic::AbcGeom::OXform(OObject(*archive, kTop), std::string("mesh"), *timeSamplePtr);
				meshobj = new Alembic::AbcGeom::OPolyMesh(*xform, std::string("mesh"), *timeSamplePtr);
			}
			catch (const std::exception &ex) {
				DfxGlobal::console::println(std::string("Alembic Error: ") + ex.what());
				
				archive = nullptr;
				timeSamplePtr = nullptr;
				xform = nullptr;
				meshobj = nullptr;
			}
		}
	}

	void  DfxCAlembicObjectCache::clear() {

	}

	void  DfxCAlembicObjectCache::done() {
		delete archive;
		delete timeSamplePtr;
		delete xform;
		delete meshobj;

		archive = nullptr;
		timeSamplePtr = nullptr;
		xform = nullptr;
		meshobj = nullptr;
	}

	void  DfxCAlembicObjectCache::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>>  DfxCAlembicObjectCache::exportData() {
		return mapEntityProperties(this);
	}

	bool  DfxCAlembicObjectCache::isExportable() {
		return true;
	}

	void *DfxCAlembicObjectCache::creator(DfxGlobalEntityName uniquename) {
		return new DfxCAlembicObjectCache(uniquename);
	}

	void  DfxCAlembicObjectCache::initialize() {
		DfxCSimulationEntity::initialize();

		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inIndex,"in_index", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inSequence, "sequence", new DfxEntityPropertyTypes::DfxEnumPropertyType());
		DfxEntityDB::createProperty(inDirPath, "cache_directory", new DfxEntityPropertyTypes::DirPathPropertyType());
		DfxEntityDB::createProperty(inPrefix, "prefix", new DfxEntityPropertyTypes::StringPropertyType());
		DfxEntityDB::createProperty(inFPS, "FPS", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inStartFrame, "start_frame", new DfxEntityPropertyTypes::FloatPropertyType());
	}

	void DfxCAlembicObjectCache::sampleMeshData(Dfx3dMeshData *mesh, DfxAlembicMeshProxy &ap) {
		using namespace  Alembic::AbcGeom;

		if (mesh->getVertexCount() > 0) {

			ap.g_verts = new Alembic::Abc::float32_t[mesh->getVertexCount() * 3];
			ap.g_normals = new Alembic::Abc::float32_t[mesh->getVertexCount() * 3];
			ap.g_uvs = new  Alembic::Abc::float32_t[mesh->getVertexCount() * 2];
			ap.g_indices = new Alembic::Abc::int32_t[mesh->getIndexCount()];
			ap.g_counts = new Alembic::Abc::int32_t[mesh->getIndexCount() / 3];

			size_t vertCounter = 0;
			size_t uvCounter = 0;

			for (size_t inx = 0; inx < mesh->getIndexCount() / 3; inx++) {
				ap.g_counts[inx] = 3;
			}

			for (size_t inx = 0; inx < mesh->getVertexCount(); inx++) {

				glm::vec3 vert = mesh->getVertex(inx);
				glm::vec3 norm = mesh->getNormal(inx);

				//vert = glm::vec3(trans * glm::vec4(vert * handle->getInitialScale(), 1.f));

				ap.g_verts[vertCounter] = vert.x;
				ap.g_verts[vertCounter + 1] = vert.y;
				ap.g_verts[vertCounter + 2] = vert.z;

				ap.g_normals[vertCounter] = norm.x;
				ap.g_normals[vertCounter + 1] = norm.y;
				ap.g_normals[vertCounter + 2] = norm.z;

				ap.g_uvs[uvCounter] = 0.f;
				ap.g_uvs[uvCounter + 1] = 0.f;

				vertCounter += 3;
				uvCounter += 2;
			}

			for (size_t inx = 0; inx < mesh->getIndexCount(); inx++) {
				ap.g_indices[inx] = mesh->getIndex(inx);
			}

			OV2fGeomParam::Sample uvsamp(V2fArraySample((const V2f *)ap.g_uvs, mesh->getVertexCount()), kFacevaryingScope);

			ON3fGeomParam::Sample nsamp(N3fArraySample((const N3f *)ap.g_normals, mesh->getVertexCount()), kFacevaryingScope);

			OPolyMeshSchema::Sample *mesh_samp = new OPolyMeshSchema::Sample(
				V3fArraySample((const V3f *)ap.g_verts, mesh->getVertexCount()),
				Int32ArraySample(ap.g_indices, mesh->getIndexCount()),
				Int32ArraySample(ap.g_counts, mesh->getIndexCount() / 3),
				uvsamp, nsamp);

			ap.xform = xform;
			ap.mesh = meshobj;
			ap.sample = mesh_samp;
			
		}
		else {

			ap.g_verts = nullptr;
			ap.g_normals = nullptr;
			ap.g_uvs = nullptr;
			ap.g_indices = nullptr;
			ap.g_counts = nullptr;
			ap.mesh = nullptr;
			ap.sample = nullptr;
			ap.xform = nullptr;
		}
	}

	DfxBounds DfxCAlembicObjectCache::calcMeshBounds(Dfx3dMeshData *mdata) {
		DfxBounds itemBounds;
		itemBounds.empty();

		for (size_t inx = 0; inx < mdata->getVertexCount(); inx++) {
			glm::vec3 vert = mdata->getVertex(inx);
			itemBounds.add(vert.x, vert.y, vert.z);
		}

		return itemBounds;
	}

	void DfxCAlembicObjectCache::cacheSequence() {

	}

	void DfxCAlembicObjectCache::clearAlembicProxy(DfxAlembicMeshProxy &prx) {

		delete[] prx.g_verts;
		delete[] prx.g_normals;
		delete[] prx.g_uvs;
		delete[] prx.g_indices;
		delete[] prx.g_counts;
	}

	void DfxCAlembicObjectCache::cache() {

		using namespace Alembic::AbcGeom;

		if (propertyHasBinding(inRef)) {
			using namespace DfxEntityPropertyTypes;

			UID *uid = (UID*)getProperty(inRef);
			Dfx3dMeshData *meshdata = dfxutil::findMeshData(uid);

			if (meshdata != nullptr) {

				DfxAlembicMeshProxy prox;

				sampleMeshData(meshdata, prox);

				if (prox.sample != nullptr) {
					
					DfxBounds bounds = calcMeshBounds(meshdata);

					Box3d childBounds;
					childBounds.makeEmpty();
					childBounds.extendBy(V3d(bounds.xmin, bounds.ymin, bounds.zmin));
					childBounds.extendBy(V3d(bounds.xmax, bounds.ymax, bounds.zmax));

					prox.xform->getSchema().getChildBoundsProperty().set(childBounds);

					Alembic::Abc::M44d mat;
					mat.makeIdentity();

					XformOp transOp(kMatrixOperation);
					XformSample xf_samp;
					xf_samp.addOp(transOp, mat);

					prox.xform->getSchema().set(xf_samp);
					prox.mesh->getSchema().set(*prox.sample);

					clearAlembicProxy(prox);
				}
			}
	
		}
	}
}