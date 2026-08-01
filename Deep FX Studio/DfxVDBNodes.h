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

#pragma once

#include "DfxVDBGlobal.h"

#ifdef USE_OPENVDB_LIB 

#include <DfxEntity.h>
#include <DfxEntityBasicPropertyTypes.h>
#include <DfxEntityDB.h>
#include <DfxGlobal.h>
#include <DfxUtility.h>
#include <DfxTempIndexedMesh.h>

#undef foreach
#include <openvdb/openvdb.h>
#include <DfxVDBCommon.h>


namespace DfxNodes {

	inline void VDBInitialize() { openvdb::initialize(); }

	// Utility Class
	class VDBUtility {
	public:

		typedef struct {
			size_t a[3];
			size_t b[3];
		}SplitQuad;

		static DfxAny *findGridData(DfxEntityPropertyTypes::UID *uid) {

			DfxEntityDataInstance *inst = DfxEntityDB::getDataInstance(uid->getRootID());

			if (inst != nullptr) {

				DfxEntity  *ent = inst->getDfxEntity();

				if (ent->hasDataStoreProperty("@vdb_grid")) {

					size_t iid = ent->dataStorePropertyID("@vdb_grid");
					DfxAny *data = (DfxAny*)ent->getProperty(iid);

					return data;
				}
			}

			return nullptr;
		}

		static SplitQuad quadToTriangles(Mesh *mesh, int counter) {
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
	};

	static DfxGlobalEntityName VDBMeshToVolume_Identifier = "2F2X252X_DFX_VDB_MESH_TO_VOLUME_NODE";

	class VDBMeshToVolume : public DfxEntity {
	private:
		typedef struct {
			std::vector<openvdb::Vec3s> points;
			std::vector<openvdb::Vec4I> polys;

			unsigned int polygonCount();
		}VDB_TempMesh;

	private:
		VDB_TempMesh sample3DMeshData(Dfx3dMeshData *, openvdb::math::Transform xform, glm::mat4 transMatrix, glm::vec3 scale);
	public:
		static const DfxPropertyID outRef			= 0;
		static const DfxPropertyID inRef			= 1;
		static const DfxPropertyID inVoxelSize		= 2;
		static const DfxPropertyID inNarrowBand		= 3;
		static const DfxPropertyID inPosition		= 4;
		static const DfxPropertyID inRotation		= 5;
		static const DfxPropertyID inScale			= 6;
		static const DfxPropertyID grid				= 7;

		VDBMeshToVolume(DfxGlobalEntityName uniquename);

		virtual ~VDBMeshToVolume();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	static DfxGlobalEntityName VDBVolumeToMesh_Identifier = "2F2X252X_DFX_VDB_VOLUME_TO_MESH_NODE";

	class VDBVolumeToMesh : public DfxEntity {
	public:
		static const DfxPropertyID outRef			= 0;
		static const DfxPropertyID inRef			= 1;
		static const DfxPropertyID inAdaptivity		= 2;
		static const DfxPropertyID inIso			= 3;
		static const DfxPropertyID mesh				= 4;

		VDBVolumeToMesh(DfxGlobalEntityName uniquename);

		virtual ~VDBVolumeToMesh();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	static DfxGlobalEntityName VDBVolumeToPoints_Identifier = "2F2X252X_DFX_VDB_VOLUME_TO_POINTS_NODE";

	class VDBVolumeToPoints : public DfxEntity {
	public:
		static const DfxPropertyID inRef	 = 0;
		static const DfxPropertyID inMode	 = 1;
		static const DfxPropertyID outPoints = 2;

		VDBVolumeToPoints(DfxGlobalEntityName uniquename);

		virtual ~VDBVolumeToPoints();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	static DfxGlobalEntityName VDBBoolean_Identifier = "2F2X252X_DFX_VDB_BOOLEAN_NODE";

	class VDBBoolean : public DfxEntity {
	public:
		static const DfxPropertyID outRef		= 0;
		static const DfxPropertyID inRef01		= 1;
		static const DfxPropertyID inRef02		= 2;
		static const DfxPropertyID inType		= 3;
		static const DfxPropertyID grid			= 4;

		VDBBoolean(DfxGlobalEntityName uniquename);

		virtual ~VDBBoolean();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	static DfxGlobalEntityName VDBDilate_Identifier = "2F2X252X_DFX_VDB_DILATE_DILATE_NODE";

	class VDBDilate : public DfxEntity {
	public:
		static const DfxPropertyID outRef		= 0;
		static const DfxPropertyID inRef		= 1;
		static const DfxPropertyID inType		= 2;
		static const DfxPropertyID inSize		= 3;
		static const DfxPropertyID grid			= 4;

		VDBDilate(DfxGlobalEntityName uniquename);

		virtual ~VDBDilate();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	static DfxGlobalEntityName VDBFilter_Identifier = "2F2X252X_DFX_VDB_FILTER_NODE";

	class VDBFilter : public DfxEntity {
	public:
		static const DfxPropertyID outRef		= 0;
		static const DfxPropertyID inRef		= 1;
		static const DfxPropertyID inType		= 2;
		static const DfxPropertyID inWidth		= 3;
		static const DfxPropertyID inIterations	= 4;
		static const DfxPropertyID grid			= 5;

		VDBFilter(DfxGlobalEntityName uniquename);

		virtual ~VDBFilter();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	static DfxGlobalEntityName VDBResample_Identifier = "2F2X252X_DFX_VDB_RESAMPLE_NODE";

	class VDBResample : public DfxEntity {
	public:
		static const DfxPropertyID outRef			= 0;
		static const DfxPropertyID inRef			= 1;
		static const DfxPropertyID inVoxelSize		= 2;
		static const DfxPropertyID grid				= 3;

		VDBResample(DfxGlobalEntityName uniquename);

		virtual ~VDBResample();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};


	static DfxGlobalEntityName VDBPointsToVolume_Identifier = "2F2X252X_DFX_VDB_POINTS_TO_VOLUME_NODE";

	class VDBPointsToVolume : public DfxEntity {
	public:
		static const DfxPropertyID outRef			= 0;
		static const DfxPropertyID inPoints			= 1;
		static const DfxPropertyID inSizes			= 2;
		static const DfxPropertyID inVelocities		= 3;
		static const DfxPropertyID inUniformSize	= 4;
		static const DfxPropertyID inVoxelSize		= 5;
		static const DfxPropertyID grid				= 6;
		

		VDBPointsToVolume(DfxGlobalEntityName uniquename);

		virtual ~VDBPointsToVolume();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	static DfxGlobalEntityName VDBFromFile_Identifier = "2F2X252X_DFX_VDB_FROM_FILE_NODE";

	class VDBFromFile : public DfxEntity {
	public:
		static const DfxPropertyID outRef		= 0;
		static const DfxPropertyID inType		= 1;	
		static const DfxPropertyID inIndex		= 2;
		static const DfxPropertyID inFileName	= 3;
		

		VDBFromFile(DfxGlobalEntityName uniquename);

		virtual ~VDBFromFile();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};

	static DfxGlobalEntityName VDBToFile_Identifier = "2F2X252X_DFX_VDB_TO_FILE_NODE";

	class VDBToFile : public DfxEntity {
	public:
		static const DfxPropertyID inRef		= 0;
		static const DfxPropertyID inType		= 1;
		static const DfxPropertyID inIndex		= 2;
		static const DfxPropertyID inDir		= 3;
		static const DfxPropertyID inPrefix		= 4;

		VDBToFile(DfxGlobalEntityName uniquename);

		virtual ~VDBToFile();

		void compute() override;

		void onDataChanged(DfxEntityDataInstance *instance) override;

		void importData(std::vector<std::pair<std::string, std::string>> data) override;

		std::vector<std::pair<std::string, std::string>> exportData() override;

		bool isExportable() override;

		static void *creator(DfxGlobalEntityName uniquename);

		static void initialize();
	};
}


#endif