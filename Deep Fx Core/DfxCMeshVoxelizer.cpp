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

#include "DfxCMeshVoxelisingFunctions.h"
#include "DfxCMeshVoxelizer.h"

namespace dfxcore {

	MeshVoxelizer::MeshVoxelizer(DfxGlobalEntityName uniquename) :DfxEntity(uniquename) {
		setLogicalName("Mesh Voxelizer");

		using namespace DfxEntityPropertyTypes;


		*((float*)getProperty(inVoxelSize)) = 0.04f;

		((DfxEnum*)getProperty(inSource))->add("Volume");
		((DfxEnum*)getProperty(inSource))->add("Surface");
		((Float3*)getProperty(inScale))->x = 1.0;
		((Float3*)getProperty(inScale))->y = 1.0;
		((Float3*)getProperty(inScale))->z = 1.0;

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inSource, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inVoxelSize, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inScale, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outPointArray, DfxEntityPropertyState::OUTPUT_STATE);
	}

	MeshVoxelizer::~MeshVoxelizer() {

	}

	void MeshVoxelizer::compute() {

		using namespace DfxEntityPropertyTypes;

		UID *uid = (UID*)getProperty(inRef);
		float voxSize  = *(float*)getProperty(inVoxelSize);
		int   source   = ((DfxEnum*)getProperty(inSource))->getCurrentIndex();
		Float3 *_scale = (Float3*)getProperty(inScale);
		Float3Array *output = (Float3Array *)getProperty(outPointArray);

		Dfx3dMeshData *mesh = dfxutil::findMeshData(uid);

		if (mesh != nullptr) {

			int vcount = mesh->getVertexCount();
			int faces = mesh->getIndexCount() / 3;

			if (isPropertyBindingDirty(inRef) ||
				isPropertyBindingDirty(inVoxelSize) ||
				isPropertyBindingDirty(inSource) ||
				isPropertyBindingDirty(inScale)) {

				if (mesh->getVertexCount() > 3 && ((int)(mesh->getIndexCount() / 3)) >= 1) {

					std::vector<glm::vec3> points;

					DfxGlobal::info::message("Voxelizing Mesh...");

					if (source == 0) {
						glm::vec3 scale(_scale->x, _scale->y, _scale->z);
						voxelizeMeshVolume(*mesh, scale, points, voxSize, true);
					}
					else {
						glm::vec3 scale(_scale->x, _scale->y, _scale->z);
						voxelizeMesh(*mesh, scale, points, voxSize);
					}

					if (points.size() > 0) {
						output->resize(points.size());

						for (int i = 0; i < points.size(); i++) {
							(*output)[i].x = points[i].x;
							(*output)[i].y = points[i].y;
							(*output)[i].z = points[i].z;
						}

						DfxGlobal::console::println("Voxel Count: " + std::to_string(points.size()) + "\n");
						makePropertyBindingDirty(outPointArray);
					}
				}
				else {
					output->deleteArray();
				}
			}
		}

		cleanAllProperties();
		getDataInstance()->makeClean();
		makeChildrenDirty();
		DfxGlobal::info::message("");
	}

	void MeshVoxelizer::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void MeshVoxelizer::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> MeshVoxelizer::exportData() {
		return mapEntityProperties(this);
	}

	bool MeshVoxelizer::isExportable() {
		return true;
	}

	void *MeshVoxelizer::creator(DfxGlobalEntityName uniquename) {
		return new MeshVoxelizer(uniquename);
	}

	void MeshVoxelizer::initialize() {

		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType);
		DfxEntityDB::createProperty(inVoxelSize, "voxel_size", new DfxEntityPropertyTypes::FloatPropertyType);
		DfxEntityDB::createProperty(inSource, "source", new DfxEntityPropertyTypes::DfxEnumPropertyType);
		DfxEntityDB::createProperty(inScale, "scale", new DfxEntityPropertyTypes::Float3PropertyType);
		DfxEntityDB::createProperty(outPointArray, "points", new DfxEntityPropertyTypes::Float3ArrayPropertyType);
	}

	

	
}

