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


#include <vector>
#include <DfxGlobal.h>
#include <glm/mat4x4.hpp>
#include <sstream>
#include <SgGeoAdapterInterface.h>
#include <Mesh.h>

#ifdef NDEBUG
#undef _DEBUG
#endif 
#include <PxPhysicsAPI.h>
#include <NvBlast.h>
#include <NvBlastAssert.h>
#include <NvBlastGlobals.h>
#include <NvBlastExtAuthoring.h>
#include <NvBlastExtAuthoringMesh.h>
#include <NvBlastExtAuthoringBondGenerator.h>
#include <NvBlastExtAuthoringCutout.h>
#include <NvBlastExtAuthoringFractureTool.h>
#include <NvBlastExtAuthoringMeshCleaner.h>
#include <NvBlastPxCallbacks.h>

#include "DfxNvBlastException.h"
#include "DfxNvBlastUtil.h"


namespace dfxblast {

	//bool initSuccess = false;
	static physx::PxFoundation*	gFoundation = nullptr;
	static physx::PxPhysics*	gPhysics = nullptr;
	static physx::PxCooking*	gCooking = nullptr;

	struct FractureTool {
		Nv::Blast::FractureTool *ftool;
		Nv::Blast::Mesh* mesh;
	};

	struct SliceOptions {
		int32_t x_slices;
		int32_t y_slices;
		int32_t z_slices;
		float angleVariation;
		float offsetVariation;
	};

	struct CutOptions {
		glm::vec3 plane_normal;
		glm::vec3 plane_point;
		float amplitude;
		float frequency;
		int octaves;
		glm::vec3 segments;
	};

	struct CutoutOptions {
		glm::vec3 plane_normal;
		glm::vec3 plane_point;
		uint8_t* bitmap;
		int32_t map_width;
		int32_t map_height;
	};

	struct VoronoiOptions {
		int32_t cell_count;
		int32_t rand_seed;
	};


	class IndexedBlastMesh {
	private:

		std::vector<uint32_t> indexArray;
		std::vector<Nv::Blast::Vertex> vertexArray;

		
		bool findIndex(float x, float y, float z, unsigned int & out_inx) {

			for (size_t inx = 0; inx < vertexArray.size(); inx++) {

				if (x == vertexArray[inx].p.x &&
					y == vertexArray[inx].p.y &&
					z == vertexArray[inx].p.z) {

					out_inx = inx;
					return true;
				}
			}

			return false;
		}

	public:
		void buildIndexedMesh(Nv::Blast::Triangle *input, uint32_t tri_count) {
			vertexArray.clear();
			indexArray.clear();

			for (size_t inx = 0; inx < tri_count; inx++) {

				unsigned rinx = 0;

				if (!findIndex(input[inx].a.p.x, input[inx].a.p.y, input[inx].a.p.z, rinx)) {
					vertexArray.push_back(input[inx].a);
					indexArray.push_back(vertexArray.size() - 1);
				}
				else {
					indexArray.push_back(rinx);
				}

				if (!findIndex(input[inx].b.p.x, input[inx].b.p.y, input[inx].b.p.z, rinx)) {
					vertexArray.push_back(input[inx].b);
					indexArray.push_back(vertexArray.size() - 1);
				}
				else {
					indexArray.push_back(rinx);
				}

				if (!findIndex(input[inx].c.p.x, input[inx].c.p.y, input[inx].c.p.z, rinx)) {
					vertexArray.push_back(input[inx].c);
					indexArray.push_back(vertexArray.size() - 1);
				}
				else {
					indexArray.push_back(rinx);
				}
				
			}
		}

		std::vector<uint32_t> &getIndexArray() {
			return indexArray;
		}

		std::vector<Nv::Blast::Vertex> &getVertexArray() {
			return vertexArray;
		}
	};

	class CustomErrorCallback : public Nv::Blast::ErrorCallback
	{
		virtual void reportError(Nv::Blast::ErrorCode::Enum code, const char* msg, const char* file, int line) override
		{
			std::stringstream str;
			bool critical = false;
			switch (code)
			{
			case Nv::Blast::ErrorCode::eNO_ERROR:			str << "[Info]";				critical = false; break;
			case Nv::Blast::ErrorCode::eDEBUG_INFO:			str << "[Debug Info]";			critical = false; break;
			case Nv::Blast::ErrorCode::eDEBUG_WARNING:		str << "[Debug Warning]";		critical = false; break;
			case Nv::Blast::ErrorCode::eINVALID_PARAMETER:	str << "[Invalid Parameter]";	critical = true;  break;
			case Nv::Blast::ErrorCode::eINVALID_OPERATION:	str << "[Invalid Operation]";	critical = true;  break;
			case Nv::Blast::ErrorCode::eOUT_OF_MEMORY:		str << "[Out of] Memory";		critical = true;  break;
			case Nv::Blast::ErrorCode::eINTERNAL_ERROR:		str << "[Internal Error]";		critical = true;  break;
			case Nv::Blast::ErrorCode::eABORT:				str << "[Abort]";				critical = true;  break;
			case Nv::Blast::ErrorCode::ePERF_WARNING:		str << "[Perf Warning]";		critical = false; break;
			default:							NVBLAST_ASSERT(false);
			}
#if NV_DEBUG || NV_CHECKED
			str << file << "(" << line << "): ";
#else 
			NV_UNUSED(file);
			NV_UNUSED(line);
#endif				
			str << " " << msg << "\n";
			DfxGlobal::console::println(str.str());

			if (critical)
			{
				DfxGlobal::console::println("Authoring failed. Exiting.");
				throw DfxNvBlastException();
				//exit(-1);
			}
		}
	};

	static CustomErrorCallback errorCallback;
	
	class DfxNvBlastGlobal {
	private:
		static DfxNvBlastGlobal *instance;

		DfxNvBlastGlobal();

		
		static bool initSuccess;
	private:

		FractureTool newFractureTool(deepsg::SgGeoAdapterInterface *mesh, glm::mat4 tran = glm::mat4(1.0));
		void extractFracturePieces(Nv::Blast::FractureTool *, std::vector<Mesh*> &arr);
		void extractFracturePieces2(Nv::Blast::FractureTool *, std::vector<Mesh*> &arr );
	public:
		virtual ~DfxNvBlastGlobal();

		static DfxNvBlastGlobal *getInstance();

		physx::PxFoundation*	foundation() const;
		physx::PxPhysics*		physics() const;
		physx::PxCooking*		cooking() const;

	public:

		static void init();
		static bool isInitiallized();

		std::vector<Mesh*> sliceFracture(deepsg::SgGeoAdapterInterface*, SliceOptions, glm::mat4 trans);
		std::vector<Mesh*> planeFracture(deepsg::SgGeoAdapterInterface*, CutOptions, glm::mat4 trans);
		std::vector<Mesh*> voronoiFracture(deepsg::SgGeoAdapterInterface*, VoronoiOptions, glm::mat4 tran = glm::mat4(1.0),
			std::vector<glm::vec3> samples = std::vector<glm::vec3>(), int sites_per_cluster = 0, float cluster_radius = 0.0f, bool use_clustering = false, int materialID = 0);
		std::vector<Mesh*> cutoutFracture(deepsg::SgGeoAdapterInterface*, CutoutOptions, glm::mat4 tran = glm::mat4(1.0));
	};
}