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

#include "DfxVDBGlobal.h"

#ifdef USE_OPENVDB_LIB 

#include <DfxVDBNodes.h>

#undef foreach
#include <openvdb/util/Util.h>
#include <openvdb/tools/GridTransformer.h>
#include <openvdb/tools/MeshToVolume.h>
#include <openvdb/tools/VolumeToMesh.h>
#include <openvdb/tools/ParticlesToLevelSet.h>
#include <openvdb/tools/Composite.h>
#include <openvdb/tools/LevelSetFilter.h>

namespace DfxNodes {

	//Utility functions
	openvdb::FloatGrid::Ptr *toGridPointer(void *dat) {
		return (openvdb::FloatGrid::Ptr*)dat;
	}

	unsigned int VDBMeshToVolume::VDB_TempMesh::polygonCount() {
		return polys.size();
	}

	VDBMeshToVolume::VDBMeshToVolume(DfxGlobalEntityName uniquename):DfxEntity(uniquename) {
		setLogicalName("VDB Mesh To Volume");

		((DfxTraitUID<size_t>*)getProperty(outRef))->setRootID(getInstanceID());

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inVoxelSize, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inNarrowBand, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPosition, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inRotation, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inScale, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);

		setPropertyState(grid, DfxEntityPropertyState::DATA_STORE_STATE);

		*(float*)getProperty(inVoxelSize) = 0.2f;
		*(float*)getProperty(inNarrowBand) = 3.0f;

		((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->x = 1.0f;
		((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->y = 1.0f;
		((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->z = 1.0f;

		VDBInitialize();
	}

	VDBMeshToVolume::~VDBMeshToVolume() {

		if(!((DfxAny*)getProperty(grid))->isNull())
			((DfxAny*)getProperty(grid))->deleteData< openvdb::FloatGrid::Ptr>();
	}

	void VDBMeshToVolume::compute() {
		using namespace DfxEntityPropertyTypes;

		((DfxAny*)getProperty(grid))->deleteData< openvdb::FloatGrid::Ptr>();

		if (propertyHasBinding(inRef)) {

			auto in_uid = (DfxTraitUID<size_t>*)getProperty(inRef);
			Dfx3dMeshData *mdata = dfxutil::findMeshData(in_uid);

			if (mdata != nullptr) {
				if (mdata->getVertexCount() > 0) {

					float vsize = *(float*)getProperty(inVoxelSize);
					float nband = *(float*)getProperty(inNarrowBand);

					DfxAny *gridPtr = (DfxAny*)getProperty(grid);

					Float3 *_pos = (Float3*)getProperty(inPosition);
					glm::vec3 pos(_pos->x, _pos->y, _pos->z);

					Float3 *_rot = (Float3*)getProperty(inRotation);
					glm::vec3 rot(_rot->x, _rot->y, _rot->z);

					Float3 *_scl = (Float3*)getProperty(inScale);
					glm::vec3 scl(_scl->x, _scl->y, _scl->z);

					glm::mat4 transMatrix = glm::mat4(1.0f);

					transMatrix = glm::translate(transMatrix, pos);

					transMatrix *= glm::rotate(glm::mat4(), glm::radians(rot.x), glm::vec3(1.f, 0.0, 0.0));
					transMatrix *= glm::rotate(glm::mat4(), glm::radians(rot.y), glm::vec3(0.f, 1.0, 0.0));
					transMatrix *= glm::rotate(glm::mat4(), glm::radians(rot.z), glm::vec3(0.f, 0.0, 1.0));

					openvdb::math::Transform::Ptr xform = openvdb::math::Transform::createLinearTransform(vsize);

					auto sample = sample3DMeshData(mdata, *xform, transMatrix, scl);

					openvdb::FloatGrid::Ptr *_ngrid = new  openvdb::FloatGrid::Ptr();

					openvdb::tools::QuadAndTriangleDataAdapter<openvdb::Vec3s, openvdb::Vec4I> tmsh(sample.points, sample.polys);

					*_ngrid = openvdb::tools::meshToVolume<openvdb::FloatGrid>(tmsh, *xform, nband, nband);
					gridPtr->setData(_ngrid);

				}
			}
		}

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void VDBMeshToVolume::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void VDBMeshToVolume::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> VDBMeshToVolume::exportData() {
		return mapEntityProperties(this);
	}

	bool VDBMeshToVolume::isExportable() {
		return true;
	}

	void *VDBMeshToVolume::creator(DfxGlobalEntityName uniquename) {
		return new VDBMeshToVolume(uniquename);
	}

	void VDBMeshToVolume::initialize() {

		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType);
		DfxEntityDB::createProperty(inVoxelSize, "voxel_size", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inNarrowBand, "narrow_band", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inPosition, "in_position", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inRotation, "in_rotation", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inScale, "in_scale", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(grid, "@vdb_grid", new DfxEntityPropertyTypes::AnyPropertyType);
	}

	VDBMeshToVolume::VDB_TempMesh  VDBMeshToVolume::sample3DMeshData(Dfx3dMeshData *mdata, openvdb::math::Transform xform, glm::mat4 transMatrix, glm::vec3 scale) {
		VDB_TempMesh tm;

		for (int i = 0; i < mdata->getVertexCount(); i++) {
			glm::vec3 scaledPos = mdata->getVertex(i) * scale;
			glm::vec3 transPos = glm::vec3(transMatrix * glm::vec4(scaledPos, 1.0f));
			openvdb::Vec3s pos = xform.worldToIndex(openvdb::Vec3s(transPos.x,
				transPos.y,
				transPos.z));
			tm.points.push_back(pos);
		}

		int counter = 0;

		if (mdata->getIndexCount() / 3 > 0) {
			for (int i = 0; i < mdata->getIndexCount() / 3; i++) {
				tm.polys.push_back(openvdb::Vec4I(mdata->getIndex(counter),
					mdata->getIndex(counter + 1),
					mdata->getIndex(counter + 2),
					openvdb::util::INVALID_IDX));

				counter += 3;
			}
		}

		return tm;
	}


	VDBVolumeToMesh::VDBVolumeToMesh(DfxGlobalEntityName uniquename): DfxEntity(uniquename) {
		setLogicalName("VDB Volume To Mesh");

		((DfxTraitUID<size_t>*)getProperty(outRef))->setRootID(getInstanceID());

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		//setPropertyState(inAdaptivity, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inIso, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);

		setPropertyState(mesh, DfxEntityPropertyState::DATA_STORE_STATE);

		*(float*)getProperty(inAdaptivity) = 0.2f;
		*(float*)getProperty(inIso) = 0.0f;

		VDBInitialize();
	}

	VDBVolumeToMesh::~VDBVolumeToMesh() {

	}

	void VDBVolumeToMesh::compute() {

		Dfx3dMeshData *mdata = (Dfx3dMeshData*)getProperty(mesh);
		mdata->clearIndexBuffer();
		mdata->clearVertexBuffer();


		if (propertyHasBinding(inRef)) {
			
			auto in_uid = (DfxTraitUID<size_t>*)getProperty(inRef);
			DfxAny *anyGridPtr = VDBUtility::findGridData(in_uid);

			float adaptivity = *(float*)getProperty(inAdaptivity);
			float iso = *(float*)getProperty(inIso);

			if (anyGridPtr != nullptr) {

				if (!anyGridPtr->isNull()) {
					openvdb::FloatGrid::Ptr *gridPtr = toGridPointer(anyGridPtr->getData());

					if ((*gridPtr)->activeVoxelCount() > 0) {

						std::vector<openvdb::Vec3s> points;
						std::vector<openvdb::Vec3I> triangles;
						std::vector<openvdb::Vec4I> quads;

						openvdb::tools::volumeToMesh(*(*gridPtr), points, quads, iso);
						//openvdb::tools::volumeToMesh(*(*gridPtr), points, triangles, quads, iso, adaptivity);

						if (triangles.size() > 0 || quads.size() > 0) {

							Mesh *mesh = new Mesh;

							std::vector<openvdb::Vec3s> points;
							std::vector<openvdb::Vec3I> triangles;
							std::vector<openvdb::Vec4I> quads;

							openvdb::tools::volumeToMesh(*(*gridPtr), points, quads, iso);
							//openvdb::tools::volumeToMesh(*grid, points, triangles, quads, 0.f, 0.2f);

							mesh->indices = new unsigned int[quads.size() * 4];
							mesh->verts = new glm::vec3[points.size()];
							mesh->normals = new glm::vec3[points.size()];
							mesh->nbFaces = quads.size();
							mesh->nbVerts = points.size();

							int vindex = 0;
							int iindex = 0;

							for (int j = 0; j < mesh->nbVerts; j++) {
								mesh->verts[j] = glm::vec3(points[j].x(),
									points[j].y(),
									points[j].z());
							}

							for (int j = 0; j < mesh->nbFaces; j++) {
								mesh->indices[iindex] = quads[j].x();
								mesh->indices[iindex + 1] = quads[j].y();
								mesh->indices[iindex + 2] = quads[j].z();
								mesh->indices[iindex + 3] = quads[j].w();

								iindex += 4;
							}

							// create gradient op
							openvdb::math::Transform::Ptr transform = (*gridPtr)->transformPtr();
							openvdb::FloatGrid::Accessor accessor = (*gridPtr)->getAccessor();
							openvdb::math::Gradient<openvdb::math::UniformScaleMap, openvdb::math::CD_2ND> gradientOp;
							openvdb::math::UniformScaleMap::Ptr gridMap = transform->map<openvdb::math::UniformScaleMap>();
							openvdb::Coord ijk;
							openvdb::Vec3f grad;
							openvdb::Vec3d indexPos;

							for (int i = 0; i < mesh->nbVerts; i++) {
								//ijk = transform->worldToIndexCellCentered(points[v]);
								ijk = transform->worldToIndexNodeCentered(points[i]);
								grad = gradientOp.result<openvdb::FloatGrid::Accessor>(*gridMap, accessor, ijk);
								mesh->normals[i].x = grad[0];
								mesh->normals[i].y = grad[1];
								mesh->normals[i].z = grad[2];
							}

							mdata->resizeVertexBuffer(mesh->nbVerts);
							mdata->resizeIndexBuffer(mesh->nbFaces * 6);

							for (int inj = 0; inj < mesh->nbVerts; inj++) {
								glm::vec3 v(mesh->verts[inj].x, mesh->verts[inj].y, mesh->verts[inj].z);
								glm::vec3 n(mesh->normals[inj].x, mesh->normals[inj].y, mesh->normals[inj].z);

								mdata->setVertex(inj, v);
								mdata->setNormal(inj, n);
								mdata->setColor(inj, glm::vec3(0.69f, 0.69f, 0.69f));
							}

							int tri_counter = 0;
							int quad_counter = 0;
							for (int inj = 0; inj < mesh->nbFaces; inj++) {

								VDBUtility::SplitQuad split = VDBUtility::quadToTriangles(mesh, quad_counter);

								mdata->setIndex(tri_counter, split.a[2]);
								mdata->setIndex(tri_counter + 1, split.a[1]);
								mdata->setIndex(tri_counter + 2, split.a[0]);

								mdata->setIndex(tri_counter + 3, split.b[2]);
								mdata->setIndex(tri_counter + 4, split.b[1]);
								mdata->setIndex(tri_counter + 5, split.b[0]);

								quad_counter += 4;
								tri_counter += 6;
							}

							delete[] mesh->verts;
							delete[] mesh->normals;
							delete[] mesh->indices;

							delete mesh;
						}
					}
				}
			}
		}
		
		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void VDBVolumeToMesh::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void VDBVolumeToMesh::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> VDBVolumeToMesh::exportData() {
		return mapEntityProperties(this);
	}

	bool VDBVolumeToMesh::isExportable() {
		return true;
	}

	void *VDBVolumeToMesh::creator(DfxGlobalEntityName uniquename) {
		return new VDBVolumeToMesh(uniquename);
	}

	void VDBVolumeToMesh::initialize() {

		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType);
		DfxEntityDB::createProperty(inAdaptivity, "adaptivity", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inIso, "iso", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(mesh, "@mesh_data", new DfxEntityPropertyTypes::Dfx3DMeshPropertyType);
	}
	
	VDBVolumeToPoints::VDBVolumeToPoints(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {
		setLogicalName("VDB Volume To Points");

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inMode, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outPoints, DfxEntityPropertyState::OUTPUT_STATE);

		((DfxEnum*)getProperty(inMode))->add("All");
		((DfxEnum*)getProperty(inMode))->add("Interior");
		((DfxEnum*)getProperty(inMode))->add("Exterior");

		VDBInitialize();
	}

	VDBVolumeToPoints::~VDBVolumeToPoints() {

	}

	void VDBVolumeToPoints::compute() {
		using namespace DfxEntityPropertyTypes;

		Float3Array *out_positions = (Float3Array*)getProperty(outPoints);
		out_positions->deleteArray();

		if (propertyHasBinding(inRef)) {

			auto in_uid = (DfxTraitUID<size_t>*)getProperty(inRef);
			DfxAny *anyGridPtr = VDBUtility::findGridData(in_uid);
			int mode = ((DfxEnum*)getProperty(inMode))->getCurrentIndex();
			
			if (anyGridPtr != nullptr) {

				if (!anyGridPtr->isNull()) {
					std::vector<glm::vec3> vdb_points;
					openvdb::FloatGrid::Ptr *gridPtr = toGridPointer(anyGridPtr->getData());

					for (openvdb::FloatGrid::ValueOnIter iter = (*gridPtr)->beginValueOn(); iter; ++iter) {
						if (iter.isVoxelValue()) {

							if (mode == 0) {
								auto vpos = (*gridPtr)->transform().indexToWorld(iter.getCoord());
								vdb_points.push_back(glm::vec3(vpos.x(), vpos.y(), vpos.z()));
							}
							else if (mode == 1) {
								if (iter.getValue() < 0.f) {
									auto vpos = (*gridPtr)->transform().indexToWorld(iter.getCoord());
									vdb_points.push_back(glm::vec3(vpos.x(), vpos.y(), vpos.z()));
								}
							}
							else {
								if (iter.getValue() > 0.f) {
									auto vpos = (*gridPtr)->transform().indexToWorld(iter.getCoord());
									vdb_points.push_back(glm::vec3(vpos.x(), vpos.y(), vpos.z()));
								}
							}					
						}
					}

					if (vdb_points.size() > 0) {
						out_positions->resize(vdb_points.size());

						for (size_t i = 0; i < vdb_points.size(); i++) {

							(*out_positions)[i].x = vdb_points[i].x;
							(*out_positions)[i].y = vdb_points[i].y;
							(*out_positions)[i].z = vdb_points[i].z;
						}
					}
				}
			}
		}

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void VDBVolumeToPoints::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void VDBVolumeToPoints::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> VDBVolumeToPoints::exportData() {
		return mapEntityProperties(this);
	}

	bool VDBVolumeToPoints::isExportable() {
		return true;
	}

	void *VDBVolumeToPoints::creator(DfxGlobalEntityName uniquename) {
		return new VDBVolumeToPoints(uniquename);
	}

	void VDBVolumeToPoints::initialize() {

		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType);
		DfxEntityDB::createProperty(inMode, "mode", new DfxEntityPropertyTypes::DfxEnumPropertyType);
		DfxEntityDB::createProperty(outPoints, "points", new DfxEntityPropertyTypes::Float3ArrayPropertyType);
		
	}

	VDBBoolean::VDBBoolean(DfxGlobalEntityName uniquename): DfxEntity(uniquename) {
		setLogicalName("VDB Boolean");

		((DfxTraitUID<size_t>*)getProperty(outRef))->setRootID(getInstanceID());

		setPropertyState(inRef01, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inRef02, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inType, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);

		setPropertyState(grid, DfxEntityPropertyState::DATA_STORE_STATE);

		((DfxEnum*)getProperty(inType))->add("Union");
		((DfxEnum*)getProperty(inType))->add("Difference");
		((DfxEnum*)getProperty(inType))->add("Intersection");

		VDBInitialize();
	}

	VDBBoolean::~VDBBoolean() {

		if (!((DfxAny*)getProperty(grid))->isNull())
			((DfxAny*)getProperty(grid))->deleteData< openvdb::FloatGrid::Ptr>();
	}

	void VDBBoolean::compute() {
		using namespace DfxEntityPropertyTypes;

		((DfxAny*)getProperty(grid))->deleteData< openvdb::FloatGrid::Ptr>();

		if (propertyHasBinding(inRef01) && propertyHasBinding(inRef02)) {

			int type = ((DfxEnum*)getProperty(inType))->getCurrentIndex();

			auto in_uid01 = (DfxTraitUID<size_t>*)getProperty(inRef01);
			auto in_uid02 = (DfxTraitUID<size_t>*)getProperty(inRef02);

			DfxAny *anyGridPtr01 = VDBUtility::findGridData(in_uid01);
			DfxAny *anyGridPtr02 = VDBUtility::findGridData(in_uid02);

			if (anyGridPtr01 != nullptr && anyGridPtr02 != nullptr) {

				if (!anyGridPtr01->isNull() && !anyGridPtr02->isNull()) {

					auto gridPtr01 = toGridPointer(anyGridPtr01->getData());
					auto gridPtr02 = toGridPointer(anyGridPtr02->getData());

					if ((*gridPtr01)->activeVoxelCount() > 0 && (*gridPtr02)->activeVoxelCount() > 0) {

						openvdb::FloatGrid::Ptr *result = new  openvdb::FloatGrid::Ptr();
						*result = (*gridPtr01)->deepCopy();
						openvdb::FloatGrid::Ptr grid02Copy = (*gridPtr02)->deepCopy();

						if (type == 0) {
							openvdb::tools::csgUnion(*(*result), *grid02Copy);
						}
						else if (type == 1) {
							openvdb::tools::csgDifference(*(*result), *grid02Copy);
						}
						else if (type == 2) {
							openvdb::tools::csgIntersection(*(*result), *grid02Copy);
						}

						((DfxAny*)getProperty(grid))->setData(result);
					}
				}
			}
		}

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void VDBBoolean::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void VDBBoolean::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> VDBBoolean::exportData() {
		return mapEntityProperties(this);
	}

	bool VDBBoolean::isExportable() {
		return true;
	}

	void *VDBBoolean::creator(DfxGlobalEntityName uniquename) {
		return new VDBBoolean(uniquename);
	}

	void VDBBoolean::initialize() {

		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inRef01, "in_ref01", new DfxEntityPropertyTypes::UIDPropertyType);
		DfxEntityDB::createProperty(inRef02, "in_ref02", new DfxEntityPropertyTypes::UIDPropertyType);
		DfxEntityDB::createProperty(inType, "type", new DfxEntityPropertyTypes::DfxEnumPropertyType());
		DfxEntityDB::createProperty(grid, "@vdb_grid", new DfxEntityPropertyTypes::AnyPropertyType());
	}
	

	VDBDilate::VDBDilate(DfxGlobalEntityName uniquename):DfxEntity(uniquename) {
		setLogicalName("VDB Dilate");
		
		((DfxTraitUID<size_t>*)getProperty(outRef))->setRootID(getInstanceID());

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inType, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inSize, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);

		setPropertyState(grid, DfxEntityPropertyState::DATA_STORE_STATE);

		((DfxEnum*)getProperty(inType))->add("Dilate");
		((DfxEnum*)getProperty(inType))->add("Erode");

		*(float*)getProperty(inSize) = 0.025;

		VDBInitialize();
	}

	VDBDilate::~VDBDilate() {
		if (!((DfxAny*)getProperty(grid))->isNull())
			((DfxAny*)getProperty(grid))->deleteData< openvdb::FloatGrid::Ptr>();
	}

	void VDBDilate::compute() {

		using namespace DfxEntityPropertyTypes;

		((DfxAny*)getProperty(grid))->deleteData< openvdb::FloatGrid::Ptr>();

		if (propertyHasBinding(inRef)) {

			int type = ((DfxEnum*)getProperty(inType))->getCurrentIndex();

			auto in_uid = (DfxTraitUID<size_t>*)getProperty(inRef);
			auto offset = *(float*)getProperty(inSize);

			DfxAny *anyGridPtr = VDBUtility::findGridData(in_uid);

			if (anyGridPtr != nullptr) {

				if (!anyGridPtr->isNull()) {
					openvdb::FloatGrid::Ptr *gridPtr = toGridPointer(anyGridPtr->getData());

					if ((*gridPtr)->activeVoxelCount() > 0) {

						openvdb::FloatGrid::Ptr *result = new  openvdb::FloatGrid::Ptr();
						*result = (*gridPtr)->deepCopy();

						openvdb::tools::LevelSetFilter<openvdb::FloatGrid> filter(*(*result));
						filter.setGrainSize(1);

						if (type == 0) {
							filter.offset(offset * -1.f);
							filter.dilate(1);
						}
						else if (type == 1) {
							filter.offset(offset);
							filter.erode(1);
						}

						((DfxAny*)getProperty(grid))->setData(result);
					}
				}
			}
		}

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void VDBDilate::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void VDBDilate::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> VDBDilate::exportData() {
		return mapEntityProperties(this);
	}

	bool VDBDilate::isExportable() {
		return true;
	}

	void *VDBDilate::creator(DfxGlobalEntityName uniquename) {
		return new VDBDilate(uniquename);
	}

	void VDBDilate::initialize() {
		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType);
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType);
		DfxEntityDB::createProperty(inType, "type", new DfxEntityPropertyTypes::DfxEnumPropertyType);
		DfxEntityDB::createProperty(inSize, "offset", new DfxEntityPropertyTypes::FloatPropertyType);
		DfxEntityDB::createProperty(grid, "@vdb_grid", new DfxEntityPropertyTypes::AnyPropertyType());
	}
	


	VDBFilter::VDBFilter(DfxGlobalEntityName uniquename) :DfxEntity(uniquename) {
		setLogicalName("VDB Filters");

		((DfxTraitUID<size_t>*)getProperty(outRef))->setRootID(getInstanceID());

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inType, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inWidth, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inIterations, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);
		
		((DfxEnum*)getProperty(inType))->add("Mean");
		((DfxEnum*)getProperty(inType))->add("Median");
		((DfxEnum*)getProperty(inType))->add("Gaussian");
		((DfxEnum*)getProperty(inType))->add("Laplacian");
		((DfxEnum*)getProperty(inType))->add("Mean Curvatue ");

		setPropertyState(grid, DfxEntityPropertyState::DATA_STORE_STATE);
		
		*(float*)getProperty(inWidth) = 1.f;
		*(float*)getProperty(inIterations) = 1.f;

		VDBInitialize();
	}

	VDBFilter::~VDBFilter() {
		if (!((DfxAny*)getProperty(grid))->isNull())
			((DfxAny*)getProperty(grid))->deleteData< openvdb::FloatGrid::Ptr>();
	}

	void VDBFilter::compute(){
		using namespace DfxEntityPropertyTypes;

		((DfxAny*)getProperty(grid))->deleteData< openvdb::FloatGrid::Ptr>();

		if (propertyHasBinding(inRef)) {

			int type = ((DfxEnum*)getProperty(inType))->getCurrentIndex();
			float width = *(float*)getProperty(inWidth);
			int iterations = (*(float*)getProperty(inIterations) > 0) ? *(float*)getProperty(inIterations) : 1.0f;
			auto in_uid = (DfxTraitUID<size_t>*)getProperty(inRef);
			DfxAny *anyGridPtr = VDBUtility::findGridData(in_uid);

			if (anyGridPtr != nullptr) {

				if (!anyGridPtr->isNull()) {
					openvdb::FloatGrid::Ptr *gridPtr = toGridPointer(anyGridPtr->getData());

					if ((*gridPtr)->activeVoxelCount() > 0) {

						openvdb::FloatGrid::Ptr *result = new  openvdb::FloatGrid::Ptr();
						*result = (*gridPtr)->deepCopy();

						openvdb::tools::LevelSetFilter<openvdb::FloatGrid> filter(*(*result));
						filter.setGrainSize(1);

						
						if (type == 0) {
							for (int i = 0; i < iterations; i++) {
								filter.mean(width);
							}
						}
						else if (type == 1) {
							for (int i = 0; i < iterations; i++) {
								filter.median(width);
							}
						}
						else if (type == 2) {
							for (int i = 0; i < iterations; i++) {
								filter.gaussian(width);
							}
						}
						else if (type == 3) {
							for (int i = 0; i < iterations; i++) {
								filter.laplacian();
							}
						}
						else if (type == 4) {
							for (int i = 0; i < iterations; i++) {
								filter.meanCurvature();
							}
						}

						((DfxAny*)getProperty(grid))->setData(result);
					}
				}
			}
		}

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void VDBFilter::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void VDBFilter::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> VDBFilter::exportData() {
		return mapEntityProperties(this);
	}

	bool VDBFilter::isExportable() {
		return true;
	}

	void *VDBFilter::creator(DfxGlobalEntityName uniquename) {
		return new VDBFilter(uniquename);
	}

	void VDBFilter::initialize() {
		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType);
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType);
		DfxEntityDB::createProperty(inType, "type", new DfxEntityPropertyTypes::DfxEnumPropertyType);
		DfxEntityDB::createProperty(inWidth, "width", new DfxEntityPropertyTypes::FloatPropertyType);
		DfxEntityDB::createProperty(inIterations, "iterations", new DfxEntityPropertyTypes::FloatPropertyType);
		DfxEntityDB::createProperty(grid, "@vdb_grid", new DfxEntityPropertyTypes::AnyPropertyType());

		VDBInitialize();
	}	


	VDBResample::VDBResample(DfxGlobalEntityName uniquename) :DfxEntity(uniquename) {
		setLogicalName("VDB Resample");

		((DfxTraitUID<size_t>*)getProperty(outRef))->setRootID(getInstanceID());

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inVoxelSize, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);

		setPropertyState(grid, DfxEntityPropertyState::DATA_STORE_STATE);
	}

	VDBResample::~VDBResample() {
		if (!((DfxAny*)getProperty(grid))->isNull())
			((DfxAny*)getProperty(grid))->deleteData< openvdb::FloatGrid::Ptr>();
	}

	void VDBResample::compute() {
		using namespace DfxEntityPropertyTypes;

		((DfxAny*)getProperty(grid))->deleteData< openvdb::FloatGrid::Ptr>();

		if (propertyHasBinding(inRef)) {

			float new_vs = *(float*)getProperty(inVoxelSize);
			
			auto in_uid = (DfxTraitUID<size_t>*)getProperty(inRef);
			DfxAny *anyGridPtr = VDBUtility::findGridData(in_uid);

			if (anyGridPtr != nullptr) {

				if (!anyGridPtr->isNull()) {
					openvdb::FloatGrid::Ptr *gridPtr = toGridPointer(anyGridPtr->getData());

					if ((*gridPtr)->activeVoxelCount() > 0) {

						openvdb::FloatGrid::Ptr *result = new  openvdb::FloatGrid::Ptr();
						
						*result = openvdb::FloatGrid::create(2.0);
						
						(*result)->setGridClass(openvdb::GRID_LEVEL_SET);
			
						(*result)->setTransform(openvdb::math::Transform::createLinearTransform(new_vs));
						
						openvdb::tools::resampleToMatch<openvdb::tools::BoxSampler>(*(*gridPtr), *(*result));

						((DfxAny*)getProperty(grid))->setData(result);
					}
				}
			}
		}

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void VDBResample::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void VDBResample::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> VDBResample::exportData() {
		return mapEntityProperties(this);
	}

	bool VDBResample::isExportable() {
		return true;
	}

	void *VDBResample::creator(DfxGlobalEntityName uniquename) {
		return new VDBResample(uniquename);
	}

	void VDBResample::initialize() {
		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType);
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType);
		DfxEntityDB::createProperty(inVoxelSize, "voxelsize", new DfxEntityPropertyTypes::FloatPropertyType);
		DfxEntityDB::createProperty(grid, "@vdb_grid", new DfxEntityPropertyTypes::AnyPropertyType());
	}
	

	VDBPointsToVolume::VDBPointsToVolume(DfxGlobalEntityName uniquename) :DfxEntity(uniquename) {
		setLogicalName("VDB Points To Volume");

		((DfxTraitUID<size_t>*)getProperty(outRef))->setRootID(getInstanceID());

		setPropertyState(inPoints, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inSizes, DfxEntityPropertyState::INPUT_STATE);
		//setPropertyState(inVelocities, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inUniformSize, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inVoxelSize, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);

		setPropertyState(grid, DfxEntityPropertyState::DATA_STORE_STATE);

		*(float*)DfxEntity::getProperty(inVoxelSize) = 0.1f;
		*(float*)DfxEntity::getProperty(inUniformSize) = 0.2f;


		VDBInitialize();
	}

	VDBPointsToVolume::~VDBPointsToVolume() {
		if (!((DfxAny*)getProperty(grid))->isNull())
			((DfxAny*)getProperty(grid))->deleteData< openvdb::FloatGrid::Ptr>();
	}

	void VDBPointsToVolume::compute() {

		using namespace DfxEntityPropertyTypes;

		float in_vsize = *(float*)DfxEntity::getProperty(inVoxelSize);
		float in_psize = *(float*)DfxEntity::getProperty(inUniformSize);

		((DfxAny*)getProperty(grid))->deleteData< openvdb::FloatGrid::Ptr>();


		if (propertyHasBinding(inPoints)) {

			Float3Array *parr = (Float3Array*)getProperty(inPoints);
			FloatArray *sarr = (FloatArray*)getProperty(inSizes);

			if (!parr->isNull() && parr->size() > 0) {

				auto fpoints = std::make_unique<glm::vec3[]>(parr->size());
				auto fsizes = std::make_unique<float[]>(parr->size());

				for (size_t inx = 0; inx < parr->size(); inx++) {

					fpoints[inx].x = (*parr)[inx].x;
					fpoints[inx].y = (*parr)[inx].y;
					fpoints[inx].z = (*parr)[inx].z;

					bool useUniform = true;

					if (!sarr->isNull()) {
						if (sarr->size() > inx) {
							fsizes[inx] = sarr->get(inx);
							useUniform = false;
						}
					}

					if (useUniform) {
						fsizes[inx] = in_psize;
					}
				}

				OpenvdbParticleList particleList;
				particleList.addParticlesAndSizes(fpoints.get(), parr->size(), fsizes.get());

				openvdb::FloatGrid::Ptr *result = new openvdb::FloatGrid::Ptr();
				*result = openvdb::createLevelSet<openvdb::FloatGrid>(in_vsize, 3.0);

				openvdb::tools::ParticlesToLevelSet<openvdb::FloatGrid> raster(*(*result));

				raster.setGrainSize(1);
				raster.rasterizeSpheres(particleList);
				raster.finalize();

				((DfxAny*)getProperty(grid))->setData(result);

			}
		}

		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void VDBPointsToVolume::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void VDBPointsToVolume::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> VDBPointsToVolume::exportData() {
		return mapEntityProperties(this);
	}

	bool VDBPointsToVolume::isExportable() {
		return true;
	}

	void *VDBPointsToVolume::creator(DfxGlobalEntityName uniquename) {
		return new VDBPointsToVolume(uniquename);
	}

	void VDBPointsToVolume::initialize() {
		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType);
		DfxEntityDB::createProperty(inPoints, "points", new DfxEntityPropertyTypes::Float3ArrayPropertyType);
		DfxEntityDB::createProperty(inSizes, "sizes", new DfxEntityPropertyTypes::FloatArrayPropertyType);
		DfxEntityDB::createProperty(inVelocities, "velocities", new DfxEntityPropertyTypes::Float3ArrayPropertyType);
		DfxEntityDB::createProperty(inVoxelSize, "voxel_size", new DfxEntityPropertyTypes::FloatPropertyType);
		DfxEntityDB::createProperty(inUniformSize, "uniform_size", new DfxEntityPropertyTypes::FloatPropertyType);
		DfxEntityDB::createProperty(grid, "@vdb_grid", new DfxEntityPropertyTypes::AnyPropertyType());
	}
	

	VDBFromFile::VDBFromFile(DfxGlobalEntityName uniquename) :DfxEntity(uniquename) {
		setLogicalName("VDB From File");

		setPropertyState(inType, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inIndex, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inFileName, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);

		((DfxEnum*)getProperty(inType))->add("Single");
		((DfxEnum*)getProperty(inType))->add("Sequence");

		VDBInitialize();
	}

	VDBFromFile::~VDBFromFile() {

	}

	void VDBFromFile::compute() {

	}

	void VDBFromFile::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void VDBFromFile::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> VDBFromFile::exportData() {
		return mapEntityProperties(this);
	}

	bool VDBFromFile::isExportable() {
		return true;
	}

	void *VDBFromFile::creator(DfxGlobalEntityName uniquename) {
		return new VDBFromFile(uniquename);
	}

	void VDBFromFile::initialize() {
		DfxEntityDB::createProperty(inType, "type", new DfxEntityPropertyTypes::DfxEnumPropertyType);
		DfxEntityDB::createProperty(inIndex, "index", new DfxEntityPropertyTypes::FloatPropertyType);
		DfxEntityDB::createProperty(inFileName, "filename", new DfxEntityPropertyTypes::StringPropertyType);
		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType);
	}


	VDBToFile::VDBToFile(DfxGlobalEntityName uniquename) :DfxEntity(uniquename) {
		setLogicalName("VDB To File");

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inType, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inIndex, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inDir, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPrefix, DfxEntityPropertyState::INPUT_STATE);

		((DfxEnum*)getProperty(inType))->add("Single");
		((DfxEnum*)getProperty(inType))->add("Sequence");

		VDBInitialize();
	}

	VDBToFile::~VDBToFile() {

	}

	void VDBToFile::compute() {

	}

	void VDBToFile::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void VDBToFile::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> VDBToFile::exportData() {
		return mapEntityProperties(this);
	}

	bool VDBToFile::isExportable() {
		return true;
	}

	void *VDBToFile::creator(DfxGlobalEntityName uniquename) {
		return new VDBToFile(uniquename);
	}

	void VDBToFile::initialize() {
		DfxEntityDB::createProperty(inRef,   "in_ref", new DfxEntityPropertyTypes::UIDPropertyType);
		DfxEntityDB::createProperty(inType,  "type", new DfxEntityPropertyTypes::DfxEnumPropertyType);
		DfxEntityDB::createProperty(inIndex, "index", new DfxEntityPropertyTypes::FloatPropertyType);
		DfxEntityDB::createProperty(inDir,   "directory", new DfxEntityPropertyTypes::DirPathPropertyType);
		DfxEntityDB::createProperty(inPrefix,"prefix", new DfxEntityPropertyTypes::StringPropertyType);
	}
	
}

#endif