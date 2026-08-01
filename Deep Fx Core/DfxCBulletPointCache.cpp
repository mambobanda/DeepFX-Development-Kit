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

#include <DfxObjectItem.h>
#include <glm\gtx\normal.hpp>
#include <exception>
#include "DfxCBulletPointCache.h"

namespace dfxcore {

	namespace bullet {
		
		DfxCBulletPointCache::DfxCBulletPointCache(DfxGlobalEntityName uniquename): DfxCSimulationEntity(uniquename, DFX_CUSTOM_ITEM) {
			setLogicalName("Bullet Point Cache");

			DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outID);
			uid->setRootID(getInstanceID());

			setPropertyState(outID, DfxEntityPropertyState::OUTPUT_STATE);
			setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inBuffer, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inType, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inSequence, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inIndex, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inPrefix, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inDirPath, DfxEntityPropertyState::INPUT_STATE);

			((DfxEnum*)getProperty(inType))->add("Memory");
			((DfxEnum*)getProperty(inType))->add("Disk");

			((DfxEnum*)getProperty(inSequence))->add("Yes");
			((DfxEnum*)getProperty(inSequence))->add("No");
			((DfxEnum*)getProperty(inSequence))->setCurrentIndex(1);

			*(float*)getProperty(inIndex) = 0.0f;
			*(float*)getProperty(inBuffer) = 0.0f;

			*(std::string*)getProperty(inDirPath) = "c:/";
			*(std::string*)getProperty(inPrefix)  = "default";

			archive = nullptr;
			timeSamplePtr = nullptr;

		}

		DfxCBulletPointCache::~DfxCBulletPointCache() {

			if (archive != nullptr) {
				delete archive;
			}
		}

		void DfxCBulletPointCache::compute() {
			
			int inx = ((DfxEnum*)getProperty(inType))->getCurrentIndex();

			if (inx == 0) {
				cacheCurrentIndexToMemory();
			}
			else {
				cacheCurrentIndexToDisc();
			}

			cleanAllProperties();
			getDataInstance()->makeClean();
			makeChildrenDirty();
		}

		void DfxCBulletPointCache::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void DfxCBulletPointCache::init() {
			using namespace DfxEntityPropertyTypes;

			int inx = ((DfxEnum*)getProperty(inType))->getCurrentIndex();
			bool toDisk = ((DfxEnum*)getProperty(inType))->getCurrentIndex() == 1 ? true : false;
			bool sequence = ((DfxEnum*)getProperty(inSequence))->getCurrentIndex() == 0 ? true : false;
			std::string dir = *(std::string*)getProperty(inDirPath);
			std::string pre = *(std::string*)getProperty(inPrefix);

			UID *uid = (UID*)getProperty(outID);
			uid->clearChildren();

			if (propertyHasBinding(inRef)) {
				uid->addChild((UID*)getProperty(inRef));
			}

			if (!sequence && toDisk) {
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
					archive = new Alembic::AbcGeom::OArchive( Alembic::AbcCoreOgawa::WriteArchive(), fullpath.str());
					timeSamplePtr = new Alembic::AbcGeom::TimeSamplingPtr( new Alembic::AbcGeom::TimeSampling(1.0 / 24.0, 0.0));
				}
				catch (const std::exception &ex) {
					DfxGlobal::console::println(std::string("Alembic Error: ") + ex.what());
					archive = nullptr;
				}
			}
			

		}

		void DfxCBulletPointCache::clear() {
			
		}

		void DfxCBulletPointCache::done() {
			delete archive;
			delete timeSamplePtr;

			archive = nullptr;
			timeSamplePtr = nullptr;

			clearObjects();
		}

		void DfxCBulletPointCache::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> DfxCBulletPointCache::exportData() {
			return mapEntityProperties(this);
		}

		bool DfxCBulletPointCache::isExportable() {
			return true;
		}

		void *DfxCBulletPointCache::creator(DfxGlobalEntityName uniquename) {
			return new DfxCBulletPointCache(uniquename);
		}

		void DfxCBulletPointCache::draw() {
			using namespace DfxEntityPropertyTypes;

			initializeOpenGLFunctions();

			UID *uid = (UID*)getProperty(inRef);
			int index = *(float*)getProperty(inIndex);

			auto sims = findSimulationEntities(uid);

			for (auto &sim : sims) {

				if (sim->UniqueName() == DfxCBulletRigidObject_Identifier) {
					DfxCBulletRigidObject *robj = (DfxCBulletRigidObject*)sim;

					for (auto handle : robj->getRigidHandles()) {

						Dfx3dMeshData *mesh = handle->getMeshDataPtr();
						BulletCacheObject *cache = nullptr;

						auto it = cacheMap.find(mesh->getID());

						if (it != cacheMap.end()) {

							SimulationFrame *frame = it->second->cache.getFrame(index);
							if (frame == nullptr) frame = it->second->cache.getFrame(index - 1);

							if (frame != nullptr) {
								if (frame->isCached()) {
									if (mesh->getVertexCount() > 0) {

										glm::vec3 selColor(1.f, 0.498f, 0.164);

										glBegin(GL_TRIANGLES);
										size_t counter = 0;
										for (size_t inx = 0; inx < mesh->getIndexCount() / 3; inx++) {

											auto vindex1 = mesh->getIndex(counter);
											auto vindex2 = mesh->getIndex(counter + 1);
											auto vindex3 = mesh->getIndex(counter + 2);

											glm::vec3 pos1;
											glm::vec3 pos2;
											glm::vec3 pos3;

											glm::vec3 col1;
											glm::vec3 col2;
											glm::vec3 col3;

											pos1 = frame->getParticles()[vindex1];;
											pos2 = frame->getParticles()[vindex2];;
											pos3 = frame->getParticles()[vindex3];;

											
											col1 = mesh->getColor(vindex1);
											col2 = mesh->getColor(vindex2);
											col3 = mesh->getColor(vindex3);

											glm::vec3 nom = glm::triangleNormal(pos1, pos2, pos3);

											glNormal3f(nom.x, nom.y, nom.z);

											glColor3f(col1.x, col1.y, col1.z);
											glVertex3f(pos1.x, pos1.y, pos1.z);

											glColor3f(col2.x, col2.y, col2.z);
											glVertex3f(pos2.x, pos2.y, pos2.z);

											glColor3f(col3.x, col3.y, col3.z);
											glVertex3f(pos3.x, pos3.y, pos3.z);

											counter += 3;
										}
										glEnd();
									}
								}
							}
						}
					}
				}
			}
		}

		void DfxCBulletPointCache::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outID, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inIndex, "in_index", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inBuffer, "in_buffer", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inType, "type", new DfxEntityPropertyTypes::DfxEnumPropertyType());
			DfxEntityDB::createProperty(inSequence, "sequence", new DfxEntityPropertyTypes::DfxEnumPropertyType());
			DfxEntityDB::createProperty(inDirPath, "cache_directory", new DfxEntityPropertyTypes::DirPathPropertyType());
			DfxEntityDB::createProperty(inPrefix,"prefix", new DfxEntityPropertyTypes::StringPropertyType());
		}
		
		std::vector<size_t> DfxCBulletPointCache::findRigidObjects() {
			using namespace DfxEntityPropertyTypes;
			
			std::vector<size_t> objs;

			if (propertyHasBinding(inRef)) {

				UID *uid = (UID*)getProperty(inRef);

				DfxTraitUIDTraversalActions t_actions;

				t_actions.next_id = [&objs](size_t id) {

					auto dinstance = DfxEntityDB::getDataInstance(id);

					if (dinstance != nullptr) {
						DfxEntity *ent = dinstance->getDfxEntity();

						if (ent->UniqueName() == DfxCBulletRigidObject_Identifier) {
							objs.push_back(id);
						}
					}
					return 0;
				};

				t_actions.traverse_children = [](size_t id) {

					auto dinstance = DfxEntityDB::getDataInstance(id);

					if (dinstance != nullptr) {
						DfxEntity *ent = dinstance->getDfxEntity();

						if (ent->UniqueName() == DfxCBulletRigidObject_Identifier) {
							return false;
						}
					}

					return true;
				};

				uid->traverseUIDTree(t_actions);
			}

			return objs;
		}
	
		void DfxCBulletPointCache::cacheCurrentIndexToMemory() {
			using namespace DfxEntityPropertyTypes;

			UID *uid = (UID*)getProperty(inRef);
			int index = *(float*)getProperty(inIndex);

			auto sims = findSimulationEntities(uid);

			for (auto &sim: sims) {

				if (sim->UniqueName() == DfxCBulletRigidObject_Identifier) {
					DfxCBulletRigidObject *robj = (DfxCBulletRigidObject*)sim;
					
					for (auto &handle : robj->getRigidHandles()) {

						BulletCacheObject *cache = nullptr;
						
						Dfx3dMeshData *mesh = handle->getMeshDataPtr();
						auto it = cacheMap.find(mesh->getID());

						if (it == cacheMap.end()) {
							cache = new BulletCacheObject();
							std::pair<size_t, BulletCacheObject*> p(mesh->getID(), cache);

							cacheMap.insert(p);
						}
						else {
							cache = it->second;
						}
								
						if (mesh->getVertexCount() > 0) {

							SimulationFrame *frame = new SimulationFrame();
							frame->initFrame(mesh->getVertexCount());
								
							glm::mat4 trans = handle->getBulletTransformNoScale();

							for (size_t inx = 0; inx < mesh->getVertexCount(); inx++) {

								glm::vec3 vert = mesh->getVertex(inx);

								vert = glm::vec3(trans * glm::vec4(vert * handle->getInitialScale(), 1.f));

								frame->getParticles()[inx] = vert;

							}

							frame->setIsCached(true);
							cache->cache.insertFrame(index, frame);
						}
						
					}
				}
			}
		}

		Alembic::AbcGeom::OPolyMeshSchema::Sample *DfxCBulletPointCache::sampleMeshData(Dfx3dMeshData *mesh, DfxAlembicMeshProxy &ap) {
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

				OV2fGeomParam::Sample uvsamp( V2fArraySample((const V2f *)ap.g_uvs, mesh->getVertexCount()), kFacevaryingScope);

				ON3fGeomParam::Sample nsamp( N3fArraySample((const N3f *)ap.g_normals, mesh->getVertexCount()), kFacevaryingScope);

				OPolyMeshSchema::Sample *mesh_samp = new OPolyMeshSchema::Sample(
					V3fArraySample((const V3f *)ap.g_verts, mesh->getVertexCount()),
					Int32ArraySample(ap.g_indices, mesh->getIndexCount()),
					Int32ArraySample(ap.g_counts, mesh->getIndexCount() / 3),
					uvsamp, nsamp);

				return mesh_samp;
			}
			else {
				return nullptr;

				ap.g_verts = nullptr;
				ap.g_normals = nullptr;
				ap.g_uvs = nullptr;
				ap.g_indices = nullptr;
				ap.g_counts = nullptr;
			}
		}

		DfxBounds DfxCBulletPointCache::calcMeshBounds(Dfx3dMeshData *mdata) {
			DfxBounds itemBounds;
			itemBounds.empty();

			for (size_t inx = 0; inx < mdata->getVertexCount(); inx++) {
				glm::vec3 vert = mdata->getVertex(inx);
				itemBounds.add(vert.x, vert.y, vert.z);
			}

			return itemBounds;
		}

		void DfxCBulletPointCache::cacheCurrentIndexToDisc() {

			using namespace DfxEntityPropertyTypes;

			if (archive == nullptr) return;

			UID *uid = (UID*)getProperty(inRef);
			int index = *(float*)getProperty(inIndex);
			bool sequence = ((DfxEnum*)getProperty(inSequence))->getCurrentIndex() == 0 ? true : false;

			auto sims = findSimulationEntities(uid);

			for (auto &sim : sims) {

				if (sim->UniqueName() == DfxCBulletRigidObject_Identifier) {
					DfxCBulletRigidObject *robj = (DfxCBulletRigidObject*)sim;

					if (robj->getRigidHandles().size() > 0) {

						for (auto &handle : robj->getRigidHandles()) {
							using namespace  Alembic::AbcGeom;

							bool sampleMesh = false;

							Alembic::AbcGeom::OPolyMesh *meshobj = nullptr;
							Alembic::AbcGeom::OXform *xform = nullptr;
							OPolyMeshSchema::Sample *mesh_samp = nullptr;

							Dfx3dMeshData *mesh = handle->getMeshDataPtr();
							auto it = alembicPolyMeshMap.find(mesh->getID());

							if (it == alembicPolyMeshMap.end()) {

								xform = new Alembic::AbcGeom::OXform(OObject(*archive, kTop), std::string("mesh_") + std::to_string(mesh->getID()), *timeSamplePtr);
								
								meshobj = new Alembic::AbcGeom::OPolyMesh(*xform, std::string("mesh_") + std::to_string(mesh->getID()), *timeSamplePtr);


								DfxAlembicMeshProxy alMObj;
								alMObj.mesh = meshobj;
								alMObj.xform = xform;
								mesh_samp = sampleMeshData(mesh, alMObj);
								alMObj.sample = mesh_samp;

								std::pair<size_t, DfxAlembicMeshProxy> p(mesh->getID(), alMObj);

								alembicPolyMeshMap.insert(p);

								DfxBounds bounds = calcMeshBounds(mesh);

								Box3d childBounds;
								childBounds.makeEmpty();
								childBounds.extendBy(V3d(bounds.xmin, bounds.ymin, bounds.zmin));
								childBounds.extendBy(V3d(bounds.xmax, bounds.ymax, bounds.zmax));

								xform->getSchema().getChildBoundsProperty().set(childBounds);

								sampleMesh = true;
							}
							else {
								meshobj = it->second.mesh;
								xform = it->second.xform;
								mesh_samp = it->second.sample;
							}


							if (mesh_samp != nullptr) {
								glm::mat4 trans = handle->getBulletTransform();

								const float *mt = glm::value_ptr(trans);
								Alembic::Abc::M44d mat(
									mt[0], mt[1], mt[2], mt[3],
									mt[4], mt[5], mt[6], mt[7],
									mt[8], mt[9], mt[10], mt[11],
									mt[12], mt[13], mt[14], mt[15]);

								XformOp transOp(kMatrixOperation);
								XformSample xf_samp;
								xf_samp.addOp(transOp, mat);

								xform->getSchema().set(xf_samp);
								meshobj->getSchema().set(*mesh_samp);

								
							}
						}
					}
				}
			}
		}

		void DfxCBulletPointCache::clearObjects() {

			for (auto &ch : cacheMap) {
				delete ch.second;
			}

			for (auto &ch : alembicPolyMeshMap) {
				delete ch.second.mesh;
				delete ch.second.xform;

				delete[] ch.second.g_verts;
				delete[] ch.second.g_normals;
				delete[] ch.second.g_uvs;
				delete[] ch.second.g_indices;
				delete[] ch.second.g_counts;
			}

			cacheMap.clear();
			alembicPolyMeshMap.clear();
		}
	}
}